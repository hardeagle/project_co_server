#include "gate_session.h"

#include<sys/types.h>
#include<sys/socket.h>

#include <functional>
#include <string>

#include "core/message.hpp"
#include "core/rpc/rpc_manager.h"
#include "core/session.h"
#include "core/util/util.h"

#include "log/glog.h"

#include "gate_peer_session.h"
#include "gate_server.h"

const static uint32_t s_limit = 2048;

namespace Eayew {

GateSession::GateSession(uint16_t server_id, int fd, GateServer& server)
    : m_fd(fd)
    , m_rMsgs(s_limit)
    , m_wMsgs(s_limit)
    , m_gateServer(server) {
    m_id = (uint64_t(server_id) << 48) + (uint64_t(getCurSecond()) << 16) + (fd & 0xFFFF);
    LOG(INFO) << "Construct GateSession, id " << m_id;
}

void GateSession::run() {
    go std::bind(&GateSession::sync_read, shared_from_this());
    go std::bind(&GateSession::sync_write, shared_from_this());
    go std::bind(&GateSession::dispatch, shared_from_this());
}

void GateSession::push(Message&& msg) {
    if (m_wMsgs.size() == s_limit) {
        LOG(WARNING) << "gate session full";
    }
    m_wMsgs << std::move(msg);
}

void GateSession::sync_read() {
    for(;;) {
        Message msg;
        auto head_len = Message::LEN_SIZE;
        if (!eio(recv, m_fd, msg.wbuffer(), head_len, MSG_WAITALL)) {
            LOG(ERROR) << "eio fail, close or error ";
            return;
        }
        msg.commit(head_len);
        uint16_t body_len = msg.length() - head_len;
        msg.prepare(body_len);
        if (!eio(recv, m_fd, msg.wbuffer(), body_len, MSG_WAITALL)) {
            LOG(ERROR) << "eio fail, close or error ";
            return;
        }
        msg.commit(body_len);
        msg.forceSetSessionId(id());

        m_rMsgs << std::move(msg);
    }
}

void GateSession::sync_write() {
    for (;;) {
        if (m_wMsgs.size() == 0) {
            LOG(WARNING) << "gate session empty";
        }
        Message msg;
        m_wMsgs >> msg;
        write(m_fd, msg.data(), msg.size());
    }
}

void GateSession::dispatch() {
    for (;;) {
        Message msg;
        m_rMsgs >> msg;
        uint16_t receiver_id = msg.receiverId();
        auto session = m_gateServer.getGatePeerSession(receiver_id);
        if (!session) {
            LOG(ERROR) << "Invalid receiver " << receiver_id;
            continue;
        }
        session->push(std::move(msg));
    }
}

}