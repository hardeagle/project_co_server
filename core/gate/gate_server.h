#ifndef __CORE_GATE_GATE_SERVER_H__
#define __CORE_GATE_GATE_SERVER_H__

#include <memory>
#include <unordered_map>

#include <libgo/libgo.h>

#include <ppconsul/agent.h>
#include <ppconsul/consul.h>
#include <ppconsul/kv.h>

#include "core/message.h"

namespace Eayew {

class GateSession;
class GatePeerSession;
class Message;

class GateServer : public std::enable_shared_from_this<GateServer> {
public:
    using ptr = std::shared_ptr<GateServer>;

    GateServer();

    int type() const { return m_type; }

    std::shared_ptr<GatePeerSession> getGatePeerSession(uint16_t type);

    std::shared_ptr<GateSession> getSession(uint64_t id);

    void run();

private:
    void init();

    void consulServer();

    void discoverServer();


private:
    uint16_t m_type;
    uint16_t m_id;
    uint16_t m_port;
    int16_t m_wsPort;
    std::string m_ip;
    std::string m_name;

    std::string m_serverId;
    ppconsul::Consul m_consul;
    ppconsul::agent::Agent m_agent;
    ppconsul::kv::Kv m_kv;

    co_timer m_timer;

    std::unordered_map<uint64_t, uint64_t> m_sessionIdToRoleIds;
    std::unordered_map<uint64_t, std::shared_ptr<GateSession> > m_sessions;

    std::unordered_map<int, std::unordered_map<std::string, std::shared_ptr<GatePeerSession>>> m_gpSessions;
};

}

#endif