#ifndef __CORE_NATS_NATS_H__
#define __CoRE_NATS_NATS_H__

#include  <memory>

#include <nats/nats.h>

#include <core/message.h>

namespace Eayew {

class Nats {
public:
    using ptr = std::shared_ptr<Nats>;

    Nats(std::string& ip, uint16_t port);

    bool init();

    void publish();

    void subcribe();

    void request();

private:
    std::string m_ip;
    uint16_t m_port;
    std::string m_password;

    natsConnection* m_conn;
    natsStatistics* m_stats;
    natsOptions* m_opts;

};

}

#endif