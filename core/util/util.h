
#ifndef __CORE_UTIL_UTIL_H__
#define __CORE_UTIL_UTIL_H__

#include <time.h>

#include <string>

namespace Eayew {

uint32_t getCurSecond();

std::string serverId(const std::string& name, u_int16_t type, const std::string& ip, u_int16_t port);

uint16_t getTypeByName(const std::string& name);

}

#endif