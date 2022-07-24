#ifndef __LOGIC_LOGIN_LOGIN_SERVER_H__
#define __LOGIC_LOGIN_LOGIN_SERVER_H__

#include "core/base/base_server.h"

class ServerResource;

class LoginServer : public Eayew::BaseServer {
public:
    using ptr = std::shared_ptr<LoginServer>;

protected:
    virtual void beforeRun() override;

private:
    void initServlet();

};

#endif