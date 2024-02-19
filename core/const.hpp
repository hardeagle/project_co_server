#ifndef __CORE_CONST_HPP__
#define __CORE_CONST_HPP__

namespace Eayew {

enum ServerType {
    EST_GATE = 1,
    EST_LOGIN = 2,
    EST_GAME = 3,
    EST_CHAT = 4,
    EST_RANK = 5,
    EST_TASK = 6,
};

enum CloseMsgId {
    ECMI_WebsocketSession    = 101,
    ECMI_GatePeerSession     = 102,
    ECMI_WorkRoutine         = 103,
};

}

#endif