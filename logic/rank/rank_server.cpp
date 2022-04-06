#include "rank_server.h"

#include "log/glog.h"

#include "core/servlet.h"

void RankServer::beforeRun() {
    initByConfig("./json/rank_server.json");
}