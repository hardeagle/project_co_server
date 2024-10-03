#include "game_server.h"

#include <co/all.h>

#include "core/servlet.h"


void GameServer::beforeRun() {
    initByConfig("./json/game_server.json");

    initServlet();
}

void GameServer::initServlet() {
    //servlet()->addServlet(0,  10, nullptr);
}