#include "connection.h"

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <libgo/libgo.h>

#include "log/glog.h"

#include "core/message.h"
#include "core/session.h"

// Connection::Connection(const std::string& ip, int port)
//     : m_ip(ip)
//     , m_count(0)
//     , m_port(port) {
// }

// void Connection::run() {
//     sync_connect();
//     go [this, self = shared_from_this()] {
//         sync_read();
//     };
// }

// void Connection::sync_write(uint16_t msg_id, uint16_t receiver_id, std::string& buffer) {
//     Eayew::Message msg;
//     msg.setSender(1);
//     msg.setReceiver(receiver_id);
//     msg.setSessionId(0);
//     msg.setMsgId(msg_id);
//     msg.writeData(buffer);
//     write(m_fd, msg.data(), msg.size());
// }

// void Connection::sync_connect() {
//     m_fd = socket(AF_INET, SOCK_STREAM, 0);
//     sockaddr_in addr;
//     addr.sin_family = AF_INET;
//     addr.sin_port = htons(m_port);
//     addr.sin_addr.s_addr = inet_addr(m_ip.data());
//     if (-1 == connect(m_fd, (sockaddr*)&addr, sizeof(addr))) {
//         LOG(ERROR) << "connect fail, port " << m_port;
//         return;
//     }
// }

// void Connection::sync_read() {
//     for (;;) {
//         LOG(INFO) << "---read begin---";
//         auto head_len = Eayew::Message::LEN_SIZE;
//         Eayew::Message msg;
//         if (!Eayew::eio(recv, m_fd, msg.wbuffer(), head_len, MSG_WAITALL)) {
//             LOG(ERROR) << "eio fail, close or error ";
//             return;
//         }
//         msg.commit(head_len);
//         int body_len = msg.length() - head_len;
//         LOG(INFO) << "body len " << body_len;
//         msg.prepare(body_len);
//         if (!Eayew::eio(recv, m_fd, msg.wbuffer(), body_len, MSG_WAITALL)) {
//             LOG(ERROR) << "eio fail, close or error ";
//             return;
//         }
//         msg.commit(body_len);

//         LOG(WARNING) << msg.strInfo();
//         LOG(INFO) << "---read end---,  " << ++m_count;
//     }
// }

Connection::Connection()
    : Eayew::Session() {
}

bool Connection::sync_connect(const std::string& ip, uint16_t port, uint16_t sender, uint16_t receiver) {
    if (!Eayew::Session::sync_connect(ip, port)) {
        LOG(ERROR) << "sync_connect fail";
        return false;
    }
    m_sender = sender;
    m_receiver = receiver;

    // Eayew::Message msg;
    // msg.senderId(sender);
    // msg.receiverId(receiver);
    // write(m_fd, msg.data(), msg.size());
    return true;
}