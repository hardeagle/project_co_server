#include "base_routine.h"

#include "core/base/base_server.h"
#include "core/servlet.h"

namespace Eayew {

BaseRoutine::BaseRoutine(BaseServer& server)
    : m_server(server) {
}

void BaseRoutine::run() {
    go [&] {
        while (true) {
            std::string buffer;
            m_rChannel >> buffer;
        }
    };
}

void BaseRoutine::operator<<(std::string& buffer) {
    m_rChannel << buffer;
}

}