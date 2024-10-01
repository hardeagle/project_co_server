
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <json/json.h>

#include <memory>

#include "log/glog.h"

#include <time.h>

#include "core/redis/redis_manager.h"
#include "core/util/util.h"
// #define CPPHTTPLIB_OPENSSL_SUPPORT
// #include "core/httplib.h"
#include "core/ws_session.h"

#include <sys/socket.h>
#include <arpa/inet.h>

#include "logic/protocol/login.pb.h"

const static int s_limit = 2048;

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
//             ELOG << "wake up begin...";
//             m_chan << true;
//             ELOG << "wake up end...";
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
//                         ELOG << "sleep begin....";
//                         m_chan >> flag;
//                         ELOG << "sleep end....";
//                     } else {
//                         m_mutex.unlock();
//                     }
//                     break;
//                 }

//                 m_mutex.lock();
//                 m_msgs.pop();
//                 m_msgs.front();
//                 if (m_msgs.size() % 100000 == 0) {
//                     WLOG << "+++consume m_msgs size " << m_msgs.size();
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

// class Point {
// public:
//     using ptr = std::shared_ptr<Point>;

//     Point() {
//         LOG << "无参构造函数";
//     }

//     Point(int x, int y, int z)
//         : m_x(x)
//         , m_y(y)
//         , m_z(z) {
//         LOG << "有参构造函数";
//     }

//     Point(const Point& other)
//         : m_x(other.m_x)
//         , m_y(other.m_y)
//         , m_z(other.m_z) {
//         LOG << "拷贝构造函数";
//     }

//     Point(Point&& other) {
//         LOG << "移动构造函数";
//         m_x = std::exchange(other.m_x, 0);
//         m_y = std::exchange(other.m_y, 0);
//         m_z = std::exchange(other.m_z, 0);
//     }

//     Point& operator=(const Point& other) {
//         LOG << "赋值构造函数";
//         m_x = other.m_x;
//         m_y = other.m_y;
//         m_z = other.m_z;
//         return *this;
//     }

//     Point& operator=(Point&& other) {
//         LOG << "移动构造函数";
//         m_x = std::exchange(other.m_x, 0);
//         m_x = std::exchange(other.m_y, 0);
//         m_x = std::exchange(other.m_z, 0);
//         return *this;
//     }

//     int m_x;
//     int m_y;
//     int m_z;

//     std::string strInfo() {
//         std::stringstream ss;
//         ss << "m_x " << m_x << " ,m_y " << m_y << " ,m_z " << m_z;
//         return ss.str();
//     }
// };


// void funTest(Point const& p) {
//     WLOG << "std::is_rvalue_reference<decltype(p)>::value " << std::is_rvalue_reference<decltype(p)>::value;

//     //p.m_y = 122;

//     WLOG << "&p " << &p;
//     //LOG << "funTest " << p.strInfo();
//     // auto ptmp = std::forward<Point>(p);
//     // LOG << "funTest ptemp" << ptmp.strInfo();

//     WLOG << "1111";
//     std::deque<Point> points;
//     points.push_back(std::move(p));
//     WLOG << "222";
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
//         WLOG << "---produce begin " << m_chan.size();
//         WLOG << "std::is_rvalue_reference<decltype(val)>::value " << std::is_rvalue_reference<decltype(val)>::value;

//         if (m_chan.size() == s_limit) {
//             ELOG << "------------------------full";
//         }
//         m_chan << std::forward<Point>(val);
//         WLOG << "---produce end " << m_chan.size();
//     }

//     void run() {
//         // go co_scheduler(m_sched) [this, self = this->template shared_from_this()] {
//         //     for (;;) {
//         //         WLOG << "+++consume begin... " << m_chan.size();
//         //         if (m_chan.size() == 0) {
//         //             ELOG << "------------------------empty";
//         //         }
//         //         T val;
//         //         m_chan >> val;
//         //         LOG << "val " << val.strInfo();

//         //         WLOG << "+++consume end... " << m_chan.size();
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
//     ELOG << "&vecs " << &vecs;
//     return vecs;
// }

// std::list<Point> testList() {
//     std::list<Point> vecs;
//     vecs.push_back(Point(1, 1, 1));
//     vecs.push_back(Point(2, 2, 2));
//     vecs.push_back(Point(3, 3, 3));
//     ELOG << "&vecs " << &vecs;
//     //return vecs;
//     return {};
// }

