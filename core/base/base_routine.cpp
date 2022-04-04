#include "base_routine.h"

#include "core/servlet.h"

namespace Eayew {

void BaseRoutine::run() {
    go [&] {
        while (true) {
            std::string buffer;
            m_rChannel >> buffer;
            m_servlet->doRequest(buffer);
        }
    };
}

void BaseRoutine::operator<<(std::string& buffer) {
    m_rChannel << buffer;
}

}