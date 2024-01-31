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

std::string BaseRoleInfoSetKey(uint64_t id) {
    std::string key = PREFIX + "base_role_info_set_";
    key += std::to_string(id);
    return key;
}

std::string RoleIdToGameIdSetKey(uint64_t id) {
    std::string key = PREFIX + "roleid_to_gameid_set_";
    key += std::to_string(id);
    return key;
}

std::string RankZsetKey(uint32_t id) {
    std::string key = PREFIX + "game_zset_";
    key += std::to_string(id);
    return key;
}