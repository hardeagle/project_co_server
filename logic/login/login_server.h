#ifndef __LOGIC_LOGIN_LOGIN_SERVER_H__
#define __LOGIC_LOGIN_LOGIN_SERVER_H__

#include "core/const.hpp"
#include "core/base/base_server.h"

class ServerResource;

class LoginServer : public Eayew::BaseServer {
public:
    using ptr = std::shared_ptr<LoginServer>;

    LoginServer() : Eayew::BaseServer(Eayew::ServerType::EST_LOGIN) {}

protected:
    virtual void beforeRun() override;

private:
    void initServlet();

};

#endif