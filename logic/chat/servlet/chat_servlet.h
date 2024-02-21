#ifndef __LOGIC_LOGIN_SERVLET_LOGIN_SERVLET_H__
#define __LOGIC_LOGIN_SERVLET_LOGIN_SERVLET_H__

#include "core/message.h"
#include "core/servlet.h"
#include "core/session.h"


class ChatServlet : public Eayew::Servlet {
public:
    virtual bool doRequest(Eayew::Session::ptr session, Eayew::Message::ptr msg);

private:
    bool doLoad(Eayew::Session::ptr session, Eayew::Message::ptr msg);
    bool doChat(Eayew::Session::ptr session, Eayew::Message::ptr msg);
};

#endif