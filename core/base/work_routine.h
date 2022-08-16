#ifndef __CORE_BASE_WORK_ROUTINE_H__
#define __CORE_BASE_WORK_ROUTINE_H__

#include <functional>
#include <memory.h>

#include <libgo/libgo.h>

#include "core/message.hpp"

namespace Eayew {

class ServletDispatchRange;
class Session;

class WorkRoutine : public std::enable_shared_from_this<WorkRoutine> {
public:
    using ptr = std::shared_ptr<WorkRoutine>;

    WorkRoutine(uint32_t id);

    uint32_t id() { return m_id; }

    void setOnMessage(std::function<void(Message&& msg)> cb) {
        m_onMessageCB = cb;
    }

    void push(Message&& msg);

    void run();

private:
    uint32_t m_id;

    co_chan<Message> m_rMsgs;

    std::function<void(Message&& msg)> m_onMessageCB;
};

}

#endif