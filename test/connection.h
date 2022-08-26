#ifndef __TEST_CONNECTION_H__
#define __TEST_CONNECTION_H__

#include <atomic>
#include <memory>

#include "core/session.h"

// class Connection : public std::enable_shared_from_this<Connection> {
// public:
//     using ptr = std::shared_ptr<Connection>;

//     Connection(const std::string& ip, int port);

//     int fd() const { return m_fd; }

//     void run();

//     void sync_write(uint16_t msg_id, uint16_t receiver_id, std::string& buffer);

// private:
//     void sync_connect();

//     void sync_read();

// private:
//     int m_fd;

//     std::string m_ip;
//     int m_port;

//     std::atomic_int32_t m_count;
// };

class Connection : public Eayew::Session {
public:
    Connection();

    bool sync_connect(const std::string& ip, uint16_t port, uint16_t sender, uint16_t receiver);

private:
    uint16_t m_sender;
    uint16_t m_receiver;

};

#endif