#include "gate_server_session.h"

#include <functional>
#include <string>

#include <libgo/libgo.h>

#include "core/message.hpp"
#include "core/session.h"

#include "log/glog.h"

#include "base_server.h"

namespace Eayew {

GateServerSession::GateServerSession(int fd)
    : Session(fd) {
}

// void GateServerSession::run() {
//     go [this, self = shared_from_this()] {
//         sync_read();
//     };

//     go [this, self = shared_from_this()] {
//         sync_write();
//     };
// }

// void GateServerSession::sync_read() {
//     for (;;) {
//         auto head_len = Message::LEN_SIZE;
//         Message msg;
//         if (!eio(recv, m_fd, msg.wbuffer(), head_len, MSG_WAITALL)) {
//             LOG(ERROR) << "eio fail, close or error ";
//             return;
//         }
//         msg.commit(head_len);
//         int body_len = msg.length() - head_len;
//         msg.prepare(body_len);
//         if (!eio(recv, m_fd, msg.wbuffer(), body_len, MSG_WAITALL)) {
//             LOG(ERROR) << "eio fail, close or error ";
//             return;
//         }
//         msg.commit(body_len);

//         LOG(INFO) << "onMessageCB";
//         if (m_onMessageCB != nullptr) {
//             m_onMessageCB(std::move(msg));
//         }
//     }
// }

// void GateServerSession::sync_write() {
//     for (;;) {
//         if (m_wMsgs.size() == 0) {
//             LOG(WARNING) << "gate server session empty";
//         }

//         Message msg;
//         m_wMsgs >> msg;
//         write(m_fd, msg.data(), msg.size());
//     }
// }

// void GateServerSession::send(Message&& msg) {
//     if (m_wMsgs.size() == s_limit) {
//         LOG(WARNING) << "gate server session full";
//     }

//     m_wMsgs << std::move(msg);
// }

void GateServerSession::send(Message&& msg, const google::protobuf::Message& gpm) {
    LOG(INFO) << "msg begin " << msg.strInfo();
    auto nsize = gpm.ByteSizeLong();
    if (nsize > msg.size()) {
        msg.prepare(nsize - msg.size());
    }
    gpm.SerializeToArray(msg.zdata(), nsize);
    msg.forceSetLength(Message::HEAD_LEN + nsize);
    msg.forceSetMsgId(msg.msgId() + 1);
    LOG(INFO) << "msg end " << msg.strInfo();

    Session::send(std::move(msg));
}

}