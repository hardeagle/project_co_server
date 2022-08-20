#ifndef __CORE_BASE_GATE_SERVER_SESSION_H__
#define __CORE_BASE_GATE_SERVER_SESSION_H__

#include <memory>

#include <google/protobuf/message.h>

#include <libgo/libgo.h>

#include "core/message.hpp"
#include "core/session.h"

namespace Eayew {

class  BaseServer;

class GateServerSession : public Session {
public:
    using ptr = std::shared_ptr<GateServerSession>;

    GateServerSession(int fd);

    virtual void send(Message&& msg, const google::protobuf::Message& gpm) override;

private:
};

}

#endif