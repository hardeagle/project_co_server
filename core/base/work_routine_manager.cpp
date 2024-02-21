#include "work_routine_manager.h"


#include <unistd.h>
#include <sys/syscall.h>

#include "core/const.hpp"
#include "core/message.h"
#include "core/servlet.h"
#include "core/session.h"

#include "log/glog.h"

const static uint32_t s_limit = 40960;

namespace Eayew {

WorkRoutine::WorkRoutine(uint32_t id)
    : m_id(id)
    , m_rMsgs(s_limit) {
}

void WorkRoutine::push(Message::ptr msg) {
    if (m_rMsgs.size() == s_limit) {
        LOG(WARNING) << "work routine full";
    }

    m_rMsgs << msg;
}

void WorkRoutine::run() {
    for (;;) {
        if (m_rMsgs.size() == 0) {
            LOG(WARNING) << "work routine empty";
        }

        Message::ptr msg;
        m_rMsgs >> msg;
        if (msg->msgId() == CloseMsgId::ECMI_WorkRoutine) {
            LOG(INFO) << "exit work routine";
            break;
        }
        if (m_onMessageCB != nullptr) {
            m_onMessageCB(msg);
        }
        //m_servlet->doRequest(m_session, std::move(msg));
    }
}

WorkRoutineManager::WorkRoutineManager(std::shared_ptr<ServletDispatchRange> servlet)
    : m_servlet(servlet) {
    m_scheduler = co::Scheduler::Create();
    m_timer = std::make_shared<co::CoTimer>(std::chrono::milliseconds(1), m_scheduler);
}

void WorkRoutineManager::run() {
    std::thread t([=] {
        m_scheduler->Start(1); 
    });
    t.detach();
}

void WorkRoutineManager::dispatch(std::shared_ptr<Session> s, Message::ptr msg) {
    auto id = msg->sessionId();
    if (m_wrs.find(id) == m_wrs.end()) {
        auto routine = std::make_shared<WorkRoutine>(id);
        routine->setOnMessage([&, s](Message::ptr m) {
            m_servlet->doRequest(s, m);
        });
        m_wrs[id] = routine;
        go co_scheduler(m_scheduler) [routine] {
            routine->run();
        };
    }
    m_wrs[id]->push(msg);
    if (msg->msgId() == CloseMsgId::ECMI_WorkRoutine) {
        m_wrs.erase(id);
        LOG(INFO) << "del work routine size after " << m_wrs.size();
    }
}

}