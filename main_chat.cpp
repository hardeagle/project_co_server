#include <memory>

#include "logic/chat/chat_server.h"

#include <co/all.h>

int main(int argc, char* argv[]) {
    auto chat_server = std::make_shared<ChatServer>();
    chat_server->run();

    return 0;
}