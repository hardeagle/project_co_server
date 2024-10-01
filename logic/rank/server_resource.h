#ifndef __LOGIC_RANK_SERVER_RESOURCE_H__
#define __LOGIC_RANK_SERVER_RESOURCE_H__

#include <functional>
#include <memory>

#include <co/all.h>

namespace Eayew {
    class RedisManager;
    class TimerManager;
}

typedef std::function<void(void)> TimerCB;

class ServerResource {
public:
    using ptr = std::shared_ptr<ServerResource>;

    static ptr get() {
        static ptr res = std::make_shared<ServerResource>();
        return res;
    }

    void init(co::Sched* sched);

    std::shared_ptr<Eayew::TimerManager> timerMgr() { 
        return m_timerMgr; 
    }

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
    std::shared_ptr<Eayew::TimerManager> m_timerMgr;

    std::shared_ptr<Eayew::RedisManager> m_redisMgr;
    std::shared_ptr<Eayew::RedisManager> m_pikaMgr;
    std::shared_ptr<Eayew::RedisManager> m_tendisMgr;
};

#endif