#ifndef __CORE_GATE_GATE_WSS_SESSION_H__
#define __CORE_GATE_GATE_WSS_SESSION_H__

#include <memory>
#include "core/ws_session.h"

namespace Eayew {

class GateWssSession : public WSSession {
public:
    using ptr = std::shared_ptr<GateWssSession>;
};

}

#endif