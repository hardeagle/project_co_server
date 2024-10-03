#include "connection.h"

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <co/all.h>

#include "core/message.h"
#include "core/session.h"

Connection::Connection()
    : Eayew::Session() {
}

bool Connection::sync_connect(const std::string& ip, uint16_t port, uint16_t sender, uint16_t receiver) {
    if (!Eayew::Session::sync_connect(ip, port)) {
        ELOG << "sync_connect fail";
        return false;
    }
    m_sender = sender;
    m_receiver = receiver;

    return true;
}