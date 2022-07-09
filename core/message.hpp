#ifndef __CORE_MESSAGE_H__
#define __CORE_MESSAGE_H__

#include <memory>
#include <string>
#include <string_view>

#include "log/glog.h"

#include "buffer.hpp"

namespace Eayew {

constexpr int BuFFER_HEAD_RESERVED = 14;

// length(2) + sender_id(2) + receiver_id(2) + mgs_id(2) + session_id(4) + role_id(4)

class Message final {
public:
    using ptr = std::shared_ptr<Message>;

    static const int LEN_SIZE = 2;
    static const int HEAD_LEN = 12;

    // static buffer_ptr createBuffer(size_t capacity = 64, uint32_t head_reserved = BuFFER_HEAD_RESERVED) {
    //     return std::make_shared<buffer>();
    // }

    Message() {
        m_data = std::make_shared<buffer>();
    }

    void setSender(uint16_t sender_id) {
        m_senderId = sender_id;
    }

    void setReceiver(uint16_t recevier_id) {
        m_receiverId = recevier_id;
    }

    void setMsgId(uint16_t msg_id) {
        m_msgId = msg_id;
    }

    void setSessionId(uint32_t session_id) {
        m_sessionId = session_id;
    }

    void forceSetSessionId(uint32_t session_id) {
        uint32_t* p = (uint32_t*)(m_data->data() + 8);
        *p = session_id;
    }

    void writeData(std::string_view sv) {
        uint16_t length = HEAD_LEN + sv.size();
        m_data->writeBack(&length, 1);
        m_data->writeBack(&m_senderId, 1);
        m_data->writeBack(&m_receiverId, 1);
        m_data->writeBack(&m_msgId, 1);
        m_data->writeBack(&m_sessionId, 1);
        m_data->writeBack(sv.data(), sv.size());
    }

    buffer_ptr getBuffer() {
        return m_data;
    }

    const char* data() const {
        return m_data ? m_data->data() : nullptr;
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

    uint32_t sessionId() {
        return *((uint32_t*)(m_data->data() + 8));
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
                    << " receiver " << receiverId() << " session id " << sessionId() << " msg id " << msgId();
    }

private:
    uint16_t m_senderId = 0;
    uint16_t m_receiverId = 0;
    uint16_t m_msgId = 0;
    uint32_t m_sessionId = 0;
    buffer_ptr m_data;
};

};

#endif