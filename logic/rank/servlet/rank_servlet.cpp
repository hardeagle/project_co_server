#include "rank_servlet.h"

#include <memory>

#include <json/json.h>

#include "log/glog.h"

#include "core/redis/redis_manager.h"
#include "core/util/util.h"

#include "logic/common/error_code.h"
#include "logic/common/redis_key.h"
#include "logic/protocol/public.pb.h"

#include "logic/protocol/rank_id.pb.h"
#include "logic/protocol/rank.pb.h"
#include "logic/login/server_resource.h"

bool RankServlet::doRequest(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    if (msg->roleId() == 0) {
        ELOG << "fuck roleid zero";
        return true;
    }
    auto id = msg->realMsgId();
    switch (id) {
        case RankProtocol::ID::C2S_RANK_LOAD:
            return doLoad(session, msg);
        case RankProtocol::ID::C2S_RANK_UPDATE:
            return doUpdate(session, msg);
        case RankProtocol::ID::C2S_RANK_MYSELF:
            return doMyself(session, msg);
        default:
            ELOG << "invalid id " << id;
            return true;
    }

    return true;
}

bool RankServlet::doLoad(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doLoad begin...";
    RankProtocol::C2S_RankLoad req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }
    RankProtocol::S2C_RankLoad resp;
    do {
        resp.set_subtype(req.subtype());
        auto roleid = msg->roleId();
        auto gameid = ServerResource::get()->redisMgr()->get<uint32_t>(RoleIdToGameIdSetKey(roleid));
        LOG << "gameid " << gameid;
        if (gameid == 0) {
            LOG << "fuck gameid";
            break;
        }
        auto rankkey = RankZsetKey(gameid, req.subtype());
        auto scores = ServerResource::get()->redisMgr()->zrevrange<uint64_t, uint32_t>(rankkey, 0, 49);
        std::set<std::string> keys;
        for (const auto& val : scores) {
            LOG << "rank data id " << val.first << " score " << val.second;
            keys.insert(BaseRoleInfoSetKey(val.first));
        }
        auto index = 0;
        std::map<uint64_t, std::shared_ptr<PublicProtocol::BaseRoleInfo>> bris;
        auto roles = ServerResource::get()->redisMgr()->mget<std::string>(keys);
        for (auto& role : roles) {
            auto bri = std::make_shared<PublicProtocol::BaseRoleInfo>();
            if (!bri->ParseFromString(role)) {
                WLOG << "parseFromeString fail";
                continue;
            }
            bris[bri->role_id()] = bri;
        }

        for (const auto& val: scores) {
            auto bri = bris[val.first];
            if (!bri) {
                WLOG << "Invalid id " << val.first;
                continue;
            }
            auto ri = resp.add_ris();
            ri->set_role_id(bri->role_id());
            ri->set_rank(++index);
            ri->set_name(bri->name());
            ri->set_avatarurl(bri->avatarurl());
            ri->set_score(val.second);   // ?
            LOG << "ri " << ri->DebugString();
        }

        {
            LOG << "myself begin...";
            auto rank = ServerResource::get()->redisMgr()->zrevrank<uint32_t, uint64_t>(rankkey, roleid);
            auto score = ServerResource::get()->redisMgr()->zscore(rankkey, roleid);
            auto myself = resp.mutable_myself();
            myself->set_rank(rank + 1);
            myself->set_score(score);
            LOG << "myself end... rank " << rank << "score " << score;
        }
    } while(false);
    // LOG << "resp " << resp.DebugString();

    session->send(covertRspMsg(msg, resp));
    ELOG << "doLoad end...";
    return true;
}


bool RankServlet::doUpdate(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doUpdate begin...";
    RankProtocol::C2S_RankUpdate req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }

    RankProtocol::S2C_RankUpdate resp;
    do {
        auto roleid = msg->roleId();
        auto key = RoleIdToGameIdSetKey(roleid);
        LOG << "key " << key;
        auto gameid = ServerResource::get()->redisMgr()->get<uint32_t>(key);
        LOG << "gameid " << gameid;
        if (gameid == 0) {
            LOG << "fuck gameid";
            break;
        }
        auto rkey = RankZsetKey(gameid, req.subtype());
        LOG << "rkey " << rkey << " roleid " << roleid;
        auto score = ServerResource::get()->redisMgr()->zscore(rkey, roleid);
        LOG << "gameid " << gameid << " roleid " << roleid << " subtype " << req.subtype() << " origin score " << score << " cur score " << req.score();
        if (req.score() > score) {
            ServerResource::get()->redisMgr()->zadd(rkey, req.score(), roleid);
        }
    } while(false);

    // LOG << "resp " << resp.DebugString();
    session->send(covertRspMsg(msg, resp));
    ELOG << "doUpdate end...";
    return true;
}

bool RankServlet::doMyself(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doMyself begin...";
    RankProtocol::C2S_RankMyself req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }
    RankProtocol::S2C_RankMyself resp;
    do {
        resp.set_subtype(req.subtype());
        auto roleid = msg->roleId();
        auto gameid = ServerResource::get()->redisMgr()->get<uint32_t>(RoleIdToGameIdSetKey(roleid));
        LOG << "gameid " << gameid;
        if (gameid == 0) {
            LOG << "fuck gameid";
            break;
        }
        auto rankkey = RankZsetKey(gameid, req.subtype());
        auto rank = ServerResource::get()->redisMgr()->zrevrank<uint32_t, uint64_t>(rankkey, roleid);
        auto score = ServerResource::get()->redisMgr()->zscore(rankkey, roleid);
        LOG << "doMyself rank " << rank << " score " << score;
        resp.mutable_myself()->set_rank(rank + 1);
        resp.mutable_myself()->set_score(score);
    } while(false);
    LOG << "resp " << resp.DebugString();

    session->send(covertRspMsg(msg, resp));
    ELOG << "doMyself end...";
    return true;
}
