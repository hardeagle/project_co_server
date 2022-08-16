#ifndef __CORE_BASE_GATE_SERVER_SESSION_H__
#define __CORE_BASE_GATE_SERVER_SESSION_H__

#include <memory>

#include <libgo/libgo.h>

#include "core/message.hpp"
#include "core/session.h"

namespace Eayew {

class  BaseServer;

class GateServerSession : public std::enable_shared_from_this<GateServerSession>, public Session {
public:
    using ptr = std::shared_ptr<GateServerSession>;

    GateServerSession(int fd);

    void senderType(int v) { m_senderType = v; }
    int senderType() const { return m_senderType; }

    void receiverType(int v) { m_receiverType = v; }
    int receiverType() const { return m_receiverType; }

    void run();

    void sync_read();
    void sync_write();

    virtual void send(Message&& msg) override;

private:
    int m_senderType;
    int m_receiverType;

    int m_id;
    int m_fd;

    co_chan<Message> m_wMsgs;
};

}

#endif