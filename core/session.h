#ifndef __CORE_SESSION_HPP__
#define __CORE_SESSION_HPP__

#include <sys/socket.h>
#include <arpa/inet.h>

#include <functional>

#include <google/protobuf/message.h>

#include "log/glog.h"

#include "core/message.hpp"

namespace Eayew {

template <typename OriginF, typename... Args>
bool eio(OriginF fn, int __fd, char* __buf, size_t n_bytes, Args&&... args) {
    for (;;) {
        auto len = fn(__fd, __buf, n_bytes, std::forward<Args>(args)...);
        LOG(WARNING) << "len " << len << " ,n_tytes " << n_bytes;
        if (len == n_bytes) {
            return true;
        } else if (0 == len) {
            LOG(ERROR) << "close";
            return false;
        } else if (-1 == len) {
            if (errno == EINTR || errno==EAGAIN) {
                LOG(WARNING) << "errno " << errno;
                continue;
            } else {
                LOG(ERROR) << "errno " << errno;
                return false;
            }
        } else {
            LOG(WARNING) << "not enough, len " << len << " n_bytes " << n_bytes;
        }
        n_bytes -= len;
        __buf += len;
    }
    return true;
}


class Session {
public:
    using ptr = std::shared_ptr<Session>;

    virtual void send(Message&& msg) {}
    virtual void send(Message&& msg, const google::protobuf::Message& gpm) {}


    void setOnMessage(std::function<void(Message&& msg)>);
    void setOnClose(std::function<void()>);

protected:
    std::function<void(Message&& msg)> m_onMessageCB;
    std::function<void()> m_onCloseCB;
};

}


#endif