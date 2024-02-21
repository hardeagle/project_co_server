#include "rpc_manager.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <libgo/libgo.h>

#include "log/glog.h"

#include "core/message.h"

#include "rpc_session.h"

namespace Eayew {

RpcManager::RpcManager(int type)
    : m_type(type) { 
}

RpcSession::ptr RpcManager::getClient(int type) {
    auto it = m_rpcSessions.find(type);
    if (it != m_rpcSessions.end()) {
        return it->second;
    }
    // connect
    return RpcSession::ptr();
}

int RpcManager::nextRpcId() {
    if (++m_rpcId >= 0xFFFFFFF) {
        m_rpcId = 1;
    }
    return m_rpcId;
}

void RpcManager::init(const std::string& file) {
    // boost::property_tree::ptree root;
    // boost::property_tree::read_json(file, root);
    // boost::property_tree::ptree servers = root.get_child("servers");
    // BOOST_FOREACH (boost::property_tree::ptree::value_type& node, servers) {
    //     auto name = node.second.get<std::string>("name");
    //     auto type = node.second.get<int>("type");
    //     auto ip = node.second.get<std::string>("ip");
    //     auto port = node.second.get<int>("port");
    //     auto num = node.second.get<int>("num");
    //     for (int i = 0; i < num; ++i) {
    //         auto rs = std::make_shared<RpcSession>(ip, port);
    //         rs->senderType(this->type());
    //         rs->receiverType(type);
    //         rs->run();
    //         m_rpcSessions[type][rs->fd()] = rs;
    //     }
    // }
}

void RpcManager::call(int type, std::string& req) {
    RpcSession::ptr rs = getClient(type);
    if (!rs) {
        LOG(ERROR) << "getClient fail";
        return;
    }
    // Message msg(0);
    // rs->send(std::move(msg));
}

void RpcManager::call(int type, std::string& req, std::string& rsp) {
    RpcSession::ptr rs = getClient(type);
    if (!rs) {
        LOG(ERROR) << "getClient fail";
        return;
    }

    // int rpc_id = nextRpcId();
    // Message msg(0);
    // rs->send(std::move(msg));

    // co_chan<std::string> channel;
    // m_channels[rpc_id] = channel;
    // channel >> rsp;
    // m_channels.erase(rpc_id);
}

void RpcManager::dispatch(std::string& buf) {
    // parse buf
    int rpc_id = 0;
    std::string body;
    m_channels[rpc_id] << body;
}

}