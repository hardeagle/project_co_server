#ifndef __CORE_REDIS_POOL_H__
#define __CORE_REDIS_POOL_H__

#include <memory>

#include <boost/exception/all.hpp>
#include <boost/lexical_cast.hpp>

#include <hiredis/hiredis.h>

#include <libgo/libgo.h>

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

    template<typename T>
    std::set<T> get() {
        try {
            if (!checkReply(m_reply)) {
                LOG(ERROR) << "reply error";
                return {};
            }
            if (m_reply->type == REDIS_REPLY_NIL) {
                return {};
            }
            std::set<T> vals;
            if (m_reply->type == REDIS_REPLY_INTEGER) {
                vals.insert(std::move(boost::lexical_cast<T>(m_reply->integer)));
                return true;
            }
            if (m_reply->type == REDIS_REPLY_STRING) {
                vals.insert(std::move(boost::lexical_cast<T>(std::string(m_reply->str, m_reply->len))));
                return true;
            }
            if (m_reply->type == REDIS_REPLY_ARRAY) {
                for (size_t i = 0; i < m_reply->elements; ++i) {
                    vals.insert(std::move(boost::lexical_cast<T>(std::string(
                        m_reply->element[i]->str, m_reply->element[i]->len
                    ))));
                }
            }
        } catch(...) {
            LOG(ERROR) << "get fail";
            return {};
        }
    }

private:
    redisReply* m_reply;
};

class RedisConnection {
public:
    using ptr = std::shared_ptr<RedisConnection>;

    RedisConnection(const std::string& ip, uint16_t port) {
        m_ctx = redisConnect(ip.data(), port);
        if (m_ctx == NULL || m_ctx->err) {
            if (m_ctx) {
                LOG(ERROR) << "RedisConnection error: " << m_ctx->err;
                redisFree(m_ctx);
            } else {
                LOG(ERROR) << "RedisConnection error, can't allocate redis context";
            }
        }
    }

    ~RedisConnection() {
        if (m_ctx) {
            redisFree(m_ctx);
        }
    }

    template<typename ...Args>
    RedisResult exec(Args... args) {
        std::vector<std::string> cmds;
        convert(cmds, args...);
        size_t argc = cmds.size();
        std::vector<const char*> argvs(argc);
        std::vector<size_t> argvs_len(argc);
        for (size_t i = 0; i < argc; ++i) {
            argvs[i] = cmds[i].data();
            argvs_len[i] = cmds[i].size();
        }
        redisReply *reply = (redisReply*)redisCommandArgv(m_ctx, argc, &argvs[0], &argvs_len[0]);
        return reply ? RedisResult(reply) : RedisResult();
    }

private:
    template<typename T, typename ...Args>
    void convert(std::vector<T>& vecs, Args... args) {
        (..., vecs.emplate_back(std::move(boost::lexical_cast<T>(args))));
    }

private:
    redisContext* m_ctx;
};

class RedisManager {
public:
    using ptr = std::shared_ptr<RedisManager>;

    RedisManager(const std::string& ip, uint16_t port)
        : m_pool([=] {
            return new RedisConnection(ip, port);
        }, NULL, 1024, 1000) {
        m_pool.Reserve(1000);
    }

    // set -----------------------------------------
    template<typename T>
    bool sadd(const std::string& key, const T& member) {
        auto rc = get();
        if (!rc) {
            LOG(ERROR) << "get fail";
            return false;
        }
        rc->exec("sadd", key, member);
        return true;
    }

    template<typename T>
    std::set<T> smembers(const std::string& key) {
        auto rc = get();
        if (!rc) {
            LOG(ERROR) << "get fail";
            return {};
        }
        return rc->exec("smembers", key).get<T>();
    }

private:
    RedisConnection::ptr get() {
        return m_pool.Get();
    }

private:
    co::ConnectionPool<RedisConnection> m_pool;
};


}

#endif