// template<typename T>
// class chanel : public co_chan<T> {
// public:
//     explicit chanel(std::size_t capacity = 0)
//         : co_chan<T>(capacity) {
//     }
// };

int64_t getCurMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void insertGameInfo() {

    auto key = "pcs_game_info_hash";
    Eayew::RedisManager redisMgr("127.0.0.1", 6379);
    {
        LoginProtocol::GameInfo gi;
        gi.set_gameid(1);
        gi.set_platform(1);
        gi.set_name("monster_elimination");
        gi.set_appid("tt09ee7f5396d5f796");
        gi.set_secret("0c0008a7e2a2179e9afbc12879f3b7b0aad0efdb");
        std::string str;
        gi.SerializeToString(&str);
        redisMgr.hset(key, gi.gameid(), str);
    }

    {
        LoginProtocol::GameInfo gi;
        gi.set_gameid(2);
        gi.set_platform(1);
        gi.set_name("star_elimination");
        gi.set_appid("tt5c2d2a90c316a8d9");
        gi.set_secret("40a13a4ea1a0c6b9c19d79267c1881851066dd83");
        std::string str;
        gi.SerializeToString(&str);
        redisMgr.hset(key, gi.gameid(), str);
    }

    {
        LoginProtocol::GameInfo gi;
        gi.set_gameid(3);
        gi.set_platform(1);
        gi.set_name("all_star_elimination");
        gi.set_appid("tt157063cd8a40408a02");
        gi.set_secret("82a41e490c412bf4972194b301a60b0b1097afd1");
        std::string str;
        gi.SerializeToString(&str);
        redisMgr.hset(key, gi.gameid(), str);
    }

    {
        LoginProtocol::GameInfo gi;
        gi.set_gameid(5);
        gi.set_platform(1);
        gi.set_name("arrow_legend");
        gi.set_appid("ttf54183fc6a703de302");
        gi.set_secret("108892c20b9775087d541ee586eb150163adc2a4");
        std::string str;
        gi.SerializeToString(&str);
        redisMgr.hset(key, gi.gameid(), str);
    }

    {
        LoginProtocol::GameInfo gi;
        gi.set_gameid(14);
        gi.set_platform(1);
        gi.set_name("all_happy");
        gi.set_appid("ttb069c6d42f5463fe02");
        gi.set_secret("5e059ccf72ade5e99680bb38087a7854dea54855");
        std::string str;
        gi.SerializeToString(&str);
        redisMgr.hset(key, gi.gameid(), str);
    }

    {
        LoginProtocol::GameInfo gi;
        gi.set_gameid(15);
        gi.set_platform(1);
        gi.set_name("gobang");
        gi.set_appid("tt8cd2fa6f13e3569302");
        gi.set_secret("9a081429644e919701c06cd406dab81f631a2acc");
        std::string str;
        gi.SerializeToString(&str);
        redisMgr.hset(key, gi.gameid(), str);
    }

    {
        LoginProtocol::GameInfo gi;
        gi.set_gameid(16);
        gi.set_platform(1);
        gi.set_name("xkzm_lily");
        gi.set_appid("tt765000ede967a8c602");
        gi.set_secret("0e00180e19b2d19e63f469314e47eae06ed71653");
        std::string str;
        gi.SerializeToString(&str);
        redisMgr.hset(key, gi.gameid(), str);
    }
}

void test(const std::string& ip) {
    go([&]() {
        LOG << "---2ip " << ip;
    });
}

