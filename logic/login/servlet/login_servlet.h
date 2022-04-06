#ifndef __LOGIC_LOGIN_SERVLET_LOGIN_SERVLET_H__
#define __LOGIC_LOGIN_SERVLET_LOGIN_SERVLET_H__

#include "core/servlet.h"

class LoginServlet : public Eayew::Servlet {
public:
    virtual bool doRequest(std::string& buffer);

private:
    void doLoad(std::string& buffer);
};

#endif