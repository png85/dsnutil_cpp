project(dsnutil-cpp)
cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR}/include)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11)
endif(CMAKE_COMPILER_IS_GNUCXX)

file(GLOB dsnutil_cpp_HDRS include/dsnutil/*.h)
file(GLOB dsnutil_cpp_SRCS src/*.cpp)

add_library(dsnutil_cpp STATIC ${dsnutil_cpp_SRCS} ${dsnutil_cpp_HDRS})

install(TARGETS dsnutil_cpp ARCHIVE DESTINATION lib)
install(DIRECTORY include/dsnutil DESTINATION include)

set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 99)
set(CPACK_PACKAGE_VERSION_PATCH 1)
include(CPack)
