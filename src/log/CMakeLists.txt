set(dsnutil_cpp_log_SOURCES ../../include/dsnutil/log/base.h
    ../../include/dsnutil/log/init.h init.cpp
    ../../include/dsnutil/log/sinkmanager.h sinkmanager.cpp
    ../../include/dsnutil/log/stream_interceptor.h
    ../../include/dsnutil/log/types.h
    ../../include/dsnutil/log/util.h util.cpp)
set(dsnutil_cpp_log_LIBRARY dsnutil_cpp-log)

if(WIN32)
    if("x${CMAKE_BUILD_TYPE}" STREQUAL "xDebug")
        set(dsnutil_cpp_log_LIBRARY dsnutil_cpp-logd)
    endif("x${CMAKE_BUILD_TYPE}" STREQUAL "xDebug")
endif(WIN32)

add_library(${dsnutil_cpp_log_LIBRARY} ${LIB_TYPE} ${dsnutil_cpp_log_SOURCES})
set_target_properties(${dsnutil_cpp_log_LIBRARY} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON)
if(dsnutil_cpp_BUILD_SHARED_LIBS)
    target_link_libraries(${dsnutil_cpp_log_LIBRARY} ${Boost_LIBRARIES})
endif(dsnutil_cpp_BUILD_SHARED_LIBS)

if(WIN32)
    set_target_properties(${dsnutil_cpp_log_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${dsnutil_cpp_log_LIBRARY}_EXPORTS")
endif(WIN32)

if(UNIX AND dsnutil_cpp_BUILD_SHARED_LIBS)
    set_target_properties(${dsnutil_cpp_log_LIBRARY} PROPERTIES
        SONAME ${dsnutil_cpp_LIB_SOVERSION}
        VERSION ${dsnutil_cpp_LIB_VERSION})
endif(UNIX AND dsnutil_cpp_BUILD_SHARED_LIBS)

generate_export_header(${dsnutil_cpp_log_LIBRARY}
    BASE_NAME dsnutil_cpp_log
    EXPORT_MACRO_NAME dsnutil_cpp_log_EXPORT
    EXPORT_FILE_NAME ${dsnutil_cpp_BINARY_DIR}/include/dsnutil/log/dsnutil_cpp_log_Export.h
    STATIC_DEFINE dsnutil_cpp_log_BUILT_AS_STATIC)

install(TARGETS ${dsnutil_cpp_log_LIBRARY} RUNTIME DESTINATION bin ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
install(FILES ${dsnutil_cpp_BINARY_DIR}/include/dsnutil/log/dsnutil_cpp_log_Export.h DESTINATION include/dsnutil/log)

set(dsnutil_cpp_log_LIBRARY ${dsnutil_cpp_log_LIBRARY} PARENT_SCOPE)
