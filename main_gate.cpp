
#include <memory>

#include "core/gate/gate_server.h"

#include "log/glog.h"

int main(int argc, char* argv[]) {
    // GLog glog(argv[0]);
    flag::set_value("cout", "true");
    flag::set_value("log_daily", "true");
    flag::set_value("max_log_file_num", "32");
    flag::parse(argc, argv);

    auto gate_server = std::make_shared<Eayew::GateServer>();
    gate_server->run();

    return 0;
}