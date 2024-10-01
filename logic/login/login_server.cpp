#include "login_server.h"

#include <co/all.h>

#include "core/servlet.h"

#include "logic/protocol/login_id.pb.h"
#include "logic/login/servlet/login_servlet.h"
#include "logic/login/server_resource.h"

DEF_string(ip, "0.0.0.0", "http server ip");
DEF_int32(port, 8977, "http server port");
DEF_string(key, "", "private key file");
DEF_string(ca, "", "certificate file");

void LoginServer::beforeRun() {
    LOG << "begin...";
    initByConfig("./json/login_server.json");

    ServerResource::get()->init(workSched());

    LOG << "123";
    initServlet();
    LOG << "end...";
}

void LoginServer::initServlet() {  
    Eayew::Servlet::ptr tmp_servlet;
    tmp_servlet.reset(new LoginServlet);
    servlet()->addServlet(LoginProtocol::ID::C2S_LOGIN_BASE, LoginProtocol::ID::C2S_LOGIN_TOP, tmp_servlet);
}

void LoginServer::initHttpServer() {
    mainSched()->go([&]() {
        http::Server().on_req([](const http::Req& req, http::Res& res) {
            if (req.is_method_get()) {
                cout << "req url " << req.url();
                if (req.url() == "/hello") {
                    res.set_status(200);
                    res.set_body("hello get");
                } else {
                    res.set_status(404);
                }
            } else {
                res.set_status(405); // method not allowed
            }
        }).start(FLG_ip.c_str(), FLG_port, FLG_key.c_str(), FLG_ca.c_str());
    });
}