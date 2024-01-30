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
        auto redis_mgr = ServerResource::get()->redisMgr();
        auto role_id = redis_mgr->get<uint64_t>(LoginNameToRoleIdKey(req.loginname()));
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
        resp.set_role_id(role_id);
        msg.roleId(role_id);

        PublicProtocol::BaseRoleInfo bri;
        bri.set_role_id(role_id);
        bri.set_name(req.role_name());
        bri.set_avatarurl(req.avatarurl());
        std::string serial;
        bri.SerializeToString(&serial);
        ServerResource::get()->redisMgr()->set("base_role_info", serial);
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

    session->send(std::move(msg));

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
// {
//     httplib::SSLClient cli("developer.toutiao.com");
//     // cli.set_ca_cert_path("./ca-bundle.crt");
//     cli.enable_server_certificate_verification(false);
//     if (auto res = cli.Get("/api/apps/jscode2session?appid=tt5c2d2a90c316a8d9&secret=40a13a4ea1a0c6b9c19d79267c1881851066dd83&code=EoHEtFObgpzwz4wzo1Fh2wlSK-_8EiK4Ul7dE_CCwQk6chOT8GCqWUCHe60nhBXD7UZB7R5-zghV9WMR_uFaBykvaD9YJQLmix6mhzV_2jqcWmQZxxqMq_FQtj8")) {
//         LOG(INFO) << res->status;
//         LOG(INFO) << res->get_header_value("Content-Type");
//         LOG(INFO) << res->body;
//     } else {
//         LOG(INFO) << "error code: " << res.error();
//     #ifdef CPPHTTPLIB_OPENSSL_SUPPORT
//         auto result = cli.get_openssl_verify_result();
//         if (result) {
//         LOG(INFO) << "verify error: " << X509_verify_cert_error_string(result);
//         }
//     #endif
//     }

// }


    // {
    //     httplib::SSLClient cli("developer.toutiao.com");
    //     cli.enable_server_certificate_verification(false);

    //     if (auto res = cli.Get("/api/apps/jscode2session?appid=tt5c2d2a90c316a8d9&secret=40a13a4ea1a0c6b9c19d79267c1881851066dd83&code=EoHEtFObgpzwz4wzo1Fh2wlSK-_8EiK4Ul7dE_CCwQk6chOT8GCqWUCHe60nhBXD7UZB7R5-zghV9WMR_uFaBykvaD9YJQLmix6mhzV_2jqcWmQZxxqMq_FQtj8")) {
    //         LOG(INFO) << res->status;
    //         LOG(INFO) << res->get_header_value("Content-Type");
    //         LOG(INFO) << res->body;
    //     } else {
    //         LOG(ERROR) << "error code: " << res.error();
    //         auto result = cli.get_openssl_verify_result();
    //         if (result) {
    //             LOG(INFO) << "verify error: " << X509_verify_cert_error_string(result);
    //         }
    //     }
    // }

    LoginProtocol::S2C_LoginOpenid resp;
    do {
        auto gi_mgr = ServerResource::get()->gameInfoMgr();
        auto gi = gi_mgr->get(req.gameid());
        if (!gi) {
            LOG(ERROR) << "Not exist game id " << req.gameid();
            break;
        }

        // httplib::Client cli("https://developer.toutiao.com");
        // httplib::SSLClient cli("developer.toutiao.com");
        // cli.set_ca_cert_path("/etc/nginx/ssl/eayew/eayew.com_bundle.crt");
        // cli.enable_server_certificate_verification(false);

        httplib::SSLClient cli("developer.toutiao.com");
        // httplib::SSLClient cli("google.com");
        // httplib::SSLClient cli("www.youtube.com");
        // cli.set_ca_cert_path("/etc/nginx/ssl/eayew/eayew.com_bundle.crt");
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
            // if (res->status != httplib::StatusCode::OK_200) {
            //     LOG(ERROR) << "http response status " << res->status;
            //     break;
            // }
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

    session->send(std::move(covertRspMsg(msg, resp)));
    LOG(ERROR) << "doLoad end...";
    return true;
}

