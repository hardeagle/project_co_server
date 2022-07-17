#ifndef __CORE_MESSAGE_H__
#define __CORE_MESSAGE_H__

#include <memory>
#include <string>
#include <string_view>

#include "log/glog.h"

#include "buffer.hpp"

namespace Eayew {

constexpr int BuFFER_HEAD_RESERVED = 14;

// length(2) + sender_id(2) + receiver_id(2) + mgs_id(2) + session_id(4) + role_id/checksum(8)

class Message final {
public:
    using ptr = std::shared_ptr<Message>;

    static const int LEN_SIZE = 2;
    static const int HEAD_LEN = 24;

    // static buffer_ptr createBuffer(size_t capacity = 64, uint32_t head_reserved = BuFFER_HEAD_RESERVED) {
    //     return std::make_shared<buffer>();
    // }

    Message() {
        m_data = std::make_shared<buffer>();
    }

    ~Message()
    {
    }

    //Message(const Message&) = delete;

    //Message& operator=(const Message&) = delete;

    // Message(Message&& other) noexcept
    //     : m_senderId(std::exchange(other.m_senderId, 0))
    //     , m_receiverId (std::exchange(other.m_receiverId, 0))
    //     , m_msgId(std::exchange(other.m_msgId, 0))
    //     , m_sessionId(std::exchange(other.m_sessionId, 0))
    //     , m_roleId(std::exchange(other.m_roleId, 0))
    //     , m_data(std::move(other.m_data)) {
    //     LOG(ERROR) << "move000000";
    // }

    // Message& operator=(Message&& other) noexcept
    // {
    //     if (this != std::addressof(other)) {
    //         m_senderId = std::exchange(other.m_senderId, 0);
    //         m_receiverId = std::exchange(other.m_receiverId, 0);
    //         m_msgId = std::exchange(other.m_msgId, 0);
    //         m_sessionId = std::exchange(other.m_sessionId, 0);
    //         m_roleId = std::exchange(other.m_roleId, 0);
    //         m_data = std::move(other.m_data);
    //         LOG(ERROR) << "move1111";
    //     }
    //     return *this;
    // }

    void setSender(uint16_t sender_id) {
        m_senderId = sender_id;
    }

    void setReceiver(uint16_t recevier_id) {
        m_receiverId = recevier_id;
    }

    void setMsgId(uint16_t msg_id) {
        m_msgId = msg_id;
    }

    void setSessionId(uint64_t session_id) {
        m_sessionId = session_id;
    }

    void forceSetSessionId(uint64_t session_id) {
        uint64_t* p = (uint64_t*)(m_data->data() + 8);
        *p = session_id;
    }

    void forceSetRoleId(uint64_t role_id) {
        uint64_t* p = (uint64_t*)(m_data->data() + 16);
        *p = role_id;
    }

    void writeData(std::string_view sv) {
        uint16_t length = HEAD_LEN + sv.size();
        m_data->writeBack(&length, 1);
        m_data->writeBack(&m_senderId, 1);
        m_data->writeBack(&m_receiverId, 1);
        m_data->writeBack(&m_msgId, 1);
        m_data->writeBack(&m_sessionId, 1);
        m_data->writeBack(&m_roleId, 1);
        m_data->writeBack(sv.data(), sv.size());
    }

    buffer_ptr getBuffer() {
        return m_data;
    }

    const char* data() const {
        return m_data ? m_data->data() : nullptr;
    }

    const char* realData() const {
        return m_data ? m_data->data() + HEAD_LEN : nullptr;
    }

    char* wbuffer() {
        return m_data ? m_data->wdata() : nullptr;
    }

    void commit(size_t n) {
        m_data->commit(n);
    }

    void prepare(size_t n) {
        m_data->prepare(n);
    }

    size_t size() const {
        return m_data ? m_data->size() : 0;
    }

    size_t realSize() const {
        return m_data ? m_data->size() - HEAD_LEN : 0;
    }

    bool isValid() {
        if (size() < HEAD_LEN) {
            return false;
        }
        int len = *((uint32_t*)(m_data->data()));
        if (len != size()) {
            return false;
        }
        return true;
    }

    uint16_t length() {
        return *((uint16_t*)(m_data->data() + 0));
    }

    uint16_t senderId() {
        return *((uint16_t*)(m_data->data() + 2));
    }

    uint16_t receiverId() {
        return *((uint16_t*)(m_data->data() + 4));
    }

    uint16_t msgId() {
        return *((uint16_t*)(m_data->data() + 6));
    }

    uint32_t realMsgId() {
        auto rid = receiverId();
        auto mid = msgId();
        return (uint32_t(rid) << 16) + mid;
    }

    uint64_t sessionId() {
        return *((uint64_t*)(m_data->data() + 8));
    }

    uint64_t roleId() {
        return *((uint64_t*)(m_data->data() + 16));
    }

    void clear() {
        m_senderId = 0;
        m_receiverId = 0;
        m_msgId = 0;
        m_sessionId = 0;
        if (m_data) {
            m_data->clear();
        }
    }

    void debugString() {
        LOG(INFO) << "message size " << size() << " length " << length() << " sender " << senderId()
                    << " receiver " << receiverId() << " session id " << sessionId() << " msg id " << msgId()
                    << " ,role id " << roleId();
    }

    std::string strInfo() {
        std::stringstream ss;
        ss << "message size " << size() << " length " << length() << " sender " << senderId()
                    << " receiver " << receiverId() << " session id " << sessionId() << " msg id " << msgId()
                    << " ,role id " << roleId();
        return ss.str();
    }

private:
    uint16_t m_senderId = 0;
    uint16_t m_receiverId = 0;
    uint16_t m_msgId = 0;
    uint64_t m_sessionId = 0;
    uint64_t m_roleId = 0;
    buffer_ptr m_data;
};

};

#endif