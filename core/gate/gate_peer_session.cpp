#include "gate_peer_session.h"

#include "log/glog.h"

namespace Eayew {

GatePeerSession::GatePeerSession()
    : Session() {
}

bool GatePeerSession::sync_connect(const std::string& ip, uint16_t port, uint16_t sender, uint16_t receiver) {
    if (!Session::sync_connect(ip, port)) {
        LOG(ERROR) << "sync_connect fail";
        return false;
    }
    m_sender = sender;
    m_receiver = receiver;

    Message msg(0);
    msg.senderId(sender);
    msg.receiverId(receiver);
    write(m_fd, msg.data(), msg.size());
    return true;
}

}