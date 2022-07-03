#include "util.h"


namespace Eayew {

uint32_t getCurSecond() {
    time_t t = time(NULL);
    return uint32_t(t);
}

}