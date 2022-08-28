#ifndef __CORE_UTIL_BASE_H__
#define __CORE_UTIL_BASE_H__

#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif