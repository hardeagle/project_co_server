#include "base_server.h"

#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <libgo/libgo.h>

#include "core/message.hpp"
#include "core/servlet.h"
#include "core/rpc/rpc_manager.h"
#include "core/util/util.h"

#include "log/glog.h"

#include "base_routine.h"
#include "work_routine.h"
#include "gate_server_session.h"
#include "rpc_server_session.h"

namespace Eayew {

BaseServer::BaseServer()
    : m_agent(m_consul)
    , m_kv(m_consul) {
    m_servlet = std::make_shared<ServletDispatchRange>();
    m_workScheduler = co::Scheduler::Create();
}

void BaseServer::run() {
    beforeRun();

    m_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = inet_addr(m_ip.data());
    socklen_t len = sizeof(addr);
    if (-1 == bind(m_fd, (sockaddr*)&addr, len)) {
        LOG(ERROR) << "bind fail, port " << m_port;
        return;
    }
    if (-1 == listen(m_fd, 2048)) {
        LOG(ERROR) << "listen fail";
        return;
    }

    LOG(INFO) << "listen success, type " << m_type << " ip " << m_ip << " port " << m_port;

    go [&] {
        for(;;) {
            int fd = accept(m_fd, (sockaddr*)&addr, &len);
            if (-1 == fd) {
                if (EAGAIN == errno || EINTR == errno) {
                    continue;
                }
                LOG(ERROR) << "accept error";
                return;
            }

            const int len = Message::HEAD_LEN;
            char buf[len];
            int rlen = read(fd, buf, len);
            if (rlen != len) {
                LOG(ERROR) << "Invalid rpc connect, rlen " << rlen << " ,fd " << fd;
                continue;
            }
            uint16_t body_size = *((uint16_t*)buf);
            LOG(INFO) << "rlen " << rlen;
            LOG(INFO) << "size " << body_size;
            uint16_t sender_type = *((uint16_t*)&buf[2]);
            uint16_t receiver_type = *((uint16_t*)&buf[4]);
            if (receiver_type != type()) {
                LOG(ERROR) << "Invalid server type " << receiver_type << " type " << type();
                continue;
            }

            LOG(INFO) << "accept successs, fd " << fd << " sender type " << sender_type << " receiver type " << receiver_type << " buf " << buf;

            if (1 == sender_type) {
                auto ss = std::make_shared<GateServerSession>(fd);
                ss->senderType(sender_type);
                ss->receiverType(receiver_type);
                ss->setOnMessage([&, ss](Message&& msg) {
                    LOG(INFO) << "onMessage";
                    auto id = msg.sessionId();
                    auto it = m_workRoutines.find(id);
                    if (it == m_workRoutines.end()) {
                        auto routine = std::make_shared<WorkRoutine>(id);
                        routine->setOnMessage([&, ss](Message&& msg) {
                            m_servlet->doRequest(ss, std::move(msg));
                        });
                        m_workRoutines[id] = routine;
                        go co_scheduler(m_workScheduler) [routine] {
                            routine->run();
                        };
                    }
                    m_workRoutines[id]->push(std::move(msg));
                });
                ss->setOnClose([]() {
                    LOG(INFO) << "onClose";
                });
                m_gateSessions[sender_type] = ss;
                ss->run();
            } else {
                auto ss = std::make_shared<RpcServerSession>(fd);
                ss->senderType(sender_type);
                ss->receiverType(receiver_type);
                m_rpcSessions[sender_type] = ss;
                ss->run();
            }

        }
    };


    m_workThreads.emplace_back([this, self = shared_from_this()] {
        m_workScheduler->Start(); 
    });

    consulServer();

    // for (auto& t : m_workThreads) {
    //     t.detach();
    // }

    co_sched.Start(2);
}

void BaseServer::rpcDispatch(std::string& msg) {
    int id;
    std::string buffer;
    std::unordered_map<int, BaseRoutine::ptr>::iterator it = m_baseRoutines.find(id);
    if (it == m_baseRoutines.end()) {
        m_baseRoutines[id] = std::make_shared<BaseRoutine>(*this);
        m_baseRoutines[id]->run();
    }
    *(m_baseRoutines[id]) << buffer;
}

void BaseServer::initByConfig(const std::string& file) {
    boost::property_tree::ptree root;
    boost::property_tree::read_json(file, root);
    m_name = root.get<std::string>("name");
    m_type = root.get<int>("type");
    m_ip = root.get<std::string>("ip");
    m_port = root.get<int>("port");
    m_serverId = serverId(m_name, m_type, m_ip, m_port);

    m_rpcManager = std::make_shared<RpcManager>(m_type);
    m_rpcManager->init(file);
}

void BaseServer::consulServer() {
    m_agent.registerService(std::to_string(m_type),
        ppconsul::agent::TcpCheck{m_ip, m_port, std::chrono::seconds(10), std::chrono::milliseconds(1)},
        ppconsul::agent::kw::deregisterCriticalServiceAfter = std::chrono::minutes(1),
        ppconsul::agent::kw::address = m_ip,
        ppconsul::agent::kw::port = m_port,
        ppconsul::agent::kw::id = m_serverId
    );

    // m_timer.ExpireAt(std::chrono::seconds(10), [this, self = shared_from_this()] {
    //     discoverServer();
    // });
}

void BaseServer::discoverServer() {
    // auto servers = m_agent.services();
    // for (auto [id, si] : servers) {
    //     LOG(INFO) << "discoverServer id " << id << " name " << si.name;
    //     if (id == m_serverId) {
    //         continue;
    //     }
    //     uint16_t st = std::atoi(si.name.data());
    //     auto it = m_gpSessions.find(st);
    //     if (it != m_gpSessions.end()) {
    //         auto it1 = it->second.find(id);
    //         if (it1 != it->second.end()) {
    //             LOG(INFO) << "exist,  id " << si.id;
    //             continue;
    //         }
    //     }
    //     auto gps = std::make_shared<GatePeerSession>(si.address, si.port, *this);
    //     LOG(INFO) << "config self type " << type() << " rpc type " << st;
    //     gps->senderType(type());
    //     gps->receiverType(st);
    //     gps->run();
    //     m_gpSessions[st][si.id] = gps;
    // }

    // m_timer.ExpireAt(std::chrono::seconds(60), [this, self = shared_from_this()] {
    //     discoverServer();
    // });
}

}