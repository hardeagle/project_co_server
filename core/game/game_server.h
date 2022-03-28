#ifndef __CORE_GAME_GAME_SERVER_H__
#define __CORE_GAME_GAME_SERVER_H__

#include <memory>
#include <unordered_map>

namespace Eayew {

class GameSession;

class GameServer {
public:
    using ptr = std::shared_ptr<GameServer>;

    void run();

private:
    std::unordered_map<int, std::shared_ptr<GameSession> > m_sessions;
};

}
#endif