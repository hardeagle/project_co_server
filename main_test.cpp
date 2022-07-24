#include "test/connection.h"

//#include <boost/lexical_cast.hpp>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <libgo/libgo.h>

#include <jemalloc/jemalloc.h>

#include "core/util/util.h"

#include "log/glog.h"

#include "logic/login/protocol/login.pb.h"


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

    // std::string ip = "127.0.0.1";
    // int port = 9101;
    // auto con = std::make_shared<Connection>(ip, port);
    // con->run();

    // {
    //     std::string msg("1");
    //     for (int i = 0; i < 6000; ++i) {
    //         msg += "1";
    //     }
    //     LOG(WARNING) << "msg " << msg;

    //     LoginProtocol::C2S_LoginLoad req;
    //     req.set_loginname(msg);
    //     std::string data;
    //     req.SerializeToString(&data);
    //     con->sync_write(1001, 2, data);
    // }

    // {
    //     std::string msg("1");
    //     for (int i = 0; i < 60; ++i) {
    //         msg += "1";
    //     }
    //     LOG(WARNING) << "msg " << msg;

    //     LoginProtocol::C2S_LoginLoad req;
    //     req.set_loginname(msg);
    //     std::string data;
    //     req.SerializeToString(&data);
    //     con->sync_write(1001, 2, data);
    // }

    std::list<Connection::ptr> cons;

    for (int i = 0; i < 1000; ++i) {

        go [&, i] {

            std::string ip = "127.0.0.1";
            int port = 9101;
            auto con = std::make_shared<Connection>(ip, port);
            con->run();
            cons.push_back(con);

            for (int j = 0; j < 10000; ++j) {
                std::string msg("1");
                for (int k = 0; k < 100; ++k) {
                    msg += "1";
                }
                //LOG(WARNING) << "msg " << msg;

                LoginProtocol::C2S_LoginLogin req;
                req.set_loginname(msg);
                std::string data;
                req.SerializeToString(&data);
                con->sync_write(1001, 2, data);

                co_sleep(200);
                //LOG(ERROR) << "send,  , fd " << con->fd() << " ,i " << i << " ,j " << j;
            }
        };
    }

    // {
    //     std::string msg("1");
    //     for (int i = 0; i < 1024000; ++i) {
    //         msg += "1";
    //     }
    //     LOG(WARNING) << "msg " << msg;

    //     LoginProtocol::C2S_LoginLoad req;
    //     req.set_loginname(msg);
    //     std::string data;
    //     req.SerializeToString(&data);
    //     con->sync_write(1001, 2, data);
    // }

    co_sched.Start(6);

    return 0;
}