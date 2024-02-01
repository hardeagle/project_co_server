#ifndef __LOGIC_LOGIN_SERVLET_LOGIN_SERVLET_H__
#define __LOGIC_LOGIN_SERVLET_LOGIN_SERVLET_H__

#include "core/message.h"
#include "core/servlet.h"
#include "core/session.h"


class LoginServlet : public Eayew::Servlet {
public:
    virtual bool doRequest(Eayew::Session::ptr session, Eayew::Message&& msg);

private:
    bool doLogin(Eayew::Session::ptr session, Eayew::Message&& msg);
    bool doCreate(Eayew::Session::ptr session, Eayew::Message&& msg);
    bool doLoad(Eayew::Session::ptr session, Eayew::Message&& msg);
    bool doOpenid(Eayew::Session::ptr session, Eayew::Message&& msg);
    bool doUpdate(Eayew::Session::ptr session, Eayew::Message&& msg);
};

#endif