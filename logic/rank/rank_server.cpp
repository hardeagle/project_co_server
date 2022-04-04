#include "rank_server.h"

#include "log/glog.h"

#include "core/servlet.h"

void RankServer::beforeRun() {
    initRpc("./json/rank_server.json");
}