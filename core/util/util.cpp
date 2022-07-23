#include "util.h"

#include <sstream>

#include "log/glog.h"

namespace Eayew {

uint32_t getCurSecond() {
    time_t t = time(NULL);
    return uint32_t(t);
}

std::string serverId(const std::string& name, uint16_t type, const std::string& ip, uint16_t port) {
    std::stringstream ss;
    ss << name << "_" << type << "_" << ip << "_" << port;
    return std::move(ss.str());
}

uint16_t getTypeByName(const std::string& name) {
    uint16_t type = 0;
    if (name == "gate") {
        type = 1;
    } else if (name == "login") {
        type = 2;
    } else if (name == "game") {
        type = 3;
    }
    return type;
}


}