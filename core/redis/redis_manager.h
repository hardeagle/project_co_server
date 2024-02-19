#ifndef __CORE_REDIS_MANAGER_H__
#define __CORE_REDIS_MANAGER_H__

#include <memory>

#include <boost/exception/all.hpp>
#include <boost/lexical_cast.hpp>

#include <hiredis/hiredis.h>

#include <libgo/libgo.h>

#include  "log/glog.h"

namespace Eayew {

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
    T get() {
        try {
            if (!checkReply(m_reply)) {
                LOG(ERROR) << "reply error";
                return {};
            }
            if (m_reply->type == REDIS_REPLY_NIL) {
                return {};
            }
            if (m_reply->type == REDIS_REPLY_INTEGER) {
                return boost::lexical_cast<T>(m_reply->integer);
            }
            if (m_reply->type == REDIS_REPLY_STRING) {
                return boost::lexical_cast<T>(std::string(m_reply->str, m_reply->len));
            }
            if (m_reply->type == REDIS_REPLY_ARRAY && m_reply->elements != 0) {
                return boost::lexical_cast<T>(std::string(m_reply->element[0]->str, m_reply->element[0]->len));
            }
        } catch(...) {
            LOG(ERROR) << "get fail " << boost::current_exception_diagnostic_information();
            return {};
        }
        return {};
    }

    template<typename T, bool>
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
                return vals;
            }
            if (m_reply->type == REDIS_REPLY_STRING) {
                vals.insert(std::move(boost::lexical_cast<T>(std::string(m_reply->str, m_reply->len))));
                return vals;
            }
            if (m_reply->type == REDIS_REPLY_ARRAY) {
                for (size_t i = 0; i < m_reply->elements; ++i) {
                    vals.insert(std::move(boost::lexical_cast<T>(std::string(
                        m_reply->element[i]->str, m_reply->element[i]->len
                    ))));
                }
                return vals;
            }
        } catch(...) {
            LOG(ERROR) << "get fail " << boost::current_exception_diagnostic_information();
            return {};
        }
    }

    template<typename K, typename V>
    std::map<K, V> get() {
        try {
            if (!checkReply(m_reply)) {
                LOG(ERROR) << "reply error";
                return {};
            }
            if (m_reply->type == REDIS_REPLY_NIL) {
                return {};
            }
            if (m_reply->type != REDIS_REPLY_ARRAY) {
                return {};
            }
            if ((m_reply->elements & 0x1) != 0) {
                return {};
            }
            std::map<K, V> vals;
            for (size_t i = 0; i < m_reply->elements; i += 2) {
                K key = boost::lexical_cast<K>(std::string(m_reply->element[i]->str, m_reply->element[i]->len));
                V val = boost::lexical_cast<V>(std::string(m_reply->element[i + 1]->str, m_reply->element[i + 1]->len));
                vals[key] = val;
            }
            return vals;
        } catch(...) {
            LOG(ERROR) << "get fail " << boost::current_exception_diagnostic_information();
            return {};
        }
        return {};
    }

    template<typename K, typename V, bool>
    std::list<std::pair<K, V>> get() {
        try {
            if (!checkReply(m_reply)) {
                LOG(ERROR) << "reply error";
                return {};
            }
            if (m_reply->type == REDIS_REPLY_NIL) {
                return {};
            }
            if (m_reply->type != REDIS_REPLY_ARRAY) {
                return {};
            }
            if ((m_reply->elements & 0x1) != 0) {
                return {};
            }
            std::list<std::pair<K, V>> vals;
            for (size_t i = 0; i < m_reply->elements; i += 2) {
                K key = boost::lexical_cast<K>(std::string(m_reply->element[i]->str, m_reply->element[i]->len));
                V val = boost::lexical_cast<V>(std::string(m_reply->element[i + 1]->str, m_reply->element[i + 1]->len));
                vals.push_back(std::make_pair(key,val));
            }
            return vals;
        } catch(...) {
            LOG(ERROR) << "get fail " << boost::current_exception_diagnostic_information();
            return {};
        }
        return {};
    }

