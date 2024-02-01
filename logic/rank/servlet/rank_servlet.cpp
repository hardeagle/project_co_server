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
#include "logic/rank/server_resource.h"

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
        auto gameid = ServerResource::get()->redisMgr()->get<uint32_t>(RoleIdToGameIdSetKey(msg.roleId()));
        auto scores = ServerResource::get()->redisMgr()->zrevrange<uint64_t, uint32_t>(RankZsetKey(gameid), 0, 100);
        std::set<std::string> keys;
        for (auto [id, score] : scores) {
            LOG(INFO) << "rank data id " << id << " score " << score;
            keys.insert(BaseRoleInfoSetKey(id));
        }
        auto roles = ServerResource::get()->redisMgr()->mget<std::string>(keys);
        for (auto& role : roles) {
            PublicProtocol::BaseRoleInfo bri;
            if (!bri.ParseFromString(role)) {
                LOG(WARNING) << "parseFromeString fail";
                continue;
            }

            auto rri = resp.add_rris();
            rri->set_role_id(bri.role_id());
            rri->set_name(bri.name());
            rri->set_avatarurl(bri.avatarurl());
            rri->set_rank(scores[bri.role_id()]);   // ?
            LOG(INFO) << "rri " << rri->DebugString();
        }
    } while(false);

    session->send(std::move(covertRspMsg(msg, resp)));
    LOG(ERROR) << "doLoad end... resp " << resp.DebugString();
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
        auto roleid = msg.roleId();
        auto gameid = ServerResource::get()->redisMgr()->get<uint32_t>(RoleIdToGameIdSetKey(roleid));
        ServerResource::get()->redisMgr()->zadd(RankZsetKey(gameid), req.score(), roleid);
    } while(false);

    LOG(INFO) << "resp " << resp.DebugString();
    session->send(std::move(covertRspMsg(msg, resp)));
    LOG(ERROR) << "doUpdate end...";
    return true;
}

