#ifndef __CORE_SESSION_HPP__
#define __CORE_SESSION_HPP__

#include <sys/socket.h>
#include <arpa/inet.h>

#include <functional>

#include <google/protobuf/message.h>

#include <co/all.h>

#include "core/message.h"

namespace Eayew {

template <typename OriginF, typename... Args>
bool eio(OriginF fn, int __fd, char* __buf, size_t n_bytes, Args&&... args) {
    for (;;) {
        auto len = fn(__fd, __buf, n_bytes, std::forward<Args>(args)...);
        if (len == n_bytes) {
            return true;
        } else if (0 == len) {
            ELOG << "close";
            return false;
        } else if (-1 == len) {
            if (errno == EINTR || errno==EAGAIN) {
                WLOG << "errno " << errno;
                continue;
            } else {
                ELOG << "errno " << errno;
                return false;
            }
        } else {
            WLOG << "not enough, len " << len << " n_bytes " << n_bytes;
        }
        n_bytes -= len;
        __buf += len;
    }
    return true;
}


class Session : public std::enable_shared_from_this<Session> {
public:
    using ptr = std::shared_ptr<Session>;

    Session(uint32_t fd = -1);

    int64_t id() { return m_id; }

    uint16_t sender() { return m_sender; }
    void sender(uint16_t v) { m_sender = v; }

    uint16_t receiver() { return m_receiver; }
    void receiver(uint16_t v) { m_receiver = v; }

    void setrSched(co::Sched* s) { m_rSched = s; }
    void setwSched(co::Sched* s) { m_wSched = s; }

    void setOnMessage(std::function<void(Message::ptr)>);
    void setOnClose(std::function<void(uint64_t)>);

    bool sync_connect(const std::string& ip, uint16_t port);

    bool closeMsg(uint16_t msgid, uint64_t sessionid);

    virtual bool start(bool accept = true) { return true; }
    virtual void run();

    virtual void send(Message::ptr msg);

protected:

private:
    void sync_read();
    void sync_write();

protected:
    std::function<void(Message::ptr)> m_onMessageCB;
    std::function<void(uint64_t)> m_onCloseCB;

    uint64_t m_id;

    std::string m_ip;
    uint16_t m_port;
    uint32_t m_fd;

    uint16_t m_sender;
    uint16_t m_receiver;

    co::chan<Message::ptr> m_rMsgs;
    co::chan<Message::ptr> m_wMsgs;

    co::Sched* m_rSched;
    co::Sched* m_wSched;
};

}


#endif