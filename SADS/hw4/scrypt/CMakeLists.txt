#
# scrypt/CMakeLists.txt --
#

cmake_minimum_required(VERSION 2.4 FATAL_ERROR)

if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(scrypt VERSION 0.4.0 LANGUAGES C)

find_package(Threads)
find_package(PkgConfig)

if(CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall)
endif(CMAKE_COMPILER_IS_GNUCC)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME misc COMMAND check-scrypt)

