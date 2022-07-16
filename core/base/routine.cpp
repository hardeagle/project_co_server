#include "routine.h"


#include <unistd.h>
#include <sys/syscall.h>

#include "core/servlet.h"
#include "core/session.h"

#include "log/glog.h"

namespace Eayew {

Routine::Routine(uint32_t id, std::shared_ptr<ServletDispatchRange> servlet, std::shared_ptr<Session> session)
    : m_id(id)
    , m_servlet(servlet)
    , m_session(session) {
}

void Routine::operator<<(Message&& msg) {
    //LOG(ERROR) << "-----rotine << , m_rMsgs size " << m_rMsgs.size() << " ,thread id " << syscall(SYS_gettid);
    //m_rMsgs.emplace(std::move(msg));
    m_rMsgs.push(std::move(msg));
    if (m_rMsgs.size() == 1) {
        //LOG(ERROR) << "wake up begin...";
        m_rChan << true;        //
        //LOG(ERROR) << "wake up end...";
    }
}

void Routine::push(Message&& msg) {
    //LOG(ERROR) << "-----rotine << , m_rMsgs size " << m_rMsgs.size() << " ,thread id " << syscall(SYS_gettid);
    //m_rMsgs.emplace(std::move(msg));
    m_rMsgs.push(std::move(msg));
    if (m_rMsgs.size() == 1) {
        //LOG(ERROR) << "wake up begin...";
        m_rChan << true;        //
        //LOG(ERROR) << "wake up end...";
    }
}

void Routine::run() {
    go [this, self = shared_from_this()] {
        for (;;) {
            //LOG(ERROR) << "----m_rMsgs.size() " << m_rMsgs.size();

            while(m_rMsgs.size() == 0) {
                //LOG(ERROR) << "sleep beign";
                bool flag;
                m_rChan >> flag;    // 
                //LOG(ERROR) << "sleep end, flag " << flag;
            }
            auto msg = m_rMsgs.front();
            m_rMsgs.pop();

            //LOG(WARNING) << "-------Routine run";

            m_servlet->doRequest(m_session, std::move(msg));
        }
    };
}


}