#include "chat_server.h"

#include "log/glog.h"

#include "core/servlet.h"

void ChatServer::beforeRun() {
    LOG(INFO) << "ChatServer begin...";
    initByConfig("./json/chat_server.json");
    LOG(INFO) << "ChatServer end...";
}

void ChatServer::test() {

}