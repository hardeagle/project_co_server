#ifndef __LOGIC_TASK_TASK_SERVER_H__
#define __LOGIC_TASK_TASK_SERVER_H__

#include "core/const.hpp"
#include "core/base/base_server.h"

class TaskServer : public Eayew::BaseServer {
public:
    using ptr = std::shared_ptr<TaskServer>;

    RankServer() : Eayew::BaseServer(Eayew::ServerType::EST_TASK) {}

    virtual void beforeRun() override;

private:

};



#endif