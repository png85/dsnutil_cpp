set(dsnutil_cpp_chrono_SOURCES ../../include/dsnutil/chrono.hpp
    ../../include/dsnutil/chrono/clock_type.hpp
    ../../include/dsnutil/chrono/duration.hpp duration.cpp
    ../../include/dsnutil/chrono/duration_types.hpp
    ../../include/dsnutil/chrono/time_point.hpp time_point.cpp
    ../../include/dsnutil/chrono/timer.hpp timer.cpp)
set(dsnutil_cpp_chrono_LIBRARY dsnutil_cpp-chrono)
if(WIN32)
    if("x${CMAKE_BUILD_TYPE}" STREQUAL "xDebug")
        set(dsnutil_cpp_chrono_LIBRARY dsnutil_cpp_chronod)
    endif("x${CMAKE_BUILD_TYPE}" STREQUAL "xDebug")
endif(WIN32)

add_library(${dsnutil_cpp_chrono_LIBRARY} ${LIB_TYPE} ${dsnutil_cpp_chrono_SOURCES})
set_target_properties(${dsnutil_cpp_chrono_LIBRARY} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON)
if(WIN32)
    set_target_properties(${dsnutil_cpp_chrono_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${dsnutil_cpp_chrono_LIBRARY}_EXPORTS")
endif(WIN32)

if(UNIX AND dsnutil_cpp_BUILD_SHARED_LIBS)
    set_target_properties(${dsnutil_cpp_chrono_LIBRARY} PROPERTIES
        SONAME ${dsnutil_cpp_LIB_SOVERSION}
        VERSION ${dsnutil_cpp_LIB_VERSION})
endif(UNIX AND dsnutil_cpp_BUILD_SHARED_LIBS)

generate_export_header(${dsnutil_cpp_chrono_LIBRARY}
    BASE_NAME dsnutil_cpp_chrono
    EXPORT_MACRO_NAME dsnutil_cpp_chrono_EXPORT
    EXPORT_FILE_NAME ${dsnutil_cpp_BINARY_DIR}/include/dsnutil/chrono/dsnutil_cpp_chrono_Export.h
    STATIC_DEFINE dsnutil_cpp_chrono_BUILT_AS_STATIC)

install(TARGETS ${dsnutil_cpp_chrono_LIBRARY} RUNTIME DESTINATION bin ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
install(FILES ${dsnutil_cpp_BINARY_DIR}/include/dsnutil/chrono/dsnutil_cpp_chrono_Export.h DESTINATION include/dsnutil/chrono)

set(dsnutil_cpp_chrono_LIBRARY ${dsnutil_cpp_chrono_LIBRARY} PARENT_SCOPE)
