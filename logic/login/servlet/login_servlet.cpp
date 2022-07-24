#include "login_servlet.h"

#include "log/glog.h"

#include "logic/login/protocol/login_id.pb.h"
#include "logic/login/protocol/login.pb.h"

bool LoginServlet::doRequest(Eayew::Session::ptr session, Eayew::Message&& msg) {
    auto id = msg.realMsgId();
    switch (id) {
        case LoginProtocol::ID::C2S_LOGIN_LOGIN:
            return doLogin(session, std::move(msg));
        case LoginProtocol::ID::C2S_LOGIN_CREATE:
            return doCreate(session, std::move(msg));
        case LoginProtocol::ID::C2S_LOGIN_LOAD:
            return doLoad(session, std::move(msg));
        default:
            LOG(ERROR) << "invalid id " << id;
            return true;
    }

    return true;
}

bool LoginServlet::doLogin(Eayew::Session::ptr session, Eayew::Message&& msg) {
    return true;
}

bool LoginServlet::doCreate(Eayew::Session::ptr session, Eayew::Message&& msg) {
    return true;
}

bool LoginServlet::doLoad(Eayew::Session::ptr session, Eayew::Message&& msg) {
    LOG(INFO) << "doLoad begin...";
    LOG(WARNING) << msg.strInfo();

    auto sessionId = msg.sessionId();
    LOG(WARNING) << "session id " << sessionId;

    LoginProtocol::C2S_LoginLoad req;
    if (!req.ParseFromArray(msg.realData(), msg.realSize())) {
        LOG(ERROR) << "ParseFromArray fail ";
        return false;
    }
    //LOG(INFO) << "req " << req.DebugString();

    LOG(WARNING) << "-----0doLoad " << msg.strInfo();

    msg.forceSetRoleId(43980465111168);

    LOG(WARNING) << "-----1doLoad " << msg.strInfo();

    session->send(std::move(msg));

    LOG(ERROR) << "doLoad end...";
    return true;
}

