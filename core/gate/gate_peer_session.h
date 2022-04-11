#ifndef __CORE_GATE_GATE_PEER_SESSION_H__
#define __CORE_GATE_GATE_PEER_SESSION_H__

#include <memory>

#include <libgo/libgo.h>

namespace Eayew {

class GateServer;

class GatePeerSession : public std::enable_shared_from_this<GatePeerSession> {
public:
    using ptr = std::shared_ptr<GatePeerSession>;

    GatePeerSession(const std::string& ip, int port, GateServer& server);

    int fd() const { return m_fd; }

    int senderType() const { return m_senderType; }
    void senderType(int v) { m_senderType = v; }

    int receiverType() const { return m_receiverType; }
    void receiverType(int v) { m_receiverType = v; }

    void run();

    void sync_write(std::string& buffer);

private:
    void sync_connect();

    void sync_read();

private:
    int m_fd;
    int m_senderType;
    int m_receiverType;

    std::string m_ip;
    int m_port;

    GateServer& m_gateServer;
};

}

#endif