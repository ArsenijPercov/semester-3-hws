#
# scrypt/test/CMakeLists.txt --
#

cmake_minimum_required(VERSION 2.4 FATAL_ERROR)

if (POLICY CMP0048)
	cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

pkg_check_modules(CHECK REQUIRED check)

include_directories(${CMAKE_SOURCE_DIR}/src ${CHECK_INCLUDE_DIRS})
link_directories(${CHECK_LIBRARY_DIRS})

add_executable(check-scrypt check-scrypt.c)

target_link_libraries(check-scrypt scrypt
	${CHECK_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT})
