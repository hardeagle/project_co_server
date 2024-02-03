#ifndef __LOGIC_RANK_SERVER_RESOURCE_H__
#define __LOGIC_RANK_SERVER_RESOURCE_H__

#include <memory>

#include <libgo/libgo.h>

namespace Eayew {
    class RedisManager;
}

typedef std::function<void(void)> TimerCB;

class ServerResource {
public:
    using ptr = std::shared_ptr<ServerResource>;

    static ptr get() {
        static ptr res = std::make_shared<ServerResource>();
        return res;
    }

    void init(std::shared_ptr<co::CoTimer> timer);

    std::shared_ptr<co::CoTimer> timerMgr() { return m_timer; }

    std::shared_ptr<Eayew::RedisManager> redisMgr() {
        return m_redisMgr;
    }

    std::shared_ptr<Eayew::RedisManager> pikaMgr() {
        return m_pikaMgr;
    }

    std::shared_ptr<Eayew::RedisManager> tendisMgr() {
        return m_tendisMgr;
    }

private:
    std::shared_ptr<co::CoTimer> m_timer;

    std::shared_ptr<Eayew::RedisManager> m_redisMgr;
    std::shared_ptr<Eayew::RedisManager> m_pikaMgr;
    std::shared_ptr<Eayew::RedisManager> m_tendisMgr;
};

#endif