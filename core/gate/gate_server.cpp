#include "gate_server.h"

#include <sys/socket.h>
#include <arpa/inet.h>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <libgo/libgo.h>

#include "core/message.h"
#include "core/rpc/rpc_manager.h"

#include "log/glog.h"
#include "core/const.hpp"
#include "core/util/util.h"

#include "gate_session.h"
#include "gate_ws_session.h"
#include "gate_peer_session.h"

namespace Eayew {

GateServer::GateServer()
    : m_type(ServerType::GATE)
    , m_agent(m_consul)
    , m_kv(m_consul)
    , m_timer(std::chrono::milliseconds(1), &co_sched) {
    auto ip = getIP();
    LOG(ERROR) << "ip " << ip;
}

GatePeerSession::ptr GateServer::getGatePeerSession(uint16_t type) {
    auto it = m_gpSessions.find(type);
    if (it != m_gpSessions.end()) {
        return it->second.begin()->second;
    }

    // 断线重连
    // auto servers = m_agent.services();
    // for (auto [id, si] : servers) {
    //     LOG(INFO) << "discoverServer id " << id << " name " << si.name;
    //     uint16_t st = std::atoi(si.name.data());
    //     if (st == type) {
    //         auto gps = std::make_shared<GatePeerSession>(si.address, si.port, *this);
    //         LOG(INFO) << "config self type " << this->type() << " rpc type " << st;
    //         gps->senderType(this->type());
    //         gps->receiverType(st);
    //         gps->run();
    //         m_gpSessions[st][id] = gps;
    //         return gps;
    //     }
    // }
    return nullptr;
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
        LOG(ERROR) << "bind error, port " << m_port;
        return;
    }
    if (-1 == listen(accept_fd, 2048)) {
        LOG(ERROR) << "listen error";
        return;
    }

    LOG(INFO) << "listen success, type " << m_type << " ip " << m_ip << " port " << m_port;

    go [&] {
        for(;;) {
            int fd = accept(accept_fd, (sockaddr*)&addr, &len);
            if (-1 == fd) {
                if (EAGAIN == errno || EINTR == errno) {
                    continue;
                }
                LOG(ERROR) << "accept error";
                return;
            }

            LOG(INFO) << "accept success, fd " << fd;
            auto gs = std::make_shared<GateWsSession>(fd);
            auto gs_id = gs->id();
            gs->setOnMessage([&, gs_id, self = shared_from_this()](Message&& msg) {
                uint16_t receiver_id = msg.receiverId();
                auto gps = getGatePeerSession(receiver_id);
                if (!gps) {
                    LOG(ERROR) << "Invalid receiver " << receiver_id;
                    return;
                }
                msg.sessionId(gs_id);
                LOG(INFO) << "onMessage, receiver_id " << receiver_id << " msg " << msg.strInfo();
                gps->send(std::move(msg));
            });
            gs->setOnClose([&](uint64_t id) {
                m_wsSessions.erase(id);
            });
            m_wsSessions[gs->id()] = gs;
            gs->start();
            gs->run();
        }
    };

    consulServer();

    co_sched.Start(1, 1);
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

    LOG(INFO) << "m_ip " << m_ip << " m_port " << m_port << " m_name " << m_name << " m_serverId " << m_serverId;
}

void GateServer::consulServer() {
    m_agent.registerService(std::to_string(m_type),
        ppconsul::agent::TcpCheck{m_ip, m_port, std::chrono::seconds(10), std::chrono::milliseconds(1)},
        ppconsul::agent::kw::deregisterCriticalServiceAfter = std::chrono::minutes(1),
        ppconsul::agent::kw::address = m_ip,
        ppconsul::agent::kw::port = m_port,
        ppconsul::agent::kw::id = m_serverId
    );

    m_timer.ExpireAt(std::chrono::seconds(3), [this, self = shared_from_this()] {
        discoverServer();
    });
}

void GateServer::discoverServer() {
    auto servers = m_agent.services();
    for (auto [id, si] : servers) {
        // LOG(INFO) << "discoverServer id " << id << " name " << si.name;
        if (id == m_serverId) {
            continue;
        }
        uint16_t st = std::atoi(si.name.data());
        auto it = m_gpSessions.find(st);
        if (it != m_gpSessions.end()) {
            auto it1 = it->second.find(id);
            if (it1 != it->second.end()) {
                // LOG(INFO) << "exist,  id " << si.id;
                continue;
            }
        }
        LOG(INFO) << "config self type " << type() << " rpc type " << st;

        auto gps = std::make_shared<GatePeerSession>();
        gps->sync_connect(si.address, si.port, type(), st);
        gps->setOnMessage([&](Message&& msg) {
            auto session_id = msg.sessionId();
            auto s = getWsSession(session_id);
            if (!s) {
                LOG(ERROR) << "Invalid session " << session_id;
                return;
            }
            if (msg.msgId() == 1002 || msg.msgId() == 1004) {
                if (msg.roleId() != 0) {
                    m_sessionIdToRoleIds[session_id] = msg.roleId();
                }
            } else if (m_sessionIdToRoleIds.find(session_id) == m_sessionIdToRoleIds.end()) {
                LOG(ERROR) << "dispatch error, session id " << session_id;
            }
            LOG(INFO) << "onMessage " << msg.strInfo();
            s->send(std::move(msg));
        });
        gps->run();
        m_gpSessions[st][si.id] = gps;
    }

    m_timer.ExpireAt(std::chrono::seconds(1), [this, self = shared_from_this()] {
        discoverServer();
    });
}

}