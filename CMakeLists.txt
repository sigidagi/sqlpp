cmake_minimum_required(VERSION 2.6)

project(test)

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/build/cmake/Modules/")

#find_package(Qt REQUIRED)
find_package(SOCI REQUIRED)

if (NOT EXISTS $ENV{ORACLE_HOME})
    message(FATAL_ERROR "Oracle not install or ORACLE_HOME enviromental variable not set")
endif()

message(STATUS "Oracle home = $ENV{ORACLE_HOME}")

include_directories(
            ${SOCI_INCLUDE_DIR}
            $ENV{ORACLE_HOME}/rdbms/public
)

link_directories(
            ${SOCI_LIBRARY} 
            $ENV{ORACLE_HOME}/lib
)

add_executable(test 
               main.cpp 
               console.cpp 
               orasql.cpp
               interpreter.cpp
)

target_link_libraries(test soci_core soci_oracle clntsh occi nnz11 ncurses pthread)

