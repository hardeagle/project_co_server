#ifndef __LOGIC_LOGIN_SERVER_RESOURCE_H__
#define __LOGIC_LOGIN_SERVER_RESOURCE_H__

#include <memory>

namespace Eayew {
    class RedisManager;
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

    void init();

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

private:
    std::shared_ptr<Eayew::RedisManager> m_redisMgr;
    std::shared_ptr<Eayew::RedisManager> m_pikaMgr;
    std::shared_ptr<Eayew::RedisManager> m_tendisMgr;

    std::shared_ptr<AccountManager> m_accountMgr;
    std::shared_ptr<GameInfoManager> m_gameInfoMgr;
    std::shared_ptr<IdManager> m_idMgr;

};

#endif