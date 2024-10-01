#ifndef __TEST_BENCH_RANK_SERVLET_H__
#define __TEST_BENCH_RANK_SERVLET_H__

#include "core/servlet.h"


class BenchRankServlet : public Eayew::Servlet {
public:
    virtual bool doRequest(Eayew::Session::ptr session, Eayew::Message::ptr msg);

private:
    bool doLoad(Eayew::Session::ptr session, Eayew::Message::ptr msg);
    bool doUpdate(Eayew::Session::ptr session, Eayew::Message::ptr msg);
    bool doMyself(Eayew::Session::ptr session, Eayew::Message::ptr msg);
};

#endif