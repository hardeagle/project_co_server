#include "redis_key.h"

static const std::string PREFIX = "pcs_";

std::string LoginNameToRoleIdSetKey(const std::string& loginname) {
    std::string key = PREFIX + "loginname_to_roleid_set_";
    key += loginname;
    return key;
}

std::string RoleIdIncrementKey() {
    static std::string key;
    key = PREFIX + "role_id_increment";
    return key;
}