int main(int argc, char* argv[]) {
    // GLog glog(argv[0]);
    flag::set_value("cout", "true");
    flag::set_value("log_daily", "true");
    flag::set_value("max_log_file_num", "32");
    flag::parse(argc, argv);


    //co_opt.debug = co::dbg_all;

    LOG << "---begin---";

    // insertGameInfo();

    test("127.1.1.1");

    std::string ip = "127.0.0.1";
    LOG << "ip " << ip;
    test(ip);

    int limit = 10240;
    co::wait_group wg;
    wg.add(limit);

    for (int i = 0; i < limit - 1; i++) {
        go([=] {
            Eayew::RedisManager redisMgr("127.0.0.1", 6379);

            // auto val = redisMgr.get<uint64_t>("pcs_loginname_to_roleid_set__000377I8D0oQvvZS7YBWXRDUe41H8-HBRvu");
            auto val = redisMgr.hincrby("pcs_id_increment", "role_id", 1);
            LOG << "val " << val;
            wg.done();
        });
    }

    co::pool p(
        []() { return (void*) new Eayew::RedisConnection("127.0.0.1", 6379); }, // ccb
        [](void* p) { delete (Eayew::RedisConnection*) p; }, // dcb
        8192                                // cap
    );

    go([&]() {
        co::pool_guard<Eayew::RedisConnection> rds(p);
        auto rc = rds.get();
        if (!rc) {
            ELOG << "get fail";
            return;
        }
        // auto val = rc->exec("get", "pcs_loginname_to_roleid_set__000377I8D0oQvvZS7YBWXRDUe41H8-HBRvu").get<uint32>();
        // LOG << "---val " << val;
        // rc->exec("hset", "hello_world", "333", "101");
        // rc->exec("set", "hello_world_set", "333");
        uint32 val = rc->exec("get", "hello_world_set").get<uint32>();
        LOG << "val " << val;
        
        // Eayew::RedisManager redisMgr("127.0.0.1", 6379);
        // auto val2  = redisMgr.get<uint64_t>("pcs_loginname_to_roleid_set__000377I8D0oQvvZS7YBWXRDUe41H8-HBRvu");
        // LOG << "val2 " << val2;

        wg.done();

    });


    wg.wait();

    // if (false) {
    //     httplib::SSLClient cli("https://developer.toutiao.com");
    //     std::string params = "/api/apps/jscode2session";
    //     params += "?appid=";
    //     params += "tt157063cd8a40408a02";
    //     params += "&secret=";
    //     params += "82a41e490c412bf4972194b301a60b0b1097afd1";
    //     params += "&code=";
    //     params += "_CsFcXCY1fQtGhYPUb18NI1O0ksIzTwz--W0pYykpbc229fknOsrYDnoltueY1okvL7CyiEya9fwGe3NKWblPi2bJdYNy0aUNrwgmUF8heBHvME2bq2oGRBeX0o";
    //     if (auto res = cli.Get(params)) {
    //         // if (res->status != httplib::StatusCode::OK_200) {
    //         //     ELOG << "http response status " << res->status;
    //         // }

    //         ELOG << "http response status " << res->status;
    //         LOG << "resp body " << res->body;
    //         Json::Value root;
    //         Json::Reader reader;
    //         if (reader.parse(res->body, root)) {
    //             LOG << "parse resp " << root;
    //             auto ret = root["error"].asInt64();
    //             if (ret != 0) {
    //                 ELOG << "resp error ret " << ret;
    //             }
    //         } else {
    //             ELOG << "parse error, body " << res->body;
    //         }
    //     } else {
    //         auto err = res.error();
    //         ELOG << "http fail err " << err;
    //     }
    // }


    // {
    //     httplib::SSLClient cli("developer.toutiao.com");
    //     cli.enable_server_certificate_verification(false);

    //     if (auto res = cli.Get("/api/apps/jscode2session?appid=tt5c2d2a90c316a8d9&secret=40a13a4ea1a0c6b9c19d79267c1881851066dd83&code=EoHEtFObgpzwz4wzo1Fh2wlSK-_8EiK4Ul7dE_CCwQk6chOT8GCqWUCHe60nhBXD7UZB7R5-zghV9WMR_uFaBykvaD9YJQLmix6mhzV_2jqcWmQZxxqMq_FQtj8")) {
    //         LOG << res->status;
    //         LOG << res->get_header_value("Content-Type");
    //         LOG << res->body;
    //     } else {
    //         ELOG << "error code: " << res.error();
    //         auto result = cli.get_openssl_verify_result();
    //         if (result) {
    //             LOG << "verify error: " << X509_verify_cert_error_string(result);
    //         }
    //     }
    // }

    // {
    //     httplib::SSLClient cli("eayew.com");
    //     cli.set_ca_cert_path("/etc/nginx/ssl/eayew/eayew.com_bundle.cr");

    //     cli.enable_server_certificate_verification(false);

    //     auto res = cli.Get("/sgs/rank/RankApiHandler/rankingList?gameid=13&openid=_000qbWZzL5CIn0jSMeOhvxwB6Y0_PcCToLX&limit=3");


    //         cout << res->status << endl;
    //         cout << res->get_header_value("Content-Type") << endl;
    //         cout << res->body << endl;


    //     LOG << "status " << res->status << " body " << res->body;

    // }

    // {
    //     httplib::Client cli("http://124.222.229.211:8080");
    //     auto res = cli.Get("/sgs/rank/RankApiHandler/rankingList?gameid=13&openid=_000qbWZzL5CIn0jSMeOhvxwB6Y0_PcCToLX&limit=3");
    //     res->status;
    //     res->body;
    //     LOG << "status " << res->status << " body " << res->body;
    // }

    // {
    //     httplib::Client cli("http://124.222.229.211:8080");
    //     if (auto res = cli.Get("/sgs/rank/RankApiHandler/rankingList?gameid=13&openid=_000qbWZzL5CIn0jSMeOhvxwB6Y0_PcCToLX&limit=3")) {
    //         //if (res->status == httplib::StatusCode::OK_200) {
    //         if (true) {
    //             std::cout << res->body << std::endl;

    //             Json::Value root;
    //             Json::Reader reader;
    //             if (reader.parse(res->body, root)) {
    //                 std::cout << "ret: " << root["ret"].asInt() << std::endl;
    //                 std::cout << "rank: " << root["ranking"].asInt() << std::endl;
    //                 LOG << "ris " << root["ris"] << " isArray " << root["ris"].isArray() << " size " << root["ris"].size();
    //                 for (int32_t i = 0; i < root["ris"].size(); i++) {
    //                     LOG << "openid " << root["ris"][i]["openid"].asString();
    //                     LOG << "nickname " << root["ris"][i]["nickname"].asString();
    //                     LOG << "avatar_url " << root["ris"][i]["avatar_url"].asString();
    //                     LOG << "score " << root["ris"][i]["score"].asString();


    //                     LOG << "openid " << root["ris"][i]["openid"];
    //                     LOG << "nickname " << root["ris"][i]["nickname"];
    //                     LOG << "avatar_url " << root["ris"][i]["avatar_url"];
    //                     LOG << "score " << root["ris"][i]["score"];
    //                 }
    //             } else {
    //                 std::cout << "parse error!" << std::endl;
    //             }
    //         }
    //     } else {
    //         auto err = res.error();
    //         std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
    //     }
    // }


    // std::string m_ip = "0.0.0.0";
    // auto m_port = 9777;

    // int accept_fd = socket(AF_INET, SOCK_STREAM, 0);

    // int opt = 1;
    // setsockopt(accept_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // sockaddr_in addr;
    // addr.sin_family = AF_INET;
    // addr.sin_port = htons(m_port);
    // addr.sin_addr.s_addr = inet_addr(m_ip.data());
    // socklen_t len = sizeof(addr);
    // if (-1 == bind(accept_fd, (sockaddr*)&addr, len)) {
    //     ELOG << "bind error, port " << m_port;
    //     return 0;
    // }
    // if (-1 == listen(accept_fd, 2048)) {
    //     ELOG << "listen error";
    //     return 0;
    // }

    // for (;;) {
    //     int fd = accept(accept_fd, (sockaddr*)&addr, &len);
    //     LOG << "accept "  << fd;
    //     Eayew::WsSession ws(fd);
    //     ws.start();

    //     uint32_t icount = 0;
    //     uint32_t count = 0;

    //     static const uint32_t MAX_SIZE = 64 * 1024;
    //     char buffs[MAX_SIZE];
    //     uint32_t index = 0;
    //     uint32_t rlen = 0;
    //     for (;;) {
    //         if (index < rlen) {
    //             WLOG << "memmove index " << index << " rlen " << rlen;
    //             memcpy(&buffs[0], &buffs[index], rlen - index);
    //             index = rlen - index;
    //         } else {
    //             index = 0;
    //         }
    //         rlen = read(fd, &buffs[index], MAX_SIZE - index);
    //         if (0 == rlen) {
    //             ELOG << "close";
    //             return 0;
    //         } else if (-1 == rlen) {
    //             if (errno == EINTR || errno==EAGAIN) {
    //                 WLOG << "errno " << errno;
    //                 continue;
    //             } else {
    //                 ELOG << "errno " << errno;
    //                 return 0;
    //             }
    //         }

    //         icount++;
    //         WLOG << "---rlen " << rlen << " index " << index;
    //         rlen += index;
    //         index = 0;
    //         while (index < rlen) {
    //             char datas[MAX_SIZE] = {};
    //             int len = 0;
    //             int pos = 0;
    //             auto wsft = ws.getFrame((unsigned char*)&buffs[index], rlen - index, (unsigned char*)&datas[0], rlen - index, &len, &pos);
    //             if (wsft != 129 || index + len + pos - 1 > rlen) {
    //                 WLOG << "break, wsft " << wsft << " len " << len << " pos " << pos << " rlen " << rlen << " index " << index << " count " << count++ << " icount " << icount  << " datas " << datas;
    //                 break;
    //             }
    //             index = index + len + pos - 1;
    //             WLOG << "loop, wsft " << wsft << " len " << len << " pos " << pos << " rlen " << rlen << " index " << index << " count " << count++ << " icount " << icount << " datas " << datas;

    //             // auto p = (uint16_t*)(&buffs[index]);
    //             // LOG << "p " << p << " data size " << *p;
    //             // auto size = *p;
    //             // if (size > MAX_SIZE) {
    //             //     ELOG << "overflow , size " << size << " MAX_SIZE" << MAX_SIZE;
    //             //     return;
    //             // }
    //             // if (size > rlen - index) {
    //             //     WLOG << "warning " << " rlen " << rlen << " index " << index << " size " << size; // 还需要做下处理，粘包
    //             //     break;
    //             // }
    //             // LOG << "len " << rlen << " size " << size << " index " << index;
    //             // Message msg(size - Message::HEAD_LEN);
    //             // memcpy(msg.data(), &buffs[index], size);
    //             // if (m_onMessageCB != nullptr) {
    //             //     LOG << "msg " << msg.strInfo();
    //             //     m_onMessageCB(std::move(msg));
    //             // }
    //             // index += size;
    //             // WLOG << "loop, len " << rlen << " index " << index;
    //         }
    //         WLOG << "once read, len " << rlen << " index " << index;
    //     }

    //     // int count = 0;
    //     // for (auto i = 0; i < 10000; ++i) {

    //     //     static constexpr size_t HANDSHAKE_STREAMBUF_SIZE = 1024 * 1024;
    //     //     char buffs[HANDSHAKE_STREAMBUF_SIZE] = {};
    //     //     auto rlen = read(fd, &buffs[0], HANDSHAKE_STREAMBUF_SIZE);
    //     //     LOG << "rlen " << rlen;
    //     //     //LOG << "buffs " << buffs;

    //     //     int index = 0;
    //     //     while (index < rlen) {
    //     //         int len = 0;
    //     //         int pos = 0;
    //     //         char buffs1[HANDSHAKE_STREAMBUF_SIZE] = {};
    //     //         auto wsft = ws.getFrame((unsigned char*)&buffs[index], rlen - index, (unsigned char*)&buffs1[0], HANDSHAKE_STREAMBUF_SIZE, &len, &pos);
    //     //         LOG << " buffs1 " << buffs1 << " wsft " << wsft << " count " << count++ << " rlen " << rlen << " index " << index << " len " << len  << " pos " << pos;
    //     //         index = index + len + pos - 1;
    //     //     }


    //     //     LOG << " index " << index << " rlen " << rlen << "   i " << i;


    //     //     // std::string str("12345678987654321");
    //     //     // rlen = ws.makeFrame(Eayew::WebSocketFrameType::TEXT_FRAME, (unsigned char*)str.data(), str.size(), (unsigned char*)&buffs1[0], HANDSHAKE_STREAMBUF_SIZE);
    //     //     // LOG << "rlen " << rlen;
    //     //     // auto wlen = write(fd, &buffs1[0], rlen);
    //     //     // LOG << "wlen " << rlen;
    //     // }

    // }

    //auto vs = testCon();
    // std::vector<Point> vs;
    // vs = testCon();
    // ELOG << "&vs " << &vs;

    // {
    //     //std::list<Point> vs;
    //     auto vs = testList();
    //     ELOG << "&vs " << &vs << " ,vs size " << vs.size();
    //     for (auto k : vs) {
    //         ELOG << "info " << k.strInfo();
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
    //         LOG << "---produce m_msgs size " << test->size();
    //     };
    // }

    // Point p1(1, 2, 3);
    // LOG<< "p1 " << p1.strInfo();
    // //auto p2 = p1;
    // Point p2;
    // p2 = p1;
    // LOG<< "p2 " << p2.strInfo();

    // auto& p3 = p1;
    // LOG<< "p3 " << p3.strInfo();

    // Point p1(1, 2, 3);
    // WLOG << "&p1 " << &p1 << " data " << p1.strInfo();;
    // auto tmp = std::move(p1);
    // WLOG << "11";
    // funTest(std::forward<Point>(tmp));
    // WLOG << "&p1 " << &p1 << " data " << p1.strInfo();;

    // std::deque<Point> points;
    // points.push_back(std::forward<Point>(p1));

    // WLOG << "&tmp " << &tmp << " data " << tmp.strInfo();;


    // for (auto& v : points) {
    //     WLOG << "&v " << &v << " data " << v.strInfo();;
    // }

    // auto p2 = std::forward<Point>(p1);

    // std::vector<int> vecs;
    // sum(vecs, 1, 2, 3, 4, 5, 6);
    // for (auto v : vecs) {
    //     ELOG << "v " << v;
    // }

    // Eayew::RedisManager redisMgr("127.0.0.1", 6379);
    // { // string
    //     auto key = "string_test";
    //     redisMgr.set(key, "111");
    //     std::string val = redisMgr.get<std::string>(key);
    //     WLOG << "string val " << val;
    // }
    // { // sorted set
    //     auto key = "sorted_set_test";
    //     redisMgr.zadd(key, 100, "test_100");
    //     redisMgr.zadd(key, 200, "test_200");
    //     redisMgr.zadd(key, 300, "test_300");
    //     auto vals = redisMgr.zrevrange<std::string, uint32_t>(key, 0 , -1);
    //     for (auto [k, v] : vals) {
    //         ELOG << "k " << k << " ,v " << v;
    //     }
    // }

    // Eayew::RedisManager tendisMgr("127.0.0.1", 51002);
    // { // string
    //     auto key = "string_test_tendis";
    //     tendisMgr.set(key, "111");
    //     std::string val = redisMgr.get<std::string>(key);
    //     WLOG << "string val " << val;
    // }
    // { // sorted set
    //     auto key = "sorted_set_test_tendis";
    //     tendisMgr.zadd(key, 100, "test_100");
    //     tendisMgr.zadd(key, 200, "test_200");
    //     tendisMgr.zadd(key, 300, "test_300");
    //     auto vals = tendisMgr.zrevrange<std::string, uint32_t>(key, 0 , -1);
    //     for (auto [k, v] : vals) {
    //         ELOG << "k " << k << " ,v " << v;
    //     }
    // }

    // points << std::move(p1);
    // points >> p1;

    // LOG << "-------------------------------------------------";

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
    // WLOG << "begin... begin ts " << begin << " total num " << total_num << " p_routine num " << pnum << " c_routine num " << cnum;

    // //Point::ptr p(new Point(1, 1, 1));
    // Point p(1, 1, 1);
    // for (auto i = 0; i < pnum; ++i) {
    //     go [&, i] {
    //         for (int j = 0; j < pcount; ++j) {
    //             //Point p(j, j, j);
    //             //Point::ptr p(new Point(1, 1, 1));
    //             points << std::move(p);
    //             //LOG << "produce size " << points.size();
    //         }
    //         wg << nullptr;
    //         WLOG << "produce i " << i << " points.size() " << points.size();
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
    //                 //LOG << "consume size " << points.size();
    //             }
    //             wg << nullptr;
    //             WLOG << "consume i " << i << " points.size() " << points.size();
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
    //                 WLOG << "produce end... end ts " << end << " dur(ms) " << end - begin;
    //             }
    //         }
    //         auto end = getCurMs();
    //         WLOG << "produce & consume end...  " << " end ts " << end << " dur(ms) " << end - begin;
    //     };
    // }



    // co_sched.Start(6);
    // for (int i = 0; i < 10; ++i) {
    //     go [=] {
    //         ELOG << i;
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
    //     ELOG << "name: " << name;
    //     ELOG << "type: " << type;
    //     ELOG << "ip: " << ip;
    //     ELOG << "port: " << port;
    //     ELOG << "num: " << num;
    // }

    // time_t t = time(NULL);
    // WLOG << "t " << t;

    // LOG << "cur time " << Eayew::getCurSecond();

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
    //     ELOG << "bind error, port " << m_port;
    //     return 0;
    // }
    // if (-1 == listen(accept_fd, 2048)) {
    //     ELOG << "listen error";
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
    //             ELOG << "accept error";
    //             return;
    //         }

    //         LOG << "accept success, fd " << fd;
    //         //fds.push_back(fd);

    //         // auto session = std::make_shared<GateSession>(m_id, fd, *this);
    //         // m_sessions[session->id()] = session;
    //         // session->run();

    //         //co::CoDebugger::getInstance().GetAllInfo().c_str();
    //     }
    // };

    // co_sched.Start(8, 8);




    LOG << "---end---";

    return 0;
}