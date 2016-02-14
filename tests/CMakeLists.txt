#
# libdsnutil_cpp unit tests
set(test_SOURCES finally.cpp singleton.cpp observable.cpp observing_ptr.cpp pretty_print.cpp exception.cpp
    throwing_assert.cpp countof.cpp map_sort.cpp hexdump.cpp reverse.cpp parallel_for.cpp threadpool.cpp
    reference_counted.cpp intrusive_ptr.cpp make_intrusive.cpp lambda_unique_ptr.cpp bitfield.cpp)

#
# libdsnutil_cpp-base64 unit tests
if(dsnutil_cpp_WITH_BASE64)
    list(APPEND test_SOURCES base64.cpp)
endif(dsnutil_cpp_WITH_BASE64)


#
# libdsnutil_cpp-log unit tests
if(dsnutil_cpp_WITH_BOOST_LOG)
    list(APPEND test_SOURCES log_init.cpp log_base.cpp log_sinkmanager.cpp log_stream_interceptor.cpp)
endif(dsnutil_cpp_WITH_BOOST_LOG)

#
# libdsnutil_cpp-chrono unit tests
if(dsnutil_cpp_WITH_CHRONO)
    list(APPEND test_SOURCES chrono_time_point.cpp chrono_duration.cpp chrono_timer.cpp)
endif(dsnutil_cpp_WITH_CHRONO)

list(SORT test_SOURCES)

option(dsnutil_cpp_WITH_COVERAGE "Enable code coverage" OFF)
if(dsnutil_cpp_WITH_COVERAGE)
    include(CodeCoverage)
endif(dsnutil_cpp_WITH_COVERAGE)

foreach(test_src ${test_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    set_target_properties(${test_name} PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)
    target_link_libraries(${test_name} ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(${test_name} ${dsnutil_cpp_LIBRARY})
    target_link_libraries(${test_name} ${Boost_LIBRARIES})
    if(dsnutil_cpp_WITH_BOOST_LOG)
        target_link_libraries(${test_name} ${dsnutil_cpp_log_LIBRARY})
    endif(dsnutil_cpp_WITH_BOOST_LOG)
    if(dsnutil_cpp_WITH_BASE64)
        target_link_libraries(${test_name} ${dsnutil_cpp_base64_LIBRARY})
    endif(dsnutil_cpp_WITH_BASE64)
    if(dsnutil_cpp_WITH_CHRONO)
        target_link_libraries(${test_name} ${dsnutil_cpp_chrono_LIBRARY})
    endif(dsnutil_cpp_WITH_CHRONO)
    add_test(NAME ${test_name} COMMAND ${test_name} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    install(TARGETS ${test_name} RUNTIME DESTINATION tests/${CMAKE_BUILD_TYPE})
    if(dsnutil_cpp_WITH_COVERAGE)
        set_target_properties(${test_name} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS_COVERAGE}"
            LINK_FLAGS "${CMAKE_CXX_FLAGS_COVERAGE}")
        setup_target_for_coverage(${test_name}_coverage ${test_name} ${test_name}.coverage)
    endif(dsnutil_cpp_WITH_COVERAGE)
endforeach(test_src)
