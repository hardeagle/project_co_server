#include "rpc_session.h"

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <libgo/libgo.h>

#include "log/glog.h"

#include "core/message.hpp"

#include "rpc_manager.h"

namespace Eayew {

RpcSession::RpcSession(const std::string& ip, int port)
    : m_ip(ip)
    , m_port(port) {
}

void RpcSession::run() {
    sync_connect();

    go [this, self = shared_from_this()] {
        sync_read();
    };

    //go std::bind(&RpcSession::sync_read, shared_from_this());
}

void RpcSession::sync_write(std::string& buffer) {
    Message::ptr msg;
    msg->writeData(buffer);
    write(m_fd, msg->data(), msg->size());
}

void RpcSession::sync_write(uint32_t session_id, std::string& buf) {
    Message msg;
    msg.setSender(senderType());
    msg.setReceiver(receiverType());
    msg.setSessionId(session_id);
    msg.writeData(buf);
    write(m_fd, msg.data(), msg.size());
}

void RpcSession::sync_connect() {
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = inet_addr(m_ip.data());
    if (-1 == connect(m_fd, (sockaddr*)&addr, sizeof(addr))) {
        LOG(ERROR) << "connect fail, port " << m_port;
        return;
    }

    LOG(INFO) << "sync connect success, fd " << m_fd << " ip " << m_ip << " port " << m_port;

    Message msg;
    msg.setSender(senderType());
    msg.setReceiver(receiverType());
    msg.writeData("");
    LOG(INFO) << "msg " << msg.data() << " size " << msg.size();
    write(m_fd, msg.data(), msg.size());
}

void RpcSession::sync_read() {
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
        std::string body;
        m_rpcManager->dispatch(body);
    }
}

}