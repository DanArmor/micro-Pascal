# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dan/micro-Pascal

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dan/micro-Pascal/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/testQueue.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/testQueue.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/testQueue.dir/flags.make

tests/CMakeFiles/testQueue.dir/structures/testQueue.cpp.o: tests/CMakeFiles/testQueue.dir/flags.make
tests/CMakeFiles/testQueue.dir/structures/testQueue.cpp.o: ../tests/structures/testQueue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dan/micro-Pascal/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/testQueue.dir/structures/testQueue.cpp.o"
	cd /home/dan/micro-Pascal/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testQueue.dir/structures/testQueue.cpp.o -c /home/dan/micro-Pascal/tests/structures/testQueue.cpp

tests/CMakeFiles/testQueue.dir/structures/testQueue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testQueue.dir/structures/testQueue.cpp.i"
	cd /home/dan/micro-Pascal/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dan/micro-Pascal/tests/structures/testQueue.cpp > CMakeFiles/testQueue.dir/structures/testQueue.cpp.i

tests/CMakeFiles/testQueue.dir/structures/testQueue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testQueue.dir/structures/testQueue.cpp.s"
	cd /home/dan/micro-Pascal/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dan/micro-Pascal/tests/structures/testQueue.cpp -o CMakeFiles/testQueue.dir/structures/testQueue.cpp.s

# Object files for target testQueue
testQueue_OBJECTS = \
"CMakeFiles/testQueue.dir/structures/testQueue.cpp.o"

# External object files for target testQueue
testQueue_EXTERNAL_OBJECTS =

tests/testQueue: tests/CMakeFiles/testQueue.dir/structures/testQueue.cpp.o
tests/testQueue: tests/CMakeFiles/testQueue.dir/build.make
tests/testQueue: tests/CMakeFiles/testQueue.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dan/micro-Pascal/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable testQueue"
	cd /home/dan/micro-Pascal/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testQueue.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/testQueue.dir/build: tests/testQueue

.PHONY : tests/CMakeFiles/testQueue.dir/build

tests/CMakeFiles/testQueue.dir/clean:
	cd /home/dan/micro-Pascal/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/testQueue.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/testQueue.dir/clean

tests/CMakeFiles/testQueue.dir/depend:
	cd /home/dan/micro-Pascal/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dan/micro-Pascal /home/dan/micro-Pascal/tests /home/dan/micro-Pascal/build /home/dan/micro-Pascal/build/tests /home/dan/micro-Pascal/build/tests/CMakeFiles/testQueue.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/testQueue.dir/depend

