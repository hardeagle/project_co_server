#include "base_server.h"

#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <libgo/libgo.h>

#include "core/rpc/rpc_manager.h"

#include "log/glog.h"

#include "base_routine.h"
#include "rpc_server_session.h"

namespace Eayew {

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

    int port = 9013;

    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t len = sizeof(addr);
    if (-1 == bind(m_fd, (sockaddr*)&addr, len)) {
        LOG(ERROR) << "bind fail, port " << port;
        return;
    }
    if (-1 == listen(m_fd, 5)) {
        LOG(ERROR) << "listen fail";
    }

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
            const int len = 24;
            char buf[len];
            int rlen = read(fd, buf, len);
            if (rlen != len) {
                LOG(ERROR) << "Invalid rpc connect";
                continue;
            }
            int sender_type;
            int receiver_type;
            if (receiver_type != type()) {
                LOG(ERROR) << "Invalid server type " << receiver_type << " type " << type();
                continue;
            }
            auto ss = std::make_shared<RpcServerSession>(fd);
            ss->senderType(sender_type);
            ss->receiverType(receiver_type);
            m_rpcServerSessions[sender_type] = ss;
            ss->run();
        }
    };

    co_sched.Start();
}

void BaseServer::dispatch(std::string& msg) {
    int id;
    std::string buffer;
    std::unordered_map<int, BaseRoutine::ptr>::iterator it = m_baseRoutines.find(id);
    if (it == m_baseRoutines.end()) {
        m_baseRoutines[id] = std::make_shared<BaseRoutine>();
        m_baseRoutines[id]->run();
    }
    *(m_baseRoutines[id]) << buffer;
}

void BaseServer::initRpc(const std::string& file) {
    boost::property_tree::ptree root;
    boost::property_tree::read_json(file, root);
    m_name = root.get<std::string>("name");
    m_type = root.get<int>("type");
    m_ip = root.get<std::string>("ip");
    m_port = root.get<int>("port");

    m_rpcManager->init(file);
}

}