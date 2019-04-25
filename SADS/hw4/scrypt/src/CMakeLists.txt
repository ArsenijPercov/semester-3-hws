#
# scrypt/src/CMakeLists.txt --
#

cmake_minimum_required(VERSION 2.4 FATAL_ERROR)

if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

include_directories(${PROJECT_BINARY_DIR}/src)

add_library(scrypt scrypt.c)

add_executable(main main.c)
target_link_libraries(main scrypt)
