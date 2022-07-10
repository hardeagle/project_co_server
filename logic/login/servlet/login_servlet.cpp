#include "login_servlet.h"

#include "log/glog.h"

#include "logic/login/protocol/login_id.pb.h"
#include "logic/login/protocol/login.pb.h"

bool LoginServlet::doRequest(Eayew::Session::ptr session, Eayew::Message&& msg) {
    auto id = msg.realMsgId();
    switch (id) {
        case LoginProtocol::ID::C2S_LOGIN_LOAD:
            return doLoad(session, std::move(msg));
            break;
    }

    return true;
}

bool LoginServlet::doLoad(Eayew::Session::ptr session, Eayew::Message&& msg) {
    LOG(INFO) << "doLoad";
    LOG(WARNING) << msg.strInfo();

    LoginProtocol::C2S_LoginLoad req;
    if (!req.ParseFromArray(msg.realData(), msg.realSize())) {
        LOG(ERROR) << "ParseFromArray fail ";
        return false;
    }
    //LOG(INFO) << "req " << req.DebugString();

    msg.forceSetRoleId(43980465111168);

    session->send(std::move(msg));

    return true;
}

