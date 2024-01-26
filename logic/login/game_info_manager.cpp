#include "game_info_manager.h"

#include "log/glog.h"

#include "core/redis/redis_manager.h"
#include "logic/login/server_resource.h"
#include "logic/login/protocol/login.pb.h"


void GameInfoManager::init() {
    auto redis_mgr = ServerResource::get()->redisMgr();
    auto results = redis_mgr->hgetall<int32_t, std::string>("pcs_game_info_hash");
    for (auto [key, val] : results) {
        GameInfo::ptr gi = std::make_shared<GameInfo>();
        auto res = gi->proto().ParseFromString(val);
        if (!res) {
            LOG(ERROR) << "ParseFromString fail";
            break;
        }
        LOG(INFO) << "pcs_game_info_hash game id " << key << " val " << val;
        m_gameinfos[key] = gi;
    }
}

GameInfo::ptr GameInfoManager::get(uint32_t gameid) {
    auto it = m_gameinfos.find(gameid);
    return it != m_gameinfos.end() ? it->second : GameInfo::ptr();
}