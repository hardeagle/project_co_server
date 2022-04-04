

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <libgo/libgo.h>

#include "log/glog.h"


int main(int argc, char* argv[]) {
    GLog glog(argv[0]);

    LOG(INFO) << "---begin---";

    for (int i = 0; i < 10; ++i) {
        go [=] {
            LOG(ERROR) << i;
        };
    }

    boost::property_tree::ptree root;
    boost::property_tree::read_json("gate.json", root);
    boost::property_tree::ptree servers = root.get_child("servers");
    BOOST_FOREACH(boost::property_tree::ptree::value_type& node, servers) {
        auto name = node.second.get<std::string>("name");
        auto type = node.second.get<int>("type");
        auto ip = node.second.get<std::string>("ip");
        auto port = node.second.get<int>("port");
        auto num = node.second.get<int>("num");
        LOG(ERROR) << "name: " << name;
        LOG(ERROR) << "type: " << type;
        LOG(ERROR) << "ip: " << ip;
        LOG(ERROR) << "port: " << port;
        LOG(ERROR) << "num: " << num;
    }

    co_sched.Start();

    LOG(INFO) << "---end---";

    return 0;
}