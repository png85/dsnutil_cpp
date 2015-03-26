
list(APPEND example_BINS "finally" "pretty_print" "parallel_for" "threadpool")

foreach(example ${example_BINS})
    add_executable(${example} "${example}.cpp" ${dsnutil_cpp_HDRS})
    target_link_libraries(${example} ${CMAKE_THREAD_LIBS_INIT} dsnutil_cpp)
endforeach(example)
