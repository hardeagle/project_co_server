#ifndef __CORE_RPC_RPC_MANAGER_H__
#define __CORE_RPC_RPC_MANAGER_H__

#include <atomic>
#include <memory>
#include <map>
#include <string>
#include <unordered_map>

#include <co/all.h>

namespace Eayew {

class RpcSession;

class RpcManager {
public:
    using ptr = std::shared_ptr<RpcManager>;

    RpcManager(int type);

    int type() const { return m_type; }

    void init(const std::string& file);

    void call(int type, std::string& req);

    void call(int type, std::string& req, std::string& rsp);

    void dispatch(std::string& buf);

private:
    std::shared_ptr<RpcSession> getClient(int type);

    int nextRpcId();

private:
    int m_type;

    std::atomic<int> m_rpcId;

    std::unordered_map<int, std::shared_ptr<RpcSession> > m_rpcSessions;

    std::map<int, co::chan<std::string> > m_channels;
};

}

#endif