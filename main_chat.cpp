#include <memory>

#include "logic/chat/chat_server.h"

#include "log/glog.h"

int main(int argc, char* argv[]) {
    GLog glog(argv[0]);

    auto chat_server = std::make_shared<ChatServer>();
    chat_server->run();

    //chat_server->test();

    return 0;
}