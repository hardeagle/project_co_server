#include <memory>

#include "logic/login/login_server.h"

#include <co/all.h>

int main(int argc, char* argv[]) {
    flag::set_value("log_daily", "true");
    flag::parse(argc, argv);

    auto server = std::make_shared<LoginServer>();
    server->run();

    return 0;
}