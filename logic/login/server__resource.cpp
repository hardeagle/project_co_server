#include "server_resource.h"

#include "core/base/timer_manager.h"
#include "core/redis/redis_manager.h"

#include "account_manager.h"
#include "game_info_manager.h"
#include "id_manager.h"


void ServerResource::init(co::Sched* sched) {
    LOG << "init begin...";
    m_redisMgr = std::make_shared<Eayew::RedisManager>("127.0.0.1", 6379);
    LOG << "init end..111.";
    //m_pikaMgr = std::make_shared<Eayew::RedisManager>("127.0.0.1", 9221);
    //m_tendisMgr = std::make_shared<Eayew::RedisManager>("127.0.0.1", 51002);
    LOG << "init end..222.";

    m_accountMgr = std::make_shared<AccountManager>();
    LOG << "init end..333.";

    m_gameInfoMgr = std::make_shared<GameInfoManager>();
    sched->go([this]() {
        m_gameInfoMgr->init();
    });
    LOG << "init end..444.";

    m_idMgr = std::make_shared<IdManager>();
    LOG << "init end..555.";

    LOG << "timer manager begin...";
    m_timeMgr = std::make_shared<Eayew::TimerManager>(sched);
    LOG << "init end...";
}