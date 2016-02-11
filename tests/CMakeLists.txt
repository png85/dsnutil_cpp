set(test_SOURCES finally.cpp singleton.cpp observable.cpp observing_ptr.cpp pretty_print.cpp exception.cpp
    throwing_assert.cpp countof.cpp map_sort.cpp hexdump.cpp reverse.cpp)
if(dsnutil_cpp_WITH_BASE64)
    list(APPEND test_SOURCES base64.cpp)
endif(dsnutil_cpp_WITH_BASE64)

foreach(test_src ${test_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    set_target_properties(${test_name} PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)
    target_link_libraries(${test_name} ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(${test_name} ${dsnutil_cpp_LIBRARY})
    target_link_libraries(${test_name} ${Boost_LIBRARIES})
    # TODO(png): log4cpp
    # TODO(png): boost::log
    # TODO(png): dsnutil_cpp-log
    if(dsnutil_cpp_WITH_BASE64)
        target_link_libraries(${test_name} ${dsnutil_cpp_base64_LIBRARY})
    endif(dsnutil_cpp_WITH_BASE64)
    add_test(NAME ${test_name} COMMAND ${test_name} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    install(TARGETS ${test_name} RUNTIME DESTINATION tests/${CMAKE_BUILD_TYPE})
endforeach(test_src)
