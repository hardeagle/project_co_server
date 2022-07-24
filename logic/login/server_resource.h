#ifndef __LOGIC_LOGIN_SERVER_RESOURCE_H__
#define __LOGIC_LOGIN_SERVER_RESOURCE_H__

#include <memory>

namespace Eayew {
    class RedisManager;
}

class AccountManager;

class ServerResource {
public:
    using ptr = std::shared_ptr<ServerResource>;

    static ptr get() {
        static ptr res = std::make_shared<ServerResource>();
        return res;
    }

    void init();

    std::shared_ptr<AccountManager> accountMgr() {
        return m_accountMgr;
    }

    std::shared_ptr<Eayew::RedisManager> redisMgr() {
        return m_redisMgr;
    }

    std::shared_ptr<Eayew::RedisManager> pikaMgr() {
        return m_pikaMgr;
    }

private:
    std::shared_ptr<AccountManager> m_accountMgr;
    std::shared_ptr<Eayew::RedisManager> m_redisMgr;
    std::shared_ptr<Eayew::RedisManager> m_pikaMgr;
};

#endif