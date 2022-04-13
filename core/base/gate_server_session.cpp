#include "gate_server_session.h"

#include <functional>
#include <string>

#include <libgo/libgo.h>

#include "core/message.hpp"

#include "log/glog.h"

#include "base_server.h"

namespace Eayew {

GateServerSession::GateServerSession(int fd)
    : m_fd(fd) {
}

void GateServerSession::run() {
    go [this, self = shared_from_this()] {
        sync_read();
    };
}

void GateServerSession::sync_read() {
    for (;;) {
        const int head_len = 4;
        Message msg;
        int rlen = read(m_fd, msg.wbuffer(), head_len);
        if (rlen != head_len) {
            LOG(ERROR) << "Invalid head length, rlen " << rlen;
            return;
        }
        msg.commit(head_len);
        int body_len = msg.length() - head_len;
        rlen = read(m_fd, msg.wbuffer(), body_len);
        if (rlen != body_len) {
            LOG(ERROR) << "Invalid body length " << body_len << " real " << rlen;
            return;
        }
        msg.commit(body_len);
        std::string data(msg.data(), msg.size());
        m_baseServer->gateDispatch(data);

        // const int head_len = 4;
        // char head_buf[head_len];
        // int rlen = read(m_fd, head_buf, head_len);
        // if (rlen != head_len) {
        //     LOG(ERROR) << "Invalid head length";
        //     return; // 异常待处理
        // }
        // int body_len = 0;
        // char body_buf[2048];
        // rlen = read(m_fd, body_buf, body_len);
        // if (rlen != body_len) {
        //     LOG(ERROR) << "Invalid body length";
        //     return;
        // }
        // // parse
        // std::string msg;
        // m_baseServer->gateDispatch(msg);
    }
}

void GateServerSession::sync_write() {
    std::string buffer;
    write(m_fd, buffer.data(), buffer.size());
}


}