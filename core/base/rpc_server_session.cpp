#include "rpc_server_session.h"

#include <functional>
#include <string>

#include "log/glog.h"

#include "base_server.h"

namespace Eayew {

RpcServerSession::RpcServerSession(int fd)
    : m_fd(fd) {
}

void RpcServerSession::run() {
    go std::bind(&RpcServerSession::sync_read, shared_from_this());
}

void RpcServerSession::operator<<(std::string& buffer) {

}

void RpcServerSession::sync_read() {
    while (true) {
        const int head_len = 4;
        char head_buf[head_len];
        int rlen = read(m_fd, head_buf, head_len);
        if (rlen != head_len) {
            LOG(ERROR) << "Invalid head length";
            return;
        }
        int body_len = 0;
        char body_buf[2048];
        rlen = read(m_fd, body_buf, body_len);
        if (rlen != body_len) {
            LOG(ERROR) << "Invalid body length";
            return;
        }
        // parse
        std::string msg;
        m_baseServer->rpcDispatch(msg);
    }
}

void RpcServerSession::sync_write() {
    std::string buffer;
    write(m_fd, buffer.data(), buffer.size());
}

}