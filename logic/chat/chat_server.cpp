#include "chat_server.h"

#include "log/glog.h"

#include "core/servlet.h"

void ChatServer::beforeRun() {
    initByConfig("./json/chat_server.json");
}

void ChatServer::test() {

}