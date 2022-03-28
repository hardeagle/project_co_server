#ifndef __CORE_GATE_GATE_SERVER_H__
#define __CORE_GATE_GATE_SERVER_H__

#include <memory>
#include <unordered_map>

#include <boost/asio.hpp>

namespace Eayew {

class GateSession;

class GateServer {
public:
    using ptr = std::shared_ptr<GateServer>;

    void run(int port);

private:

    std::unordered_map<int, std::shared_ptr<GateSession> > m_sessions;
};

}

#endif