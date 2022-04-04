#include <memory>

#include "logic/game/game_server.h"

#include "log/glog.h"

int main(int argc, char* argv[]) {
    GLog glog(argv[0]);

    auto game_server = std::make_shared<GameServer>();
    game_server->run();

    return 0;
}