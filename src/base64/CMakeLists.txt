set(dsnutil_cpp_base64_SOURCES ../../include/dsnutil/base64.h base64.cpp)
set(dsnutil_cpp_base64_LIBRARY dsnutil_cpp-base64)
if(WIN32)
    if("x${CMAKE_BUILD_TYPE}" STREQUAL "xDebug")
        set(dsnutil_cpp_base64_LIBRARY dsnutil_cpp-base64d)
    endif("${CMAKE_BUILD_TYPE}" STREQUAL "xDebug")
    file(GLOB dsnutil_cpp_base64_RC_SOURCES *.rc)
    list(APPEND dsnutil_cpp_base64_SOURCES ${dsnutil_cpp_base64_RC_SOURCES})
endif(WIN32)

add_library(${dsnutil_cpp_base64_LIBRARY} ${LIB_TYPE} ${dsnutil_cpp_base64_SOURCES})
set_target_properties(${dsnutil_cpp_base64_LIBRARY} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set_target_properties(${dsnutil_cpp_base64_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${dsnutil_cpp_base64_LIBRARY}_EXPORTS")
endif(WIN32)

if(UNIX AND dsnutil_cpp_BUILD_SHARED_LIBS)
    set_target_properties(${dsnutil_cpp_base64_LIBRARY} PROPERTIES
        SONAME "${dsnutil_cpp_base64_LIBRARY}"
        SOVERSION ${dsnutil_cpp_LIB_SOVERSION}
        VERSION ${dsnutil_cpp_LIB_VERSION})
endif(UNIX AND dsnutil_cpp_BUILD_SHARED_LIBS)

generate_export_header(${dsnutil_cpp_base64_LIBRARY}
    BASE_NAME dsnutil_cpp_base64
    EXPORT_MACRO_NAME dsnutil_cpp_base64_EXPORT
    EXPORT_FILE_NAME ${dsnutil_cpp_BINARY_DIR}/include/dsnutil/base64/dsnutil_cpp_base64_Export.h
    STATIC_DEFINE dsnutil_cpp_base64_BUILT_AS_STATIC)

install(TARGETS ${dsnutil_cpp_base64_LIBRARY} RUNTIME DESTINATION bin ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
install(FILES ${dsnutil_cpp_BINARY_DIR}/include/dsnutil/base64/dsnutil_cpp_base64_Export.h DESTINATION include/dsnutil/base64)

set(dsnutil_cpp_base64_LIBRARY ${dsnutil_cpp_base64_LIBRARY} PARENT_SCOPE)
