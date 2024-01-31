#ifndef __LOGIC_COMMON_REDIS_KEY_H__
#define __LOGIC_COMMON_REDIS_KEY_H__

#include <string>

std::string LoginNameToRoleIdSetKey(const std::string& loginname);

std::string RoleIdIncrementKey();

#endif