
#ifndef __CORE_MESSAGE_H__
#define __CORE_MESSAGE_H__

#include <memory>
#include <string>
#include <string_view>

#include "log/glog.h"

namespace Eayew {

// length(2) + sender_id(2) + receiver_id(2) + mgs_id(2) + session_id(4) + role_id/checksum(8)

class Message final {
public:
    using ptr = std::shared_ptr<Message>;

    static const uint32_t LEN_SIZE = 2;
    static const uint32_t SENDER_ID_SIZE = 2;
    static const uint32_t RECEIVER_ID_SIZE = 2;
    static const uint32_t MSG_ID_SIZE = 2;
    static const uint32_t SESSION_ID_SIZE = 8;
    static const uint32_t ROLE_ID_SIZE = 8;
    static const uint32_t HEAD_LEN = LEN_SIZE + SENDER_ID_SIZE + RECEIVER_ID_SIZE + MSG_ID_SIZE + SESSION_ID_SIZE + ROLE_ID_SIZE;

    Message()
        : m_data(nullptr)
        , m_capacity(0) {
    }

    Message(uint16_t ds) {
        m_capacity = ds + HEAD_LEN;
        m_data = (char*)malloc(m_capacity);
        length(m_capacity);
    }

    ~Message() {
        // if (m_data && m_capacity > 0) {
        //     free(m_data);
        // }
    }

    Message(const Message& other) noexcept {
        m_data = other.m_data;
        m_capacity = other.m_capacity;
        // LOG(ERROR) << "copy";
    }

    Message& operator=(const Message& other) noexcept {
        if (this != std::addressof(other)) {
            m_data = other.m_data;
            m_capacity = other.m_capacity;
            // LOG(ERROR) << "assign";
        }
        return *this;
    }

    Message(Message&& other) noexcept
        : m_data(std::exchange(other.m_data, nullptr))
        , m_capacity(std::exchange(other.m_capacity, 0)) {
        //LOG(ERROR) << "move0";
    }

    Message& operator=(Message&& other) noexcept {
        if (this != std::addressof(other)) {
            m_data = std::exchange(other.m_data, nullptr);
            m_capacity = std::exchange(other.m_capacity, 0);
            //LOG(ERROR) << "move1111";
        }
        return *this;
    }

    void length(uint16_t val) { *((decltype(val)*)(m_data)) = val; }
    auto length() { return *((uint16_t*)(m_data)); }

    void senderId(uint16_t val) { *((decltype(val)*)(m_data + LEN_SIZE)) = val; }
    auto senderId() { return *((uint16_t*)(m_data + LEN_SIZE)); }

    void receiverId(uint16_t val) { *((decltype(val)*)(m_data + LEN_SIZE + SENDER_ID_SIZE)) = val; }
    auto receiverId() { return *((uint16_t*)(m_data + LEN_SIZE + SENDER_ID_SIZE)); }

    void msgId(uint16_t val) { *((decltype(val)*)(m_data + LEN_SIZE + SENDER_ID_SIZE + RECEIVER_ID_SIZE)) = val; }
    auto msgId() { return *((uint16_t*)(m_data + LEN_SIZE + SENDER_ID_SIZE + RECEIVER_ID_SIZE)); }

    void sessionId(uint64_t val) { *((decltype(val)*)(m_data + LEN_SIZE + SENDER_ID_SIZE + RECEIVER_ID_SIZE + MSG_ID_SIZE)) = val; }
    auto sessionId() { return *((uint64_t*)(m_data + LEN_SIZE + SENDER_ID_SIZE + RECEIVER_ID_SIZE + MSG_ID_SIZE)); }

    void roleId(uint64_t val) { *((decltype(val)*)(m_data + LEN_SIZE + SENDER_ID_SIZE + RECEIVER_ID_SIZE + MSG_ID_SIZE + SESSION_ID_SIZE)) = val; }
    auto roleId() { return *((uint64_t*)(m_data + LEN_SIZE + SENDER_ID_SIZE  + RECEIVER_ID_SIZE + MSG_ID_SIZE + SESSION_ID_SIZE)); }

    uint32_t realMsgId() {
        auto rid = receiverId();
        auto mid = msgId();
        return (uint32_t(rid) << 16) + mid;
    }

    char* data() {
        return m_data;
    }

    uint16_t size() {
        return length();
    }

    char* pdata() {
        return m_data + HEAD_LEN;
    }

    uint16_t psize() {
        return length() - HEAD_LEN;
    }

    void write(const char* p, uint16_t size) {
        if (size > m_capacity - HEAD_LEN) {
            LOG(ERROR) << "segment fault";
            return;
        }
        memcpy(m_data + HEAD_LEN, p, size);
    }

    void prepare(uint16_t size) {
        if (size < m_capacity - HEAD_LEN) {
            return;
        }
        auto ptmp = (char*)malloc(size - HEAD_LEN);
        memcpy(ptmp, m_data, m_capacity);
        free(m_data);
        m_capacity = size - HEAD_LEN;
        m_data = ptmp;
    }

    std::string strInfo() {
        std::stringstream ss;
        ss << "message size " << size() << " length " << length() << " sender " << senderId()
                    << " receiver " << receiverId() << " session id " << sessionId() << " msg id " << msgId()
                    << " ,role id " << roleId();
        return ss.str();
    }

private:
    size_t nextPow2(size_t x) {
        if (!(x & (x - 1))) {
            return x;
        }
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        return x + 1;
    }

private:
    char* m_data;
    size_t m_capacity;
};

};

#endif