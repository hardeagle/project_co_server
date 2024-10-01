#include "bench_login_servlet.h"

// #define CPPHTTPLIB_OPENSSL_SUPPORT
// #define CPPHTTPLIB_USE_POLL
// #include "core/httplib.h"

#include <json/json.h>

#include <co/all.h>

#include "core/const.hpp"
#include "core/redis/redis_manager.h"
#include "core/util/util.h"

#include "logic/common/error_code.h"
#include "logic/common/redis_key.h"
#include "logic/protocol/public.pb.h"

#include "logic/login/game_info_manager.h"
#include "logic/login/id_manager.h"
#include "logic/protocol/login_id.pb.h"
#include "logic/protocol/login.pb.h"
#include "logic/login/server_resource.h"
#include "logic/protocol/rank.pb.h"
#include "logic/protocol/rank_id.pb.h"


bool BenchLoginServlet::doRequest(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    auto id = msg->realMsgId();
    switch (id) {
        case LoginProtocol::ID::S2C_LOGIN_LOGIN:
            return doLogin(session, msg);
        case LoginProtocol::ID::S2C_LOGIN_CREATE:
            return doCreate(session, msg);
        case LoginProtocol::ID::S2C_LOGIN_LOAD:
            return doLoad(session, msg);
        case LoginProtocol::ID::S2C_LOGIN_OPENID:
            return doOpenid(session, msg);
        case LoginProtocol::ID::S2C_LOGIN_UPDATE:
            return doUpdate(session, msg);
        default:
            ELOG << "invalid id " << id;
            return true;
    }

    return true;
}

bool BenchLoginServlet::doLogin(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doLogin begin...";
    LoginProtocol::S2C_LoginLogin req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }

    LOG << "doLogin end...";
    return true;
}

bool BenchLoginServlet::doCreate(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doCreate begin...";
    LoginProtocol::S2C_LoginCreate req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }
    LOG << "req " << req.DebugString();

    LoginProtocol::C2S_LoginLoad resp;
    resp.set_role_id(req.role_id());
    resp.set_gameid(1);

    auto nsize = resp.ByteSizeLong();
    auto nmsg = co::make_shared<Eayew::Message>(nsize);
    resp.SerializeToArray(nmsg->pdata(), nsize);
    nmsg->senderId(Eayew::EST_GATE);
    nmsg->receiverId(Eayew::EST_LOGIN);
    nmsg->msgId(LoginProtocol::C2S_LOGIN_LOAD);
    nmsg->roleId(msg->roleId());
    nmsg->sessionId(msg->sessionId());
    session->send(nmsg);
    LOG << "doCreate end... resp " << resp.DebugString();
    return true;
}

bool BenchLoginServlet::doLoad(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doLoad begin...";
    LoginProtocol::C2S_LoginLoad req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }
    LOG << "doLoad req " << req.DebugString();
    RankProtocol::C2S_RankMyself resp;
    resp.set_subtype(2);

    auto nsize = resp.ByteSizeLong();
    auto nmsg = co::make_shared<Eayew::Message>(nsize);
    resp.SerializeToArray(nmsg->pdata(), nsize);
    nmsg->senderId(Eayew::EST_GATE);
    nmsg->receiverId(Eayew::EST_RANK);
    nmsg->msgId(RankProtocol::C2S_RANK_MYSELF);
    nmsg->roleId(msg->roleId());
    nmsg->sessionId(msg->sessionId());

    session->send(nmsg);
    ELOG << "doLoad end... resp " << resp.DebugString();
    return true;
}

bool BenchLoginServlet::doOpenid(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doOpenid begin...";
    LoginProtocol::C2S_LoginOpenid req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }


    // session->send(covertRspMsg(msg, resp));
    ELOG << "doOpenid end...";
    return true;
}

bool BenchLoginServlet::doUpdate(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doUpdate begin...";
    LoginProtocol::C2S_LoginUpdate req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }
    
    LOG << "doUpdate end...";
    return true;
}

