#include "server_resource.h"

#include "core/redis/redis_manager.h"

void ServerResource::init(std::shared_ptr<co::CoTimer> timer) {
    m_timer = timer;

    m_redisMgr = std::make_shared<Eayew::RedisManager>("127.0.0.1", 6379);
    //m_pikaMgr = std::make_shared<Eayew::RedisManager>("127.0.0.1", 9221);
    //m_tendisMgr = std::make_shared<Eayew::RedisManager>("127.0.0.1", 51002);
}

