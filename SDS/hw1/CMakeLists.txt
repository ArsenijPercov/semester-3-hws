cmake_minimum_required(VERSION 2.4 FATAL_ERROR)
if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)
project(rpn-stack VERSION 0.4.0 LANGUAGES C)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "-g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

find_package(Threads)
find_package(PkgConfig)

if(CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -Werror)
endif(CMAKE_COMPILER_IS_GNUCC)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME misc1 COMMAND check_rpn-stack)
add_test(NAME misc2 COMMAND check_rpn)