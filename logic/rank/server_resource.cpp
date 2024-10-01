#include "server_resource.h"

#include "core/base/timer_manager.h"
#include "core/redis/redis_manager.h"

void ServerResource::init(co::Sched* sched) {
    m_timerMgr = std::make_shared<Eayew::TimerManager>(sched);

    m_redisMgr = std::make_shared<Eayew::RedisManager>("127.0.0.1", 6379);
    //m_pikaMgr = std::make_shared<Eayew::RedisManager>("127.0.0.1", 9221);
    //m_tendisMgr = std::make_shared<Eayew::RedisManager>("127.0.0.1", 51002);
}

