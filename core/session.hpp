#ifndef __CORE_SESSION_HPP__
#define __CORE_SESSION_HPP__

#include "core/message.hpp"

namespace Eayew {

class Session {
public:
    using ptr = std::shared_ptr<Session>;

    virtual void send(Message&& msg) {}

};

}


#endif