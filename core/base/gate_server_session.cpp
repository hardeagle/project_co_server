#include "gate_server_session.h"

#include <co/all.h>

namespace Eayew {

GateServerSession::GateServerSession(int fd)
    : Session(fd) {
}

}