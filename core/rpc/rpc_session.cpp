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
    m_message = std::make_shared<Message>();
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

void RpcSession::sync_write(uint32_t sender_id, uint32_t received_id, uint32_t role_id,
                uint32_t session_id, std::string& buf) {
    Message::ptr msg;
    msg->setSender(sender_id);
    msg->setReceiver(received_id);
    msg->setRoleId(role_id);
    msg->setSessionId(session_id);
    msg->writeData(buf);
    write(m_fd, msg->data(), msg->size());
}

void RpcSession::sync_connect() {
    int port = 9030;
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (-1 == connect(m_fd, (sockaddr*)&addr, sizeof(addr))) {
        LOG(ERROR) << "connect fail, port " << port;
        return;
    }
    m_message->setSender(senderType());
    m_message->setReceiver(receiverType());
    m_message->writeData("");
    write(m_fd, m_message->data(), m_message->size());
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