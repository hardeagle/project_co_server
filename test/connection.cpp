#include "connection.h"

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <libgo/libgo.h>

#include "log/glog.h"

#include "core/message.hpp"

Connection::Connection(const std::string& ip, int port)
    : m_ip(ip)
    , m_port(port) {
}

void Connection::run() {
    sync_connect();
    go [this, self = shared_from_this()] {
        sync_read();
    };
}

void Connection::sync_write(uint16_t msg_id, uint16_t receiver_id, std::string& buffer) {
    Eayew::Message msg;
    msg.setSender(1);
    msg.setReceiver(receiver_id);
    msg.setSessionId(0);
    msg.setMsgId(msg_id);
    msg.writeData(buffer);
    LOG(INFO) << "receiver id " << msg.receiverId();
    write(m_fd, msg.data(), msg.size());
}

void Connection::sync_connect() {
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = inet_addr(m_ip.data());
    if (-1 == connect(m_fd, (sockaddr*)&addr, sizeof(addr))) {
        LOG(ERROR) << "connect fail, port " << m_port;
        return;
    }
}

void Connection::sync_read() {
    for (;;) {
        LOG(INFO) << "---read begin---";
        auto head_len = Eayew::Message::LEN_SIZE;
        Eayew::Message msg;
        auto rlen = read(m_fd, msg.wbuffer(), head_len);
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

        msg.debugString();
        LOG(INFO) << "---read end---";
    }
}