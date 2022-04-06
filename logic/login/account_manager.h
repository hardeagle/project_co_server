#ifndef __LOGIC_LOGIN_ACCOUNT_MANAGER_H__
#define __LOGIC_LOGIN_ACCOUNT_MANAGER_H__

#include <map>
#include <memory>

class AccountManager {
public:
    using ptr = std::shared_ptr<AccountManager>;


private:
    std::map<std::string, int> m_loginnames;
};


#endif