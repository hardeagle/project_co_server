#include "rank_servlet.h"

#include <json/json.h>

#include "log/glog.h"

#include "core/redis/redis_manager.h"
#include "core/util/util.h"

#include "logic/common/error_code.h"
#include "logic/common/redis_key.h"
#include "logic/protocol/public.pb.h"

#include "logic/rank/protocol/rank_id.pb.h"
#include "logic/rank/protocol/rank.pb.h"

bool RankServlet::doRequest(Eayew::Session::ptr session, Eayew::Message&& msg) {
    auto id = msg.realMsgId();
    switch (id) {
        case RankProtocol::ID::C2S_RANK_LOAD:
            return doLoad(session, std::move(msg));
        case RankProtocol::ID::C2S_RANK_UPDATE:
            return doUpdate(session, std::move(msg));
        default:
            LOG(ERROR) << "invalid id " << id;
            return true;
    }

    return true;
}

bool RankServlet::doLoad(Eayew::Session::ptr session, Eayew::Message&& msg) {
    LOG(INFO) << "doLoad begin...";
    LOG(WARNING) << msg.strInfo();
    RankProtocol::C2S_RankLoad req;
    if (!req.ParseFromArray(msg.pdata(), msg.psize())) {
        LOG(ERROR) << "ParseFromArray fail";
        return false;
    }
    RankProtocol::S2C_RankLoad resp;
    do {

    } while(false);

    session->send(std::move(covertRspMsg(msg, resp)));
    LOG(ERROR) << "doLoad end...";
    return true;
}


bool RankServlet::doUpdate(Eayew::Session::ptr session, Eayew::Message&& msg) {
    LOG(INFO) << "doUpdate begin...";
    LOG(WARNING) << msg.strInfo();
    RankProtocol::C2S_RankUpdate req;
    if (!req.ParseFromArray(msg.pdata(), msg.psize())) {
        LOG(ERROR) << "ParseFromArray fail";
        return false;
    }

    RankProtocol::S2C_RankUpdate resp;
    do {
    } while(false);

    LOG(INFO) << "resp " << resp.DebugString();
    session->send(std::move(covertRspMsg(msg, resp)));
    LOG(ERROR) << "doUpdate end...";
    return true;
}

