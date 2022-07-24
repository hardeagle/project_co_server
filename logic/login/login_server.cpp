#include "login_server.h"

#include "core/servlet.h"

#include "log/glog.h"


#include "logic/login/protocol/login_id.pb.h"
#include "logic/login/servlet/login_servlet.h"
#include "logic/login/server_resource.h"


void LoginServer::beforeRun() {
    initByConfig("./json/login_server.json");

    ServerResource::get()->init();

    initServlet();
}

void LoginServer::initServlet() {  
    Eayew::Servlet::ptr tmp_servlet;
    tmp_servlet.reset(new LoginServlet);
    servlet()->addServlet(LoginProtocol::ID::C2S_LOGIN_BASE, LoginProtocol::ID::C2S_LOGIN_TOP, tmp_servlet);

}