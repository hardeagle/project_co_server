#include "gate_server.h"

#include <sys/socket.h>
#include <arpa/inet.h>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <libgo/libgo.h>

#include "core/message.hpp"
#include "core/rpc/rpc_manager.h"

#include "log/glog.h"
#include "core/util/util.h"

#include "gate_session.h"
#include "gate_peer_session.h"

namespace Eayew {

GateServer::GateServer()
    : m_agent(m_consul)
    , m_kv(m_consul)
    , m_timer(std::chrono::milliseconds(1), &co_sched) {    
}

GatePeerSession::ptr GateServer::getGatePeerSession(uint16_t type) {
    auto it = m_gpSessions.find(type);
    if (it != m_gpSessions.end()) {
        return it->second.begin()->second;
    }

    auto servers = m_agent.services();
    for (auto [id, si] : servers) {
        LOG(INFO) << "discoverServer id " << id << " name " << si.name;
        uint16_t st = std::atoi(si.name.data());
        if (st == type) {
            auto gps = std::make_shared<GatePeerSession>(si.address, si.port, *this);
            LOG(INFO) << "config self type " << this->type() << " rpc type " << st;
            gps->senderType(this->type());
            gps->receiverType(st);
            gps->run();
            m_gpSessions[st][id] = gps;
            return gps;
        }
    }
    return nullptr;
}


GatePeerSession::ptr GateServer::getPeerSession(int type) {
    std::unordered_map<int, GatePeerSession::ptr>::iterator it = m_peerSessions.find(type);
    return it != m_peerSessions.end() ? it->second : nullptr;
}

GateSession::ptr GateServer::getSession(uint64_t id) {
    std::unordered_map<uint64_t, GateSession::ptr>::iterator it = m_sessions.find(id);
    return it != m_sessions.end() ? it->second : nullptr;
}

void GateServer::run() {
    init();

    int accept_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(accept_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = inet_addr(m_ip.data());
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

            auto session = std::make_shared<GateSession>(m_id, fd, *this);
            m_sessions[session->id()] = session;
            session->run();
        }
    };

    consulServer();

    co_sched.Start(6);
}

void GateServer::dispatch(Message&& msg) {
    auto session_id = msg.sessionId();
    auto session = getSession(session_id);
    LOG(WARNING) << msg.strInfo();
    LOG(INFO) << "dispatche id " << session_id;
    if (!session) {
        LOG(ERROR) << "Invalid session " << session_id;
        return;
    }
    LOG(INFO) << "---dispatch id " << session_id << " ,msg id " << msg.msgId() << " ,role id " << msg.roleId()
                << " ,real msg id " << msg.realMsgId();
    if (msg.msgId() == 1001 || msg.msgId() == 1002) {
        if (msg.roleId() != 0) {
            m_sessionToRoleIds[session_id] = msg.roleId();
        }
    } else if (m_sessionToRoleIds.find(session_id) == m_sessionToRoleIds.end()) {
        LOG(ERROR) << "dispatch error, session id " << session_id;
    }

    session->push(std::move(msg));
}

void GateServer::init() {
    const std::string file = "./json/gate_server.json";
    boost::property_tree::ptree root;
    boost::property_tree::read_json(file, root);
    m_name = root.get<std::string>("name");
    m_type = root.get<uint16_t>("type");
    m_ip = root.get<std::string>("ip");
    m_port = root.get<uint16_t>("port");
    m_serverId = serverId(m_name, m_type, m_ip, m_port);

    // boost::property_tree::ptree servers = root.get_child("servers");
    // BOOST_FOREACH (boost::property_tree::ptree::value_type& node, servers) {
    //     auto name = node.second.get<std::string>("name");
    //     auto type = node.second.get<uint16_t>("type");
    //     auto ip = node.second.get<std::string>("ip");
    //     auto port = node.second.get<uint16_t>("port");

    //     auto num = node.second.get<int>("num");
    //     num = 1;
    //     for (int i = 0; i < num; ++i) {
    //         auto gps = std::make_shared<GatePeerSession>(ip, port, *this);
    //         LOG(INFO) << "config self type " << this->type() << " rpc type " << type;
    //         gps->senderType(this->type());
    //         gps->receiverType(type);
    //         gps->run();
    //         m_peerSessions[type] = gps;
    //     }
    // }
}

void GateServer::consulServer() {
    m_agent.registerService(std::to_string(m_type),
        ppconsul::agent::TcpCheck{m_ip, m_port, std::chrono::seconds(10), std::chrono::milliseconds(1)},
        ppconsul::agent::kw::deregisterCriticalServiceAfter = std::chrono::minutes(1),
        ppconsul::agent::kw::address = m_ip,
        ppconsul::agent::kw::port = m_port,
        ppconsul::agent::kw::id = m_serverId
    );

    m_timer.ExpireAt(std::chrono::seconds(10), [this, self = shared_from_this()] {
        discoverServer();
    });
}

void GateServer::discoverServer() {
    auto servers = m_agent.services();
    for (auto [id, si] : servers) {
        LOG(INFO) << "discoverServer id " << id << " name " << si.name;
        if (id == m_serverId) {
            continue;
        }
        uint16_t st = std::atoi(si.name.data());
        auto it = m_gpSessions.find(st);
        if (it != m_gpSessions.end()) {
            auto it1 = it->second.find(id);
            if (it1 != it->second.end()) {
                LOG(INFO) << "exist,  id " << si.id;
                continue;
            }
        }
        auto gps = std::make_shared<GatePeerSession>(si.address, si.port, *this);
        LOG(INFO) << "config self type " << type() << " rpc type " << st;
        gps->senderType(type());
        gps->receiverType(st);
        gps->run();
        m_gpSessions[st][si.id] = gps;
    }

    m_timer.ExpireAt(std::chrono::seconds(60), [this, self = shared_from_this()] {
        discoverServer();
    });
}

}