#ifndef __LOGIC_GAME_INFO_MANAGER_H__
#define __LOGIC_GAME_INFO_MANAGER_H__

#include <map>
#include <memory>

#include "logic/login/protocol/login.pb.h"

class GameInfo {
public:
    using ptr = std::shared_ptr<GameInfo>;

    int32_t gamid() { return m_proto.gameid(); }
    int32_t platform() { return m_proto.platform(); }
    const std::string& name() { return m_proto.name(); }
    const std::string& appid() { return m_proto.appid(); }
    const std::string& secret() { return m_proto.secret(); }

    LoginProtocol::GameInfo& proto() { return m_proto; }

private:
    LoginProtocol::GameInfo m_proto;
};

class GameInfoManager {
public:
    using ptr = std::shared_ptr<GameInfoManager>;

    void init();

    GameInfo::ptr get(uint32_t gameid);
    void add(uint32_t gameid, GameInfo::ptr gi);

private:
    std::map<uint32_t, GameInfo::ptr> m_gameinfos;
};


#endif