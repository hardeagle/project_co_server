#ifndef __CORE_GAME_GAME_SESSION_H__
#define __CORE_GAME_GAME_SESSION_H__

#include <memory>

#include <libgo/libgo.h>

namespace Eayew {

class GameSession {
public:
    using ptr = std::shared_ptr<GameSession>;

    int id() const { return m_id; }

    int fd() const { return m_fd; }

    void run();

    void operator<<(std::string& buffer);

private:
    int m_id;
    int m_fd;

    co_chan<std::string> m_rChannel;
};

}

#endif