project(dsnutil-cpp)
cmake_minimum_required(VERSION 3.1)

set(dsnutil_cpp_VERSION_MAJOR 0)
set(dsnutil_cpp_VERSION_MINOR 99)
set(dsnutil_cpp_VERSION_PATCH 19)

option(dsnutil_cpp_BUILD_AS_SUBMODULE "Build dsnutil_cpp as git submodule?" OFF)
option(dsnutil_cpp_BUILD_SHARED_LIBS "Build dsnutil_cpp as shared libraries" ON)
option(dsnutil_cpp_BUILD_DOCS "Build dsnutil_cpp docs if Doxygen is available?" ON)
option(dsnutil_cpp_WITH_BASE64 "Build libdsnutil_cpp-base64" ON)
option(dsnutil_cpp_WITH_BOOST_LOG "Enable support for boost::log based logging API" ON)
option(dsnutil_cpp_WITH_TESTS "Enable tests for dsnutil_cpp" ON)

#
# Append cmake-modules/ to module path for custom macros
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

#
# When not building as a git submodule we want to include CPack for binary packages
if(NOT dsnutil_cpp_BUILD_AS_SUBMODULE)
    #
    # Version info for source/binary packages
    set(CPACK_PACKAGE_VERSION_MAJOR ${dsnutil_cpp_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${dsnutil_cpp_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${dsnutil_cpp_VERSION_PATCH})
    set(CPACK_PACKAGE_DESCRIPTION "#das-system Networks C++ utility libraries")
    if(WIN32)
        set(CPACK_GENERATOR "ZIP")
    endif(WIN32)
    include(CPack)
endif(NOT dsnutil_cpp_BUILD_AS_SUBMODULE)

#
# Setup lib type based on user choice
if(dsnutil_cpp_BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
else()
    set(LIB_TYPE STATIC)
endif(dsnutil_cpp_BUILD_SHARED_LIBS)

#
# Add our own include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


include(WriteCompilerDetectionHeader)
list(APPEND supported_compilers GNU Clang)
if(WIN32)
	list(APPEND supported_compilers MSVC)
endif(WIN32)
if(APPLE)
	list(APPEND supported_compilers AppleClang)
endif(APPLE)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/dsnutil)
write_compiler_detection_header(FILE "${CMAKE_CURRENT_BINARY_DIR}/include/dsnutil/compiler_features.h"
                                PREFIX dsnutil_cpp
                                COMPILERS ${supported_compilers}
                                VERSION 3.1.0
                                FEATURES
                                    cxx_alias_templates
                                    cxx_attribute_deprecated
                                    cxx_auto_type
                                    cxx_constexpr
                                    cxx_deleted_functions
                                    cxx_noexcept
                                    cxx_nullptr
                                    cxx_range_for
                                    cxx_strong_enums)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/dsnutil/compiler_features.h DESTINATION include/dsnutil)

set(Boost_COMPONENTS "")
if(dsnutil_cpp_WITH_TESTS)
    # Building tests requires Boost::Test
    list(APPEND Boost_COMPONENTS unit_test_framework)
endif(dsnutil_cpp_WITH_TESTS)

#
# If boost::log is requested we'll need to locate additional Boost headers & libs...
if(dsnutil_cpp_WITH_BOOST_LOG)
    list(APPEND Boost_COMPONENTS system thread log log_setup date_time chrono filesystem)
    add_definitions(-DWITH_BOOST_LOG)
endif(dsnutil_cpp_WITH_BOOST_LOG)

message(STATUS "Boost_COMPONENTS: ${Boost_COMPONENTS}")
find_package(Boost 1.54 REQUIRED COMPONENTS ${Boost_COMPONENTS})
include_directories(${Boost_INCLUDE_DIRS})
if(NOT Boost_USE_STATIC_LIBS)
  add_definitions(-DBOOST_ALL_DYN_LINK)
endif(NOT Boost_USE_STATIC_LIBS)

#
# Get compiler flags & libraries for threading support
find_package(Threads REQUIRED)


# This is required to auto-generate proper *_EXPORT headers for Windows DLLs
include(GenerateExportHeader)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

configure_file(version.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/dsnutil/dsnutil_cpp_version.h)

#
# libdsnutil_cpp: Main library
file(GLOB_RECURSE dsnutil_cpp_HDRS include/dsnutil/*.h include/dsnutil/*.hpp)
file(GLOB dsnutil_cpp_SRCS src/*.cpp)
if(WIN32)
    file(GLOB dsnutil_cpp_RC_SRCS src/*.rc)
    list(APPEND dsnutil_cpp_SRCS ${dsnutil_cpp_RC_SRCS})
endif(WIN32)
set(dsnutil_cpp_LIBRARY dsnutil_cpp)
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(dsnutil_cpp_LIBRARY dsnutil_cppd)
endif(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
add_library(${dsnutil_cpp_LIBRARY} ${LIB_TYPE} ${dsnutil_cpp_SRCS} ${dsnutil_cpp_HDRS})
set_target_properties(${dsnutil_cpp_LIBRARY} PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)
if(WIN32)
    set_target_properties(${dsnutil_cpp_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${dsnutil_cpp_LIBRARY}_EXPORTS")
endif(WIN32)
if(UNIX AND dsnutil_cpp_BUILD_SHARED_LIBS)
    set_target_properties(${dsnutil_cpp_LIBRARY} PROPERTIES SONAME "dsnutil_cpp"
                                                            SOVERSION ${dsnutil_cpp_VERSION_MAJOR}.${dsnutil_cpp_VERSION_MINOR}.${dsnutil_cpp_VERSION_PATCH}
                                                            VERSION ${dsnutil_cpp_VERSION_MAJOR}.${dsnutil_cpp_VERSION_MINOR}.${dsnutil_cpp_VERSION_PATCH})
endif(UNIX AND dsnutil_cpp_BUILD_SHARED_LIBS)
generate_export_header(${dsnutil_cpp_LIBRARY}
                       BASE_NAME dsnutil_cpp
                       EXPORT_MACRO_NAME dsnutil_cpp_EXPORT
                       EXPORT_FILE_NAME include/dsnutil/dsnutil_cpp_Export.h
                       STATIC_DEFINE dsnutil_cpp_BUILT_AS_STATIC)
install(TARGETS ${dsnutil_cpp_LIBRARY} ARCHIVE DESTINATION lib
                                       LIBRARY DESTINATION lib
                                       RUNTIME DESTINATION bin)
install(DIRECTORY include/dsnutil DESTINATION include)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/dsnutil/dsnutil_cpp_Export.h DESTINATION include/dsnutil)


#
# libdsnutil_cpp-log: Optional boost::log based logging API
if(dsnutil_cpp_WITH_BOOST_LOG)
    set(dsnutil_cpp_log_LIBRARY dsnutil_cpp-log)
    if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(dsnutil_cpp_log_LIBRARY dsnutil_cpp-logd)
    endif(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(GLOB dsnutil_cpp_log_SRCS src/log/*.cpp)
    if(WIN32)
        file(GLOB dsnutil_cpp_log_RC_SRCS src/log/*.rc)
        list(APPEND dsnutil_cpp_log_SRCS ${dsnutil_cpp_log_RC_SRCS})
    endif(WIN32)
    add_library(${dsnutil_cpp_log_LIBRARY} ${LIB_TYPE} ${dsnutil_cpp_log_SRCS} ${dsnutil_cpp_HDRS})
    set_target_properties(${dsnutil_cpp_log_LIBRARY} PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)
    if(WIN32)
        set_target_properties(${dsnutil_cpp_log_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${dsnutil_cpp_LIBRARY}_EXPORTS;${dsnutil_cpp_log_LIBRARY}_EXPORTS")
    endif(WIN32)
    if(UNIX AND dsnutil_cpp_BUILD_SHARED_LIBS)
        set_target_properties(${dsnutil_cpp_log_LIBRARY} PROPERTIES SONAME "dsnutil_cpp-log"
                                                                    SOVERSION ${dsnutil_cpp_VERSION_MAJOR}.${dsnutil_cpp_VERSION_MINOR}.${dsnutil_cpp_VERSION_PATCH}
                                                                    VERSION ${dsnutil_cpp_VERSION_MAJOR}.${dsnutil_cpp_VERSION_MINOR}.${dsnutil_cpp_VERSION_PATCH})
								endif(UNIX AND dsnutil_cpp_BUILD_SHARED_LIBS)
    if(NOT Boost_USE_STATIC_LIBS)
      target_link_libraries(${dsnutil_cpp_log_LIBRARY} ${Boost_LIBRARIES})
    endif(NOT Boost_USE_STATIC_LIBS)
    generate_export_header(${dsnutil_cpp_log_LIBRARY}
                           BASE_NAME dsnutil_cpp_log
                           EXPORT_MACRO_NAME dsnutil_cpp_log_EXPORT
                           EXPORT_FILE_NAME include/dsnutil/log/dsnutil_cpp_log_Export.h
                           STATIC_DEFINE dsnutil_cpp_log_BUILT_AS_STATIC)
    install(TARGETS ${dsnutil_cpp_log_LIBRARY} ARCHIVE DESTINATION lib
                                               LIBRARY DESTINATION lib
                                               RUNTIME DESTINATION bin)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/dsnutil/log/dsnutil_cpp_log_Export.h DESTINATION include/dsnutil/log)
endif(dsnutil_cpp_WITH_BOOST_LOG)


#
# libdsnutil_cpp-base64: Optional Base64 encoder/decoder functions
if(dsnutil_cpp_WITH_BASE64)
    set(dsnutil_cpp_base64_LIBRARY dsnutil_cpp-base64)
    if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(dsnutil_cpp_base64_LIBRARY dsnutil_cpp-base64d)
    endif(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(GLOB dsnutil_cpp_base64_SRCS src/base64/*.cpp)
    if(WIN32)
        file(GLOB dsnutil_cpp_base64_RC_SRCS src/base64/*.rc)
        list(APPEND dsnutil_cpp_base64_SRCS ${dsnutil_cpp_base64_RC_SRCS})
    endif(WIN32)
    add_library(${dsnutil_cpp_base64_LIBRARY} ${LIB_TYPE} ${dsnutil_cpp_base64_SRCS} ${dsnutil_cpp_HDRS})
    if(WIN32)
        set_target_properties(${dsnutil_cpp_base64_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${dsnutil_cpp_base64_LIBRARY}_EXPORTS")
    endif(WIN32)
    if(UNIX AND dsnutil_cpp_BUILD_SHARED_LIBS)
        set_target_properties(${dsnutil_cpp_base64_LIBRARY} PROPERTIES SONAME "dsnutil_cpp-base64"
                                                                       SOVERSION ${dsnutil_cpp_VERSION_MAJOR}.${dsnutil_cpp_VERSION_MINOR}.${dsnutil_cpp_VERSION_PATCH}
                                                                       VERSION ${dsnutil_cpp_VERSION_MAJOR}.${dsnutil_cpp_VERSION_MINOR}.${dsnutil_cpp_VERSION_PATCH})
    endif(UNIX AND dsnutil_cpp_BUILD_SHARED_LIBS)
    generate_export_header(${dsnutil_cpp_base64_LIBRARY}
                           BASE_NAME dsnutil_cpp_base64
                           EXPORT_MACRO_NAME dsnutil_cpp_base64_EXPORT
                           EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/dsnutil/base64/dsnutil_cpp_base64_Export.h
                           STATIC_DEFINE dsnutil_cpp_base64_BUILT_AS_STATIC)
    install(TARGETS ${dsnutil_cpp_base64_LIBRARY} ARCHIVE DESTINATION lib
                                                  LIBRARY DESTINATION lib
                                                  RUNTIME DESTINATION bin)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/dsnutil/base64/dsnutil_cpp_base64_Export.h DESTINATION include/dsnutil/base64)
endif(dsnutil_cpp_WITH_BASE64)


#
# Enable testing and CDash submission
if(NOT dsnutil_cpp_BUILD_AS_SUBMODULE)
  include(CTest)
  enable_testing()
endif(NOT dsnutil_cpp_BUILD_AS_SUBMODULE)

if(dsnutil_cpp_WITH_TESTS)
    add_subdirectory(tests)
endif(dsnutil_cpp_WITH_TESTS)


# Add target for API docs if Doxygen is available
if(NOT dsnutil_cpp_BUILD_AS_SUBMODULE)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        option(dsnutil_cpp_WITH_DOCS "Generate API docs using Doxygen during build?" ON)
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile DESTINATION share/doc/dsnutil_cpp)
        include(DocTarget)
        if(dsnutil_cpp_WITH_DOCS)
            add_custom_target(force_doc ALL DEPENDS doc)
            install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc DESTINATION share/doc/dsnutil_cpp)
        endif(dsnutil_cpp_WITH_DOCS)
    endif(DOXYGEN_FOUND)
endif(NOT dsnutil_cpp_BUILD_AS_SUBMODULE)

install(FILES README.md DESTINATION share/doc/dsnutil_cpp)
