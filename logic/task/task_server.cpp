#include "task_server.h"

#include "log/glog.h"

#include "core/servlet.h"

void TaskServer::beforeRun() {
    initRpc("./json/task_server.json");
}