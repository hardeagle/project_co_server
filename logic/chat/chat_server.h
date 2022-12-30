#ifndef __LOGIC_CHAT_CHAT_SERVER_H__
#define __LOGIC_CHAT_CHAT_SERVER_H__

#include "core/const.hpp"
#include "core/base/base_server.h"

class ChatServer : public Eayew::BaseServer {
public:
    using ptr = std::shared_ptr<ChatServer>;

    ChatServer() : Eayew::BaseServer(Eayew::ServerType::CHAT) {}

    void beforeRun();

    void test();

private:

};

#endif