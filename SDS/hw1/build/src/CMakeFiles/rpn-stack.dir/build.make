# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/terz99/Workspace/homeworks/SDS/hw1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/terz99/Workspace/homeworks/SDS/hw1/build

# Include any dependencies generated for this target.
include src/CMakeFiles/rpn-stack.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/rpn-stack.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/rpn-stack.dir/flags.make

src/CMakeFiles/rpn-stack.dir/rpn-stack.o: src/CMakeFiles/rpn-stack.dir/flags.make
src/CMakeFiles/rpn-stack.dir/rpn-stack.o: ../src/rpn-stack.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/terz99/Workspace/homeworks/SDS/hw1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/rpn-stack.dir/rpn-stack.o"
	cd /home/terz99/Workspace/homeworks/SDS/hw1/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rpn-stack.dir/rpn-stack.o   -c /home/terz99/Workspace/homeworks/SDS/hw1/src/rpn-stack.c

src/CMakeFiles/rpn-stack.dir/rpn-stack.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rpn-stack.dir/rpn-stack.i"
	cd /home/terz99/Workspace/homeworks/SDS/hw1/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/terz99/Workspace/homeworks/SDS/hw1/src/rpn-stack.c > CMakeFiles/rpn-stack.dir/rpn-stack.i

src/CMakeFiles/rpn-stack.dir/rpn-stack.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rpn-stack.dir/rpn-stack.s"
	cd /home/terz99/Workspace/homeworks/SDS/hw1/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/terz99/Workspace/homeworks/SDS/hw1/src/rpn-stack.c -o CMakeFiles/rpn-stack.dir/rpn-stack.s

src/CMakeFiles/rpn-stack.dir/rpn-stack.o.requires:

.PHONY : src/CMakeFiles/rpn-stack.dir/rpn-stack.o.requires

src/CMakeFiles/rpn-stack.dir/rpn-stack.o.provides: src/CMakeFiles/rpn-stack.dir/rpn-stack.o.requires
	$(MAKE) -f src/CMakeFiles/rpn-stack.dir/build.make src/CMakeFiles/rpn-stack.dir/rpn-stack.o.provides.build
.PHONY : src/CMakeFiles/rpn-stack.dir/rpn-stack.o.provides

src/CMakeFiles/rpn-stack.dir/rpn-stack.o.provides.build: src/CMakeFiles/rpn-stack.dir/rpn-stack.o


# Object files for target rpn-stack
rpn__stack_OBJECTS = \
"CMakeFiles/rpn-stack.dir/rpn-stack.o"

# External object files for target rpn-stack
rpn__stack_EXTERNAL_OBJECTS =

src/librpn-stack.a: src/CMakeFiles/rpn-stack.dir/rpn-stack.o
src/librpn-stack.a: src/CMakeFiles/rpn-stack.dir/build.make
src/librpn-stack.a: src/CMakeFiles/rpn-stack.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/terz99/Workspace/homeworks/SDS/hw1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library librpn-stack.a"
	cd /home/terz99/Workspace/homeworks/SDS/hw1/build/src && $(CMAKE_COMMAND) -P CMakeFiles/rpn-stack.dir/cmake_clean_target.cmake
	cd /home/terz99/Workspace/homeworks/SDS/hw1/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rpn-stack.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/rpn-stack.dir/build: src/librpn-stack.a

.PHONY : src/CMakeFiles/rpn-stack.dir/build

src/CMakeFiles/rpn-stack.dir/requires: src/CMakeFiles/rpn-stack.dir/rpn-stack.o.requires

.PHONY : src/CMakeFiles/rpn-stack.dir/requires

src/CMakeFiles/rpn-stack.dir/clean:
	cd /home/terz99/Workspace/homeworks/SDS/hw1/build/src && $(CMAKE_COMMAND) -P CMakeFiles/rpn-stack.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/rpn-stack.dir/clean

src/CMakeFiles/rpn-stack.dir/depend:
	cd /home/terz99/Workspace/homeworks/SDS/hw1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/terz99/Workspace/homeworks/SDS/hw1 /home/terz99/Workspace/homeworks/SDS/hw1/src /home/terz99/Workspace/homeworks/SDS/hw1/build /home/terz99/Workspace/homeworks/SDS/hw1/build/src /home/terz99/Workspace/homeworks/SDS/hw1/build/src/CMakeFiles/rpn-stack.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/rpn-stack.dir/depend

