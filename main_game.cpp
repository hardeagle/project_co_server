#include <memory>

#include "logic/game/game_server.h"

#include <co/all.h>

int main(int argc, char* argv[]) {
    auto game_server = std::make_shared<GameServer>();
    game_server->run();

    return 0;
}