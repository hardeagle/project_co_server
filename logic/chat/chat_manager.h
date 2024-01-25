#ifndef __LOGIC_CHAT_CHAT_MANAGER_H__
#define __LOGIC_CHAT_CHAT_MANAGER_H__

#include <list>
#include <memory>
#include <string>

class ChatMsg {
public:
    using ptr = std::shared_ptr<ChatMsg>;


private:
    uint64_t m_roleId;
    std::string m_msg;
};

class ChatManager {
public:
    void add(ChatMsg::ptr msg);
    void list(std::list<ChatMsg::ptr>& msgs);

private:
    std::list<ChatMsg::ptr> m_chatMsgs;
};

#endif