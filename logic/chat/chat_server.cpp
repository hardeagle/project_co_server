#include "chat_server.h"

#include <co/all.h>

#include "core/servlet.h"

#include "logic/protocol/chat_id.pb.h"
#include "logic/chat/servlet/chat_servlet.h"

void ChatServer::beforeRun() {
    LOG << "ChatServer begin...";
    initByConfig("./json/chat_server.json");
    LOG << "ChatServer end...";

    initServlet();
}

void ChatServer::initServlet() {
    Eayew::Servlet::ptr tmp_servlet;
    tmp_servlet.reset(new ChatServlet);
    servlet()->addServlet(ChatProtocol::ID::C2S_CHAT_BASE, ChatProtocol::ID::C2S_CHAT_TOP, tmp_servlet);
}