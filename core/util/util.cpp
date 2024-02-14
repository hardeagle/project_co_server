#include "util.h"

#include <stdio.h>     
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include <sstream>

#include "log/glog.h"

namespace Eayew {

uint32_t getCurSecond() {
    time_t t = time(NULL);
    return uint32_t(t);
}

uint16_t serverPort(uint16_t type) {
    return 9000 + type * 100 + 1;
}

std::string serverId(const std::string& name, uint16_t type, const std::string& ip, uint16_t port) {
    std::stringstream ss;
    ss << name << "_" << type << "_" << ip << "_" << port;
    return std::move(ss.str());
}

uint16_t getTypeByName(const std::string& name) {
    uint16_t type = 0;
    if (name == "gate") {
        type = 1;
    } else if (name == "login") {
        type = 2;
    } else if (name == "game") {
        type = 3;
    }
    return type;
}

Message&& covertRspMsg(Message& msg, const google::protobuf::Message& gpm) {
    // LOG(INFO) << "covertRspMsg begin " << msg.strInfo();
    auto nsize = gpm.ByteSizeLong();
    if (nsize > msg.size()) {
        msg.prepare(nsize - msg.size());
    }
    gpm.SerializeToArray(msg.pdata(), nsize);
    msg.length(Message::HEAD_LEN + nsize);
    msg.msgId(msg.msgId() + 1);
    // LOG(INFO) << "covertRspMsg end " << msg.strInfo();
    return std::move(msg);
}

std::string getIP() {
    struct ifaddrs * ifAddrStruct=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    while (ifAddrStruct!=NULL) {
        if (ifAddrStruct->ifa_addr->sa_family==AF_INET) { // check it is IP4
            tmpAddrPtr=&((struct sockaddr_in*)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            //LOG(ERROR) << "ifAddrStruct->ifa_name " << ifAddrStruct->ifa_name << " addressBuffer " << addressBuffer;
            if (strcmp(ifAddrStruct->ifa_name,"lo") == 0) {
                ifAddrStruct=ifAddrStruct->ifa_next;
                continue;
            }
            return std::string(addressBuffer);
        } else if (ifAddrStruct->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            tmpAddrPtr=&((struct sockaddr_in*)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            //LOG(ERROR) << "ifAddrStruct->ifa_name " << ifAddrStruct->ifa_name << " addressBuffer " << addressBuffer;
        }
        ifAddrStruct=ifAddrStruct->ifa_next;
    }
    return "";
}

std::string getip() {
    // char hname[128];
    // struct hostent *hent;
    // int i;

    // gethostname(hname, sizeof(hname));

    // hent = gethostbyname(hname);

    // printf("hostname: %s/naddress list: ", hent->h_name);
    // for(i = 0; hent->h_addr_list[i]; i++) 
    // {
    //     printf("%s/t", inet_ntoa(*(struct in_addr*)(hent->h_addr_list[i])));
    // }
    return "";
}

}