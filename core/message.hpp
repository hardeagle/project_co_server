#ifndef __CORE_MESSAGE_H__
#define __CORE_MESSAGE_H__

#include <memory>
#include <string>
#include <string_view>

#include "buffer.hpp"

namespace Eayew {

constexpr int BuFFER_HEAD_RESERVED = 14;

// length(4) + sender_id(4) + receiver_id(4) + role_id(4) + session_id(4)

class Message final {
public:
    using ptr = std::shared_ptr<Message>;

    static buffer_ptr createBuffer(size_t capacity = 64, uint32_t head_reserved = BuFFER_HEAD_RESERVED) {
        return std::make_shared<buffer>();
    }

    Message() {
        m_data = std::make_shared<buffer>();
    }

    void setSender(uint32_t sender_id) {
        m_senderId = sender_id;
    }

    void setReceiver(uint32_t recevier_id) {
        m_receiverId = recevier_id;
    }

    void setRoleId(uint32_t role_id) {
        m_roleId = role_id;
    }

    void setSessionId(uint32_t session_id) {
        m_sessionId = session_id;
    }

    void writeData(std::string_view sv) {
        uint32_t length = 20 + sv.size();
        m_data->writeBack(&length, 1);
        m_data->writeBack(&m_senderId, 1);
        m_data->writeBack(&m_receiverId, 1);
        m_data->writeBack(&m_roleId, 1);
        m_data->writeBack(&m_sessionId, 1);
        m_data->writeBack(sv.data(), sv.size());
    }

    buffer_ptr getBuffer() {
        return m_data;
    }

    const char* data() const {
        return m_data ? m_data->data() : nullptr;
    }

    size_t size() const {
        return m_data ? m_data->size() : 0;
    }

    void reset() {
        m_type = 0;
        m_senderId = 0;
        m_receiverId = 0;
        m_roleId = 0;
        m_sessionId = 0;
        if (m_data) {
            m_data->clear();
        }
    }

private:
    uint8_t m_type = 0;
    uint32_t m_senderId = 0;
    uint32_t m_receiverId = 0;
    uint32_t m_roleId = 0;
    uint32_t m_sessionId = 0;
    buffer_ptr m_data;
};

};

#endif