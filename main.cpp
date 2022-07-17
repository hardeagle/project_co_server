

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <jemalloc/jemalloc.h>

#include <memory>

#include <libgo/libgo.h>

#include "log/glog.h"

#include <time.h>

#include "core/util/util.h"

#include <sys/socket.h>
#include <arpa/inet.h>

static int s_limit = 20480;

// class Test : public std::enable_shared_from_this<Test> {
// public:
//     using ptr = std::shared_ptr<Test>;

//     Test(){
//     }

//     void init() {
//         m_sched = co::Scheduler::Create();
//         // std::thread t2([this, self = shared_from_this()]{ m_sched->Start(4); });
//         // t2.detach();
//         m_threads.emplace_back([this, self = shared_from_this()]{ m_sched->Start(4); });
//     }

//     int size() {
//         return m_msgs.size();
//     }

//     void push(int val) {
//         m_mutex.lock();
//         m_msgs.push(val);
//         if (m_msgs.size() == 1) {
//             LOG(ERROR) << "wake up begin...";
//             m_chan << true;
//             LOG(ERROR) << "wake up end...";
//         }
//         m_mutex.unlock();
//     }

//     void run() {
//         go co_scheduler(m_sched) [this, self = shared_from_this()] {
//             for (;;) {
//                 for (;;) {
//                     m_mutex.lock();
//                     if (m_msgs.size() == 0) {
//                         m_mutex.unlock();
//                         bool flag;
//                         LOG(ERROR) << "sleep begin....";
//                         m_chan >> flag;
//                         LOG(ERROR) << "sleep end....";
//                     } else {
//                         m_mutex.unlock();
//                     }
//                     break;
//                 }

//                 m_mutex.lock();
//                 m_msgs.pop();
//                 m_msgs.front();
//                 if (m_msgs.size() % 100000 == 0) {
//                     LOG(WARNING) << "+++consume m_msgs size " << m_msgs.size();
//                 }
//                 m_mutex.unlock();
//             }
//         };
//     }

// private:
//     co::Scheduler* m_sched;
//     std::list<std::thread> m_threads;

//     co_chan<bool> m_chan;
//     co_mutex m_mutex;
//     std::queue<int> m_msgs;

// };

class Point {
public:
    Point() {
    }

    Point(int x, int y, int z)
        : m_x(x)
        , m_y(y)
        , m_z(z) {
    }

    int m_x;
    int m_y;
    int m_z;

    std::string strInfo() {
        std::stringstream ss;
        ss << "m_x " << m_x << " ,m_y " << m_y << " ,m_z " << m_z;
        return ss.str();
    }
};


template<typename T>
class Test : public std::enable_shared_from_this<Test<T>> {
public:
    using ptr = std::shared_ptr<Test>;

    Test()
        : m_chan(s_limit) {
    }

    void init() {
        m_sched = co::Scheduler::Create();
        // std::thread t2([this, self = shared_from_this()]{ m_sched->Start(4); });
        // t2.detach();
        m_threads.emplace_back([this, self = this->template shared_from_this()]{ m_sched->Start(1); });
        m_threads[0].detach();
    }

    int size() {
        return m_chan.size();
    }

    void push(T val) {
        LOG(WARNING) << "---produce begin " << m_chan.size();
        if (m_chan.size() == s_limit) {
            LOG(ERROR) << "------------------------full";
        }
        m_chan << val;
        LOG(WARNING) << "---produce end " << m_chan.size();
    }

    void run() {
        go co_scheduler(m_sched) [this, self = this->template shared_from_this()] {
            for (;;) {
                LOG(WARNING) << "+++consume begin... " << m_chan.size();
                if (m_chan.size() == 0) {
                    LOG(ERROR) << "------------------------empty";
                }
                T val;
                m_chan >> val;
                LOG(INFO) << "val " << val.strInfo();

                LOG(WARNING) << "+++consume end... " << m_chan.size();
            }
        };
    }

private:
    co::Scheduler* m_sched;
    std::vector<std::thread> m_threads;

    co_chan<T> m_chan;
};

int main(int argc, char* argv[]) {
    GLog glog(argv[0]);

    LOG(INFO) << "---begin---";

    Test<Point>::ptr test = std::make_shared<Test<Point>>();
    test->init();
    test->run();

    for (int i = 0; i < 1000; ++i) {
        go [=] {
            for (int j = 0; j < 1000; ++j) {
                test->push(Point(i, j, i+j));
            }
            LOG(INFO) << "---produce m_msgs size " << test->size();
        };
    }


    co_sched.Start(4);

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

    // std::string m_ip = "127.0.0.1";
    // auto m_port = 9001;

    // int accept_fd = socket(AF_INET, SOCK_STREAM, 0);

    // int opt = 1;
    // setsockopt(accept_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // sockaddr_in addr;
    // addr.sin_family = AF_INET;
    // addr.sin_port = htons(m_port);
    // addr.sin_addr.s_addr = inet_addr(m_ip.data());
    // socklen_t len = sizeof(addr);
    // if (-1 == bind(accept_fd, (sockaddr*)&addr, len)) {
    //     LOG(ERROR) << "bind error, port " << m_port;
    //     return 0;
    // }
    // if (-1 == listen(accept_fd, 2048)) {
    //     LOG(ERROR) << "listen error";
    //     return 0;
    // }

    // //std::list<int> fds;

    // //co_opt.debug = 32;

    // go [&] {
    //     for(;;) {
    //         int fd = accept(accept_fd, (sockaddr*)&addr, &len);
    //         if (-1 == fd) {
    //             if (EAGAIN == errno || EINTR == errno) {
    //                 continue;
    //             }
    //             LOG(ERROR) << "accept error";
    //             return;
    //         }

    //         LOG(INFO) << "accept success, fd " << fd;
    //         //fds.push_back(fd);

    //         // auto session = std::make_shared<GateSession>(m_id, fd, *this);
    //         // m_sessions[session->id()] = session;
    //         // session->run();

    //         //co::CoDebugger::getInstance().GetAllInfo().c_str();
    //     }
    // };

    // co_sched.Start(8, 8);




    LOG(INFO) << "---end---";

    return 0;
}