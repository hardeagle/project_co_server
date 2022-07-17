#ifndef __CORE_BASE_WORK_ROUTINE_H__
#define __CORE_BASE_WORK_ROUTINE_H__

#include <memory.h>
#include <libgo/libgo.h>

#include "core/message.hpp"

namespace Eayew {

class ServletDispatchRange;
class Session;

class WorkRoutine : public std::enable_shared_from_this<WorkRoutine> {
public:
    using ptr = std::shared_ptr<WorkRoutine>;

    WorkRoutine(uint32_t id, std::shared_ptr<ServletDispatchRange> servlet, std::shared_ptr<Session> session);

    uint32_t id() { return m_id; }

    void push(Message&& msg);

    void run();

private:
    uint32_t m_id;
    std::shared_ptr<ServletDispatchRange> m_servlet;
    std::shared_ptr<Session> m_session;

    co_chan<Message> m_rMsgs;
};

}

#endif