#include "rpc_session.h"

#include <libgo/libgo.h>

#include "rpc_manager.h"

namespace Eayew {

void RpcSession::run() {

}

void RpcSession::sync_write(std::string& buffer) {

}

void RpcSession::sync_write(int id, std::string& buffer) {

}

void RpcSession::sync_read() {
    while (true) {
        std::string body;
        m_rpcManager->dispatch(body);
    }
}

}