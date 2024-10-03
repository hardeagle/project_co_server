
#include <memory>

#include "core/gate/gate_server.h"

#include <co/all.h>

int main(int argc, char* argv[]) {
    flag::set_value("log_daily", "true");
    flag::parse(argc, argv);

    auto gate_server = std::make_shared<Eayew::GateServer>();
    gate_server->run();

    return 0;
}