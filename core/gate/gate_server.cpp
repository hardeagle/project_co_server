#include "gate_server.h"

#include <libgo/libgo.h>

#include "log/glog.h"

#include "gate_session.h"

namespace Eayew {

void GateServer::run(int port) {
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
    if (-1 == listen(accept_fd 5)) {
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
            auto session = std::make_shared<GateSession>();
            m_sessions[fd] = session;
            session->run();
        }
    };
}

}