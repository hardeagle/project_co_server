#include "server_resource.h"

#include "account_manager.h"

void ServerResource::init() {
    m_accountMgr = std::make_shared<AccountManager>();
}