#include "gate_ws_session.h"

#include<sys/types.h>
#include<sys/socket.h>

#include <functional>
#include <string>

#include "core/message.h"
#include "core/rpc/rpc_manager.h"
#include "core/session.h"
#include "core/ws_session.h"

#include "log/glog.h"

#include "gate_peer_session.h"


namespace Eayew {

GateWsSession::GateWsSession(int fd)
    : WsSession(fd) {
}

}