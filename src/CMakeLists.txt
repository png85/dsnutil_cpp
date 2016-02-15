set(libdsnutil_cpp_SOURCES ../include/dsnutil/bitfield.hpp
    ../include/dsnutil/countof.h
    ../include/dsnutil/exception.h exception.cpp
    ../include/dsnutil/finally.h
    ../include/dsnutil/hexdump.h hexdump.cpp
    ../include/dsnutil/intrusive_ptr.hpp
    ../include/dsnutil/lambda_unique_ptr.hpp
    ../include/dsnutil/make_reverse.h
    ../include/dsnutil/map_sort.h
    ../include/dsnutil/non_copyable.hpp
    ../include/dsnutil/non_moveable.hpp
    ../include/dsnutil/null_deleter.h
    ../include/dsnutil/observable.hpp observable.cpp
    ../include/dsnutil/observer.hpp
    ../include/dsnutil/observing_ptr.hpp
    ../include/dsnutil/parallel_for.h parallel_for.cpp
    ../include/dsnutil/pretty_print.h
    ../include/dsnutil/reference_counted.hpp reference_counted.cpp
    ../include/dsnutil/singleton.h
    ../include/dsnutil/threadpool.h threadpool.cpp
    ../include/dsnutil/throwing_assert.h)
set(dsnutil_cpp_LIBRARY dsnutil_cpp)

if(WIN32)
    file(GLOB libdsnutil_cpp_RC_SOURCES *.rc)
    list(APPEND libdsnutil_cpp_SOURCES ${libdsnutil_cpp_RC_SOURCES})
    if("x${CMAKE_BUILD_TYPE}" STREQUAL "xDebug")
        set(dsnutil_cpp_LIBRARY dsnutil_cppd)
    endif("x${CMAKE_BUILD_TYPE}" STREQUAL "xDebug")
endif(WIN32)

add_library(${dsnutil_cpp_LIBRARY} ${LIB_TYPE} ${libdsnutil_cpp_SOURCES})
set_target_properties(${dsnutil_cpp_LIBRARY} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set_target_properties(${dsnutil_cpp_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${dsnutil_cpp_LIBRARY}_EXPORTS}")
endif(WIN32)

if(UNIX AND dsnutil_cpp_BUILD_SHARED_LIBS)
    set_target_properties(${dsnutil_cpp_LIBRARY} PROPERTIES
        SONAME "${dsnutil_cpp_LIBRARY}"
        SOVERSION ${dsnutil_cpp_LIB_SOVERSION}
        VERSION ${dsnutil_cpp_LIB_VERSION})
endif(UNIX AND dsnutil_cpp_BUILD_SHARED_LIBS)

generate_export_header(${dsnutil_cpp_LIBRARY}
    BASE_NAME dsnutil_cpp
    EXPORT_MACRO_NAME dsnutil_cpp_EXPORT
    EXPORT_FILE_NAME ${dsnutil_cpp_BINARY_DIR}/include/dsnutil/dsnutil_cpp_Export.h
    STATIC_DEFINE dsnutil_cpp_BUILT_AS_STATIC)

install(TARGETS ${dsnutil_cpp_LIBRARY} RUNTIME DESTINATION bin ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
install(FILES ${dsnutil_cpp_BINARY_DIR}/include/dsnutil/dsnutil_cpp_Export.h DESTINATION include/dsnutil)

set(dsnutil_cpp_LIBRARY ${dsnutil_cpp_LIBRARY} PARENT_SCOPE)

if(dsnutil_cpp_WITH_BASE64)
    add_subdirectory(base64)
    set(dsnutil_cpp_base64_LIBRARY ${dsnutil_cpp_base64_LIBRARY} PARENT_SCOPE)
endif(dsnutil_cpp_WITH_BASE64)

if(dsnutil_cpp_WITH_CHRONO)
    add_subdirectory(chrono)
    set(dsnutil_cpp_chrono_LIBRARY ${dsnutil_cpp_chrono_LIBRARY} PARENT_SCOPE)
endif(dsnutil_cpp_WITH_CHRONO)

if(dsnutil_cpp_WITH_LOG)
    add_subdirectory(log)
    set(dsnutil_cpp_log_LIBRARY ${dsnutil_cpp_log_LIBRARY} PARENT_SCOPE)
endif(dsnutil_cpp_WITH_LOG)
