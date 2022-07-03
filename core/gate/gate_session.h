#ifndef __CORE_GATE_GATE_SESSION_H__
#define __CORE_GATE_GATE_SESSION_H__

#include <atomic>
#include <memory>

#include <libgo/libgo.h>

namespace Eayew {

class GateServer;
class Message;

class GateSession : public std::enable_shared_from_this<GateSession> {
public:
    using ptr = std::shared_ptr<GateSession>;

    GateSession(uint16_t server_id, int fd, GateServer& server);

    uint64_t id() const { return m_id; }

    void setGateServer(std::shared_ptr<GateServer> gate_server);

    void run();

    void operator<<(std::string& buffer);

private:
    void sync_read();
    void sync_write();

private:
    int m_fd;
    uint64_t m_id;

    GateServer& m_gateServer;

    co_chan<std::string> m_rChannel;

    std::shared_ptr<Message> m_rMessage;

};

}

#endif