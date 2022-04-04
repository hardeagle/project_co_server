#include "gate_server.h"

#include <sys/socket.h>
#include <arpa/inet.h>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <libgo/libgo.h>

#include "core/rpc/rpc_manager.h"

#include "log/glog.h"

#include "gate_session.h"

namespace Eayew {

void GateServer::run() {
    init();

    int port;
    int accept_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t len = sizeof(addr);
    if (-1 == bind(accept_fd, (sockaddr*)&addr, len)) {
        LOG(ERROR) << "bind error, port " << port;
        return;
    }
    if (-1 == listen(accept_fd, 5)) {
        LOG(ERROR) << "listen error";
        return;
    }

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
            auto session = std::make_shared<GateSession>(fd);
            m_sessions[fd] = session;
            session->run();
        }
    };

    co_sched.Start();
}

RpcManager::ptr GateServer::rpcManager() {
    return m_rpcManager;
}

void GateServer::init() {
    const std::string file = "./json/gate_server.json";
    boost::property_tree::ptree root;
    boost::property_tree::read_json(file, root);
    m_name = root.get<std::string>("name");
    m_type = root.get<int>("type");
    m_ip = root.get<std::string>("ip");
    m_port = root.get<int>("port");

    m_rpcManager->init(file);
}

}