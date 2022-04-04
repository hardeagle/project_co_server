
#include "log/glog.h"

#include "logic/rank/rank_server.h"

int main(int argc, char* argv[]) {
    GLog glog(argv[0]);

    //auto rank_server = std::make_shared<Eayew::Test>();
    auto rank_server = std::make_shared<RankServer>();
    rank_server->run();

    return 0;
}