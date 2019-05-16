# 
# pnmhide/test/CMakeLists.txt --
#

find_program (BASH_PROGRAM bash)

if (BASH_PROGRAM)
    add_test(mytest ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/check.sh)
endif (BASH_PROGRAM)
