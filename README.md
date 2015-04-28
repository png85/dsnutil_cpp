# libdsnutil_cpp README
(c) 2015 #das-system Networks

## General info
this library contains various little utility/helper functions and classes that have been used in other projects.

## Build dependencies
* Boost >= 1.54
* C++11 capable compiler, the following are known to work:
 * GCC >= 4.8
 * MSVC 12.0 (Visual Studio 2013)


## Optional dependencies
* log4cpp logging library


## Building [![Build Status](https://travis-ci.org/png85/dsnutil_cpp.png?branch=master)](https://travis-ci.org/png85/dsnutil_cpp)
Use CMake to configure and build as needed. Useful options might be:

* `dsnutil_cpp_WITH_LOG4CPP`   - Enable/disable log4cpp support
* `dsnutil_cpp_WITH_BOOST_LOG` - Enable/disable boost::log support
* `dsnutil_cpp_WITH_BASE64`    - Enable/disable base64 library
* `dsnutil_cpp_WITH_EXAMPLES`  - Enable/disable building example binaries and tests

The build is setup so that the library can easily be used as a git submodule and integrated into larger CMake-based builds. This can be done via the following
additions to your own CMakeLists.txt:

```CMake
set(dsnutil_cpp_WITH_EXAMPLES OFF CACHE BOOL "Build dsnutil_cpp with examples?")
add_subdirectory(dsnutil_cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dsnutil_cpp/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/dsnutil_cpp/include)
```

The last line is required so that your code can find the DLL import/export headers that are auto-generated during build.

### Libraries
The build can be configured to generate static or shared libraries with shared ones being the default setting. After building you'll end up with the following libraries:
* `libdsnutil_cpp`        - main library with random lower level utility classes
* `libdsnutil_cpp-log`    - (optional) logging API based on boost::log
* `libdsnutil_cpp-base64` - (optional) base64 encode/decode functions


### Binary packages
Due to the large variety of possible target platforms and compilers there are currently no official binary packages available.
The *package* target in the CMake generated build files can usually be used to generate one for your target platform if needed.


### API documentation
The CMake-based build will automatically detect Doxygen if its installed and add a target to the build which can be used to generate API documentation in HTML format.


### CDash
The build contains targets and configuration to drop build info on http://cdash.das-system-networks.de/
