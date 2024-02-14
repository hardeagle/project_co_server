#ifndef __CORE_GATE_GATE_PEER_SESSION_H__
#define __CORE_GATE_GATE_PEER_SESSION_H__

#include <memory>

#include "core/message.h"
#include "core/session.h"

namespace Eayew {

class Message;

class GatePeerSession : public Session {
public:
    using ptr = std::shared_ptr<GatePeerSession>;

    GatePeerSession();

    uint16_t sender() { return m_sender; }
    uint16_t receiver() { return m_receiver; }

    bool sync_connect(const std::string& ip, uint16_t port, uint16_t sender, uint16_t receiver);

private:
    uint16_t m_sender;
    uint16_t m_receiver;
};

}

#endif