private:
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

    template<typename T>
    RedisResult exec(std::string cmd, std::set<T>& params) {
        std::vector<std::string> cmds;
        cmds.emplace_back(std::move(boost::lexical_cast<std::string>(cmd)));
        for (auto& val: params) {
            cmds.emplace_back(std::move(boost::lexical_cast<std::string>(val)));
        }
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
        (..., vecs.emplace_back(std::move(boost::lexical_cast<T>(args))));
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
        }, NULL, 1024, 128) {
        m_pool.Reserve(128);
    }

    // string-------------------------------------------------------------------
    template<typename T>
    void set(const std::string& key, const T& val) {
        auto rc = get();
        if (!rc) {
            LOG(ERROR) << "get fail";
            return;
        }
        rc->exec("set", key, val);
    }

    template<typename T>
    T get(const std::string& key) {
        auto rc = get();
        if (!rc) {
            LOG(ERROR) << "get fail";
            return {};
        }
        return rc->exec("get", key).template get<T>();
    }

    template<typename T, typename U>
    std::set<T> mget(std::set<U>& keys) {
        auto rc = get();
        if (!rc) {
            LOG(ERROR) << "get fail";
            return {};
        }
        return rc->exec<U>("mget", keys).template get<T, true>();
    }

    // set ---------------------------------------------------------------------
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
    bool srem(const std::string& key, const std::set<T>& members) {
        if (members.empty()) {
            return true;
        }
        auto rc = get();
        if (!rc) {
            LOG(ERROR) << "get fail";
            return false;
        }
        rc->exec("srem", key, members);
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

    // sorted set------------------------------------------------------
    template<typename T>
    bool zadd(const std::string& key, int score, const T& member) {
        auto rc = get();
        if (!rc) {
            LOG(ERROR) << "get fail";
            return false;
        }
        rc->exec("zadd", key, score, member);
        return true;
    }

    template<typename T>
    uint32_t zscore(const std::string& key, const T& member) {
        auto rc = get();
        if (!rc) {
            LOG(ERROR) << "get fail";
            return 0;
        }
        return rc->exec("zscore", key, member).template get<uint32_t>();
    }

    template<typename T, typename U>
    T zrevrank(const std::string& key, U& member) {
        auto rc = get();
        if (!rc) {
            LOG(ERROR) << "get fail";
            return false;
        }
        return rc->exec("zrevrank", key, member).template get<T>();
    }

    template<typename K, typename V>
    std::list<std::pair<K, V>> zrevrange(const std::string& key, int start, int stop) {
        auto rc = get();
        if (!rc) {
            LOG(ERROR) << "get fail";
            return {};
        }
        return rc->exec("zrevrange", key, start, stop, "withscores").template get<K, V, true>();
    }

    // hash-----------------------------------------------------------------------------------
    template<typename T>
    uint64_t hincrby(const std::string& key, const T& field, uint32_t increment) {
        auto rc = get();
        if (!rc) {
            LOG(ERROR) << "get fail";
            return 0;
        }
        return rc->exec("hincrby", key, field, increment).template get<uint64_t>();
    }

    template<typename T, typename U>
    void hset(const std::string& key, const T& field, const U& val) {
        auto rc = get();
        if (!rc) {
            LOG(ERROR) << "get fail";
            return;
        }
        rc->exec("hset", key, field, val);
    }

    template<typename T, typename U>
    U hget(const std::string& key, const T& field) {
        auto rc = get();
        if (!rc) {
            LOG(ERROR) << "get fail";
            return {};
        }
        return rc->exec("hget", key, field).get();
    }

    template<typename K, typename V>
    std::map<K, V> hgetall(const std::string& key) {
        auto rc = get();
        if (!rc) {
            LOG(ERROR) << "hgetall fail";
            return {};
        }
        return rc->exec("hgetall", key).template get<K, V>();
    }

    template<typename T>
    void del(std::set<T>& keys) {
        auto rc = get();
        if (!rc) {
            LOG(ERROR) << "get fail";
            return;
        }
        rc->exec<T>("del", keys);
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