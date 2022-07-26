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
    auto redis_mgr = ServerResource::get()->redisMgr();
    auto role_id = redis_mgr->get<uint64_t>(LoginNameToRoleIdKey(req.loginname()));
    if (role_id == 0) {
        LOG(INFO) << "new role";
        resp.set_ret(1);
        goto End;
    }
    resp.set_role_id(role_id);

End:
    session->send(std::move(msg));
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

    auto role_id = ServerResource::get()->idMgr()->generateId(IdManager::EID_ROLE_ID);
    if (role_id <= 0) {
        LOG(ERROR) << "general id fail, role id " << role_id;
    }
    // auto bri = PublicProtocol::BaseRoleInfo{
    //     role_id, req.role_name()
    // };

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

