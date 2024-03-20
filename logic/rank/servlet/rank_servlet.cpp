#include "rank_servlet.h"

#include <memory>

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

bool RankServlet::doRequest(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    auto id = msg->realMsgId();
    switch (id) {
        case RankProtocol::ID::C2S_RANK_LOAD:
            return doLoad(session, msg);
        case RankProtocol::ID::C2S_RANK_UPDATE:
            return doUpdate(session, msg);
        case RankProtocol::ID::C2S_RANK_MYSELF:
            return doMyself(session, msg);
        default:
            LOG(ERROR) << "invalid id " << id;
            return true;
    }

    return true;
}

bool RankServlet::doLoad(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG(INFO) << "doLoad begin...";
    RankProtocol::C2S_RankLoad req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        LOG(ERROR) << "ParseFromArray fail";
        return false;
    }
    RankProtocol::S2C_RankLoad resp;
    do {
        resp.set_subtype(req.subtype());
        auto roleid = msg->roleId();
        auto gameid = ServerResource::get()->redisMgr()->get<uint32_t>(RoleIdToGameIdSetKey(roleid));
        auto rankkey = RankZsetKey(gameid, req.subtype());
        auto scores = ServerResource::get()->redisMgr()->zrevrange<uint64_t, uint32_t>(rankkey, 0, 100);
        std::set<std::string> keys;
        for (const auto& val : scores) {
            LOG(INFO) << "rank data id " << val.first << " score " << val.second;
            keys.insert(BaseRoleInfoSetKey(val.first));
        }
        auto index = 0;
        std::map<uint64_t, std::shared_ptr<PublicProtocol::BaseRoleInfo>> bris;
        auto roles = ServerResource::get()->redisMgr()->mget<std::string>(keys);
        for (auto& role : roles) {
            auto bri = std::make_shared<PublicProtocol::BaseRoleInfo>();
            if (!bri->ParseFromString(role)) {
                LOG(WARNING) << "parseFromeString fail";
                continue;
            }
            bris[bri->role_id()] = bri;
        }

        for (const auto& val: scores) {
            auto bri = bris[val.first];
            if (!bri) {
                LOG(WARNING) << "Invalid id " << val.first;
                continue;
            }
            auto ri = resp.add_ris();
            ri->set_role_id(bri->role_id());
            ri->set_rank(++index);
            ri->set_name(bri->name());
            ri->set_avatarurl(bri->avatarurl());
            ri->set_score(val.second);   // ?
            // LOG(INFO) << "ri " << ri->DebugString();
        }

        {
            auto rank = ServerResource::get()->redisMgr()->zrevrank<uint32_t, uint64_t>(rankkey, roleid);
            auto score = ServerResource::get()->redisMgr()->zscore(rankkey, roleid);
            auto myself = resp.mutable_myself();
            myself->set_rank(rank + 1);
            myself->set_score(score);
        }
    } while(false);
    // LOG(INFO) << "resp " << resp.DebugString();

    session->send(covertRspMsg(msg, resp));
    LOG(ERROR) << "doLoad end...";
    return true;
}


bool RankServlet::doUpdate(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG(INFO) << "doUpdate begin...";
    RankProtocol::C2S_RankUpdate req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        LOG(ERROR) << "ParseFromArray fail";
        return false;
    }

    RankProtocol::S2C_RankUpdate resp;
    do {
        auto roleid = msg->roleId();
        auto key = RoleIdToGameIdSetKey(roleid);
        LOG(INFO) << "key " << key;
        auto gameid = ServerResource::get()->redisMgr()->get<uint32_t>(key);
        auto score = ServerResource::get()->redisMgr()->zscore(RankZsetKey(gameid, req.subtype()), roleid);
        LOG(INFO) << "gameid " << gameid << " roleid " << roleid << " subtype " << req.subtype() << " origin score " << score << " cur score " << req.score();
        if (req.score() > score) {
            ServerResource::get()->redisMgr()->zadd(RankZsetKey(gameid, req.subtype()), req.score(), roleid);
        }
    } while(false);

    // LOG(INFO) << "resp " << resp.DebugString();
    session->send(covertRspMsg(msg, resp));
    LOG(ERROR) << "doUpdate end...";
    return true;
}

bool RankServlet::doMyself(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG(INFO) << "doMyself begin...";
    RankProtocol::C2S_RankMyself req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        LOG(ERROR) << "ParseFromArray fail";
        return false;
    }
    RankProtocol::S2C_RankMyself resp;
    do {
		return true;
        auto roleid = msg->roleId();
        auto gameid = ServerResource::get()->redisMgr()->get<uint32_t>(RoleIdToGameIdSetKey(roleid));
        if (roleid == 0 || gameid != 3) {
            LOG(INFO) << "gameid " << gameid << " roleid " << roleid;
            break;
        }
        for (uint32_t i = 1; i <= 3; ++i) {
        auto rankkey = RankZsetKey(gameid, i);
            auto rank = ServerResource::get()->redisMgr()->zrevrank<uint32_t, uint64_t>(rankkey, roleid);
            auto score = ServerResource::get()->redisMgr()->zscore(rankkey, roleid);
            auto myself = resp.add_myself();
            myself->set_rank(rank + 1);
            myself->set_score(score);
        }
    } while(false);
    LOG(INFO) << "resp " << resp.DebugString();

    session->send(covertRspMsg(msg, resp));
    LOG(ERROR) << "doMyself end...";
    return true;
}
