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

#include "gate_session.h"
#include "gate_peer_session.h"

namespace Eayew {

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
    m_type = root.get<int>("type");
    m_ip = root.get<std::string>("ip");
    m_port = root.get<int>("port");

    boost::property_tree::ptree servers = root.get_child("servers");
    BOOST_FOREACH (boost::property_tree::ptree::value_type& node, servers) {
        auto name = node.second.get<std::string>("name");
        auto type = node.second.get<int>("type");
        auto ip = node.second.get<std::string>("ip");
        auto port = node.second.get<int>("port");
        auto num = node.second.get<int>("num");
        num = 1;
        for (int i = 0; i < num; ++i) {
            auto gps = std::make_shared<GatePeerSession>(ip, port, *this);
            LOG(INFO) << "config self type " << this->type() << " rpc type " << type;
            gps->senderType(this->type());
            gps->receiverType(type);
            gps->run();
            m_peerSessions[type] = gps;
        }
    }
}

}