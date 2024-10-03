#include "gate_session.h"

#include<sys/types.h>
#include<sys/socket.h>

#include <functional>
#include <string>

#include <co/all.h>

#include "core/message.h"
#include "core/rpc/rpc_manager.h"
#include "core/session.h"

#include "gate_peer_session.h"


namespace Eayew {

GateSession::GateSession(int fd)
    : Session(fd) {
}

}