cmake_minimum_required(VERSION 2.4 FATAL_ERROR)
project(rpn-stack LANGUAGES C)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS, "-g O0 -Wall -W -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS, "-fprofile-arcs -ftest-coverage")

find_package(Threads)
find_package(PkgConfig)

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()
add_test(NAME misc COMMAND check_rpn-stack)
