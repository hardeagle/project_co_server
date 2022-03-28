
cmake_minimum_required (VERSION 3.17.0)

project (project_co_server)

set(CMKAE_CXX_COMPILER /opt/rh/devtoolset-10/root/usr/bin/g++)

include_directories(${PROJECT_SOURCE_DIR})

#ADD_DEFINTIONS(-D BOOST_ASIO_HAS_STD_COROUTINE -D BOOST_ASIO_HAS_CO_AWAIT)

#aux_source_directory(${PROJECT_SOURCE_DIR/logic SRC_LIST})
aux_source_directory(${PROJECT_SOURCE_DIR}/log SRC_LIST)


set(CMAKE_CXX_FLAGS "-std=c++2a -g -O0 ${CMAKE_CXX_FLAGS}")

add_executable(co_server main.cpp ${SRC_LIST})

set(exes co_server)
FOREACH(exe ${exes})
    target_link_libraries(${exe} -ldl)
    target_link_libraries(${exe} -lglog)
    target_link_libraries(${exe} -lhiredis)
    target_link_libraries(${exe} -llibgo)
    target_link_libraries(${exe} -lpthread)
ENDFOREACH(exe)