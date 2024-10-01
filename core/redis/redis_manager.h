#ifndef __CORE_REDIS_MANAGER_H__
#define __CORE_REDIS_MANAGER_H__

#include <memory>

#include <boost/exception/all.hpp>
#include <boost/lexical_cast.hpp>

#include <hiredis/hiredis.h>

#include <co/all.h>

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
                ELOG << "reply error";
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
            ELOG << "get fail " << boost::current_exception_diagnostic_information();
            return {};
        }
        return {};
    }

    template<typename T, bool>
    std::set<T> get() {
        try {
            if (!checkReply(m_reply)) {
                ELOG << "reply error";
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
            ELOG << "get fail " << boost::current_exception_diagnostic_information();
            return {};
        }
        return {};
    }

    template<typename K, typename V>
    std::map<K, V> get() {
        try {
            if (!checkReply(m_reply)) {
                ELOG << "reply error";
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
            ELOG << "get fail " << boost::current_exception_diagnostic_information();
            return {};
        }
        return {};
    }

    template<typename K, typename V, bool>
    std::list<std::pair<K, V>> get() {
        try {
            if (!checkReply(m_reply)) {
                ELOG << "reply error";
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
            ELOG << "get fail " << boost::current_exception_diagnostic_information();
            return {};
        }
        return {};
    }

private:
    bool checkReply(redisReply* reply) {
        if (!reply) {
            ELOG << "reply is null!";
            return false;
        }
        if (reply->type == REDIS_REPLY_ERROR) {
            ELOG << "redis reply error:" << std::string(reply->str, reply->len);
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
        LOG << "ip " << ip << " port " << port;
        m_ctx = redisConnect(ip.data(), port);
        if (m_ctx == NULL || m_ctx->err) {
            if (m_ctx) {
                ELOG << "RedisConnection error: " << m_ctx->err;
                redisFree(m_ctx);
                m_ctx = NULL;
            } else {
                ELOG << "RedisConnection error, can't allocate redis context";
            }
        }
    }

    ~RedisConnection() {
        if (m_ctx) {
            redisFree(m_ctx);
            m_ctx = NULL;
        }
    }

    template<typename T>
    RedisResult exec(std::string cmd, std::set<T>& params) {
        if (m_ctx == NULL || m_ctx->err) {
            return RedisResult();
        }

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
        if (m_ctx == NULL || m_ctx->err) {
            return RedisResult();;
        }

        std::vector<std::string> cmds;
        convert(cmds, args...);
        for (const auto& val : cmds) {
            LOG << "val " << val;
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

private:
    template<typename T, typename U, typename ...Args>
    void convert(std::vector<T>& vecs, U val,  Args... args) {
        vecs.push_back(boost::lexical_cast<std::string>(val));
        convert(vecs, args...);
    }

    template<typename T, typename ...Args>
    void convert(std::vector<T>& vecs) {
    }

private:
    redisContext* m_ctx;
};

class RedisManager {
public:
    using ptr = std::shared_ptr<RedisManager>;

    RedisManager(const std::string& ip, uint16_t port)
        : m_pool(
            [ip, port]() {
                LOG << "RedisManager ip " << ip << " port " << port; 
                return (void*) new Eayew::RedisConnection(ip, port); }, // ccb
            [](void* p) { delete (Eayew::RedisConnection*) p; }, // dcb
            128
        ) {
    }

    // RedisManager(const std::string& ip, uint16_t port)
    //     : m_pool([&]() { return (void*) co::make<RedisConnection>(ip, port); }, // ccb
    //             [](void* p) { co::del((RedisConnection*)p); },        // dcb
    //             128                                    // max capacity
    //             ) {
    // }

    // RedisManager(const std::string& ip, uint16_t port) {
    // }

    // string-------------------------------------------------------------------
    template<typename T>
    void set(const std::string& key, const T& val) {
        co::pool_guard<RedisConnection> pg(m_pool);
        pg.get()->exec("set", key, val);
    }

    template<typename T>
    T get(const std::string& key) {
        co::pool_guard<RedisConnection> pg(m_pool);
        return pg.get()->exec("get", key).template get<T>();
    }

    template<typename T, typename U>
    std::set<T> mget(std::set<U>& keys) {
        co::pool_guard<RedisConnection> pg(m_pool);
        return pg.get()->exec<U>("mget", keys).template get<T, true>();
    }

    // set ---------------------------------------------------------------------
    template<typename T>
    bool sadd(const std::string& key, const T& member) {
        co::pool_guard<RedisConnection> pg(m_pool);
        pg.get()->exec("sadd", key, member);
        return true;
    }

    template<typename T>
    bool srem(const std::string& key, const std::set<T>& members) {
        if (members.empty()) {
            return true;
        }
        co::pool_guard<RedisConnection> pg(m_pool);
        pg.get()->exec("srem", key, members);
        return true;
    }

    template<typename T>
    std::set<T> smembers(const std::string& key) {
        co::pool_guard<RedisConnection> pg(m_pool);
        return pg.get()->exec("smembers", key).get<T>();
    }

    // sorted set------------------------------------------------------
    template<typename T>
    bool zadd(const std::string& key, int score, const T& member) {
        co::pool_guard<RedisConnection> pg(m_pool);
        pg.get()->exec("zadd", key, score, member);
        return true;
    }

    template<typename T>
    uint32_t zscore(const std::string& key, const T& member) {
        co::pool_guard<RedisConnection> pg(m_pool);
        return pg.get()->exec("zscore", key, member).template get<uint32_t>();
    }

    template<typename T, typename U>
    T zrevrank(const std::string& key, U& member) {
        co::pool_guard<RedisConnection> pg(m_pool);
        return pg.get()->exec("zrevrank", key, member).template get<T>();
    }

    template<typename K, typename V>
    std::list<std::pair<K, V>> zrevrange(const std::string& key, int start, int stop) {
        co::pool_guard<RedisConnection> pg(m_pool);
        return pg.get()->exec("zrevrange", key, start, stop, "withscores").template get<K, V, true>();
    }

    // hash-----------------------------------------------------------------------------------
    template<typename T>
    uint64_t hincrby(const std::string& key, const T& field, uint32_t increment) {
        co::pool_guard<RedisConnection> pg(m_pool);
        return pg.get()->exec("hincrby", key, field, increment).template get<uint64_t>();
    }

    template<typename T, typename U>
    void hset(const std::string& key, const T& field, const U& val) {
        co::pool_guard<RedisConnection> pg(m_pool);
        pg.get()->exec("hset", key, field, val);
    }

    template<typename T, typename U>
    U hget(const std::string& key, const T& field) {
        co::pool_guard<RedisConnection> pg(m_pool);
        return pg.get()->exec("hget", key, field).get();
    }

    template<typename K, typename V>
    std::map<K, V> hgetall(const std::string& key) {
        co::pool_guard<RedisConnection> pg(m_pool);
        return pg.get()->exec("hgetall", key).template get<K, V>();
    }

    template<typename T>
    void del(std::set<T>& keys) {
        co::pool_guard<RedisConnection> pg(m_pool);
        pg.get()->exec<T>("del", keys);
    }

private:
    co::pool m_pool;
};


}

#endif