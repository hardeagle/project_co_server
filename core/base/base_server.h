#ifndef __CORE_BASE_BASE_SERVER_H__
#define __CORE_BASE_BASE_SERVER_H__

#include <memory>
#include <unordered_map>

#include <ppconsul/agent.h>
#include <ppconsul/consul.h>
#include <ppconsul/kv.h>

#include <co/all.h>

#include "core/message.h"

namespace Eayew {

class WorkRoutine;
class GateServerSession;
class RpcServerSession;
class RpcManager;
class ServletDispatchRange;
class Session;
class WorkRoutineManager;

class BaseServer : public std::enable_shared_from_this<BaseServer> {
public:
    BaseServer(uint16_t type);

    virtual ~BaseServer() {}

    uint16_t type() const { return m_type; }

    int id() const { return m_id; }

    uint16_t port() const { return m_port; }

    const std::string& ip() { return m_ip; }

    const std::string& name() { return m_name; }

    co::Sched* mainSched() { return m_mainSched; }

    co::Sched* workSched() { return m_workSched; }

    void run();

    void rpcDispatch(std::string& msg);

    void initByConfig(const std::string& file);

    virtual void beforeRun() {}

    std::shared_ptr<RpcManager> rpcManager() { return m_rpcManager; }

    std::shared_ptr<ServletDispatchRange> servlet() { return m_servlet; }

    std::shared_ptr<Session> getSession(uint32_t st);

private:
    void consulServer();

    void discoverServer();

private:
    uint16_t m_type;
    int m_id;
    uint16_t m_port;
    std::string m_ip;
    std::string m_name;

    int m_fd;

    std::string m_serverId;
    ppconsul::Consul m_consul;
    ppconsul::agent::Agent m_agent;
    ppconsul::kv::Kv m_kv;

    std::shared_ptr<RpcManager> m_rpcManager;

    std::shared_ptr<ServletDispatchRange> m_servlet;

    // std::unordered_map<int, std::shared_ptr<GateServerSession> > m_gateSessions;

    // std::unordered_map<int, std::shared_ptr<RpcServerSession> > m_rpcSessions;

    std::unordered_map<uint32_t, std::shared_ptr<Session> > m_sessions;

    std::shared_ptr<WorkRoutineManager> m_workRoutineMgr;

    co::Sched* m_mainSched;
    co::Sched* m_workSched;
};

}
#endif