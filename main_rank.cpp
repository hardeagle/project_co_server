
#include "log/glog.h"

#include "logic/rank/rank_server.h"

int main(int argc, char* argv[]) {
    // GLog glog(argv[0]);
    flag::set_value("cout", "true");
    flag::set_value("log_daily", "true");
    flag::set_value("max_log_file_num", "32");
    flag::parse(argc, argv);

    auto rank_server = std::make_shared<RankServer>();
    rank_server->run();

    return 0;
}