#include "login_server.h"

#include <ppconsul/agent.h>
#include <ppconsul/consul.h>
#include <ppconsul/kv.h>

#include "core/servlet.h"

#include "log/glog.h"


#include "logic/login/protocol/login_id.pb.h"
#include "logic/login/servlet/login_servlet.h"


// using namespace ppconsul::agent;
// using ppconsul::CheckStatus;

inline std::string get_test_datacenter()
{
    auto datacenter = std::getenv("PPCONSUL_TEST_DC");

    LOG(ERROR) << "datacenter " << datacenter;

    return datacenter ? datacenter : "ppconsul_test111";
}

template<class... AdditionalParams>
inline ppconsul::Consul create_test_consul(AdditionalParams&&... additionalParams)
{
    auto addr = std::getenv("PPCONSUL_TEST_ADDR");

    LOG(ERROR) << "addr " << addr;

    return ppconsul::Consul(
        addr ? addr : ppconsul::Default_Server_Endpoint,
        ppconsul::kw::dc = get_test_datacenter(),
        std::forward<AdditionalParams>(additionalParams)...
    );
}

void LoginServer::beforeRun() {
    initByConfig("./json/login_server.json");

    initServlet();
}

void LoginServer::initServlet() {  
    Eayew::Servlet::ptr tmp_servlet;
    tmp_servlet.reset(new LoginServlet);
    servlet()->addServlet(LoginProtocol::ID::C2S_LOGIN_BASE, LoginProtocol::ID::C2S_LOGIN_TOP, tmp_servlet);

}