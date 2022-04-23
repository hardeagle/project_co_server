#ifndef __CORE_RAD_RAD_SERVER_H__
#define __CORE_RAD_RAD_SERVER_H__

#include <memory>
#include <unordered_map>

namespace {

class RadServerInfo {
public:
    using ptr = std::shared_ptr<RadServerInfo>;

private:
    std::string m_name;

    int m_type;
    int m_id;

    std::string m_ip;
    int m_port;
};

class RadServer {
public:
    using ptr = std::shared_ptr<RadServer>;

private:


};

}

#endif