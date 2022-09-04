
cmake_minimum_required (VERSION 3.17.0)

project (project_co_server)

set(CMKAE_CXX_COMPILER /opt/rh/devtoolset-10/root/usr/bin/g++)

include_directories(${PROJECT_SOURCE_DIR})

#ADD_DEFINTIONS(-D BOOST_ASIO_HAS_STD_COROUTINE -D BOOST_ASIO_HAS_CO_AWAIT)

aux_source_directory(${PROJECT_SOURCE_DIR}/core SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/core/base SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/core/gate SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/core/redis SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/core/rpc SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/core/util SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/log SRC_LIST)


# public
aux_source_directory(${PROJECT_SOURCE_DIR}/logic/common PUBLIC_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/logic/protocol PUBLIC_SRC_LIST)

# chat server
aux_source_directory(${PROJECT_SOURCE_DIR}/logic/chat CHAT_SRC_LIST)

# game server
aux_source_directory(${PROJECT_SOURCE_DIR}/logic/game GAME_SRC_LIST)

# login server
aux_source_directory(${PROJECT_SOURCE_DIR}/logic/login LOGIN_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/logic/login/protocol LOGIN_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/logic/login/servlet LOGIN_SRC_LIST)

# rank server
aux_source_directory(${PROJECT_SOURCE_DIR}/logic/rank RANK_SRC_LIST)

# task server
aux_source_directory(${PROJECT_SOURCE_DIR}/logic/task TASK_SRC_LIST)


# test
aux_source_directory(${PROJECT_SOURCE_DIR}/test TEST_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/logic/login/protocol TEST_SRC_LIST)


#set(CMAKE_CXX_FLAGS "-std=c++2a -g -O0 ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-std=c++17 -g -O0 ${CMAKE_CXX_FLAGS}")


add_executable(co_server main.cpp ${SRC_LIST})
add_executable(chat_server main_chat.cpp ${SRC_LIST} ${CHAT_SRC_LIST})
add_executable(game_server main_game.cpp ${SRC_LIST} ${GAME_SRC_LIST})
add_executable(login_server main_login.cpp ${SRC_LIST} ${PUBLIC_SRC_LIST} ${LOGIN_SRC_LIST})
add_executable(gate_server main_gate.cpp ${SRC_LIST})
add_executable(rank_server main_rank.cpp ${SRC_LIST} ${RANK_SRC_LIST})
add_executable(test main_test.cpp ${SRC_LIST} ${TEST_SRC_LIST})

add_executable(co_test main_test_libgo.cpp ${SRC_LIST})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(exes co_server chat_server game_server login_server gate_server rank_server test co_test)
FOREACH(exe ${exes})
    target_link_libraries(${exe} -ldl)
    target_link_libraries(${exe} -lglog)
    target_link_libraries(${exe} -lhiredis)
    target_link_libraries(${exe} -ljemalloc)
    target_link_libraries(${exe} -llibgo)
    target_link_libraries(${exe} -ljemalloc)
    target_link_libraries(${exe} -lppconsul)
    target_link_libraries(${exe} -lprotobuf)
    target_link_libraries(${exe} -lpthread)
ENDFOREACH(exe)