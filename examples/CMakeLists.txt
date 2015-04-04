
list(APPEND example_BINS "finally" "pretty_print" "parallel_for" "threadpool" "singleton" "map_sort" "hexdump")

foreach(example ${example_BINS})
    add_executable(${example} "${example}.cpp" ${dsnutil_cpp_HDRS})
    target_link_libraries(${example} ${CMAKE_THREAD_LIBS_INIT} dsnutil_cpp)
    if(dsnutil_cpp_WITH_LOG4CPP)
        target_link_libraries(${example} ${LOG4CPP_LIBRARIES})
    endif(dsnutil_cpp_WITH_LOG4CPP)

    install(TARGETS ${example} RUNTIME DESTINATION examples)
endforeach(example)
