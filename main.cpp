

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <jemalloc/jemalloc.h>

#include <memory>

#include <libgo/libgo.h>

#include "log/glog.h"

#include <time.h>

#include "core/redis/redis_manager.h"
#include "core/util/util.h"
#include "core/httplib.h"
#include "core/ws_session.h"

#include <sys/socket.h>
#include <arpa/inet.h>

const static int s_limit = 20480;

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
    using ptr = std::shared_ptr<Point>;

    Point() {
        LOG(INFO) << "无参构造函数";
    }

    Point(int x, int y, int z)
        : m_x(x)
        , m_y(y)
        , m_z(z) {
        LOG(INFO) << "有参构造函数";
    }

    Point(const Point& other)
        : m_x(other.m_x)
        , m_y(other.m_y)
        , m_z(other.m_z) {
        LOG(INFO) << "拷贝构造函数";
    }

    Point(Point&& other) {
        LOG(INFO) << "移动构造函数";
        m_x = std::exchange(other.m_x, 0);
        m_y = std::exchange(other.m_y, 0);
        m_z = std::exchange(other.m_z, 0);
    }

    Point& operator=(const Point& other) {
        LOG(INFO) << "赋值构造函数";
        m_x = other.m_x;
        m_y = other.m_y;
        m_z = other.m_z;
        return *this;
    }

    Point& operator=(Point&& other) {
        LOG(INFO) << "移动构造函数";
        m_x = std::exchange(other.m_x, 0);
        m_x = std::exchange(other.m_y, 0);
        m_x = std::exchange(other.m_z, 0);
        return *this;
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


// void funTest(Point const& p) {
//     LOG(WARNING) << "std::is_rvalue_reference<decltype(p)>::value " << std::is_rvalue_reference<decltype(p)>::value;

//     //p.m_y = 122;

//     LOG(WARNING) << "&p " << &p;
//     //LOG(INFO) << "funTest " << p.strInfo();
//     // auto ptmp = std::forward<Point>(p);
//     // LOG(INFO) << "funTest ptemp" << ptmp.strInfo();

//     LOG(WARNING) << "1111";
//     std::deque<Point> points;
//     points.push_back(std::move(p));
//     LOG(WARNING) << "222";
// }


// template<typename T>
// class Test : public std::enable_shared_from_this<Test<T>> {
// public:
//     using ptr = std::shared_ptr<Test>;

//     Test()
//         : m_chan(s_limit) {
//     }

//     void init() {
//         m_sched = co::Scheduler::Create();
//         // std::thread t2([this, self = shared_from_this()]{ m_sched->Start(4); });
//         // t2.detach();
//         m_threads.emplace_back([this, self = this->template shared_from_this()]{ m_sched->Start(1); });
//         m_threads[0].detach();
//     }

//     int size() {
//         return m_chan.size();
//     }

//     void push(T&& val) {
//         LOG(WARNING) << "---produce begin " << m_chan.size();
//         LOG(WARNING) << "std::is_rvalue_reference<decltype(val)>::value " << std::is_rvalue_reference<decltype(val)>::value;

//         if (m_chan.size() == s_limit) {
//             LOG(ERROR) << "------------------------full";
//         }
//         m_chan << std::forward<Point>(val);
//         LOG(WARNING) << "---produce end " << m_chan.size();
//     }

//     void run() {
//         // go co_scheduler(m_sched) [this, self = this->template shared_from_this()] {
//         //     for (;;) {
//         //         LOG(WARNING) << "+++consume begin... " << m_chan.size();
//         //         if (m_chan.size() == 0) {
//         //             LOG(ERROR) << "------------------------empty";
//         //         }
//         //         T val;
//         //         m_chan >> val;
//         //         LOG(INFO) << "val " << val.strInfo();

//         //         LOG(WARNING) << "+++consume end... " << m_chan.size();
//         //     }
//         // };
//     }

// private:
//     co::Scheduler* m_sched;
//     std::vector<std::thread> m_threads;

//     co_chan<T> m_chan;
// };


// template<typename T, typename ...Args>
// void sum(std::vector<T>& vecs,  Args... args){
//     (..., vecs.push_back(args));
// 	//std::cout << ... << vecs.push_back(args);
// }

// std::vector<Point> testCon() {
//     std::vector<Point> vecs;
//     vecs.push_back(Point(1, 1, 1));
//     //vecs.push_back(Point(2, 2, 2));
//     //vecs.push_back(Point(3, 3, 3));
//     LOG(ERROR) << "&vecs " << &vecs;
//     return vecs;
// }

// std::list<Point> testList() {
//     std::list<Point> vecs;
//     vecs.push_back(Point(1, 1, 1));
//     vecs.push_back(Point(2, 2, 2));
//     vecs.push_back(Point(3, 3, 3));
//     LOG(ERROR) << "&vecs " << &vecs;
//     //return vecs;
//     return {};
// }

template<typename T>
class chanel : public co_chan<T> {
public:
    explicit chanel(std::size_t capacity = 0)
        : co_chan<T>(capacity) {
    }
};

int64_t getCurMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main(int argc, char* argv[]) {
    GLog glog(argv[0]);

    //co_opt.debug = co::dbg_all;

    LOG(INFO) << "---begin---";

    {
        httplib::Client cli("http://124.222.229.211:8080");
        auto res = cli.Get("/sgs/rank/RankApiHandler/rankingList?gameid=13&openid=_000qbWZzL5CIn0jSMeOhvxwB6Y0_PcCToLX&limit=3");
        res->status;
        res->body;
        LOG(INFO) << "status " << res->status << " body " << res->body;
    }

    {
        httplib::Client cli("http://124.222.229.211:8080");
        if (auto res = cli.Get("/sgs/rank/RankApiHandler/rankingList?gameid=13&openid=_000qbWZzL5CIn0jSMeOhvxwB6Y0_PcCToLX&limit=3")) {
            if (res->status == httplib::StatusCode::OK_200) {
                std::cout << res->body << std::endl;
            }
        } else {
            auto err = res.error();
            std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
        }
    }


    std::string m_ip = "0.0.0.0";
    auto m_port = 9777;

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

    for (;;) {
        int fd = accept(accept_fd, (sockaddr*)&addr, &len);
        LOG(INFO) << "accept "  << fd;
        Eayew::WsSession ws(fd);
        ws.start();

        uint32_t icount = 0;
        uint32_t count = 0;

        static const uint32_t MAX_SIZE = 64 * 1024;
        char buffs[MAX_SIZE];
        uint32_t index = 0;
        uint32_t rlen = 0;
        for (;;) {
            if (index < rlen) {
                LOG(WARNING) << "memmove index " << index << " rlen " << rlen;
                memcpy(&buffs[0], &buffs[index], rlen - index);
                index = rlen - index;
            } else {
                index = 0;
            }
            rlen = read(fd, &buffs[index], MAX_SIZE - index);
            if (0 == rlen) {
                LOG(ERROR) << "close";
                return 0;
            } else if (-1 == rlen) {
                if (errno == EINTR || errno==EAGAIN) {
                    LOG(WARNING) << "errno " << errno;
                    continue;
                } else {
                    LOG(ERROR) << "errno " << errno;
                    return 0;
                }
            }

            icount++;
            LOG(WARNING) << "---rlen " << rlen << " index " << index;
            rlen += index;
            index = 0;
            while (index < rlen) {
                char datas[MAX_SIZE] = {};
                int len = 0;
                int pos = 0;
                auto wsft = ws.getFrame((unsigned char*)&buffs[index], rlen - index, (unsigned char*)&datas[0], rlen - index, &len, &pos);
                if (wsft != 129 || index + len + pos - 1 > rlen) {
                    LOG(WARNING) << "break, wsft " << wsft << " len " << len << " pos " << pos << " rlen " << rlen << " index " << index << " count " << count++ << " icount " << icount  << " datas " << datas;
                    break;
                }
                index = index + len + pos - 1;
                LOG(WARNING) << "loop, wsft " << wsft << " len " << len << " pos " << pos << " rlen " << rlen << " index " << index << " count " << count++ << " icount " << icount << " datas " << datas;

                // auto p = (uint16_t*)(&buffs[index]);
                // LOG(INFO) << "p " << p << " data size " << *p;
                // auto size = *p;
                // if (size > MAX_SIZE) {
                //     LOG(ERROR) << "overflow , size " << size << " MAX_SIZE" << MAX_SIZE;
                //     return;
                // }
                // if (size > rlen - index) {
                //     LOG(WARNING) << "warning " << " rlen " << rlen << " index " << index << " size " << size; // 还需要做下处理，粘包
                //     break;
                // }
                // LOG(INFO) << "len " << rlen << " size " << size << " index " << index;
                // Message msg(size - Message::HEAD_LEN);
                // memcpy(msg.data(), &buffs[index], size);
                // if (m_onMessageCB != nullptr) {
                //     LOG(INFO) << "msg " << msg.strInfo();
                //     m_onMessageCB(std::move(msg));
                // }
                // index += size;
                // LOG(WARNING) << "loop, len " << rlen << " index " << index;
            }
            LOG(WARNING) << "once read, len " << rlen << " index " << index;
        }

        // int count = 0;
        // for (auto i = 0; i < 10000; ++i) {

        //     static constexpr size_t HANDSHAKE_STREAMBUF_SIZE = 1024 * 1024;
        //     char buffs[HANDSHAKE_STREAMBUF_SIZE] = {};
        //     auto rlen = read(fd, &buffs[0], HANDSHAKE_STREAMBUF_SIZE);
        //     LOG(INFO) << "rlen " << rlen;
        //     //LOG(INFO) << "buffs " << buffs;

        //     int index = 0;
        //     while (index < rlen) {
        //         int len = 0;
        //         int pos = 0;
        //         char buffs1[HANDSHAKE_STREAMBUF_SIZE] = {};
        //         auto wsft = ws.getFrame((unsigned char*)&buffs[index], rlen - index, (unsigned char*)&buffs1[0], HANDSHAKE_STREAMBUF_SIZE, &len, &pos);
        //         LOG(INFO) << " buffs1 " << buffs1 << " wsft " << wsft << " count " << count++ << " rlen " << rlen << " index " << index << " len " << len  << " pos " << pos;
        //         index = index + len + pos - 1;
        //     }


        //     LOG(INFO) << " index " << index << " rlen " << rlen << "   i " << i;


        //     // std::string str("12345678987654321");
        //     // rlen = ws.makeFrame(Eayew::WebSocketFrameType::TEXT_FRAME, (unsigned char*)str.data(), str.size(), (unsigned char*)&buffs1[0], HANDSHAKE_STREAMBUF_SIZE);
        //     // LOG(INFO) << "rlen " << rlen;
        //     // auto wlen = write(fd, &buffs1[0], rlen);
        //     // LOG(INFO) << "wlen " << rlen;
        // }

    }

    //auto vs = testCon();
    // std::vector<Point> vs;
    // vs = testCon();
    // LOG(ERROR) << "&vs " << &vs;

    // {
    //     //std::list<Point> vs;
    //     auto vs = testList();
    //     LOG(ERROR) << "&vs " << &vs << " ,vs size " << vs.size();
    //     for (auto k : vs) {
    //         LOG(ERROR) << "info " << k.strInfo();
    //     }
    // }

    // Test<Point>::ptr test = std::make_shared<Test<Point>>();
    // test->init();
    // test->run();

    // for (int i = 0; i < 1; ++i) {
    //     go [=] {
    //         for (int j = 0; j < 1; ++j) {
    //             auto point = Point(i, j, i+j);
    //             test->push(std::move(point));
    //         }
    //         LOG(INFO) << "---produce m_msgs size " << test->size();
    //     };
    // }

    // Point p1(1, 2, 3);
    // LOG(INFO)<< "p1 " << p1.strInfo();
    // //auto p2 = p1;
    // Point p2;
    // p2 = p1;
    // LOG(INFO)<< "p2 " << p2.strInfo();

    // auto& p3 = p1;
    // LOG(INFO)<< "p3 " << p3.strInfo();

    // Point p1(1, 2, 3);
    // LOG(WARNING) << "&p1 " << &p1 << " data " << p1.strInfo();;
    // auto tmp = std::move(p1);
    // LOG(WARNING) << "11";
    // funTest(std::forward<Point>(tmp));
    // LOG(WARNING) << "&p1 " << &p1 << " data " << p1.strInfo();;

    // std::deque<Point> points;
    // points.push_back(std::forward<Point>(p1));

    // LOG(WARNING) << "&tmp " << &tmp << " data " << tmp.strInfo();;


    // for (auto& v : points) {
    //     LOG(WARNING) << "&v " << &v << " data " << v.strInfo();;
    // }

    // auto p2 = std::forward<Point>(p1);

    // std::vector<int> vecs;
    // sum(vecs, 1, 2, 3, 4, 5, 6);
    // for (auto v : vecs) {
    //     LOG(ERROR) << "v " << v;
    // }

    // Eayew::RedisManager redisMgr("127.0.0.1", 6379);
    // { // string
    //     auto key = "string_test";
    //     redisMgr.set(key, "111");
    //     std::string val = redisMgr.get<std::string>(key);
    //     LOG(WARNING) << "string val " << val;
    // }
    // { // sorted set
    //     auto key = "sorted_set_test";
    //     redisMgr.zadd(key, 100, "test_100");
    //     redisMgr.zadd(key, 200, "test_200");
    //     redisMgr.zadd(key, 300, "test_300");
    //     auto vals = redisMgr.zrevrange<std::string, uint32_t>(key, 0 , -1);
    //     for (auto [k, v] : vals) {
    //         LOG(ERROR) << "k " << k << " ,v " << v;
    //     }
    // }

    // Eayew::RedisManager tendisMgr("127.0.0.1", 51002);
    // { // string
    //     auto key = "string_test_tendis";
    //     tendisMgr.set(key, "111");
    //     std::string val = redisMgr.get<std::string>(key);
    //     LOG(WARNING) << "string val " << val;
    // }
    // { // sorted set
    //     auto key = "sorted_set_test_tendis";
    //     tendisMgr.zadd(key, 100, "test_100");
    //     tendisMgr.zadd(key, 200, "test_200");
    //     tendisMgr.zadd(key, 300, "test_300");
    //     auto vals = tendisMgr.zrevrange<std::string, uint32_t>(key, 0 , -1);
    //     for (auto [k, v] : vals) {
    //         LOG(ERROR) << "k " << k << " ,v " << v;
    //     }
    // }

    // points << std::move(p1);
    // points >> p1;

    // LOG(INFO) << "-------------------------------------------------";

    // //chanel<Point::ptr> points(1024000);
    // chanel<Point> points(1024000);
    // auto total_num = 100000;
    // auto pnum = 5;
    // auto cnum = 1;
    // auto pcnum = pnum + cnum;
    // auto pcount = total_num / pnum;
    // auto ccount = total_num / cnum;
    // chanel<void> wg(pcnum);

    // auto begin = getCurMs();
    // LOG(WARNING) << "begin... begin ts " << begin << " total num " << total_num << " p_routine num " << pnum << " c_routine num " << cnum;

    // //Point::ptr p(new Point(1, 1, 1));
    // Point p(1, 1, 1);
    // for (auto i = 0; i < pnum; ++i) {
    //     go [&, i] {
    //         for (int j = 0; j < pcount; ++j) {
    //             //Point p(j, j, j);
    //             //Point::ptr p(new Point(1, 1, 1));
    //             points << std::move(p);
    //             //LOG(INFO) << "produce size " << points.size();
    //         }
    //         wg << nullptr;
    //         LOG(WARNING) << "produce i " << i << " points.size() " << points.size();
    //     };
    // }

    // {
    //     co::Scheduler* sched = co::Scheduler::Create();
    //     std::thread t2([sched]{ sched->Start(1); });
    //     t2.detach();

    //     for (auto i = 0; i < cnum; ++i) {
    //         go co_scheduler(sched) [&, i] {
    //             for (auto j = 0; j < ccount; ++j) {
    //                 //Point p;
    //                 //Point::ptr p(new Point(1, 1, 1));
    //                 points >> p;
    //                 //LOG(INFO) << "consume size " << points.size();
    //             }
    //             wg << nullptr;
    //             LOG(WARNING) << "consume i " << i << " points.size() " << points.size();
    //         };
    //     }
    // }

    // {
    //     co::Scheduler* sched = co::Scheduler::Create();
    //     std::thread t2([sched]{ sched->Start(1); });
    //     t2.detach();
    //     go co_scheduler(sched) [&] {
    //         for (auto i = 0; i < pcnum; ++i) {
    //             wg >> nullptr;

    //             if (i == pnum - 1) {
    //                 auto end = getCurMs();
    //                 LOG(WARNING) << "produce end... end ts " << end << " dur(ms) " << end - begin;
    //             }
    //         }
    //         auto end = getCurMs();
    //         LOG(WARNING) << "produce & consume end...  " << " end ts " << end << " dur(ms) " << end - begin;
    //     };
    // }



    // co_sched.Start(6);
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