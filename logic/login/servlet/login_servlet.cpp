#include "login_servlet.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#define CPPHTTPLIB_USE_POLL
#include "core/httplib.h"

#include <json/json.h>

#include "log/glog.h"

#include "core/redis/redis_manager.h"
#include "core/util/util.h"



#include "logic/common/redis_key.h"
#include "logic/protocol/public.pb.h"

#include "logic/login/game_info_manager.h"
#include "logic/login/id_manager.h"
#include "logic/login/protocol/login_id.pb.h"
#include "logic/login/protocol/login.pb.h"
#include "logic/login/server_resource.h"

bool LoginServlet::doRequest(Eayew::Session::ptr session, Eayew::Message&& msg) {
    auto id = msg.realMsgId();
    switch (id) {
        case LoginProtocol::ID::C2S_LOGIN_LOGIN:
            return doLogin(session, std::move(msg));
        case LoginProtocol::ID::C2S_LOGIN_CREATE:
            return doCreate(session, std::move(msg));
        case LoginProtocol::ID::C2S_LOGIN_LOAD:
            return doLoad(session, std::move(msg));
        case LoginProtocol::ID::C2S_LOGIN_OPENID:
            return doOpenid(session, std::move(msg));
        default:
            LOG(ERROR) << "invalid id " << id;
            return true;
    }

    return true;
}

bool LoginServlet::doLogin(Eayew::Session::ptr session, Eayew::Message&& msg) {
    LOG(INFO) << "doLogin begin...";
    LoginProtocol::C2S_LoginLogin req;
    if (!req.ParseFromArray(msg.pdata(), msg.psize())) {
        LOG(ERROR) << "ParseFromArray fail";
        return false;
    }

    LoginProtocol::S2C_LoginLogin resp;
    do {
        auto key = "loginname_to_role_id_" + req.loginname();
        LOG(INFO) << "key " << key;
        auto role_id = ServerResource::get()->redisMgr()->get<uint64_t>(key);
        LOG(INFO) << "role id " << role_id;
        if (role_id == 0) {
            LOG(INFO) << "new role";
            resp.set_ret(1);
            break;
        }
        resp.set_role_id(role_id);
        msg.roleId(role_id);
    } while (false);
    
    session->send(std::move(covertRspMsg(msg, resp)));
    LOG(INFO) << "doLogin end...";
    return true;
}

bool LoginServlet::doCreate(Eayew::Session::ptr session, Eayew::Message&& msg) {
    LOG(INFO) << "doCreate begin...";
    LoginProtocol::C2S_LoginCreate req;
    if (!req.ParseFromArray(msg.pdata(), msg.psize())) {
        LOG(ERROR) << "ParseFromArray fail";
        return false;
    }
    LOG(INFO) << "req " << req.DebugString();

    LoginProtocol::S2C_LoginCreate resp;
    do {
        auto role_id = ServerResource::get()->idMgr()->generateId();
        if (role_id <= 0) {
            LOG(ERROR) << "general id fail, role id " << role_id;
            resp.set_ret(1);
            break;
        }
        ServerResource::get()->redisMgr()->set("loginname_to_role_id_" + req.loginname(), role_id);

        resp.set_role_id(role_id);
        msg.roleId(role_id);

        PublicProtocol::BaseRoleInfo bri;
        bri.set_role_id(role_id);
        bri.set_name(req.role_name());
        bri.set_avatarurl(req.avatarurl());
        std::string serial;
        bri.SerializeToString(&serial);
        ServerResource::get()->redisMgr()->set("base_role_info_" + std::to_string(role_id), serial);
    } while(false);

    session->send(std::move(covertRspMsg(msg, resp)));
    LOG(INFO) << "doCreate end...";
    return true;
}

bool LoginServlet::doLoad(Eayew::Session::ptr session, Eayew::Message&& msg) {
    LOG(INFO) << "doLoad begin...";
    LOG(WARNING) << msg.strInfo();
    LoginProtocol::C2S_LoginLoad req;
    if (!req.ParseFromArray(msg.pdata(), msg.psize())) {
        LOG(ERROR) << "ParseFromArray fail";
        return false;
    }
    LoginProtocol::S2C_LoginLogin resp;
    do {

    } while(false);


    // auto sessionId = msg.sessionId();
    // LOG(WARNING) << "session id " << sessionId;

    // LoginProtocol::C2S_LoginLoad req;
    // if (!req.ParseFromArray(msg.realData(), msg.realSize())) {
    //     LOG(ERROR) << "ParseFromArray fail ";
    //     return false;
    // }
    // //LOG(INFO) << "req " << req.DebugString();
 
    // LOG(WARNING) << "-----0doLoad " << msg.strInfo();

    // msg.forceSetRoleId(43980465111168);

    // LOG(WARNING) << "-----1doLoad " << msg.strInfo();

    session->send(std::move(covertRspMsg(msg, resp)));

    LOG(ERROR) << "doLoad end...";
    return true;
}

bool LoginServlet::doOpenid(Eayew::Session::ptr session, Eayew::Message&& msg) {
    LOG(INFO) << "doOpenid begin...";
    LOG(WARNING) << msg.strInfo();
    LoginProtocol::C2S_LoginOpenid req;
    if (!req.ParseFromArray(msg.pdata(), msg.psize())) {
        LOG(ERROR) << "ParseFromArray fail";
        return false;
    }

    LoginProtocol::S2C_LoginOpenid resp;
    do {
        auto gi_mgr = ServerResource::get()->gameInfoMgr();
        auto gi = gi_mgr->get(req.gameid());
        if (!gi) {
            LOG(ERROR) << "Not exist game id " << req.gameid();
            break;
        }


        httplib::SSLClient cli("developer.toutiao.com");
        cli.enable_server_certificate_verification(false);

        std::string params = "/api/apps/jscode2session";
        params += "?appid=";
        params += gi->appid();
        params += "&secret=";
        params += gi->secret();
        params += "&code=";
        params += req.code();;
        LOG(INFO) << "params " << params;
        if (auto res = cli.Get(params)) {
            if (res->status != httplib::StatusCode::OK_200) {
                LOG(ERROR) << "http response status " << res->status;
                break;
            }
            std::cout << res->status << std::endl;
            std::cout << res->get_header_value("Content-Type") << std::endl;
            std::cout << res->body << std::endl;

            LOG(INFO) << "resp body " << res->body;
            Json::Value root;
            Json::Reader reader;
            if (reader.parse(res->body, root)) {
                LOG(INFO) << "parse resp " << root;
                auto ret = root["error"].asInt64();
                if (ret != 0) {
                    LOG(ERROR) << "resp error ret " << ret;
                    break;
                }
                resp.set_openid(root["openid"].asString());
            } else {
                LOG(ERROR) << "parse error, body " << res->body;
            }
        } else {
            auto err = res.error();
            LOG(ERROR) << "http fail err " << err;
            break;
        }
    } while(false);

    LOG(INFO) << "resp " << resp.DebugString();
    session->send(std::move(covertRspMsg(msg, resp)));
    LOG(ERROR) << "doOpenid end...";
    return true;
}

