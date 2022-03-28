#ifndef __CORE_GATE_SESSION_H__
#define __CORE_GATE_SESSION_H__

#include <memory>

#include <libgo/libgo.h>

namespace Eayew {

class GateSession : public std::enable_shared_from_this<GateSession> {
public:
    using ptr = std::shared_ptr<GateSession>;,

    void run();

    void operator<<(std::string& buffer);

private:
    void sync_read();
    void sync_write();

private:
    int m_id;
    int m_fd;

    co_chan<std::string> m_rChannel;
};

}

#endif