#ifndef __CORE_GATE_GATE_SERVER_H__
#define __CORE_GATE_GATE_SERVER_H__

#include <memory>
#include <unordered_map>

#include "core/message.hpp"

namespace Eayew {

class GateSession;
class GatePeerSession;
class Message;

class GateServer {
public:
    using ptr = std::shared_ptr<GateServer>;

    int type() const { return m_type; }

    std::shared_ptr<GatePeerSession> getPeerSession(int type);

    std::shared_ptr<GateSession> getSession(int id);

    void run();

    void dispatch(Message&& msg);

private:
    void init();

private:
    uint16_t m_type;
    uint16_t m_id;
    uint16_t m_port;
    std::string m_ip;
    std::string m_name;

    std::unordered_map<int, uint64_t> m_sessionToRoleIds;
    std::unordered_map<int, std::shared_ptr<GateSession> > m_sessions;
    std::unordered_map<int, std::shared_ptr<GatePeerSession> > m_peerSessions;
};

}

#endif