# CMake generated Testfile for 
# Source directory: /home/terz99/Workspace/homeworks/SDS/hw1
# Build directory: /home/terz99/Workspace/homeworks/SDS/hw1/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(misc1 "/home/terz99/Workspace/homeworks/SDS/hw1/build/test/check_rpn-stack")
add_test(misc2 "/home/terz99/Workspace/homeworks/SDS/hw1/build/test/check_rpn")
subdirs("src")
subdirs("test")
