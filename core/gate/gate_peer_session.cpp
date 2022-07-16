#include "gate_peer_session.h"

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <libgo/libgo.h>

#include "log/glog.h"

#include "core/message.hpp"
#include "core/session.h"

#include "gate_server.h"

namespace Eayew {

GatePeerSession::GatePeerSession(const std::string& ip, int port, GateServer& server)
    : m_ip(ip)
    , m_port(port)
    , m_gateServer(server) {
    m_rMessage = std::make_shared<Message>();
}

void GatePeerSession::run() {
    sync_connect();

    go [this, self = shared_from_this()] {
        sync_read();
    };
}

void GatePeerSession::sync_write(Message::ptr msg) {
    // Message msg;
    // msg.setSender(senderType());
    // msg.setReceiver(receiverType());
    // msg.setSessionId(0);
    // msg.writeData(buffer);

    LOG(WARNING) << msg->strInfo();

    write(m_fd, msg->data(), msg->size());
}

void GatePeerSession::sync_connect() {
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
    LOG(INFO) << "sender type " << senderType() << " receiver type " << receiverType();
    msg.setSender(senderType());
    msg.setReceiver(receiverType());
    msg.writeData("");
    LOG(INFO) << "msg " << msg.data() << " size " << msg.size();
    write(m_fd, msg.data(), msg.size());
}

void GatePeerSession::sync_read() {
    for (;;) {
        m_rMessage->clear();
        auto head_len = Message::LEN_SIZE;
        if (!Eayew::eio(recv, m_fd, m_rMessage->wbuffer(), head_len, MSG_WAITALL)) {
            LOG(ERROR) << "eio fail, close or error ";
            return;
        }
        m_rMessage->commit(head_len);
        auto body_len = m_rMessage->length() - head_len;
        m_rMessage->prepare(body_len);
        if (!Eayew::eio(recv, m_fd, m_rMessage->wbuffer(), body_len, MSG_WAITALL)) {
            LOG(ERROR) << "eio fail, close or error ";
            return;
        }
        m_rMessage->commit(body_len);
        m_gateServer.dispatch(m_rMessage);
    }
}

}