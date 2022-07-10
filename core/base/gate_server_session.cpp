#include "gate_server_session.h"

#include <functional>
#include <string>

#include <libgo/libgo.h>

#include "core/message.hpp"

#include "log/glog.h"

#include "base_server.h"

namespace Eayew {

GateServerSession::GateServerSession(int fd, BaseServer& server)
    : m_fd(fd)
    , m_baseServer(server) {
}

void GateServerSession::run() {
    go [this, self = shared_from_this()] {
        sync_read();
    };
}

void GateServerSession::sync_read() {
    for (;;) {
        auto head_len = Message::LEN_SIZE;
        Message msg;
        auto rlen = read(m_fd, msg.wbuffer(), head_len);
        if (rlen != head_len) {
            LOG(ERROR) << "Invalid head length, rlen " << rlen;
            return;
        }
        msg.commit(head_len);
        int body_len = msg.length() - head_len;
        msg.prepare(body_len);
        rlen = read(m_fd, msg.wbuffer(), body_len);
        if (rlen != body_len) {
            LOG(ERROR) << "Invalid body length " << body_len << " real " << rlen;
            return;
        }
        msg.commit(body_len);

        LOG(WARNING) << msg.strInfo();

        m_baseServer.gateDispatch(std::move(msg));

        // {
        //     LOG(WARNING) << msg.strInfo();
        //     std::string res(msg.data(), msg.size());
        //     write(m_fd, res.data(), res.size());
        // }
    }
}

void GateServerSession::sync_write() {
    std::string buffer;
    write(m_fd, buffer.data(), buffer.size());
}

void GateServerSession::send(Message&& msg) {
    write(m_fd, msg.data(), msg.size());
}

}