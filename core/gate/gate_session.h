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

    GateSession(int fd, GateServer& server);

    int id() const { return m_id; }

    void setGateServer(std::shared_ptr<GateServer> gate_server);

    void run();

    void operator<<(std::string& buffer);

private:
    void sync_read();
    void sync_write();

private:
    int m_id;
    int m_fd;

    GateServer& m_gateServer;

    co_chan<std::string> m_rChannel;

    std::shared_ptr<Message> m_rMessage;

    static std::atomic<int> s_id;
};

}

#endif