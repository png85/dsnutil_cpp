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


option(dsnutil_cpp_BUILD_AS_SUBMODULE "Build dsnutil_cpp as git submodule?" OFF)


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
# Option to switch between static/shared library builds
option(dsnutil_cpp_BUILD_SHARED_LIBS "Build dsnutil_cpp as shared libraries" ON)
if(dsnutil_cpp_BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
else()
    set(LIB_TYPE STATIC)
endif(dsnutil_cpp_BUILD_SHARED_LIBS)

# This is required to auto-generate proper *_EXPORT headers for Windows DLLs
include(GenerateExportHeader)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

#
# libdsnutil_cpp: Main library
file(GLOB_RECURSE dsnutil_cpp_HDRS include/dsnutil/*.h)
file(GLOB dsnutil_cpp_SRCS src/*.cpp)
add_library(dsnutil_cpp ${LIB_TYPE} ${dsnutil_cpp_SRCS} ${dsnutil_cpp_HDRS})
generate_export_header(dsnutil_cpp
                       BASE_NAME dsnutil_cpp
                       EXPORT_MACRO_NAME dsnutil_cpp_EXPORT
                       EXPORT_FILE_NAME include/dsnutil/dsnutil_cpp_Export.h
                       STATIC_DEFINE dsnutil_cpp_BUILT_AS_STATIC)
install(TARGETS dsnutil_cpp ARCHIVE DESTINATION lib
                            LIBRARY DESTINATION lib
                            RUNTIME DESTINATION bin)
install(DIRECTORY include/dsnutil DESTINATION include)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/dsnutil/dsnutil_cpp_Export.h DESTINATION include/dsnutil)


#
# libdsnutil_cpp-log: Optional boost::log based logging API
if(dsnutil_cpp_WITH_BOOST_LOG)
    file(GLOB dsnutil_cpp_log_SRCS src/log/*.cpp)
    add_library(dsnutil_cpp-log ${LIB_TYPE} ${dsnutil_cpp_log_SRCS} ${dsnutil_cpp_HDRS})
    target_link_libraries(dsnutil_cpp-log ${Boost_LIBRARIES})
    generate_export_header(dsnutil_cpp-log
                           BASE_NAME dsnutil_cpp_log
                           EXPORT_MACRO_NAME dsnutil_cpp_log_EXPORT
                           EXPORT_FILE_NAME include/dsnutil/log/dsnutil_cpp_log_Export.h
                           STATIC_DEFINE dsnutil_cpp_log_BUILT_AS_STATIC)
    install(TARGETS dsnutil_cpp-log ARCHIVE DESTINATION lib
                                    LIBRARY DESTINATION lib
                                    RUNTIME DESTINATION bin)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/dsnutil/log/dsnutil_cpp_log_Export.h DESTINATION include/dsnutil/log)
endif(dsnutil_cpp_WITH_BOOST_LOG)

#
# libdsnutil_cpp-base64: Optional Base64 encoder/decoder functions
option(dsnutil_cpp_WITH_BASE64 "Build libdsnutil_cpp-base64" ON)
if(dsnutil_cpp_WITH_BASE64)
    file(GLOB dsnutil_cpp_base64_SRCS src/base64/*.cpp)
    add_library(dsnutil_cpp-base64 ${LIB_TYPE} ${dsnutil_cpp_base64_SRCS} ${dsnutil_cpp_HDRS})
    generate_export_header(dsnutil_cpp-base64
                           BASE_NAME dsnutil_cpp_base64
                           EXPORT_MACRO_NAME dsnutil_cpp_base64_EXPORT
                           EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/dsnutil/base64/dsnutil_cpp_base64_Export.h
                           STATIC_DEFINE dsnutil_cpp_base64_BUILT_AS_STATIC)
    install(TARGETS dsnutil_cpp-base64 ARCHIVE DESTINATION lib
                                       LIBRARY DESTINATION lib
                                       RUNTIME DESTINATION bin)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/dsnutil/base64/dsnutil_cpp_base64_Export.h DESTINATION include/dsnutil/base64)
endif(dsnutil_cpp_WITH_BASE64)


if(NOT dsnutil_cpp_BUILD_AS_SUBMODULE)
    #
    # Version info for source/binary packages
    set(CPACK_PACKAGE_VERSION_MAJOR 0)
    set(CPACK_PACKAGE_VERSION_MINOR 99)
    set(CPACK_PACKAGE_VERSION_PATCH 4)
    include(CPack)
endif(NOT dsnutil_cpp_BUILD_AS_SUBMODULE)

#
# Enable testing and CDash submission
include(CTest)
enable_testing()
option(dsnutil_cpp_WITH_EXAMPLES "Build demo executables" ON)
if(dsnutil_cpp_WITH_EXAMPLES)
    add_subdirectory(examples)
endif(dsnutil_cpp_WITH_EXAMPLES)


# Add target for API docs if Doxygen is available
option(dsnutil_cpp_BUILD_DOCS "Build dsnutil_cpp docs if Doxygen is available?" ON)
if(dsnutil_cpp_BUILD_DOCS AND NOT dsnutil_cpp_BUILD_AS_SUBMODULE)
    include(DocTarget)
    if(DOXYGEN_FOUND)
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile DESTINATION share/doc/libdsnutil)
    endif(DOXYGEN_FOUND)
endif(dsnutil_cpp_BUILD_DOCS AND NOT dsnutil_cpp_BUILD_AS_SUBMODULE)

install(FILES README.md DESTINATION share/doc/dsnutil_cpp)
