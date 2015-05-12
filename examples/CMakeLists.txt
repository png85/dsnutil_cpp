#
# List of test binaries
list(APPEND example_BINS "finally" "pretty_print" "parallel_for" "threadpool" "singleton" "map_sort" "hexdump" "reverse" "countof")

#
# Append boost::log tests to list if boost::log is enabled
if(dsnutil_cpp_WITH_BOOST_LOG)
    list(APPEND example_BINS "boost_log-base" "boost_log-stream_interceptor" "boost_log-sinkmanager")
endif(dsnutil_cpp_WITH_BOOST_LOG)

#
# Append base64 test binaries to list if base64 is enabled
if(dsnutil_cpp_WITH_BASE64)
    list(APPEND example_BINS "base64")
endif(dsnutil_cpp_WITH_BASE64)

foreach(example ${example_BINS})
    add_executable(${example} "${example}.cpp" ${dsnutil_cpp_HDRS})
    target_link_libraries(${example} ${CMAKE_THREAD_LIBS_INIT} dsnutil_cpp)
    if(dsnutil_cpp_WITH_LOG4CPP)
        target_link_libraries(${example} ${LOG4CPP_LIBRARIES})
    endif(dsnutil_cpp_WITH_LOG4CPP)
    if(dsnutil_cpp_WITH_BOOST_LOG)
        target_link_libraries(${example} dsnutil_cpp-log)
        target_link_libraries(${example} ${Boost_LIBRARIES})
    endif(dsnutil_cpp_WITH_BOOST_LOG)
    if(dsnutil_cpp_WITH_BASE64)
        target_link_libraries(${example} dsnutil_cpp-base64)
    endif(dsnutil_cpp_WITH_BASE64)
    add_test("examples/${example}" ${example})
    install(TARGETS ${example} RUNTIME DESTINATION examples)
endforeach(example)
