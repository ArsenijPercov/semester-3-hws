cmake_minimum_required(VERSION 2.4 FATAL_ERROR)

pkg_check_modules(CHECK REQUIRED check)

include_directories(${CMAKE_SOURCE_DIR}/src ${CHECK_INCLUDE_DIRS})
link_directories(${CHECK_LIBRARY_DIRS})

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


add_executable(check_rpn-stack check_rpn-stack.c)
add_executable(check_rpn check_rpn.c)

target_link_libraries(check_rpn-stack rpn-stack
	${CHECK_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(check_rpn rpn rpn-stack
	${CHECK_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT})