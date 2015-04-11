project(dsnutil-cpp)
cmake_minimum_required(VERSION 2.8)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

option(dsnutil_cpp_WITH_LOG4CPP "Enable support for logging with log4cpp" ON)
if(dsnutil_cpp_WITH_LOG4CPP)
    find_package(Log4cpp REQUIRED)
    include_directories(${LOG4CPP_INCLUDE_DIR})
    add_definitions(-DWITH_LOG4CPP)
endif(dsnutil_cpp_WITH_LOG4CPP)

option(dsnutil_cpp_WITH_BOOST_LOG "Enable support for boost::log based logging" ON)
if(dsnutil_cpp_WITH_BOOST_LOG)
    find_package(Boost 1.54 REQUIRED COMPONENTS system thread log)
    include_directories(${Boost_INCLUDE_DIRS})
    add_definitions(-DBOOST_ALL_DYN_LINK -DWITH_BOOST_LOG)
endif(dsnutil_cpp_WITH_BOOST_LOG)

find_package(Threads REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11)
endif(CMAKE_COMPILER_IS_GNUCXX)

file(GLOB_RECURSE dsnutil_cpp_HDRS include/dsnutil/*.h)
file(GLOB dsnutil_cpp_SRCS src/*.cpp)

add_library(dsnutil_cpp STATIC ${dsnutil_cpp_SRCS} ${dsnutil_cpp_HDRS})

install(TARGETS dsnutil_cpp ARCHIVE DESTINATION lib)
install(DIRECTORY include/dsnutil DESTINATION include)

if(dsnutil_cpp_WITH_BOOST_LOG)
    file(GLOB dsnutil_cpp_log_SRCS src/log/*.cpp)
    add_library(dsnutil_cpp-log STATIC ${dsnutil_cpp_log_SRCS} ${dsnutil_cpp_HDRS})
    install(TARGETS dsnutil_cpp-log ARCHIVE DESTINATION lib)
endif(dsnutil_cpp_WITH_BOOST_LOG)

set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 99)
set(CPACK_PACKAGE_VERSION_PATCH 2)
include(CPack)

option(BUILD_EXAMPLES "Build demo executables" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)
