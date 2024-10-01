#include "work_routine_manager.h"


#include <unistd.h>
#include <sys/syscall.h>

#include "core/const.hpp"
#include "core/message.h"
#include "core/servlet.h"
#include "core/session.h"

#include "log/glog.h"

const static uint32_t s_limit = 4096;

namespace Eayew {

WorkRoutine::WorkRoutine(uint64_t id)
    : m_id(id)
    , m_rMsgs(s_limit) {
}

void WorkRoutine::push(Message::ptr msg) {
    // if (m_rMsgs.size() == s_limit) {
    //     WLOG << "work routine full";
    // }

    m_rMsgs << msg;
}

void WorkRoutine::run() {
    LOG << "begin...";
    for (;;) {
        // if (m_rMsgs.size() == 0) {
        //     WLOG << "work routine empty";
        // }
        Message::ptr msg;
        m_rMsgs >> msg;
        LOG << " msg " << msg->strInfo();
        if (msg->msgId() == CloseMsgId::ECMI_WorkRoutine) {
            LOG << "exit work routine";
            if (m_onExitCB != nullptr) {
                m_onExitCB(m_id);
            }
            break;
        }
        if (m_onMessageCB != nullptr) {
            LOG << "m_onMessageCB msg ", msg->strInfo();
            m_onMessageCB(msg);
        }
        //m_servlet->doRequest(m_session, std::move(msg));
    }
}

WorkRoutineManager::WorkRoutineManager(std::shared_ptr<ServletDispatchRange> servlet)
    : m_servlet(servlet) {
    // m_scheduler = co::Scheduler::Create();
    // m_timer = std::make_shared<co::CoTimer>(std::chrono::milliseconds(1), m_scheduler);
}

void WorkRoutineManager::run() {
    // std::thread t([=] {
    //     m_scheduler->Start(1); 
    // });
    // t.detach();
}

void WorkRoutineManager::dispatch(std::shared_ptr<Session> s, Message::ptr msg) {
    auto id = msg->sessionId();
    LOG << "id " << id;
    if (m_wrs.find(id) == m_wrs.end()) {
        LOG << "111";
        auto routine = std::make_shared<WorkRoutine>(id);
                LOG << "222";

        routine->setOnMessage([&, s](Message::ptr m) {
            m_servlet->doRequest(s, m);
        });
                LOG << "333";

        routine->setOnExit([&](uint64_t id) {
            m_wrs.erase(id);
            LOG << "del work routine size after " << m_wrs.size();
        });
                LOG << "444";

        m_wrs[id] = routine;
                LOG << "555";

        m_sched->go([routine] () {
            LOG << "routine run";
            routine->run();
        });
                LOG << "666";

    }
    LOG << "end...";
    m_wrs[id]->push(msg);
}

}