#ifndef __CORE_GATE_GATE_SESSION_H__
#define __CORE_GATE_GATE_SESSION_H__

#include <atomic>
#include <memory>

#include "core/message.hpp"
#include "core/session.h"

namespace Eayew {

class GateServer;
class Message;

class GateSession : public Session {
public:
    using ptr = std::shared_ptr<GateSession>;

    GateSession(int fd);

private:

};

}

#endif