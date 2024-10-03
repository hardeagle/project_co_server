#include "nats.h"

#include <co/all.h>

namespace Eayew {

Nats::Nats(std::string& ip, uint16_t port)
    : m_ip(ip)
    , m_port(port) {
}

bool Nats::init() {
    if (natsOptions_Create(&m_opts) != NATS_OK) {
        ELOG << "natsOptionas_Create fail";
        return false;
    }
    return true;
}

void Nats::publish() {
    
}

void Nats::subcribe() {

}

void Nats::request() {

}

}