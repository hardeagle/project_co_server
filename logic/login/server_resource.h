#ifndef __LOGIC_LOGIN_SERVER_RESOURCE_H__
#define __LOGIC_LOGIN_SERVER_RESOURCE_H__

#include <memory>

#include <co/co.h>

namespace Eayew {
    class RedisManager;
    class TimerManager;
}

class AccountManager;
class GameInfoManager;
class IdManager;

class ServerResource {
public:
    using ptr = std::shared_ptr<ServerResource>;

    static ptr get() {
        static ptr res = std::make_shared<ServerResource>();
        return res;
    }

    void init(co::Sched* sched);

    std::shared_ptr<Eayew::RedisManager> redisMgr() {
        return m_redisMgr;
    }

    std::shared_ptr<Eayew::RedisManager> pikaMgr() {
        return m_pikaMgr;
    }

    std::shared_ptr<Eayew::RedisManager> tendisMgr() {
        return m_tendisMgr;
    }

    std::shared_ptr<AccountManager> accountMgr() {
        return m_accountMgr;
    }

    std::shared_ptr<GameInfoManager> gameInfoMgr() {
        return m_gameInfoMgr;
    }

    std::shared_ptr<IdManager> idMgr() {
        return m_idMgr;
    }

    std::shared_ptr<Eayew::TimerManager> timeMgr() {
        return m_timeMgr;
    }

private:
    std::shared_ptr<Eayew::RedisManager> m_redisMgr;
    std::shared_ptr<Eayew::RedisManager> m_pikaMgr;
    std::shared_ptr<Eayew::RedisManager> m_tendisMgr;

    std::shared_ptr<AccountManager> m_accountMgr;
    std::shared_ptr<GameInfoManager> m_gameInfoMgr;
    std::shared_ptr<IdManager> m_idMgr;
    std::shared_ptr<Eayew::TimerManager> m_timeMgr;
};

#endif