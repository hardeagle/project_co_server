#ifndef __TEST_BENCH_LOGIN_SERVLET_H__
#define __TEST_BENCH_LOGIN_SERVLET_H__

#include "core/servlet.h"


class BenchLoginServlet : public Eayew::Servlet {
public:
    virtual bool doRequest(Eayew::Session::ptr session, Eayew::Message::ptr msg);

private:
    bool doLogin(Eayew::Session::ptr session, Eayew::Message::ptr msg);
    bool doCreate(Eayew::Session::ptr session, Eayew::Message::ptr msg);
    bool doLoad(Eayew::Session::ptr session, Eayew::Message::ptr msg);
    bool doOpenid(Eayew::Session::ptr session, Eayew::Message::ptr msg);
    bool doUpdate(Eayew::Session::ptr session, Eayew::Message::ptr msg);
};

#endif