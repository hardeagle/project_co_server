#ifndef __CORE_RPC_RPC_SESSION_H__
#define __CORE_RPC_RPC_SESSION_H__

#include <memory>

#include <libgo/libgo.h>

namespace Eayew {

class RpcManager;

class RpcSession {
public:
    using ptr = std::shared_ptr<RpcSession>;

    void run();

    void sync_write(std::string& buf);

    void sync_write(int id, std::string& buf);

private:
    void sync_read();

private:
    int m_id;
    int m_fd;

    std::shared_ptr<RpcManager> m_rpcManager;
};

}

#endif