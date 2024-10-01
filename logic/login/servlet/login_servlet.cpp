#include "login_servlet.h"

// #define CPPHTTPLIB_OPENSSL_SUPPORT
// #define CPPHTTPLIB_USE_POLL
// #include "core/httplib.h"

#include <json/json.h>

#include "log/glog.h"

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

bool LoginServlet::doRequest(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    auto id = msg->realMsgId();
    switch (id) {
        case LoginProtocol::ID::C2S_LOGIN_LOGIN:
            return doLogin(session, msg);
        case LoginProtocol::ID::C2S_LOGIN_CREATE:
            return doCreate(session, msg);
        case LoginProtocol::ID::C2S_LOGIN_LOAD:
            return doLoad(session, msg);
        case LoginProtocol::ID::C2S_LOGIN_OPENID:
            return doOpenid(session, msg);
        case LoginProtocol::ID::C2S_LOGIN_UPDATE:
            return doUpdate(session, msg);
        default:
            ELOG << "invalid id " << id;
            return true;
    }

    return true;
}

bool LoginServlet::doLogin(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doLogin begin...";
    LoginProtocol::C2S_LoginLogin req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }

    LoginProtocol::S2C_LoginLogin resp;
    do {
        auto role_id = ServerResource::get()->redisMgr()->get<uint64_t>(LoginNameToRoleIdSetKey(req.loginname()));
        LOG << "doLogin role id " << role_id;
        if (role_id == 0) {
            LOG << "new role";
            resp.set_ret(EC_LOGIN::NO_ROLE);
            break;
        }
        resp.set_role_id(role_id);
        msg->roleId(role_id);
    } while (false);
    
    session->send(covertRspMsg(msg, resp));
    LOG << "doLogin end... resp " << resp.DebugString();
    return true;
}

bool LoginServlet::doCreate(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doCreate begin...";
    LoginProtocol::C2S_LoginCreate req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }
    LOG << "req " << req.DebugString();

    LoginProtocol::S2C_LoginCreate resp;
    do {
        auto role_id = ServerResource::get()->idMgr()->generateId();
        if (role_id <= 0) {
            ELOG << "general id fail, role id " << role_id;
            resp.set_ret(EC_LOGIN::GENERATE_ID_FAIL);
            break;
        }
        LOG << "doCreate role id " << role_id;
        ServerResource::get()->redisMgr()->set(LoginNameToRoleIdSetKey(req.loginname()), role_id);
        ServerResource::get()->redisMgr()->set(RoleIdToGameIdSetKey(role_id), req.gameid());

        resp.set_role_id(role_id);
        msg->roleId(role_id);

        PublicProtocol::BaseRoleInfo bri;
        bri.set_role_id(role_id);
        bri.set_name(req.role_name());
        bri.set_avatarurl(req.avatarurl());
        std::string serial;
        bri.SerializeToString(&serial);
        ServerResource::get()->redisMgr()->set(BaseRoleInfoSetKey(role_id), serial);
    } while(false);

    session->send(covertRspMsg(msg, resp));
    LOG << "doCreate end... resp " << resp.DebugString();
    return true;
}

bool LoginServlet::doLoad(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doLoad begin...";
    LoginProtocol::C2S_LoginLoad req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }
    LOG << "doLoad req " << req.DebugString();
    LoginProtocol::S2C_LoginLogin resp;
    do {
        auto role_id = ServerResource::get()->redisMgr()->get<uint64_t>(LoginNameToRoleIdSetKey(req.loginname()));
        if ( req.role_id() != role_id || req.role_id() != msg->roleId()) {
            ELOG << "req role id " << req.role_id() << " redis roleid " << role_id << " msg role id " << msg->roleId();
            resp.set_ret(EC_LOGIN::NO_ROLE);
            break;
        }
    } while(false);

    session->send(covertRspMsg(msg, resp));
    ELOG << "doLoad end... resp " << resp.DebugString();
    return true;
}

