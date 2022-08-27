#ifndef __TEST_CONNECTION_H__
#define __TEST_CONNECTION_H__

#include <atomic>
#include <memory>

#include "core/session.h"

class Connection : public Eayew::Session {
public:
    Connection();

    bool sync_connect(const std::string& ip, uint16_t port, uint16_t sender, uint16_t receiver);

private:
    uint16_t m_sender;
    uint16_t m_receiver;

};

#endif