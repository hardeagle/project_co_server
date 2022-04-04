#include "game_server.h"

#include "log/glog.h"

#include "core/servlet.h"


void GameServer::beforeRun() {
    initRpc("./json/game_server.json");

    initServlet();
}

void GameServer::initServlet() {
    servlet()->addServlet(0,  10, nullptr);
}