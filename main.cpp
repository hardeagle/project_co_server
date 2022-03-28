
#include <libgo/libgo.h>

#include "log/glog.h"


int main(int argc, char* argv[]) {
    GLog glog(argv[0]);

    LOG(INFO) << "---begin---";

    for (int i = 0; i < 100; ++i) {
        go [=] {
            LOG(ERROR) << i;
        };
    }

    co_sched.Start();

    LOG(INFO) << "---end---";

    return 0;
}