#ifndef __CORE_GATE_GATE_WSS_SESSION_H__
#define __CORE_GATE_GATE_WSS_SESSION_H__

#include <memory>

namespace Eayew {

class GateWssSession : public std::enable_shared_from_this<GateWssSession> {
public:
    using ptr = std::shared_ptr<GateWssSession>;
};

}

#endif