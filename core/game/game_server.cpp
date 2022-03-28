#include "game_server.h"

#include <thread>

#include <libgo/libgo.h>

#include "game_session.h"

namespace Eayew {

void GameServer::run() {

    // std::thread t([] { 
    //     co_sched.Start();
    // });
    // t.detach();

    go [&] {
        while (true) {
            int id;
            std::string buffer;
            std::unordered_map<int, GameSession::ptr>::iterator it = m_sessions.find(id);
            if (it == m_sessions.end()) {
                m_sessions[id] = std::make_shared<GameSession>();
                m_sessions[id]->run();
            }
            *(m_sessions[id]) << buffer;
        }
    };

    co_sched.Start();
}

}