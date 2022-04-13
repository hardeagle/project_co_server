#include "base_server.h"

#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <libgo/libgo.h>

#include "core/servlet.h"
#include "core/rpc/rpc_manager.h"

#include "log/glog.h"

#include "base_routine.h"
#include "gate_server_session.h"
#include "rpc_server_session.h"

namespace Eayew {

BaseServer::BaseServer() {
    m_servlet = std::make_shared<ServletDispatchRange>();
}

void BaseServer::run() {

    // std::thread t([] { 
    //     co_sched.Start();
    // });
    // t.detach();

    // go [&] {
    //     while (true) {
    //         int id;
    //         std::string buffer;
    //         std::unordered_map<int, GameSession::ptr>::iterator it = m_sessions.find(id);
    //         if (it == m_sessions.end()) {
    //             m_sessions[id] = std::make_shared<
    //             m_sessions[id]->run();
    //         }
    //         *(m_sessions[id]) << buffer;
    //     }
    // };

    beforeRun();

    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = inet_addr(m_ip.data());
    socklen_t len = sizeof(addr);
    if (-1 == bind(m_fd, (sockaddr*)&addr, len)) {
        LOG(ERROR) << "bind fail, port " << m_port;
        return;
    }
    if (-1 == listen(m_fd, 5)) {
        LOG(ERROR) << "listen fail";
        return;
    }
    int opt = 1;
    setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    LOG(INFO) << "listen success, type " << m_type << " ip " << m_ip << " port " << m_port;

    go [&] {
        for(;;) {
            int fd = accept(m_fd, (sockaddr*)&addr, &len);
            if (-1 == fd) {
                if (EAGAIN == errno || EINTR == errno) {
                    continue;
                }
                LOG(ERROR) << "accept error";
                return;
            }

            const int len = 20;
            char buf[len];
            int rlen = read(fd, buf, len);
            if (rlen != len) {
                LOG(ERROR) << "Invalid rpc connect, rlen " << rlen;
                continue;
            }
            int body_size = *((int*)buf);
            LOG(INFO) << "rlen " << rlen;
            LOG(INFO) << "size " << body_size;
            uint16_t sender_type = *((uint16_t*)&buf[4]);
            uint16_t receiver_type = *((uint16_t*)&buf[6]);
            if (receiver_type != type()) {
                LOG(ERROR) << "Invalid server type " << receiver_type << " type " << type();
                continue;                                                                             
            }

            LOG(INFO) << "accept successs, fd " << fd << " sender type " << sender_type << " receiver type " << receiver_type << " buf " << buf;

            if (1 == sender_type) {
                auto ss = std::make_shared<GateServerSession>(fd);
                ss->senderType(sender_type);
                ss->receiverType(receiver_type);
                m_gateSessions[sender_type] = ss;
                ss->run();
            } else {
                auto ss = std::make_shared<RpcServerSession>(fd);
                ss->senderType(sender_type);
                ss->receiverType(receiver_type);
                m_rpcSessions[sender_type] = ss;
                ss->run();
            }

        }
    };

    co_sched.Start();
}

void BaseServer::gateDispatch(std::string& msg) {
    LOG(INFO) << "gateDispatch size " << msg.size();
}

void BaseServer::rpcDispatch(std::string& msg) {
    int id;
    std::string buffer;
    std::unordered_map<int, BaseRoutine::ptr>::iterator it = m_baseRoutines.find(id);
    if (it == m_baseRoutines.end()) {
        m_baseRoutines[id] = std::make_shared<BaseRoutine>(*this);
        m_baseRoutines[id]->run();
    }
    *(m_baseRoutines[id]) << buffer;
}

void BaseServer::initByConfig(const std::string& file) {
    boost::property_tree::ptree root;
    boost::property_tree::read_json(file, root);
    m_name = root.get<std::string>("name");
    m_type = root.get<int>("type");
    m_ip = root.get<std::string>("ip");
    m_port = root.get<int>("port");

    m_rpcManager = std::make_shared<RpcManager>(m_type);
    m_rpcManager->init(file);
}

}