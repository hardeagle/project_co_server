#ifndef __CORE_GATE_GATE_SERVER_H__
#define __CORE_GATE_GATE_SERVER_H__

#include <memory>
#include <unordered_map>

namespace Eayew {

class GateSession;
class GatePeerSession;

class GateServer {
public:
    using ptr = std::shared_ptr<GateServer>;

    int type() const { return m_type; }

    std::shared_ptr<GatePeerSession> getPeerSession(int type);

    std::shared_ptr<GateSession> getSession(int id);

    void run();

    void dispatch(std::string& buf);

private:
    void init();

private:
    int m_type;
    int m_id;
    int m_port;
    std::string m_ip;
    std::string m_name;

    std::unordered_map<int, std::shared_ptr<GateSession> > m_sessions;
    std::unordered_map<int, std::shared_ptr<GatePeerSession> > m_peerSessions;
};

}

#endif