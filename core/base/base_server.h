#ifndef __CORE_BASE_BASE_SERVER_H__
#define __CORE_BASE_BASE_SERVER_H__

#include <memory>
#include <unordered_map>

namespace Eayew {

class BaseRoutine;
class GateServerSession;
class RpcServerSession;
class RpcManager;
class ServletDispatchRange;

class BaseServer {
public:
    BaseServer();

    virtual ~BaseServer() {}

    int type() const { return m_type; }

    int id() const { return m_id; }

    uint16_t port() const { return m_port; }

    const std::string& ip() { return m_ip; }

    const std::string& name() { return m_name; }

    void run();

    void gateDispatch(std::string&& msg);

    void rpcDispatch(std::string& msg);

    void initByConfig(const std::string& file);

    virtual void beforeRun() {}

    virtual void regAndDiscServer() {}

    std::shared_ptr<RpcManager> rpcManager() { return m_rpcManager; }

    std::shared_ptr<ServletDispatchRange> servlet() { return m_servlet; }

private:
    int m_type;
    int m_id;
    uint16_t m_port;
    std::string m_ip;
    std::string m_name;

    int m_fd;

    std::shared_ptr<RpcManager> m_rpcManager;

    std::shared_ptr<ServletDispatchRange> m_servlet;

    std::unordered_map<int, std::shared_ptr<GateServerSession> > m_gateSessions;

    std::unordered_map<int, std::shared_ptr<RpcServerSession> > m_rpcSessions;

    std::unordered_map<int, std::shared_ptr<BaseRoutine> > m_baseRoutines;
};

}
#endif