#include "chat_servlet.h"

#include "log/glog.h"

#include "core/redis/redis_manager.h"
#include "core/util/util.h"

#include "logic/common/redis_key.h"
#include "logic/protocol/public.pb.h"

#include "logic/chat/protocol/chat_id.pb.h"
#include "logic/chat/protocol/chat.pb.h"


bool ChatServlet::doRequest(Eayew::Session::ptr session, Eayew::Message&& msg) {
    auto id = msg.realMsgId();
    switch (id) {
        case ChatProtocol::ID::C2S_CHAT_LOAD:
            return doLoad(session, std::move(msg));
        case ChatProtocol::ID::C2S_CHAT_CHAT:
            return doChat(session, std::move(msg));
        default:
            LOG(ERROR) << "invalid id " << id;
            return true;
    }

    return true;
}

bool ChatServlet::doLoad(Eayew::Session::ptr session, Eayew::Message&& msg) {
    LOG(INFO) << "doLoad begin...";
    ChatProtocol::C2S_ChatLoad req;
    if (!req.ParseFromArray(msg.pdata(), msg.psize())) {
        LOG(ERROR) << "ParseFromArray fail";
        return false;
    }

    ChatProtocol::S2C_ChatLoad resp;
    do {
    } while (false);
    
    session->send(std::move(covertRspMsg(msg, resp)));
    LOG(INFO) << "doLoad end...";
    return true;
}

bool ChatServlet::doChat(Eayew::Session::ptr session, Eayew::Message&& msg) {
    LOG(INFO) << "doChat begin...";
    ChatProtocol::C2S_ChatChat req;
    if (!req.ParseFromArray(msg.pdata(), msg.psize())) {
        LOG(ERROR) << "ParseFromArray fail";
        return false;
    }
    LOG(INFO) << "req " << req.DebugString();

    ChatProtocol::S2C_ChatChat resp;
    do {
    } while(false);

    session->send(std::move(covertRspMsg(msg, resp)));
    LOG(INFO) << "doChat end...";
    return true;
}
