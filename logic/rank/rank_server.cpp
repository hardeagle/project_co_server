#include "rank_server.h"

#include "log/glog.h"

#include "core/base/timer_manager.h"
#include "core/const.hpp"
#include "core/redis/redis_manager.h"
#include "core/servlet.h"

#include "logic/common/redis_key.h"
#include "logic/protocol/public.pb.h"
#include "logic/rank/protocol/rank_id.pb.h"
#include "logic/rank/protocol/rank.pb.h"
#include "logic/rank/servlet/rank_servlet.h"
#include "logic/rank/server_resource.h"

void RankServer::beforeRun() {
    initByConfig("./json/login_server.json");

    ServerResource::get()->init(timer());

    initServlet();

    initTimer();
}

void RankServer::initServlet() {
    Eayew::Servlet::ptr tmp_servlet;
    tmp_servlet.reset(new RankServlet);
    servlet()->addServlet(RankProtocol::ID::C2S_RANK_BASE, RankProtocol::ID::C2S_RANK_TOP, tmp_servlet);
}

void RankServer::initTimer() {
    ServerResource::get()->timerMgr()->addDailyTimer(23, 59, 59, [] {
        std::set<std::string> keys;
        auto results = ServerResource::get()->redisMgr()->hgetall<int32_t, std::string>("pcs_game_info_hash");
        for (auto [key, val] : results) {
            keys.insert(RankZsetKey(key, 1));
        }
        ServerResource::get()->redisMgr()->del<std::string>(keys);
        LOG(INFO) << "Del rank complete";
    });

    ServerResource::get()->timerMgr()->addIntervalTimer(30000, [&] {
        notifyTodayRank();
    }, false);

    ServerResource::get()->timerMgr()->addIntervalTimer(90000, [&] {
        notifyLevelRank();
    }, false);

    ServerResource::get()->timerMgr()->addIntervalTimer(150000, [&] {
        notifyScoreRank();
    }, false);
}

void RankServer::notifyRank(uint32_t gameid, uint32_t subtype) {
    LOG(INFO) << "Rank Notify";
    auto rankkey = RankZsetKey(gameid, subtype);
    LOG(INFO) << "rankkey " << rankkey;
    auto scores = ServerResource::get()->redisMgr()->zrevrange<uint64_t, uint32_t>(rankkey, 0, 2);
    std::set<std::string> keys;
    for (const auto& val : scores) {
        // LOG(INFO) << "rank data id " << val.first << " score " << val.second;
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

    RankProtocol::S2C_RankNotify ntf;
    for (const auto& val: scores) {
        auto bri = bris[val.first];
        if (!bri) {
            LOG(WARNING) << "Invalid id " << val.first;
            continue;
        }
        auto ri = ntf.add_ris();
        ri->set_role_id(bri->role_id());
        ri->set_rank(++index);
        ri->set_name(bri->name());
        ri->set_avatarurl(bri->avatarurl());
        ri->set_score(val.second);   // ?
        // LOG(INFO) << "ri " << ri->DebugString();
    }

    auto s = getSession(uint32_t(Eayew::ServerType::EST_GATE));
    if (s) {
        auto nsize = ntf.ByteSizeLong();
        Eayew::Message msg(nsize);
        ntf.SerializeToArray(msg.pdata(), nsize);

        uint16_t msgid = RankProtocol::S2C_RANK_NOTIFY & 0XFFFF;
        msg.msgId(msgid);
        msg.roleId(Eayew::MsgType::EMT_NOTIFY_ROLE_ID); 
        msg.sessionId(Eayew::MsgType::EMT_NOTIFY_SESSION_ID);
        msg.senderId(Eayew::ServerType::EST_GATE);
        msg.receiverId(Eayew::ServerType::EST_RANK);
        // LOG(INFO) << "notify msg " << msg.strInfo();
        s->send(std::move(msg));
    }
}

void RankServer::notifyTodayRank() {
    notifyRank(3, 1);
    ServerResource::get()->timerMgr()->addIntervalTimer(180000, [&] {
        notifyTodayRank();
    }, false);
}

void RankServer::notifyLevelRank() {
    notifyRank(3, 2);
    ServerResource::get()->timerMgr()->addIntervalTimer(180000, [&] {
        notifyLevelRank();
    }, false);
}

void RankServer::notifyScoreRank() {
    notifyRank(3, 3);
    ServerResource::get()->timerMgr()->addIntervalTimer(180000, [&] {
        notifyScoreRank();
    }, false);
}
