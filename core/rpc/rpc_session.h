#ifndef __CORE_RPC_RPC_SESSION_H__
#define __CORE_RPC_RPC_SESSION_H__

#include <memory>

#include <libgo/libgo.h>

namespace Eayew {

class Message;
class RpcManager;

class RpcSession : public std::enable_shared_from_this<RpcSession> {
public:
    using ptr = std::shared_ptr<RpcSession>;

    RpcSession(const std::string& ip, int port);

    void senderType(int v) { m_senderType = v; }
    int senderType() const { return m_senderType; }

    void receiverType(int v) { m_receiverType = v; }
    int receiverType() const { return m_receiverType; }

    int fd() const { return m_fd; }

    void run();

    void sync_write(std::string& buf);

    void sync_write(uint32_t sender_id, uint32_t received_id, uint32_t role_id,
                    uint32_t session_id, std::string& buf);

private:
    void sync_connect();

    void sync_read();

private:
    int m_senderType;
    int m_receiverType;

    std::string m_ip;
    int m_port;

    int m_id;
    int m_fd;

    std::shared_ptr<Message> m_message;

    std::shared_ptr<RpcManager> m_rpcManager;
};

}

#endif