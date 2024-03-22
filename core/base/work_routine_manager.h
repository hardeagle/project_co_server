#ifndef __CORE_BASE_WORK_ROUTINE_MANAGER_H__
#define __CORE_BASE_WORK_ROUTINE_MANAGER_H__

#include <functional>
#include <memory.h>

#include <libgo/libgo.h>

#include "core/message.h"

namespace Eayew {

class ServletDispatchRange;
class Session;

class WorkRoutine : public std::enable_shared_from_this<WorkRoutine> {
public:
    using ptr = std::shared_ptr<WorkRoutine>;

    WorkRoutine(uint64_t id);

    uint64_t id() { return m_id; }

    void setOnMessage(std::function<void(Message::ptr msg)> cb) {
        m_onMessageCB = cb;
    }

    void setOnExit(std::function<void(uint64_t id)> cb) {
        m_onExitCB = cb;
    }
    void push(Message::ptr msg);

    void run();

private:
    uint64_t m_id;

    co_chan<Message::ptr> m_rMsgs;

    std::function<void(Message::ptr msg)> m_onMessageCB;
    std::function<void(uint64_t id)> m_onExitCB;
};

class WorkRoutineManager {
public:
    using ptr = std::shared_ptr<WorkRoutineManager>;

    WorkRoutineManager(std::shared_ptr<ServletDispatchRange> servlet);

    void run();

    void dispatch(std::shared_ptr<Session> session, Message::ptr msg);

    std::shared_ptr<co::CoTimer> timer() { return m_timer; }

private:
    std::shared_ptr<ServletDispatchRange> m_servlet;

    co::Scheduler* m_scheduler;
    std::shared_ptr<co::CoTimer> m_timer;
    std::map<uint64_t, WorkRoutine::ptr> m_wrs;
};

}

#endif