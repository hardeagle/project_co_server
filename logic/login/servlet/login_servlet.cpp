#include "login_servlet.h"

#include "log/glog.h"

#include "core/redis/redis_manager.h"

#include "logic/common/redis_key.h"
#include "logic/protocol/public.pb.h"

#include "logic/login/protocol/login_id.pb.h"
#include "logic/login/protocol/login.pb.h"
#include "logic/login/server_resource.h"
#include "logic/login/id_manager.h"

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
    LOG(INFO) << "doLogin begin...";
    LoginProtocol::C2S_LoginLogin req;
    if (!req.ParseFromArray(msg.realData(), msg.realSize())) {
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
        msg.forceSetRoleId(role_id);
    } while (false);
    
    session->send(std::move(msg), resp);
    LOG(INFO) << "doLogin end...";
    return true;
}

bool LoginServlet::doCreate(Eayew::Session::ptr session, Eayew::Message&& msg) {
    LOG(INFO) << "doCreate begin...";
    LoginProtocol::C2S_LoginCreate req;
    if (!req.ParseFromArray(msg.realData(), msg.realSize())) {
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
        msg.forceSetRoleId(role_id);

        PublicProtocol::BaseRoleInfo bri;
        bri.set_role_id(role_id);
        bri.set_name(req.role_name());
        bri.set_avatarurl(req.avatarurl());
        std::string serial;
        bri.SerializeToString(&serial);
        ServerResource::get()->redisMgr()->set("base_role_info", serial);
    } while(false);

    session->send(std::move(msg), resp);
    LOG(INFO) << "doCreate end...";
    return true;
}

bool LoginServlet::doLoad(Eayew::Session::ptr session, Eayew::Message&& msg) {
    LOG(INFO) << "doLoad begin...";
    LOG(WARNING) << msg.strInfo();
    LoginProtocol::C2S_LoginLoad req;
    if (!req.ParseFromArray(msg.realData(), msg.realSize())) {
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

