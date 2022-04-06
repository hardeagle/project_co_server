#include "task_server.h"

#include "log/glog.h"

#include "core/servlet.h"

void TaskServer::beforeRun() {
    initByConfig("./json/task_server.json");
}