EC_LOGIN ttOpenid(std::string& openid, GameInfo::ptr gi, const std::string& code) {
    // httplib::SSLClient cli("developer.toutiao.com");
    // cli.enable_server_certificate_verification(false);

    // std::string params = "/api/apps/jscode2session";
    // params += "?appid=";
    // params += gi->appid();
    // params += "&secret=";
    // params += gi->secret();
    // params += "&code=";
    // params += code;
    // LOG << "params " << params;
    // if (auto res = cli.Get(params)) {
    //     if (res->status != httplib::StatusCode::OK_200) {
    //         ELOG << "http response status " << res->status;
    //         return EC_LOGIN::HTTP_TT_STATUS_ERROR;
    //     }

    //     LOG << "resp body " << res->body;
    //     Json::Value root;
    //     Json::Reader reader;
    //     if (reader.parse(res->body, root)) {
    //         // LOG << "parse resp " << root;
    //         auto ret = root["error"].asInt64();
    //         if (ret != 0) {
    //             ELOG << "resp error ret " << ret;
    //             return EC_LOGIN::HTTP_TT_RSP_FAIL;
    //         }
    //         openid = root["openid"].asString();
    //     } else {
    //         ELOG << "parse error, body " << res->body;
    //         return EC_LOGIN::HTTP_TT_RSP_PARSE_FAIL;
    //     }
    // } else {
    //     ELOG << "http fail err " << int(res.error());
    //     return EC_LOGIN::HTTP_TT_GET_FAIL;
    // }
    // return EC_LOGIN::SUCCESS;

    // http::Client c(FLG_s.c_str());

    http::Client cli("developer.toutiao.com");

    std::string params = "/api/apps/jscode2session";
    params += "?appid=";
    params += gi->appid();
    params += "&secret=";
    params += gi->secret();
    params += "&code=";
    params += code;
    LOG << "params " << params;
    cli.get(params.c_str());
    if (cli.status() != 200) {
        ELOG <<"get error " << cli.status();
        return EC_LOGIN::HTTP_TT_GET_FAIL;
    }

    LOG << "resp body " << cli.body();
    Json::Value root;
    Json::Reader reader;
    if (reader.parse(cli.body().c_str(), root)) {
        auto ret = root["error"].asInt64();
        if (ret != 0) {
            ELOG << "resp error ret " << ret;
            return EC_LOGIN::HTTP_TT_RSP_FAIL;
        }
        openid = root["openid"].asString();
        LOG << "---openid " << openid;
    } else {
        ELOG << "parse error, body " << cli.body();
        return EC_LOGIN::HTTP_TT_RSP_PARSE_FAIL;
    }

    return EC_LOGIN::SUCCESS;
}

bool LoginServlet::doOpenid(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doOpenid begin...";
    LoginProtocol::C2S_LoginOpenid req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }

    LoginProtocol::S2C_LoginOpenid resp;
    do {
        auto gi_mgr = ServerResource::get()->gameInfoMgr();
        auto gi = gi_mgr->get(req.gameid());
        if (!gi) {
            ELOG << "Not exist game id " << req.gameid();
            resp.set_ret(EC_LOGIN::GAME_ID_ILLEGAL);
            break;
        }

        std::string openid;
        auto ret = ttOpenid(openid, gi, req.code());
        if (ret != EC_LOGIN::SUCCESS) {
            ELOG << "ttOpenid ret " << ret;
            resp.set_ret(ret);
            break;
        }
        resp.set_openid(openid);
    } while(false);

    session->send(covertRspMsg(msg, resp));
    ELOG << "doOpenid end... resp " << resp.DebugString();
    return true;
}

bool LoginServlet::doUpdate(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doUpdate begin...";
    LoginProtocol::C2S_LoginUpdate req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }
    
    LoginProtocol::S2C_LoginUpdate resp;
    do {
        auto roleid = msg->roleId();
        auto val = ServerResource::get()->redisMgr()->get<std::string>(BaseRoleInfoSetKey(roleid));
        PublicProtocol::BaseRoleInfo bri;
        if (!bri.ParseFromString(val)) {
            ELOG << "parseFromString fail roleid " << roleid;
            resp.set_ret(EC_LOGIN::PARSE_FROM_STRING_FAIL);
            break;
        }
        bri.set_name(req.role_name());
        bri.set_avatarurl(req.avatarurl());
        std::string serial;
        bri.SerializeToString(&serial);
        ServerResource::get()->redisMgr()->set(BaseRoleInfoSetKey(roleid), serial);
    } while(false);

    session->send(covertRspMsg(msg, resp));
    LOG << "doUpdate end... resp " << resp.DebugString();
    return true;
}

