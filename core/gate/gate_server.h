#ifndef __CORE_GATE_GATE_SERVER_H__
#define __CORE_GATE_GATE_SERVER_H__

#include <memory>
#include <unordered_map>

namespace Eayew {

class GateSession;
class RpcManager;

class GateServer {
public:
    using ptr = std::shared_ptr<GateServer>;

    void run();

    std::shared_ptr<RpcManager> rpcManager();

private:
    void init();

private:
    int m_type;
    int m_id;
    int m_port;
    std::string m_ip;
    std::string m_name;

    std::shared_ptr<RpcManager> m_rpcManager;

    std::unordered_map<int, std::shared_ptr<GateSession> > m_sessions;
};

}

#endif