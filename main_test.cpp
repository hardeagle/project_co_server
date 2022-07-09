#include "test/connection.h"

//#include <boost/lexical_cast.hpp>

#include <libgo/libgo.h>

#include <jemalloc/jemalloc.h>

#include "log/glog.h"

#include <sys/socket.h>
#include <arpa/inet.h>


int main(int argc, char* argv[]) {
    GLog glog(argv[0]);

    // std::list<Connection::ptr> cons;

    // std::list<int> fds;

    // for (int i = 0; i < 50000; ++i) {
    //     go [&] {
    //         std::string ip = "127.0.0.1";
    //         int port = 9001;
    //         // auto con = std::make_shared<Connection>(ip, port);
    //         // con->run();

    //         //cons.push_back(con);

    //         int fd = socket(AF_INET, SOCK_STREAM, 0);
    //         sockaddr_in addr;
    //         addr.sin_family = AF_INET;
    //         addr.sin_port = htons(port);
    //         addr.sin_addr.s_addr = inet_addr(ip.data());
    //         if (-1 == connect(fd, (sockaddr*)&addr, sizeof(addr))) {
    //             LOG(ERROR) << "connect fail, port " << port;
    //             return;
    //         }

    //         LOG(INFO) << "connect success,  fd " << fd;
    //         fds.push_back(fd);


    //     };
    // }

    // for (int i = 0; i < 1000; ++i) {
    //     std::string data("msg_");
    //     data += std::to_string(i);
    //     if (i & 0x01) {
    //         con->sync_write(1001, 2, data);
    //     } else {
    //         con->sync_write(1002, 3, data);
    //     }
    //     co_sleep(100);
    // }

    std::string ip = "127.0.0.1";
    int port = 9101;
    auto con = std::make_shared<Connection>(ip, port);
    con->run();

    std::string msg("123");
    con->sync_write(1001, 2, msg);

    con->sync_write(1002, 3, msg);

    con->sync_write(1003, 2, msg);

    con->sync_write(1004, 3, msg);

    co_sched.Start();

    return 0;
}