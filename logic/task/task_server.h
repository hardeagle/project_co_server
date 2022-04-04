#ifndef __LOGIC_TASK_TASK_SERVER_H__
#define __LOGIC_TASK_TASK_SERVER_H__

#include "core/base/base_server.h"

class TaskServer : public Eayew::BaseServer {
public:
    using ptr = std::shared_ptr<TaskServer>;

    virtual void beforeRun() override;

private:

};



#endif