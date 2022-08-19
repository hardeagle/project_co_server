#ifndef __CORE_BASE_BASE_SERVER_H__
#define __CORE_BASE_BASE_SERVER_H__

#include <memory>
#include <unordered_map>

#include <libgo/libgo.h>

#include <ppconsul/agent.h>
#include <ppconsul/consul.h>
#include <ppconsul/kv.h>

#include "core/message.hpp"

namespace Eayew {

class WorkRoutine;
class GateServerSession;
class RpcServerSession;
class RpcManager;
class ServletDispatchRange;
class WorkRoutineManager;

class BaseServer : public std::enable_shared_from_this<BaseServer> {
public:
    BaseServer();

    virtual ~BaseServer() {}

    int type() const { return m_type; }

    int id() const { return m_id; }

    uint16_t port() const { return m_port; }

    const std::string& ip() { return m_ip; }

    const std::string& name() { return m_name; }

    void run();

    void rpcDispatch(std::string& msg);

    void initByConfig(const std::string& file);

    virtual void beforeRun() {}

    std::shared_ptr<RpcManager> rpcManager() { return m_rpcManager; }

    std::shared_ptr<ServletDispatchRange> servlet() { return m_servlet; }

private:
    void consulServer();

    void discoverServer();

private:
    int m_type;
    int m_id;
    uint16_t m_port;
    std::string m_ip;
    std::string m_name;

    int m_fd;

    std::string m_serverId;
    ppconsul::Consul m_consul;
    ppconsul::agent::Agent m_agent;
    ppconsul::kv::Kv m_kv;

    co_timer m_timer;

    std::shared_ptr<RpcManager> m_rpcManager;

    std::shared_ptr<ServletDispatchRange> m_servlet;

    std::unordered_map<int, std::shared_ptr<GateServerSession> > m_gateSessions;

    std::unordered_map<int, std::shared_ptr<RpcServerSession> > m_rpcSessions;

    std::shared_ptr<WorkRoutineManager> m_workRoutineMgr;
};

}
#endif