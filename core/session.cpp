#include "session.h"

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "core/message.h"
#include "core/util/util.h"

const static uint32_t s_limit = 4096;

namespace Eayew {

Session::Session(uint32_t fd)
    : m_fd(fd)
    , m_rMsgs(s_limit)
    , m_wMsgs(s_limit) {
    if (m_fd != -1) {
        m_id = (uint64_t(1) << 48) + (uint64_t(getCurSecond()) << 16) + (m_fd & 0xFFFF);
    }
}

void Session::setOnMessage(std::function<void(Message::ptr msg)> cb) {
    m_onMessageCB = cb;
}

void Session::setOnClose(std::function<void(uint64_t id)> cb) {
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
        ELOG << "connect fail, port " << m_port;
        return false;
    }
    m_id = (uint64_t(m_port) << 48) + (uint64_t(getCurSecond()) << 16) + (m_fd & 0xFFFF);
    LOG << "sync connect success, fd " << m_fd << " ip " << m_ip << " port " << m_port;
    return true;
}

bool Session::closeMsg(uint16_t msgid, uint64_t sessionid) {
    auto msg = co::make_shared<Message>(0);
    msg->msgId(msgid);
    msg->roleId(0); 
    msg->sessionId(sessionid);
    msg->senderId(this->sender());
    msg->receiverId(this->receiver());
    this->send(msg);
    LOG << "close, msg  " << msg->strInfo();
    return true;
}

void Session::run() {
    if (m_rSched == nullptr) {
        LOG << "nullptr";
    }
    LOG << "run begin...";
    m_rSched->go([this, self = shared_from_this()] {
        sync_read();
    });

    LOG << "run mid...";

    m_wSched->go([this, self = shared_from_this()] {
        sync_write();
    });
    LOG << "run end...";
}

void Session::send(Message::ptr msg) {
    LOG << "begin...";
    LOG << "send msg " << msg->strInfo();
    // if (m_wMsgs.size() == s_limit) {
    //     WLOG << "m_wMsgs full";
    // }

    m_wMsgs << msg;
    LOG << "end...";
}

// // head + body
// void Session::sync_read() {
//     for (;;) {
//         Message msg;
//         auto head_len = Message::LEN_SIZE;
//         if (!Eayew::eio(recv, m_fd, msg.wbuffer(), head_len, MSG_WAITALL)) {
//             ELOG << "eio fail, close or error ";
//             return;
//         }
//         msg.commit(head_len);
//         auto body_len = msg.length() - head_len;
//         msg.prepare(body_len);
//         if (!Eayew::eio(recv, m_fd, msg.wbuffer(), body_len, MSG_WAITALL)) {
//             ELOG << "eio fail, close or error ";
//             return;
//         }
//         msg.commit(body_len);

//         if (m_onMessageCB != nullptr) {
//             m_onMessageCB(std::move(msg));
//         }
//     }
// }

// // head + body
// void Session::sync_read() {
//     for (;;) {
//         auto head_len = Message::LEN_SIZE;
//         char buffs[head_len];
//         if (!Eayew::eio(recv, m_fd, &buffs[0], head_len, MSG_WAITALL)) {
//             ELOG << "eio fail, close or error ";
//             return;
//         }
//         auto len = *((uint16_t*)(&buffs[0]));
//         auto body_len = len - head_len;
//         Message msg(len - Message::HEAD_LEN);
//         msg.length(len);
//         if (!Eayew::eio(recv, m_fd, msg.data() + head_len, body_len, MSG_WAITALL)) {
//             ELOG << "eio fail, close or error ";
//             return;
//         }
//         LOG << "---msg " << msg.strInfo();
//         if (m_onMessageCB != nullptr) {
//             m_onMessageCB(std::move(msg));
//         }
//     }
// }

void Session::sync_read() {
    static const uint32_t MAX_SIZE = 64 * 1024;
    char buffs[MAX_SIZE];
    uint32_t index = 0;
    uint32_t rlen = 0;
    for (;;) {
        if (index < rlen) {
            WLOG << "memmove index " << index << " rlen " << rlen;
            memcpy(&buffs[0], &buffs[index], rlen - index);
            index = rlen - index;
        } else {
            index = 0;
        }
        rlen = read(m_fd, &buffs[index], MAX_SIZE - index);
        if (0 == rlen) {
            WLOG << "close";
            if (m_onCloseCB != nullptr) {
                m_onCloseCB(sender());
            }
            close(m_fd);
            return;
        } else if (-1 == rlen) {
            if (errno == EINTR || errno==EAGAIN) {
                WLOG << "errno " << errno;
                continue;
            } else {
                ELOG << "errno " << errno;
                return;
            }
        }

        WLOG << "---rlen " << rlen << " index " << index;
        rlen += index;
        index = 0;
        while (index < rlen) {
            auto p = (uint16_t*)(&buffs[index]);
            LOG << "p " << p << " data size " << *p;
            auto size = *p;
            if (size > MAX_SIZE) {
                ELOG << "overflow , size " << size << " MAX_SIZE" << MAX_SIZE;
                return;
            }
            if (size > rlen - index) {
                WLOG << "warning " << " rlen " << rlen << " index " << index << " size " << size; // 还需要做下处理，粘包
                break;
            }
            LOG << "len " << rlen << " size " << size << " index " << index;
            auto msg = co::make_shared<Message>(size - Message::HEAD_LEN);
            memcpy(msg->data(), &buffs[index], size);
            if (m_onMessageCB != nullptr) {
                //LOG << "msg " << msg.strInfo();
                m_onMessageCB(msg);
            }
            index += size;
            WLOG << "loop, len " << rlen << " index " << index;
        }
        WLOG << "once read, len " << rlen << " index " << index;
    }
}

void Session::sync_write() {
    for (;;) {
        // if (m_wMsgs.size() == 0) {
        //     WLOG << "m_wMsgs empty";
        // }

        Message::ptr msg;
        m_wMsgs >> msg;
        LOG << "sync_write " << msg->strInfo();
        write(m_fd, msg->data(), msg->size());
    }
}

}