#include "gate_server_session.h"

#include "log/glog.h"

namespace Eayew {

GateServerSession::GateServerSession(int fd)
    : Session(fd) {
}

}