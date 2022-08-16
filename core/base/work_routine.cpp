#include "work_routine.h"


#include <unistd.h>
#include <sys/syscall.h>

#include "core/message.hpp"
#include "core/servlet.h"
#include "core/session.h"

#include "log/glog.h"

const static uint32_t s_limit = 2048;

namespace Eayew {

WorkRoutine::WorkRoutine(uint32_t id)
    : m_id(id)
    , m_rMsgs(s_limit) {
}

void WorkRoutine::push(Message&& msg) {
    if (m_rMsgs.size() == s_limit) {
        LOG(WARNING) << "work routine full";
    }

    m_rMsgs << std::move(msg);
}

void WorkRoutine::run() {
    for (;;) {
        if (m_rMsgs.size() == 0) {
            LOG(WARNING) << "work routine empty";
        }

        Message msg;
        m_rMsgs >> msg;
        if (m_onMessageCB != nullptr) {
            m_onMessageCB(std::move(msg));
        }
        //m_servlet->doRequest(m_session, std::move(msg));
    }
}

}