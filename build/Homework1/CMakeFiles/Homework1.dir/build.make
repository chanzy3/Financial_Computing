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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.18.4/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.18.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/chanzy/Desktop/FV-IV/Course

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/chanzy/Desktop/FV-IV/Course/build

# Include any dependencies generated for this target.
include Homework1/CMakeFiles/Homework1.dir/depend.make

# Include the progress variables for this target.
include Homework1/CMakeFiles/Homework1.dir/progress.make

# Include the compile flags for this target's objects.
include Homework1/CMakeFiles/Homework1.dir/flags.make

Homework1/CMakeFiles/Homework1.dir/Src/Homework1.cpp.o: Homework1/CMakeFiles/Homework1.dir/flags.make
Homework1/CMakeFiles/Homework1.dir/Src/Homework1.cpp.o: ../Homework1/Src/Homework1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chanzy/Desktop/FV-IV/Course/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Homework1/CMakeFiles/Homework1.dir/Src/Homework1.cpp.o"
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework1 && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Homework1.dir/Src/Homework1.cpp.o -c /Users/chanzy/Desktop/FV-IV/Course/Homework1/Src/Homework1.cpp

Homework1/CMakeFiles/Homework1.dir/Src/Homework1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Homework1.dir/Src/Homework1.cpp.i"
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework1 && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chanzy/Desktop/FV-IV/Course/Homework1/Src/Homework1.cpp > CMakeFiles/Homework1.dir/Src/Homework1.cpp.i

Homework1/CMakeFiles/Homework1.dir/Src/Homework1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Homework1.dir/Src/Homework1.cpp.s"
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework1 && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chanzy/Desktop/FV-IV/Course/Homework1/Src/Homework1.cpp -o CMakeFiles/Homework1.dir/Src/Homework1.cpp.s

Homework1/CMakeFiles/Homework1.dir/Src/test_Homework1.cpp.o: Homework1/CMakeFiles/Homework1.dir/flags.make
Homework1/CMakeFiles/Homework1.dir/Src/test_Homework1.cpp.o: ../Homework1/Src/test_Homework1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chanzy/Desktop/FV-IV/Course/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Homework1/CMakeFiles/Homework1.dir/Src/test_Homework1.cpp.o"
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework1 && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Homework1.dir/Src/test_Homework1.cpp.o -c /Users/chanzy/Desktop/FV-IV/Course/Homework1/Src/test_Homework1.cpp

Homework1/CMakeFiles/Homework1.dir/Src/test_Homework1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Homework1.dir/Src/test_Homework1.cpp.i"
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework1 && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chanzy/Desktop/FV-IV/Course/Homework1/Src/test_Homework1.cpp > CMakeFiles/Homework1.dir/Src/test_Homework1.cpp.i

Homework1/CMakeFiles/Homework1.dir/Src/test_Homework1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Homework1.dir/Src/test_Homework1.cpp.s"
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework1 && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chanzy/Desktop/FV-IV/Course/Homework1/Src/test_Homework1.cpp -o CMakeFiles/Homework1.dir/Src/test_Homework1.cpp.s

# Object files for target Homework1
Homework1_OBJECTS = \
"CMakeFiles/Homework1.dir/Src/Homework1.cpp.o" \
"CMakeFiles/Homework1.dir/Src/test_Homework1.cpp.o"

# External object files for target Homework1
Homework1_EXTERNAL_OBJECTS =

Homework1/Homework1: Homework1/CMakeFiles/Homework1.dir/Src/Homework1.cpp.o
Homework1/Homework1: Homework1/CMakeFiles/Homework1.dir/Src/test_Homework1.cpp.o
Homework1/Homework1: Homework1/CMakeFiles/Homework1.dir/build.make
Homework1/Homework1: cfl/libcfl.a
Homework1/Homework1: test/libcfl_test.a
Homework1/Homework1: cfl/libcfl.a
Homework1/Homework1: Homework1/CMakeFiles/Homework1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/chanzy/Desktop/FV-IV/Course/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Homework1"
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework1 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Homework1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Homework1/CMakeFiles/Homework1.dir/build: Homework1/Homework1

.PHONY : Homework1/CMakeFiles/Homework1.dir/build

Homework1/CMakeFiles/Homework1.dir/clean:
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework1 && $(CMAKE_COMMAND) -P CMakeFiles/Homework1.dir/cmake_clean.cmake
.PHONY : Homework1/CMakeFiles/Homework1.dir/clean

Homework1/CMakeFiles/Homework1.dir/depend:
	cd /Users/chanzy/Desktop/FV-IV/Course/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/chanzy/Desktop/FV-IV/Course /Users/chanzy/Desktop/FV-IV/Course/Homework1 /Users/chanzy/Desktop/FV-IV/Course/build /Users/chanzy/Desktop/FV-IV/Course/build/Homework1 /Users/chanzy/Desktop/FV-IV/Course/build/Homework1/CMakeFiles/Homework1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Homework1/CMakeFiles/Homework1.dir/depend

