#include "rank_server.h"

#include "log/glog.h"

#include "core/base/timer_manager.h"
#include "core/redis/redis_manager.h"
#include "core/servlet.h"

#include "logic/common/redis_key.h"
#include "logic/rank/protocol/rank_id.pb.h"
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
    ServerResource::get()->timerMgr()->addDailyTimer(0, 0, 0, [] {
        std::set<std::string> keys;
        auto results = ServerResource::get()->redisMgr()->hgetall<int32_t, std::string>("pcs_game_info_hash");
        for (auto [key, val] : results) {
            keys.insert(RankZsetKey(key, 1));
        }
        ServerResource::get()->redisMgr()->del<std::string>(keys);
        LOG(INFO) << "Del rank complete";
    });
}