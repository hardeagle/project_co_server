#ifndef __CORE_BASE_WORK_ROUTINE_MANAGER_H__
#define __CORE_BASE_WORK_ROUTINE_MANAGER_H__

#include <functional>
#include <memory.h>

#include <co/all.h>

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

    co::chan<Message::ptr> m_rMsgs;

    std::function<void(Message::ptr msg)> m_onMessageCB;
    std::function<void(uint64_t id)> m_onExitCB;
};

class WorkRoutineManager {
public:
    using ptr = std::shared_ptr<WorkRoutineManager>;

    WorkRoutineManager(std::shared_ptr<ServletDispatchRange> servlet);

    void sched(co::Sched* s) { m_sched = s; }

    void run();

    void dispatch(std::shared_ptr<Session> session, Message::ptr msg);


private:
    std::shared_ptr<ServletDispatchRange> m_servlet;

    std::map<uint64_t, WorkRoutine::ptr> m_wrs;

    co::Sched* m_sched;
};

}

#endif