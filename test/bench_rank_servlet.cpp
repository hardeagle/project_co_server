#include "bench_rank_servlet.h"

#include <memory>

#include <co/all.h>

#include <json/json.h>

#include "log/glog.h"

#include "core/const.hpp"
#include "core/redis/redis_manager.h"
#include "core/util/util.h"

#include "logic/common/error_code.h"
#include "logic/common/redis_key.h"
#include "logic/protocol/public.pb.h"

#include "logic/protocol/rank_id.pb.h"
#include "logic/protocol/rank.pb.h"
#include "logic/login/server_resource.h"

bool BenchRankServlet::doRequest(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    if (msg->roleId() == 0) {
        ELOG << "fuck roleid zero";
        return true;
    }
    auto id = msg->realMsgId();
    switch (id) {
        case RankProtocol::ID::S2C_RANK_LOAD:
            return doLoad(session, msg);
        case RankProtocol::ID::S2C_RANK_UPDATE:
            return doUpdate(session, msg);
        case RankProtocol::ID::S2C_RANK_MYSELF:
            return doMyself(session, msg);
        default:
            ELOG << "invalid id " << id;
            return true;
    }

    return true;
}

bool BenchRankServlet::doLoad(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doLoad begin...";
    RankProtocol::C2S_RankLoad req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }
    LOG << "doLoad req " << req.DebugString();

    co::sleep(100);

    RankProtocol::C2S_RankUpdate resp;
    resp.set_subtype(2);
    resp.set_score(3);

    auto nsize = resp.ByteSizeLong();
    auto nmsg = co::make_shared<Eayew::Message>(nsize);
    resp.SerializeToArray(nmsg->pdata(), nsize);
    nmsg->senderId(Eayew::EST_GATE);
    nmsg->receiverId(Eayew::EST_RANK);
    nmsg->msgId(RankProtocol::C2S_RANK_UPDATE);
    nmsg->roleId(msg->roleId());
    nmsg->sessionId(msg->sessionId());

    session->send(nmsg);
    ELOG << "doLoad end...";
    return true;
}

bool BenchRankServlet::doUpdate(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doUpdate begin...";
    RankProtocol::C2S_RankUpdate req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }
    LOG << "doUpdate req " << req.DebugString();

    co::sleep(100);

    RankProtocol::C2S_RankMyself resp;
    resp.set_subtype(2);

    auto nsize = resp.ByteSizeLong();
    auto nmsg = co::make_shared<Eayew::Message>(nsize);
    resp.SerializeToArray(nmsg->pdata(), nsize);
    nmsg->senderId(Eayew::EST_GATE);
    nmsg->receiverId(Eayew::EST_RANK);
    nmsg->msgId(RankProtocol::C2S_RANK_MYSELF);
    nmsg->roleId(msg->roleId());
    nmsg->sessionId(msg->sessionId());

    session->send(nmsg);

    ELOG << "doUpdate end...";
    return true;
}

bool BenchRankServlet::doMyself(Eayew::Session::ptr session, Eayew::Message::ptr msg) {
    LOG << "doMyself begin...";
    RankProtocol::C2S_RankMyself req;
    if (!req.ParseFromArray(msg->pdata(), msg->psize())) {
        ELOG << "ParseFromArray fail";
        return false;
    }
    LOG << "doMyself req " << req.DebugString();

    co::sleep(100);

    RankProtocol::C2S_RankLoad resp;
    resp.set_subtype(2);

    auto nsize = resp.ByteSizeLong();
    auto nmsg = co::make_shared<Eayew::Message>(nsize);
    resp.SerializeToArray(nmsg->pdata(), nsize);
    nmsg->senderId(Eayew::EST_GATE);
    nmsg->receiverId(Eayew::EST_RANK);
    nmsg->msgId(RankProtocol::C2S_RANK_LOAD);
    nmsg->roleId(msg->roleId());
    nmsg->sessionId(msg->sessionId());

    session->send(nmsg);

    ELOG << "doMyself end...";
    return true;
}
