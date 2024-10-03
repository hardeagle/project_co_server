#include "task_server.h"

#include <co/all.h>

#include "core/servlet.h"

void TaskServer::beforeRun() {
    initByConfig("./json/task_server.json");
}