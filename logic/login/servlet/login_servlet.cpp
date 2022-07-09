#include "login_servlet.h"

#include "log/glog.h"

#include "logic/login/protocol/login_id.pb.h"
#include "logic/login/protocol/login.pb.h"

bool LoginServlet::doRequest(std::string& buffer) {
    int id;
    switch (id) {
        case LoginProtocol::ID::C2S_LOGIN_LOAD:
            doLoad(buffer);
            break;
    }

    return true;
}

void LoginServlet::doLoad(std::string& buffer) {
    LOG(INFO) << "doLoad, buffer " << buffer;
}

