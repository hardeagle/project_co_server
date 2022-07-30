#include "id_manager.h"

#include "core/redis/redis_manager.h"

#include "server_resource.h"

IdManager::IdManager() {
    // m_stepIds.resize(EID_LIMIT - 1);
    // m_curIds.resize(EID_LIMIT - 1);
    // m_maxIds.resize(EID_LIMIT - 1);

    // m_stepIds[EID_ROLE_ID] = 1000;
}

uint64_t IdManager::generateId(int eid) {
    if (eid < 0 || eid >= EID_LIMIT) {
        return 0;
    }
    if (m_curIds[eid] >= m_maxIds[eid]) {   // 有bug
        m_maxIds[eid] = ServerResource::get()->redisMgr()->hincrby("id_increment", "role_id", m_stepIds[eid]);
        m_curIds[eid] = m_maxIds[eid] - m_stepIds[eid];
    }
    return ++m_curIds[eid];
}

uint64_t IdManager::generateId() {
    auto aid = ServerResource::get()->redisMgr()->hincrby("id_increment", "role_id", 1);
    return (uint64_t(37) << 32) & aid;
}
