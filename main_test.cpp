// #include "test/connection.h"

//#include <boost/lexical_cast.hpp>

// #include <sys/socket.h>
// #include <arpa/inet.h>

// #include <libgo/libgo.h>

// #include <jemalloc/jemalloc.h>

// #include "core/message.h"
// #include "core/util/util.h"


#include <co/all.h>

#include "core/util/util.h"

#include "core/ws_session.h"

#include "logic/protocol/login.pb.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>

#include "core/websocket.h"

using namespace lightws;

// #include "logic/login/protocol/login.pb.h"

void test() {
    co::Sched* sched = nullptr;
    LOG << "run---";
    sched->go([]() {
        int i  =0;
        int j = 1;
        int k = i+j;
        LOG << "gogogog " << k;
    });
}

int main(int argc, char* argv[]) {
    // // GLog glog(argv[0]);

    flag::set_value("cout", "true");
    flag::parse(argc, argv);

    cout << "begin...\n";

    auto sched = co::next_sched();
    auto s = std::make_shared<Eayew::WsSession>(-1);
    s->setwSched(sched);
    s->setrSched(sched);
    s->setOnMessage([](Eayew::Message::ptr msg) {
        LOG << "msg " << msg->strInfo();
    });
    s->sync_connect("ws://127.0.0.1:9101");
    s->run();

    // s->sync_write(1001, 2, data);

    // auto msg = std::make_shared<Eayew::Message>(0);
    // msg->senderId(1);
    // msg->receiverId(2);
    // msg->msgId(1001);
    // msg->roleId(0);

    LoginProtocol::C2S_LoginCreate req;
    req.set_loginname("helloworld1");
    req.set_gameid(13);

    auto nsize = req.ByteSizeLong();
    auto nmsg = co::make_shared<Eayew::Message>(nsize);
    req.SerializeToArray(nmsg->pdata(), nsize);
    nmsg->senderId(1);
    nmsg->receiverId(2);
    nmsg->msgId(1003);
    nmsg->roleId(0);
    nmsg->sessionId(0);
    s->send(nmsg);

    co::WaitGroup wg;
    wg.add(1);
    wg.wait();


    // int ret = 0;
    
    // CWebsocket* ws_cli = new CWebsocket("ws://127.0.0.1:9101", false);
    // if (ws_cli == NULL) {
    //     printf("Failed to create CWebsocket\n");
    // }
    // ret = ws_cli->connect_hostname();
    // if (ret != 0) {
    //     printf("Failed to connect hostname\n");
    // }


    // auto msg = std::make_shared<Eayew::Message>(0);
    // msg->senderId(1);
    // msg->receiverId(2);
    // msg->msgId(12345678);
    // msg->roleId(77777);
    // std::string str = std::string(msg->data(), msg->size());
    // ws_cli->send(str);

    // co::WaitGroup wg;
    // wg.add(1);
    // wg.wait();

    // ret = ws_cli->send("{\"method\":\"hello_req\"}");
    // ret = ws_cli->send("{\"method\":\"goodbye_req\"}");

    // while (ws_cli->get_websocket_state() != EWebsocketState::CLOSED) {
    //   ws_cli->poll();
    //   ws_cli->dispatch();
    // }
    // delete ws_cli;


    // co::chan<Eayew::Message::ptr> msgs(1024);
    // co::chan<Eayew::Message> msgs(1024);
    // for (int i = 0; i < 100; i++) {
    //     // Eayew::Message::ptr msg = std::make_shared<Eayew::Message>(36);
    //     // msg->senderId(i);
    //     // msg->receiverId(i);
    //     // msg->msgId(i);
    //     // msg->roleId(i);
    //     Eayew::Message msg(36);
    //     msg.senderId(i);
    //     msg.receiverId(i);
    //     msg.msgId(i);
    //     msg.roleId(i);
    //     msgs << msg;
    // }
    cout << "end...";

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
    //             ELOG << "connect fail, port " << port;
    //             return;
    //         }

    //         LOG << "connect success,  fd " << fd;
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
    //     WLOG << "msg " << msg;

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
    //     WLOG << "msg " << msg;

    //     LoginProtocol::C2S_LoginLoad req;
    //     req.set_loginname(msg);
    //     std::string data;
    //     req.SerializeToString(&data);
    //     con->sync_write(1001, 2, data);
    // }

    // std::list<Connection::ptr> cons;

    // for (int i = 0; i < 100; ++i) {

    //     go [&, i] {

    //         std::string ip = "127.0.0.1";
    //         int port = 9101;
    //         auto con = std::make_shared<Connection>();
    //         con->sync_connect(ip, port, 1, 2);
    //         con->setOnMessage([](Eayew::Message::ptr msg){
    //             LOG << "msg " << msg->strInfo();
    //         });
    //         con->run();
    //         cons.push_back(con);

    //         // for (int j = 0; j < 10000; ++j) {
    //         //     std::string str("1");
    //         //     for (int k = 0; k < 100; ++k) {
    //         //         str += "1";
    //         //     }
    //         //     //WLOG << "msg " << msg;

    //         //     // LoginProtocol::C2S_LoginLogin req;
    //         //     // req.set_loginname(msg);
    //         //     // std::string data;
    //         //     // req.SerializeToString(&data);
    //         //     // con->sync_write(1001, 2, data);

    //         //     LoginProtocol::C2S_LoginCreate req;
    //         //     req.set_loginname(str);
    //         //     req.set_role_name(str);
    //         //     req.set_avatarurl("123");
    //         //     std::string data;
    //         //     req.SerializeToString(&data);

    //         //     Eayew::Message msg(data.size());
    //         //     msg.senderId(1);
    //         //     msg.receiverId(2);
    //         //     msg.msgId(1003);
    //         //     msg.roleId(0);
    //         //     msg.write(data.data(), data.size());
    //         //     LOG << "msg " << msg.strInfo();
    //         //     con->send(std::move(msg));

    //         //     //co_sleep(200);
    //         //     //ELOG << "send,  , fd " << con->fd() << " ,i " << i << " ,j " << j;
    //         // }
    //     };
    // }

    // // {
    // //     std::string msg("1");
    // //     for (int i = 0; i < 1024000; ++i) {
    // //         msg += "1";
    // //     }
    // //     WLOG << "msg " << msg;

    // //     LoginProtocol::C2S_LoginLoad req;
    // //     req.set_loginname(msg);
    // //     std::string data;
    // //     req.SerializeToString(&data);
    // //     con->sync_write(1001, 2, data);
    // // }

    // co_sched.Start(6);

    return 0;
}