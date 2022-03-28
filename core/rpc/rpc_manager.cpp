#include "rpc_manager.h"

#include <libgo/libgo.h>

#include "log/glog.h"

#include "rpc_session.h"

namespace Eayew {

RpcSession::ptr RpcManager::getClient() {
    return RpcSession::ptr();
}

int RpcManager::nextRpcId() {
    if (++m_rpcId >= 0xFFFFFFF) {
        m_rpcId = 1;
    }
    return m_rpcId;
}

void RpcManager::init() {

}

void RpcManager::call(std::string& req) {
    RpcSession::ptr rs = getClient();
    if (!rs) {
        LOG(ERROR) << "getClient fail";
        return;
    }
    rs->sync_write(req);
}

void RpcManager::call(std::string& req, std::string& rsp) {
    int rpc_id = nextRpcId();
    {
        RpcSession::ptr rs = getClient();
        if (!rs) {
            LOG(ERROR) << "getClient fail";
            return;
        }

        rs->sync_write(rpc_id, req);
    }

    co_chan<std::string> channel;
    m_channels[rpc_id] = channel;
    channel >> rsp;
    //m_channels.erase(rpc_id);
}

void RpcManager::dispatch(std::string& buf) {
    // parse buf
    int rpc_id = 0;
    std::string body;
    m_channels[rpc_id] << body;
}

}