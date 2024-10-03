#include "game_info_manager.h"

#include <co/all.h>

#include "core/redis/redis_manager.h"
#include "logic/login/server_resource.h"
#include "logic/protocol/login.pb.h"


void GameInfoManager::init() {
    auto redis_mgr = ServerResource::get()->redisMgr();
    auto results = redis_mgr->hgetall<int32_t, std::string>("pcs_game_info_hash");
    for (const auto& pair : results) {
        GameInfo::ptr gi = std::make_shared<GameInfo>();
        auto res = gi->proto().ParseFromString(pair.second);
        if (!res) {
            ELOG << "ParseFromString fail";
            break;
        }
        LOG << "pcs_game_info_hash game id " << pair.first << " val " << pair.second;
        m_gameinfos[pair.first] = gi;
    }
}

GameInfo::ptr GameInfoManager::get(uint32_t gameid) {
    auto it = m_gameinfos.find(gameid);
    return it != m_gameinfos.end() ? it->second : GameInfo::ptr();
}