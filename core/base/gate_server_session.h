#ifndef __CORE_BASE_GATE_SERVER_SESSION_H__
#define __CORE_BASE_GATE_SERVER_SESSION_H__

#include <memory>

namespace Eayew {

class  BaseServer;

class GateServerSession : public std::enable_shared_from_this<GateServerSession> {
public:
    using ptr = std::shared_ptr<GateServerSession>;

    GateServerSession(int fd);

    void senderType(int v) { m_senderType = v; }
    int senderType() const { return m_senderType; }

    void receiverType(int v) { m_receiverType = v; }
    int receiverType() const { return m_receiverType; }

    void run();

    void setBaseServer(std::shared_ptr<BaseServer> base_server);

    void sync_read();
    void sync_write();

private:
    int m_senderType;
    int m_receiverType;

    int m_id;
    int m_fd;

    std::shared_ptr<BaseServer> m_baseServer;
};

}

#endif