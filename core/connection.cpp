#include "connection.h"

#include <functional>

void Connection::run() {
    go read;
}

void Connection::operator<<(std::string& buf) {
    m_rChannel << buf;
}

void Connection::read() {
    while (true) {
        std::string buf;
        m_rChannel >> buf;
    }
}