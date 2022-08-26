#include "servlet.h"

#include "log/glog.h"

#include "message.h"

static int count = 0;

namespace Eayew {

bool ServletDispatchRange::Element::operator < (const Element& element) const {
    return to < element.to;
}

bool ServletDispatchRange::doRequest(Session::ptr session, Message&& msg) {
    auto id = msg.realMsgId();
    //LOG(INFO) << "ServletDispatchRange doRequest id: " << id;
    //LOG(WARNING) << msg.strInfo();

    auto it = m_servlets.lower_bound(Element(id));
    if (it == m_servlets.end()) {
        LOG(WARNING) << "over diaptch, id " << id;
    } else if (id >= it->from && id <= it->to) {
        ++count;
        if (count % 10000 == 0) {
            LOG(ERROR) << "-----------count" << count;
        }
        return it->servlet->doRequest(session, std::move(msg));
    } else {
        LOG(WARNING) << "wtf? id " << id;
    }

    return true;
}


}