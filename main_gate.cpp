
#include <memory>

#include "core/gate/gate_server.h"

#include "log/glog.h"

int main(int argc, char* argv[]) {
    GLog glog(argv[0]);

    auto gate_server = std::make_shared<Eayew::GateServer>();
    gate_server->run();

    return 0;
}