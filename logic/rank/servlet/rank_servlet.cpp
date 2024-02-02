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
        auto roleid = msg.roleId();
        auto gameid = ServerResource::get()->redisMgr()->get<uint32_t>(RoleIdToGameIdSetKey(roleid));
        auto rankkey = RankZsetKey(gameid, req.subtype());
        auto scores = ServerResource::get()->redisMgr()->zrevrange<uint64_t, uint32_t>(rankkey, 0, 100);
        std::set<std::string> keys;
        for (auto [id, score] : scores) {
            LOG(INFO) << "rank data id " << id << " score " << score;
            keys.insert(BaseRoleInfoSetKey(id));
        }
        auto index = 0;
        auto roles = ServerResource::get()->redisMgr()->mget<std::string>(keys);
        for (auto& role : roles) {
            PublicProtocol::BaseRoleInfo bri;
            if (!bri.ParseFromString(role)) {
                LOG(WARNING) << "parseFromeString fail";
                continue;
            }

            auto ri = resp.add_ris();
            ri->set_role_id(bri.role_id());
            ri->set_rank(++index);
            ri->set_name(bri.name());
            ri->set_avatarurl(bri.avatarurl());
            ri->set_score(scores[bri.role_id()]);   // ?
            LOG(INFO) << "ri " << ri->DebugString();
        }

        {
            auto rank = ServerResource::get()->redisMgr()->zrevrank<uint32_t, uint64_t>(rankkey, roleid);
            auto score = ServerResource::get()->redisMgr()->zscore(rankkey, roleid);
            auto myself = resp.mutable_myself();
            myself->set_rank(rank + 1);
            myself->set_score(score);
        }
    } while(false);
    LOG(INFO) << "resp " << resp.DebugString();

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
        auto roleid = msg.roleId();
        auto gameid = ServerResource::get()->redisMgr()->get<uint32_t>(RoleIdToGameIdSetKey(roleid));
        auto score = ServerResource::get()->redisMgr()->zscore(RankZsetKey(gameid, req.subtype()), roleid);
        if (req.score() > score) {
            ServerResource::get()->redisMgr()->zadd(RankZsetKey(gameid, req.subtype()), req.score(), roleid);
        }
    } while(false);

    LOG(INFO) << "resp " << resp.DebugString();
    session->send(std::move(covertRspMsg(msg, resp)));
    LOG(ERROR) << "doUpdate end...";
    return true;
}

