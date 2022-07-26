#include "redis_key.h"

static const std::string PREFIX = "EAYEW_";

std::string LoginNameToRoleIdKey(const std::string& loginname) {
    std::string key = PREFIX;
    key += loginname;
    return key;
}

std::string RoleIdIncrementKey() {
    static std::string key;
    key = PREFIX + "role_id_increment";
    return key;
}
