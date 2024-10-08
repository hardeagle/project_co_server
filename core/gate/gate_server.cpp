#include "gate_server.h"

#include <sys/socket.h>
#include <arpa/inet.h>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <co/all.h>

#include "core/const.hpp"
#include "core/message.h"
#include "core/rpc/rpc_manager.h"

#include "core/const.hpp"
#include "core/util/util.h"

#include "gate_session.h"
#include "gate_ws_session.h"
#include "gate_peer_session.h"

namespace Eayew {

GateServer::GateServer()
    : m_type(ServerType::EST_GATE)
    , m_agent(m_consul)
    , m_kv(m_consul) {
    auto ip = getIP();
    LOG << "ip " << ip;
    m_sched = co::next_sched();
}

GatePeerSession::ptr GateServer::getGatePeerSession(uint16_t type) {
    auto it = m_gpSessions.find(type);
    if (it == m_gpSessions.end()) {
        return nullptr;
    }
    if (it->second.size() == 0) {
        return nullptr;
    }
    return it->second.begin()->second;

    // 断线重连
    // auto servers = m_agent.services();
    // for (auto [id, si] : servers) {
    //     LOG << "discoverServer id " << id << " name " << si.name;
    //     uint16_t st = std::atoi(si.name.data());
    //     if (st == type) {
    //         auto gps = std::make_shared<GatePeerSession>(si.address, si.port, *this);
    //         LOG << "config self type " << this->type() << " rpc type " << st;
    //         gps->senderType(this->type());
    //         gps->receiverType(st);
    //         gps->run();
    //         m_gpSessions[st][id] = gps;
    //         return gps;
    //     }
    // }
}

GateSession::ptr GateServer::getSession(uint64_t id) {
    std::unordered_map<uint64_t, GateSession::ptr>::iterator it = m_sessions.find(id);
    return it != m_sessions.end() ? it->second : nullptr;
}

GateWsSession::ptr GateServer::getWsSession(uint64_t id) {
    std::unordered_map<uint64_t, GateWsSession::ptr>::iterator it = m_wsSessions.find(id);
    return it != m_wsSessions.end() ? it->second : nullptr;
}


void GateServer::run() {
    init();

    int accept_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(accept_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0"); //inet_addr(m_ip.data());
    socklen_t len = sizeof(addr);
    if (-1 == bind(accept_fd, (sockaddr*)&addr, len)) {
        ELOG << "bind error, port " << m_port;
        return;
    }
    if (-1 == listen(accept_fd, 2048)) {
        ELOG << "listen error";
        return;
    }

    LOG << "listen success, type " << m_type << " ip " << m_ip << " port " << m_port;

    m_sched->go([&] {
        for(;;) {
            int fd = accept(accept_fd, (sockaddr*)&addr, &len);
            if (-1 == fd) {
                if (EAGAIN == errno || EINTR == errno) {
                    close(fd);
                    continue;
                }
                ELOG << "accept error";
                close(fd);
                return;
            }

            LOG << "accept success, fd " << fd;
            auto gws = std::make_shared<GateWsSession>(fd);
            gws->setrSched(m_sched);
            gws->setwSched(m_sched);
            if (!gws->start()) {
                WLOG << "illegal ws client";
                close(fd);
                continue;
            }
            auto gws_id = gws->id();
            gws->setOnMessage([&, gws_id, self = shared_from_this()](Message::ptr msg) {
                uint16_t receiver_id = msg->receiverId();
                auto gps = getGatePeerSession(receiver_id);
                if (!gps) {
                    ELOG << "Invalid receiver " << receiver_id;
                    return;
                }
                msg->sessionId(gws_id);
                LOG << "onMessage, receiver_id " << receiver_id << " msg " << msg->strInfo();
                auto it = m_sessionIdToRoleIds.find(gws_id);
                if (it != m_sessionIdToRoleIds.end()) {
                    msg->roleId(it->second);
                }
                LOG << "onMessage, receiver_id " << receiver_id << " msg " << msg->strInfo();
                gps->send(msg);
            });
            gws->setOnClose([&](uint64_t id) {
                LOG << "GateWsSession close cb";
                // m_wsSessions[id]->closeMsg(CloseMsgId::ECMI_WebsocketSession, id);

                // m_wsSessions.erase(id);
                // m_sessionIdToRoleIds.erase(id);

                // LOG << "gws close id " << id << " m_wsSessions size " << m_wsSessions.size() << " m_sessionIdToRoleIds size " << m_sessionIdToRoleIds.size();
                // for (const auto& pair : m_gpSessions) {
                //     for (const auto& gps : pair.second) {
                //         gps.second->closeMsg(CloseMsgId::ECMI_WorkRoutine, id);
                //     }
                // }
            });
            m_wsSessions[gws->id()] = gws;
            gws->run();
        }
    });

    consulServer();

    co::wait_group wg;
    wg.add(1);
    wg.wait();
}

void GateServer::init() {
    // const std::string file = "./json/gate_server.json";
    // boost::property_tree::ptree root;
    // boost::property_tree::read_json(file, root);
    // m_name = root.get<std::string>("name");
    // m_type = root.get<uint16_t>("type");
    // m_ip = root.get<std::string>("ip");
    // m_port = root.get<uint16_t>("port");
    // m_wsPort = root.get<uint16_t>("ws_port");
    // m_serverId = serverId(m_name, m_type, m_ip, m_port);

    m_ip = getIP();
    m_port = serverPort(m_type);
    m_name = std::to_string(m_type);
    m_serverId = serverId(m_name, m_type, m_ip, m_port);

    LOG << "m_ip " << m_ip << " m_port " << m_port << " m_name " << m_name << " m_serverId " << m_serverId;
}

void GateServer::consulServer() {
    m_agent.registerService(std::to_string(m_type),
        ppconsul::agent::TcpCheck{m_ip, m_port, std::chrono::seconds(10), std::chrono::milliseconds(1)},
        ppconsul::agent::kw::deregisterCriticalServiceAfter = std::chrono::minutes(1),
        ppconsul::agent::kw::address = m_ip,
        ppconsul::agent::kw::port = m_port,
        ppconsul::agent::kw::id = m_serverId
    );

    // m_timer.ExpireAt(std::chrono::seconds(1), [this, self = shared_from_this()] {
    //     discoverServer();
    // });

    // m_sched->go([this]() {
    //     discoverServer();
    // });
    m_task.run_every([this]() {
        m_sched->go([this]() {
            discoverServer();
        });
    }, 10);
}

void GateServer::discoverServer() {
    auto servers = m_agent.services();
    for (auto& pair : servers) {
        auto& id = pair.first;
        auto& si = pair.second;
        LOG << "discoverServer id " << id << " name " << si.name;
        if (id == m_serverId) {
            continue;
        }
        uint16_t st = std::atoi(si.name.data());
        auto it = m_gpSessions.find(st);
        if (it != m_gpSessions.end()) {
            auto it1 = it->second.find(id);
            if (it1 != it->second.end()) {
                // LOG << "exist,  id " << si.id;
                continue;
            }
        }
        LOG << "config self type " << type() << " rpc type " << st;

        auto gps = std::make_shared<GatePeerSession>();
        gps->setrSched(m_sched);
        gps->setwSched(m_sched);
        if (!gps->sync_connect(si.address, si.port, type(), st)) {
            WLOG << "sync connect fail";
            continue;
        }
        gps->setOnMessage([&](Message::ptr msg) {
            if (msg->roleId() == Eayew::MsgType::EMT_NOTIFY_ROLE_ID && msg->sessionId() == Eayew::MsgType::EMT_NOTIFY_SESSION_ID) {
                LOG << "wtf notify msg " << msg->strInfo();
                for (const auto& ws : m_wsSessions) {
                    LOG << "wtf wssession id " << ws.first;
                    ws.second->send(msg);
                }
                return;
            }

            auto session_id = msg->sessionId();
            auto s = getWsSession(session_id);
            if (!s) {
                ELOG << "Invalid session " << session_id;
                return;
            }
            if ((msg->receiverId() == ServerType::EST_LOGIN) && (msg->msgId() == 1002 || msg->msgId() == 1004 || msg->msgId() == 1008)) {
                if (msg->roleId() != 0) {
                    m_sessionIdToRoleIds[session_id] = msg->roleId();
                }
            } else if (m_sessionIdToRoleIds.find(session_id) == m_sessionIdToRoleIds.end()) {
                ELOG << "dispatch error, session id " << session_id << " msg id " << msg->msgId() << " msg receiver id " << msg->receiverId();
            }
            // LOG << "onMessage " << msg.strInfo();
            s->send(msg);
        });
        gps->setOnClose([&, st, sid = si.id](uint64_t) {
            LOG << "onClose st " << st << " sid " << sid << " size " << m_gpSessions[st].size();
            m_gpSessions[st].erase(sid);
        });
        gps->run();
        m_gpSessions[st][si.id] = gps;
    }

    // m_task.run_every([this]() {
    //     m_sched->go([this]() {
    //         discoverServer();
    //     });
    // }, 10);
    // m_timer.ExpireAt(std::chrono::seconds(10), [this, self = shared_from_this()] {
    //     discoverServer();
    // });
}

}