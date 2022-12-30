#ifndef __LOGIC_RANK_RANK_SERVER_H__
#define __LOGIC_RANK_RANK_SERVER_H__

#include "core/const.hpp"
#include "core/base/base_server.h"

class RankServer : public Eayew::BaseServer {
public:
    using ptr = std::shared_ptr<RankServer>;

    RankServer() : Eayew::BaseServer(Eayew::ServerType::RANK) {}

    virtual ~RankServer() {}

    void beforeRun();

private:

};

#endif