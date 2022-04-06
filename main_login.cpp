#include <memory>

#include "logic/login/login_server.h"

#include "log/glog.h"

int main(int argc, char* argv[]) {
    GLog glog(argv[0]);

    auto server = std::make_shared<LoginServer>();
    server->run();

    return 0;
}