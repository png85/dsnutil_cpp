project(dsnutil-cpp)
cmake_minimum_required(VERSION 2.8)

#
# Append cmake-modules/ to module path for custom macros
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

#
# Add our own include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#
# Enable GCC-specific C++11 compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-std=c++11" GCC_stdcxx11)
    check_cxx_compiler_flag("-std=c++0x" GCC_stdcxx0x)

    if(GCC_stdcxx11)
        # Compiler supports -std=c++11
        message(STATUS "Using -std=c++11 to enable C++11 in GCC")
        add_definitions(-std=c++11)
    elseif(GCC_stdcxx0x)
        # Compiler doesn't support -std=c++11 but -std=c++0x
        message(STATUS "Using -std=c++0x to enable C++11 in GCC")
        add_definitions(-std=c++0x)
    else()
        # Neither -std=c++11 nor -std=c++0x are supported
        message(WARNING "Failed to determine C++11 flag for GCC :-(")
    endif()
endif(CMAKE_COMPILER_IS_GNUCXX)


#
# Look for log4cpp
find_package(Log4cpp)
if(LOG4CPP_FOUND)
    option(dsnutil_cpp_WITH_LOG4CPP "Enable support for logging with log4cpp" ON)
endif(LOG4CPP_FOUND)
if(dsnutil_cpp_WITH_LOG4CPP AND LOG4CPP_FOUND)
    include_directories(${LOG4CPP_INCLUDE_DIR})
    add_definitions(-DWITH_LOG4CPP)
endif(dsnutil_cpp_WITH_LOG4CPP AND LOG4CPP_FOUND)


#
# Look for Boost >= 1.54
find_package(Boost 1.54 REQUIRED COMPONENTS system thread log log_setup date_time chrono filesystem)
if(Boost_FOUND)
    option(dsnutil_cpp_WITH_BOOST_LOG "Enable support for boost::log based logging API" ON)
endif(Boost_FOUND)
if(Boost_FOUND AND dsnutil_cpp_WITH_BOOST_LOG)
    include_directories(${Boost_INCLUDE_DIRS})
    add_definitions(-DBOOST_ALL_DYN_LINK -DWITH_BOOST_LOG)
endif(Boost_FOUND AND dsnutil_cpp_WITH_BOOST_LOG)


#
# Get compiler flags & libraries for threading support
find_package(Threads REQUIRED)


#
# libdsnutil_cpp: Main library
file(GLOB_RECURSE dsnutil_cpp_HDRS include/dsnutil/*.h)
file(GLOB dsnutil_cpp_SRCS src/*.cpp)
add_library(dsnutil_cpp STATIC ${dsnutil_cpp_SRCS} ${dsnutil_cpp_HDRS})
install(TARGETS dsnutil_cpp ARCHIVE DESTINATION lib)
install(DIRECTORY include/dsnutil DESTINATION include)


#
# libdsnutil_cpp-log: Optional boost::log based logging API
if(dsnutil_cpp_WITH_BOOST_LOG)
    file(GLOB dsnutil_cpp_log_SRCS src/log/*.cpp)
    add_library(dsnutil_cpp-log STATIC ${dsnutil_cpp_log_SRCS} ${dsnutil_cpp_HDRS})
    install(TARGETS dsnutil_cpp-log ARCHIVE DESTINATION lib)
endif(dsnutil_cpp_WITH_BOOST_LOG)

option(dsnutil_cpp_WITH_BASE64 "Build libdsnutil_cpp-base64" ON)
if(dsnutil_cpp_WITH_BASE64)
    file(GLOB dsnutil_cpp_base64_SRCS src/base64/*.cpp)
    add_library(dsnutil_cpp-base64 STATIC ${dsnutil_cpp_base64_SRCS} ${dsnutil_cpp_HDRS})
    install(TARGETS dsnutil_cpp-base64 ARCHIVE DESTINATION lib)
endif(dsnutil_cpp_WITH_BASE64)


#
# Version info for source/binary packages
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 99)
set(CPACK_PACKAGE_VERSION_PATCH 3)
include(CPack)


#
# Enable testing and CDash submission
include(CTest)
enable_testing()
option(dsnutil_cpp_WITH_EXAMPLES "Build demo executables" ON)
if(dsnutil_cpp_WITH_EXAMPLES)
    add_subdirectory(examples)
endif(dsnutil_cpp_WITH_EXAMPLES)


# Add target for API docs if Doxygen is available
include(DocTarget)
if(DOXYGEN_FOUND)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile DESTINATION share/doc/libdsnutil)
endif(DOXYGEN_FOUND)

install(FILES README.md DESTINATION share/doc/libdsnutil)
