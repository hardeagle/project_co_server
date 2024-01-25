#include "chat_server.h"

#include "log/glog.h"

#include "core/servlet.h"

#include "logic/chat/protocol/chat_id.pb.h"
#include "logic/chat/servlet/chat_servlet.h"

void ChatServer::beforeRun() {
    LOG(INFO) << "ChatServer begin...";
    initByConfig("./json/chat_server.json");
    LOG(INFO) << "ChatServer end...";

    initServlet();
}

void ChatServer::initServlet() {
    Eayew::Servlet::ptr tmp_servlet;
    tmp_servlet.reset(new ChatServlet);
    servlet()->addServlet(ChatProtocol::ID::C2S_CHAT_BASE, ChatProtocol::ID::C2S_CHAT_TOP, tmp_servlet);
}