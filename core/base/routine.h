#ifndef __CORE_BASE_ROUTINE_H__
#define __CORE_BASE_ROUTINE_H__

#include <memory.h>
#include <libgo/libgo.h>

#include "core/message.hpp"

namespace Eayew {

class ServletDispatchRange;
class Session;

class Routine : public std::enable_shared_from_this<Routine> {
public:
    using ptr = std::shared_ptr<Routine>;

    Routine(uint32_t id, std::shared_ptr<ServletDispatchRange> servlet, std::shared_ptr<Session> session);

    uint32_t id() { return m_id; }

    void operator<<(Message&& msg);

    void push(Message&& msg);

    void run();


    // template<class T, class OriginF, typename... Args>
    // void run(T& object, OriginF fn, Args&&... args) {
    //     for (;;) {
    //         if (m_rMsgs.size() == 0) {
    //             bool flag;
    //             m_rChan >> flag;
    //         }
    //         auto&& msg = m_rMsgs.front();
    //         m_rMsgs.pop();

    //         (object.*fn)(std::forward<Args>(args)...);
    //     }
    // }

    // template<class T, class U>
    // void eval(T&& object, U fun)
    // {
    //     (object.*fun)("Using an external function");
    // }

    // eval(FUNCTOR, &FOO::m_function);

private:
    uint32_t m_id;
    std::shared_ptr<ServletDispatchRange> m_servlet;
    std::shared_ptr<Session> m_session;

    co_chan<bool> m_rChan;
    std::queue<Message> m_rMsgs;
    co_chan<bool> m_wChan;
    std::queue<Message> m_wMsgs;
};

}

#endif