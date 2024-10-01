#ifndef __LOGIC_LOGIN_ID_MANAGER_H__
#define __LOGIC_LOGIN_ID_MANAGER_H__

#include <atomic>
#include <memory>
#include <vector>

class IdManager {
public:
    using ptr = std::shared_ptr<IdManager>;

    IdManager();

    enum {
        EID_BASE = 0,
        EID_ROLE_ID,
        EID_LIMIT,
    };

    uint64_t generateId(int eid);

    uint64_t generateId();

private:
    std::vector<uint32_t> m_stepIds;
    std::vector<uint64_t> m_maxIds;
    std::vector<std::atomic<uint32_t>> m_curIds;
};

#endif