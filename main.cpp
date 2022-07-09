

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <jemalloc/jemalloc.h>

#include <libgo/libgo.h>

#include "log/glog.h"

#include <time.h>

#include <core/util/util.h>

#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    GLog glog(argv[0]);

    LOG(INFO) << "---begin---";

    // for (int i = 0; i < 10; ++i) {
    //     go [=] {
    //         LOG(ERROR) << i;
    //     };
    // }

    // boost::property_tree::ptree root;
    // boost::property_tree::read_json("gate.json", root);
    // boost::property_tree::ptree servers = root.get_child("servers");
    // BOOST_FOREACH(boost::property_tree::ptree::value_type& node, servers) {
    //     auto name = node.second.get<std::string>("name");
    //     auto type = node.second.get<int>("type");
    //     auto ip = node.second.get<std::string>("ip");
    //     auto port = node.second.get<int>("port");
    //     auto num = node.second.get<int>("num");
    //     LOG(ERROR) << "name: " << name;
    //     LOG(ERROR) << "type: " << type;
    //     LOG(ERROR) << "ip: " << ip;
    //     LOG(ERROR) << "port: " << port;
    //     LOG(ERROR) << "num: " << num;
    // }

    // time_t t = time(NULL);
    // LOG(WARNING) << "t " << t;

    // LOG(INFO) << "cur time " << Eayew::getCurSecond();

    std::string m_ip = "127.0.0.1";
    auto m_port = 9001;

    int accept_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(accept_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = inet_addr(m_ip.data());
    socklen_t len = sizeof(addr);
    if (-1 == bind(accept_fd, (sockaddr*)&addr, len)) {
        LOG(ERROR) << "bind error, port " << m_port;
        return 0;
    }
    if (-1 == listen(accept_fd, 2048)) {
        LOG(ERROR) << "listen error";
        return 0;
    }

    //std::list<int> fds;

    //co_opt.debug = 32;

    go [&] {
        for(;;) {
            int fd = accept(accept_fd, (sockaddr*)&addr, &len);
            if (-1 == fd) {
                if (EAGAIN == errno || EINTR == errno) {
                    continue;
                }
                LOG(ERROR) << "accept error";
                return;
            }

            LOG(INFO) << "accept success, fd " << fd;
            //fds.push_back(fd);

            // auto session = std::make_shared<GateSession>(m_id, fd, *this);
            // m_sessions[session->id()] = session;
            // session->run();

            //co::CoDebugger::getInstance().GetAllInfo().c_str();
        }
    };

    co_sched.Start(8, 8);

    LOG(INFO) << "---end---";

    return 0;
}