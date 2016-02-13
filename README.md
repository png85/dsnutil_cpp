# libdsnutil_cpp README
(c) 2012-2016 #das-system Networks

## General info
this library contains various little utility/helper functions and classes that have been used in other projects.

## Build dependencies
* Boost >= 1.54
* C++11 capable compiler, the following are known to work:
 * GCC >= 4.8
 * MSVC >=12.0 (Visual Studio 2013 and newer)
 * Clang from XCode on OS X
  * Mavericks build performed by Travis-CI
  * Manual tests during development on El Capitan
 * Clang >= 3.7 on Linux


## Optional dependencies
* log4cpp logging library


## Building [![Build Status](https://travis-ci.org/png85/dsnutil_cpp.png?branch=master)](https://travis-ci.org/png85/dsnutil_cpp)
Use CMake to configure and build as needed. Useful options might be:

* `dsnutil_cpp_BUILD_AS_SUBMODULE` - Build libraries as git submodule (disables Doxygen & CPack)
* `dsnutil_cpp_BUILD_SHARED_LIBS`  - Select wether to build shared (default) or static libraries
* `dsnutil_cpp_WITH_DOCS`          - Build API docs if Doxygen is available
* `dsnutil_cpp_WITH_TESTS`         - Enable/disable build of unit tests (requires Boost)

Optional features can be enabled/disabled via the following options
* `dsnutil_cpp_WITH_BOOST_LOG` - boost::log-based logging functions
* `dsnutil_cpp_WITH_BASE64`    - Base64 encoder/decoder functions
* `dsnutil_cpp_WITH_EXAMPLES`  - Build examples/tests for everything?

The build is setup so that the library can easily be used as a git submodule and integrated into larger CMake-based
builds. This can be done via the following additions to your own CMakeLists.txt:

```CMake
set(dsnutil_cpp_WITH_EXAMPLES OFF CACHE BOOL "Build dsnutil_cpp with examples?")
set(dsnutil_cpp_BUILD_DOCS OFF CACHE BOOL "Build dsnutil_cpp API docs?")
#
# ... possible other options here ...
#
add_subdirectory(dsnutil_cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dsnutil_cpp/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/dsnutil_cpp/include)
```

The last line is required so that your code can find the DLL import/export headers that are auto-generated during build.

### Libraries
The build can be configured to generate static or shared libraries with shared ones being the default setting.
After building you'll end up with the following libraries:
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
