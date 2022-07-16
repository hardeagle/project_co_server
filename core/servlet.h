#ifndef __CORE_SERVLET_H__
#define __CORE_SERVLET_H__

#include <memory>
#include <set>

#include "core/message.hpp"
#include "core/session.h"

namespace Eayew {

class Message;

class Servlet {
public:
    using ptr = std::shared_ptr<Servlet>;

    virtual ~Servlet() {}

    virtual bool doRequest(Session::ptr session, Message&& msg) {
        return true;
    }
};

class ServletDispatchRange {
public:
    using ptr = std::shared_ptr<ServletDispatchRange>;

    struct Element {
        Element(int f)
            : from(f)
            , to(f) {

        }

        Element(int f, int t, Servlet::ptr s)
            : from(f)
            , to(t)
            , servlet(s) {
        }

        bool operator < (const Element& element) const;


        int from;
        int to;
        Servlet::ptr servlet;
    };

    void addServlet(int f, int t, Servlet::ptr s) {
        Element element(f, t, s);
        m_servlets.insert(element);
    }

    bool doRequest(Session::ptr session, Message&& msg);

private:
    std::set<Element> m_servlets;

};

}

#endif