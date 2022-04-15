#include "test/connection.h"

//#include <boost/lexical_cast.hpp>

#include <libgo/libgo.h>

#include "log/glog.h"

int main(int argc, char* argv[]) {
    GLog glog(argv[0]);

    std::string ip = "127.0.0.1";
    int port = 9001;
    auto con = std::make_shared<Connection>(ip, port);
    con->run();

    for (int i = 0; i < 1000; ++i) {
        std::string data("msg_");
        data += std::to_string(i);
        if (i & 0x01) {
            con->sync_write(1001, 2, data);
        } else {
            con->sync_write(1002, 3, data);
        }
        co_sleep(100);
    }

    // std::string msg("123");
    // con->sync_write(1001, 2, msg);

    // con->sync_write(1002, 3, msg);

    // con->sync_write(1003, 2, msg);

    // con->sync_write(1004, 3, msg);

    co_sched.Start(2);

    return 0;
}