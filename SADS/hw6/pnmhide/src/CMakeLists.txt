# 
# pnmhide/src/CMakeLists.txt --
#

#put executable in parent dir
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

pkg_check_modules(NETPBM netpbm)
add_definitions(${NETPBM_CFLAGS})

add_executable(pnmhide pnmhide.c image.c hide.c)
target_link_libraries(pnmhide ${NETPBM_LDFLAGS} netpbm)

if(CMAKE_C_COMPILER_ID MATCHES "GNU")
  add_definitions(--std=c99 -O2 -Wall)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
  add_definitions(--std=c99 -O2 -Wall)
endif()
