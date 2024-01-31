#ifndef __LOGIC_COMMON_REDIS_KEY_H__
#define __LOGIC_COMMON_REDIS_KEY_H__

#include <string>

std::string LoginNameToRoleIdSetKey(const std::string& loginname);

std::string RoleIdIncrementKey();

std::string BaseRoleInfoSetKey(uint64_t id);

std::string RoleIdToGameIdSetKey(uint64_t id);

std::string RankZsetKey(uint32_t gameid);

#endif