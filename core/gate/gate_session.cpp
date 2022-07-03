#include "gate_session.h"

#include <functional>
#include <string>

#include "core/message.hpp"
#include "core/rpc/rpc_manager.h"
#include "core/util/util.h"

#include "log/glog.h"

#include "gate_peer_session.h"
#include "gate_server.h"

namespace Eayew {


GateSession::GateSession(uint16_t server_id, int fd, GateServer& server)
    : m_fd(fd)
    , m_gateServer(server) {
    m_id = (uint64_t(server_id) << 48) + (uint64_t(getCurSecond()) << 16) + (fd & 0xFFFF);
    m_rMessage = std::make_shared<Message>();
    LOG(INFO) << "Construct GateSession, id " << m_id;
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
        m_rMessage->clear();
        auto head_len = Message::LEN_SIZE;
        auto rlen = read(m_fd, m_rMessage->wbuffer(), head_len);
        if (rlen != head_len) {
            LOG(ERROR) << "Invalid head length, rlen " << rlen;
            return;
        }
        m_rMessage->commit(head_len);
        uint16_t body_len = m_rMessage->length() - head_len;
        m_rMessage->prepare(body_len);
        rlen = read(m_fd, m_rMessage->wbuffer(), body_len);
        if (rlen != body_len) {
            LOG(ERROR) << "Invalid body length " << body_len << " real " << rlen;
            return;
        }
        m_rMessage->commit(body_len);
        m_rMessage->forceSetSessionId(id());

        uint16_t receiver_id = m_rMessage->receiverId();
        auto session = m_gateServer.getPeerSession(receiver_id);
        if (!session) {
            LOG(ERROR) << "Invalid receiver " << receiver_id;
            continue;
        }
        session->sync_write(m_rMessage);
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