#include "game_session.h"

namespace Eayew {

void GameSession::run() {
    go [&] {
        while (true) {
            std::string buffer;
            m_rChannel >> buffer;
        }
    }
}

void GameSession::operator<<(std::string& buffer) {
    m_rChannel << buffer;
}

}