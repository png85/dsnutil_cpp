#
# List of test binaries
list(APPEND example_BINS "parallel_for" "threadpool" "hexdump" "reverse")

#
# Append boost::log tests to list if boost::log is enabled
if(dsnutil_cpp_WITH_BOOST_LOG)
    list(APPEND example_BINS "boost_log-base" "boost_log-stream_interceptor" "boost_log-sinkmanager")
endif(dsnutil_cpp_WITH_BOOST_LOG)


foreach(example ${example_BINS})
    add_executable(${example}_example "${example}.cpp" ${dsnutil_cpp_HDRS})
    target_link_libraries(${example}_example ${CMAKE_THREAD_LIBS_INIT} ${dsnutil_cpp_LIBRARY})
    if(dsnutil_cpp_WITH_LOG4CPP)
        target_link_libraries(${example}_example ${LOG4CPP_LIBRARIES})
    endif(dsnutil_cpp_WITH_LOG4CPP)
    if(dsnutil_cpp_WITH_BOOST_LOG)
        target_link_libraries(${example}_example ${dsnutil_cpp_log_LIBRARY})
        target_link_libraries(${example}_example ${Boost_LIBRARIES})
    endif(dsnutil_cpp_WITH_BOOST_LOG)
    install(TARGETS ${example}_example RUNTIME DESTINATION examples/${CMAKE_BUILD_TYPE})
endforeach(example)
