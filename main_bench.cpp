#include <memory>

#include <vector>
#include <list>

#include <co/all.h>

#include "core/message.h"
#include "core/ws_session.h"
#include "core/servlet.h"
#include "logic/protocol/login_id.pb.h"
#include "logic/protocol/login.pb.h"
#include "logic/protocol/rank_id.pb.h"
#include "logic/protocol/rank.pb.h"

#include "test/bench_login_servlet.h"
#include "test/bench_rank_servlet.h"

int main(int argc, char* argv[]) {
    // GLog glog(argv[0]);
    flag::set_value("cout", "true");
    flag::set_value("log_daily", "true");
    flag::set_value("max_log_file_num", "32");
    flag::parse(argc, argv);

    std::shared_ptr<Eayew::ServletDispatchRange> m_servlet = std::make_shared<Eayew::ServletDispatchRange>();
    {
        Eayew::Servlet::ptr tmp_servlet;
        tmp_servlet.reset(new BenchLoginServlet);
        m_servlet->addServlet(LoginProtocol::ID::C2S_LOGIN_BASE, LoginProtocol::ID::C2S_LOGIN_TOP, tmp_servlet);
    }
    {
        Eayew::Servlet::ptr tmp_servlet;
        tmp_servlet.reset(new BenchRankServlet);
        m_servlet->addServlet(RankProtocol::ID::C2S_RANK_BASE, RankProtocol::ID::C2S_RANK_TOP, tmp_servlet);
    }


    std::list<co::Sched*> scheds;
    std::vector<std::shared_ptr<Eayew::WsSession>> wss;
    for (int i = 0; i < 10; i++) {
        auto sched = co::next_sched();
        scheds.push_back(sched);

        {

            for (int j = 0; j < 100; j++) {
                auto s = std::make_shared<Eayew::WsSession>(-1);
                s->setwSched(sched);
                s->setrSched(sched);
                s->setOnMessage([m_servlet, s](Eayew::Message::ptr msg) {
                    LOG << "msg " << msg->strInfo();
                    m_servlet->doRequest(s, msg);
                });
                s->sync_connect("ws://127.0.0.1:9101");
                s->run();

                auto index = i * 1000 + j;
                auto prefix = std::to_string(index);
                std::string loginname;
                loginname += "rotobt_";
                loginname += prefix;

                LoginProtocol::C2S_LoginCreate req;
                req.set_loginname(loginname);
                req.set_gameid(1);

                auto nsize = req.ByteSizeLong();
                auto nmsg = co::make_shared<Eayew::Message>(nsize);
                req.SerializeToArray(nmsg->pdata(), nsize);
                nmsg->senderId(1);
                nmsg->receiverId(2);
                nmsg->msgId(1003);
                nmsg->roleId(0);
                nmsg->sessionId(0);
                s->send(nmsg);

                wss.push_back(s);
            }
        }
    }

    co::WaitGroup wg;
    wg.add(1);
    wg.wait();

    return 0;
}