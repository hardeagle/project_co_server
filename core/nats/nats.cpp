#include "nats.h"

#include "log/glog.h"

namespace Eayew {

Nats::Nats(std::string& ip, uint16_t port)
    : m_ip(ip)
    , m_port(port) {
}

bool Nats::init() {
    if (natsOptions_Create(&m_opts) != NATS_OK) {
        LOG(ERROR) << "natsOptionas_Create fail";
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