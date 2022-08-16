#include "session.h"

namespace Eayew {

void Session::setOnMessage(std::function<void(Message&& msg)> cb) {
    m_onMessageCB = cb;
}

void Session::setOnClose(std::function<void()> cb) {
    m_onCloseCB = cb;
}

}