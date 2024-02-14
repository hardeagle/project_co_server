#include "work_routine_manager.h"


#include <unistd.h>
#include <sys/syscall.h>

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
        if (msg.msgId() == 0) {
            LOG(INFO) << " exit work routine";
            break;
        }
        if (m_onMessageCB != nullptr) {
            m_onMessageCB(std::move(msg));
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

void WorkRoutineManager::dispatch(std::shared_ptr<Session> s, Message&& msg) {
    auto id = msg.sessionId();
    if (m_wrs.find(id) == m_wrs.end()) {
        auto routine = std::make_shared<WorkRoutine>(id);
        routine->setOnMessage([&, s](Message&& m) {
            m_servlet->doRequest(s, std::move(m));
        });
        m_wrs[id] = routine;
        go co_scheduler(m_scheduler) [routine] {
            routine->run();
        };
        LOG(INFO) << "m_wrs size " << m_wrs.size();
    }
    m_wrs[id]->push(std::move(msg));
    if (msg.msgId() == 0) {
        LOG(INFO) << "del work routine size before " << m_wrs.size();
        m_wrs.erase(id);
        for (const auto& wrs : m_wrs) {
            LOG(INFO) << "close id " << wrs.first;
        }
        LOG(INFO) << "del work routine size after " << m_wrs.size();
    }
}

}