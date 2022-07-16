#ifndef __LOGIC_LOGIN_SERVLET_LOGIN_SERVLET_H__
#define __LOGIC_LOGIN_SERVLET_LOGIN_SERVLET_H__

#include "core/message.hpp"
#include "core/servlet.h"
#include "core/session.h"


class LoginServlet : public Eayew::Servlet {
public:
    virtual bool doRequest(Eayew::Session::ptr session, Eayew::Message&& msg);

private:
    bool doLoad(Eayew::Session::ptr session, Eayew::Message&& msg);
};

#endif