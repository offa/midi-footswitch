cmake_minimum_required(VERSION 3.12.0)

project(plug VERSION 0.5.0)
message(STATUS "~~~ ${midi-footswitch} v${PROJECT_VERSION} ~~~")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(midibutton INTERFACE)
target_include_directories(midibutton INTERFACE
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:include>
                            )

include("${CMAKE_BINARY_DIR}/conan_paths.cmake" OPTIONAL)
add_subdirectory(test)