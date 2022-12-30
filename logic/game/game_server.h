#ifndef __LOGIC_GAME_GAME_SERVER_H__
#define __LOGIC_GAME_GAME_SERVER_H__

#include "core/const.hpp"
#include "core/base/base_server.h"

class GameServer : public Eayew::BaseServer {
public:
    using ptr = std::shared_ptr<GameServer>;

    GameServer() : Eayew::BaseServer(Eayew::ServerType::GAME) {}

    virtual void beforeRun() override;

private:
    void initServlet();

private:

};

#endif