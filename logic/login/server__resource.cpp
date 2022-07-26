#include "server_resource.h"

#include "core/redis/redis_manager.h"

#include "account_manager.h"
#include "id_manager.h"

void ServerResource::init() {
    m_redisMgr = std::make_shared<Eayew::RedisManager>("127.0.0.1", 6379);
    m_pikaMgr = std::make_shared<Eayew::RedisManager>("127.0.0.1", 9221);
    m_tendisMgr = std::make_shared<Eayew::RedisManager>("127.0.0.1", 51002);

    m_accountMgr = std::make_shared<AccountManager>();
    m_idMgr = std::make_shared<IdManager>();
}