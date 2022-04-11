#include "gate_session.h"

#include <functional>
#include <string>

#include "core/message.hpp"
#include "core/rpc/rpc_manager.h"

#include "log/glog.h"

#include "gate_peer_session.h"
#include "gate_server.h"

namespace Eayew {

std::atomic<int> GateSession::s_id = 1;

GateSession::GateSession(int fd, GateServer& server)
    : m_fd(fd)
    , m_gateServer(server) {
    m_id = ++s_id;
    m_rMessage = std::make_shared<Message>();
}

void GateSession::run() {
    go std::bind(&GateSession::sync_read, shared_from_this());
    go std::bind(&GateSession::sync_write, shared_from_this());
}

void GateSession::operator<<(std::string& buffer) {
    m_rChannel << buffer;
}

void GateSession::sync_read() {
    for(;;) {
        // const int head_len = 4;
        // char head_buf[head_len];
        // int rlen = read(m_fd, head_buf, head_len);
        // if (rlen != head_len) {
        //     LOG(ERROR) << "invalid head length";
        //     return;
        // }
        // int body_len = 0;
        // char body_buf[2048];
        // rlen = read(m_fd, body_buf, body_len);
        // if (rlen != body_len) {
        //     LOG(ERROR) << "invalid body length";
        //     return;
        // }

        m_rMessage.reset();
        const int head_len = 4;
        int rlen = read(m_fd, m_rMessage->wbuffer(), head_len);
        if (rlen != head_len) {
            LOG(ERROR) << "Invalid head length, rlen " << rlen;
            return;
        }
        int body_len = m_rMessage->length();
        rlen = read(m_fd, m_rMessage->wbuffer(), body_len);
        if (rlen != body_len) {
            LOG(ERROR) << "Invalid body length " << body_len << " real " << rlen;
            return;
        }

        uint16_t receiver_id = m_rMessage->receiverId();
        auto session = m_gateServer.getPeerSession(receiver_id);
        if (!session) {
            LOG(ERROR) << "Invalid receiver " << receiver_id;
            continue;
        }
        std::string buf = std::string(m_rMessage->data(), m_rMessage->size());
        session->sync_write(buf);
    }
}

void GateSession::sync_write() {
    for (;;) {
        std::string buffer;
        m_rChannel >> buffer;
        write(m_fd, buffer.data(), buffer.size());
    }
}

}