#ifndef __CORE_GAME_RPC_SERVER_SESSION_H__
#define __CORE_GAME_RPC_SERVER_SESSION_H__

#include <memory>

#include <libgo/libgo.h>

namespace Eayew {

class BaseServer;

class RpcServerSession : public std::enable_shared_from_this<RpcServerSession> {
public:
    using ptr = std::shared_ptr<RpcServerSession>;

    RpcServerSession(int fd);

    void senderType(int v) { m_senderType = v; }
    int senderType() const { return m_senderType; }

    void receiverType(int v) { m_receiverType = v; }
    int receiverType() const { return m_receiverType; }

    void run();

    void operator<<(std::string& buffer);

private:
    void sync_read();
    void sync_write();

private:
    int m_senderType;
    int m_receiverType;

    int m_id;
    int m_fd;

    std::shared_ptr<BaseServer> m_baseServer;

    co_chan<std::string> m_rChannel;
};

}

#endif