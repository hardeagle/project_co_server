#include "redis_key.h"

static const std::string PREFIX = "EAYEW_";

std::string LoginNameToRoleIdKey(const std::string& loginname) {
    std::string key = PREFIX;
    key += loginname;
    return key;
}
