#
# pnmhide/CMakeLists.txt --
#

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(pnmhide VERSION 0.4.0 LANGUAGES C)

find_package(PkgConfig)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(doc)

enable_testing()

if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    add_definitions(--std=c99 -O2 -Wall)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_definitions(--std=c99 -O2 -Wall)
endif()
