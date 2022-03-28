#ifndef __CORE_CONNECTION_H__
#define __CORE_CONNECTION_H__

#include <memory>

#include <libgo/libgo.h>

class Connection {
public:
    typedef std::shared_ptr<Connection> ptr;

    void run();

    void operator<<(std::string& buf);


private:
    void read();

private:
    co_chan<std::string> m_rChannel;
};

#endif