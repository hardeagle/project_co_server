#ifndef __CORE_GAME_PLAYER_ROUTINE_H__
#define __CORE_GAME_PLAYER_ROUTINE_H__

#include <memory>

#include <libgo/libgo.h>

namespace Eayew {

class Servlet;

class BaseRoutine {
public:
    using ptr = std::shared_ptr<BaseRoutine>;

    int id() const { return m_id; }

    int fd() const { return m_fd; }

    void run();

    void operator<<(std::string& buffer);

private:
    int m_id;
    int m_fd;

    std::shared_ptr<Servlet> m_servlet;

    co_chan<std::string> m_rChannel;
};

}

#endif