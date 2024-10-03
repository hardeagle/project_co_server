
#include <co/all.h>

#include "logic/rank/rank_server.h"

int main(int argc, char* argv[]) {
    flag::set_value("log_daily", "true");
    flag::parse(argc, argv);

    auto rank_server = std::make_shared<RankServer>();
    rank_server->run();

    return 0;
}