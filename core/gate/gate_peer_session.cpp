#include "gate_peer_session.h"

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <libgo/libgo.h>

#include "log/glog.h"

#include "core/message.hpp"
#include "core/session.h"

#include "gate_server.h"

const static uint32_t s_limit = 40960;

namespace Eayew {

GatePeerSession::GatePeerSession(const std::string& ip, int port, GateServer& server)
    : m_ip(ip)
    , m_port(port)
    , m_gateServer(server)
    , m_rMsgs(s_limit)
    , m_wMsgs(s_limit) {
}

void GatePeerSession::run() {
    sync_connect();

    go [this, self = shared_from_this()] {
        sync_read();
    };

    go [this, self = shared_from_this()] {
        sync_write();
    };

    go [this, self = shared_from_this()] {
        dispatch();
    };
}

void GatePeerSession::push(Message&& msg) {
    if (m_wMsgs.size() == s_limit) {
        LOG(WARNING) << "gate peer session full";
    }

    m_wMsgs << std::move(msg);
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
        Message msg;
        auto head_len = Message::LEN_SIZE;
        if (!Eayew::eio(recv, m_fd, msg.wbuffer(), head_len, MSG_WAITALL)) {
            LOG(ERROR) << "eio fail, close or error ";
            return;
        }
        msg.commit(head_len);
        auto body_len = msg.length() - head_len;
        msg.prepare(body_len);
        if (!Eayew::eio(recv, m_fd, msg.wbuffer(), body_len, MSG_WAITALL)) {
            LOG(ERROR) << "eio fail, close or error ";
            return;
        }
        msg.commit(body_len);

        m_rMsgs << std::move(msg);
    }
}

void GatePeerSession::sync_write() {
    for (;;) {
        if (m_wMsgs.size() == 0) {
            LOG(WARNING) << "gate peer session empty";
        }

        Message msg;
        m_wMsgs >> msg;
        write(m_fd, msg.data(), msg.size());
    }
}

void GatePeerSession::dispatch() {
    for(;;) {
        Message msg;
        m_rMsgs >> msg;
        m_gateServer.dispatch(std::move(msg));
    }
}

}