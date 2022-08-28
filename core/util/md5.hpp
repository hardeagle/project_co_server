
#ifndef __CORE_UTIL_MD5_HPP__
#define __CORE_UTIL_MD5_HPP__

#include "md5.h"
#include <string>

namespace Eayew {

// could be compiled separately
inline std::string md5_hash_string(const std::string& s) {
	char digest[16];
	
	md5_state_t state;
	
	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)s.c_str(), s.size());
	md5_finish(&state, (md5_byte_t *)digest);
	    
    std::string ret;
    ret.resize(16);
    std::copy(digest,digest+16,ret.begin());
    
	return ret;
}

const char hexval[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

inline std::string md5_hash_hex(const std::string& input) {
    std::string hash = md5_hash_string(input);
    std::string hex;
        
    for (size_t i = 0; i < hash.size(); i++) {
        hex.push_back(hexval[((hash[i] >> 4) & 0xF)]);
        hex.push_back(hexval[(hash[i]) & 0x0F]);
    }
    
    return hex;
}

}

#endif