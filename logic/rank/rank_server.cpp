#include "rank_server.h"

#include "log/glog.h"

#include "core/servlet.h"

#include "logic/rank/protocol/rank_id.pb.h"
#include "logic/rank/servlet/rank_servlet.h"
#include "logic/rank/server_resource.h"

void RankServer::beforeRun() {
    initByConfig("./json/login_server.json");

    ServerResource::get()->init();

    initServlet();
}

void RankServer::initServlet() {
    Eayew::Servlet::ptr tmp_servlet;
    tmp_servlet.reset(new RankServlet);
    servlet()->addServlet(RankProtocol::ID::C2S_RANK_BASE, RankProtocol::ID::C2S_RANK_TOP, tmp_servlet);
}