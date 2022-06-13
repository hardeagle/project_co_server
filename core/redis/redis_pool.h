#ifndef __CORE_REDIS_POOL_H__
#define __CORE_REDIS_POOL_H__

#include <boost/exception/all.hpp>
#include <boost/lexical_cast.hpp>

#include <hiredis/hiredis.h>

#include  "log/glog.h"

namespace Eayew {

bool checkReply(redisReply* reply) {
    if (!reply) {
        LOG(ERROR) << "reply is null!";
        return false;
    }
    if (reply->type == REDIS_REPLY_ERROR) {
        LOG(ERROR) << "redis reply error:" << std::string(reply->str, reply->len);
        return false;
    }
    return true;
}

class RedisResult {
public:

    RedisResult()
        : m_reply(nullptr) {
    }

    RedisResult(redisReply* reply)
        : m_reply(reply) {
    }

    ~RedisResult() {
        if (m_reply) {
            freeReplyObject(m_reply);
        }
    }

    template<typename T>
    bool get(T& val) {
        try {
            if (!checkReply(m_reply)) {
                LOG(ERROR) << "reply error";
                return false;
            }
            if (m_reply->type == REDIS_REPLY_NIL) {
                return true;
            }
            if (m_reply->type == REDIS_REPLY_INTEGER) {
                val = boost::lexical_cast<T>(m_reply->integer);
                return true;
            }
            if (m_reply->type == REDIS_REPLY_STRING) {
                val = boost::lexical_cast<T>(std::string(m_reply->str, m_reply->len));
                return true;
            }
            if (m_reply->type == REDIS_REPLY_ARRAY && m_reply->elements != 0) {
                val = boost::lexical_cast<T>(std::string(m_reply->element[0]->str, m_reply->element[0]->len));
            }
        } catch(...) {
            LOG(ERROR) << "get fail";
            return false;
        }
        return true;
    }

private:
    redisReply* m_reply;
};

class RedisPool {

};

}

#endif