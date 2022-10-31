#ifndef __CORE_GATE_GATE_WS_SESSION_H__
#define __CORE_GATE_GATE_WS_SESSION_H__

#include <memory>
#include "core/ws_session.h"

namespace Eayew {

class GateWsSession : public WsSession {
public:
    using ptr = std::shared_ptr<GateWsSession>;

    GateWsSession(int fd);
};

}

#endif