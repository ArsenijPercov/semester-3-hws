cmake_minimum_required(VERSION 2.4 FATAL_ERROR)

include_directories(${PROJECT_BINARY_DIR}/src)

add_library(rpn-stack rpn-stack.c)
