#include "test/connection.h"

#include <libgo/libgo.h>

#include "log/glog.h"

int main(int argc, char* argv[]) {
    GLog glog(argv[0]);

    std::string ip = "127.0.0.1";
    int port = 9001;
    auto con = std::make_shared<Connection>(ip, port);
    con->run();

    std::string msg("123");
    con->sync_write(1001, 2, msg);

    co_sched.Start();

    return 0;
}