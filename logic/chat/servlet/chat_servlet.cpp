#include "chat_servlet.h"

#include "log/glog.h"

#include "core/redis/redis_manager.h"
#include "core/util/util.h"

#include "logic/common/redis_key.h"
#include "logic/protocol/public.pb.h"

#include "logic/protocol/chat_id.pb.h"
#include "logic/protocol/chat.pb.h"


bool ChatServlet::doRequest(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    auto id = msg->realMsgId();
    switch (id) {
        case ChatProtocol::ID::C2S_CHAT_LOAD:
            return doLoad(session, msg);
        case ChatProtocol::ID::C2S_CHAT_CHAT:
            return doChat(session, msg);
        default:
            ELOG << "invalid id " << id;
            return true;
    }

    return true;
}

bool ChatServlet::doLoad(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doLoad begin...";
    // ChatProtocol::C2S_ChatLoad req;
    // if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
    //     ELOG << "ParseFromArray fail";
    //     return false;
    // }

    // ChatProtocol::S2C_ChatLoad resp;
    // do {
    // } while (false);
    
    // session->send(covertRspMsg(msg, resp));
    LOG << "doLoad end...";
    return true;
}

bool ChatServlet::doChat(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doChat begin...";
    // ChatProtocol::C2S_ChatChat req;
    // if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
    //     ELOG << "ParseFromArray fail";
    //     return false;
    // }
    // LOG << "req " << req.DebugString();

    // ChatProtocol::S2C_ChatChat resp;
    // do {
    // } while(false);

    // session->send(covertRspMsg(msg, resp));
    LOG << "doChat end...";
    return true;
}
