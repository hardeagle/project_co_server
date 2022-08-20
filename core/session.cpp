#include "session.h"

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <libgo/libgo.h>

#include "core/util/util.h"

#include "log/glog.h"

const static uint32_t s_limit = 40960;

namespace Eayew {

Session::Session(uint32_t fd)
    : m_fd(fd)
    , m_rMsgs(s_limit)
    , m_wMsgs(s_limit) {
    if (m_fd != -1) {
        m_id = (uint64_t(1) << 48) + (uint64_t(getCurSecond()) << 16) + (m_fd & 0xFFFF);
    }
}

void Session::setOnMessage(std::function<void(Message&& msg)> cb) {
    m_onMessageCB = cb;
}

void Session::setOnClose(std::function<void()> cb) {
    m_onCloseCB = cb;
}

bool Session::sync_connect(const std::string& ip, uint16_t port) {
    m_ip = ip;
    m_port = port;
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = inet_addr(m_ip.data());
    if (-1 == connect(m_fd, (sockaddr*)&addr, sizeof(addr))) {
        LOG(ERROR) << "connect fail, port " << m_port;
        return false;
    }
    m_id = (uint64_t(1) << 48) + (uint64_t(getCurSecond()) << 16) + (m_fd & 0xFFFF);
    LOG(INFO) << "sync connect success, fd " << m_fd << " ip " << m_ip << " port " << m_port;
    return true;
    // m_sender = sender;
    // m_receiver = receiver;

    // Message msg;
    // msg.setSender(sender);
    // msg.setReceiver(receiver);
    // msg.writeData("");
    // write(m_fd, msg.data(), msg.size());
}

void Session::run() {
    go [this, self = shared_from_this()] {
        sync_read();
    };

    go [this, self = shared_from_this()] {
        sync_write();
    };
}

void Session::send(Message&& msg) {
    LOG(INFO) << "send msg " << msg.strInfo();
    if (m_wMsgs.size() == s_limit) {
        LOG(WARNING) << "m_wMsgs full";
    }

    m_wMsgs << std::move(msg);
}

void Session::sync_read() {
    for (;;) {
        Message msg;
        auto head_len = Message::LEN_SIZE;
        if (!Eayew::eio(recv, m_fd, msg.wbuffer(), head_len, MSG_WAITALL)) {
            LOG(ERROR) << "eio fail, close or error ";
            return;
        }
        msg.commit(head_len);
        auto body_len = msg.length() - head_len;
        msg.prepare(body_len);
        if (!Eayew::eio(recv, m_fd, msg.wbuffer(), body_len, MSG_WAITALL)) {
            LOG(ERROR) << "eio fail, close or error ";
            return;
        }
        msg.commit(body_len);

        if (m_onMessageCB != nullptr) {
            m_onMessageCB(std::move(msg));
        }
    }
}

void Session::sync_write() {
    for (;;) {
        if (m_wMsgs.size() == 0) {
            LOG(WARNING) << "m_wMsgs empty";
        }

        Message msg;
        m_wMsgs >> msg;
        LOG(INFO) << "sync_write " << msg.strInfo();
        write(m_fd, msg.data(), msg.size());
    }
}

}