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
include Homework2/CMakeFiles/Homework2.dir/depend.make

# Include the progress variables for this target.
include Homework2/CMakeFiles/Homework2.dir/progress.make

# Include the compile flags for this target's objects.
include Homework2/CMakeFiles/Homework2.dir/flags.make

Homework2/CMakeFiles/Homework2.dir/Src/Homework2.cpp.o: Homework2/CMakeFiles/Homework2.dir/flags.make
Homework2/CMakeFiles/Homework2.dir/Src/Homework2.cpp.o: ../Homework2/Src/Homework2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chanzy/Desktop/FV-IV/Course/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Homework2/CMakeFiles/Homework2.dir/Src/Homework2.cpp.o"
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework2 && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Homework2.dir/Src/Homework2.cpp.o -c /Users/chanzy/Desktop/FV-IV/Course/Homework2/Src/Homework2.cpp

Homework2/CMakeFiles/Homework2.dir/Src/Homework2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Homework2.dir/Src/Homework2.cpp.i"
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework2 && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chanzy/Desktop/FV-IV/Course/Homework2/Src/Homework2.cpp > CMakeFiles/Homework2.dir/Src/Homework2.cpp.i

Homework2/CMakeFiles/Homework2.dir/Src/Homework2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Homework2.dir/Src/Homework2.cpp.s"
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework2 && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chanzy/Desktop/FV-IV/Course/Homework2/Src/Homework2.cpp -o CMakeFiles/Homework2.dir/Src/Homework2.cpp.s

Homework2/CMakeFiles/Homework2.dir/Src/test_Homework2.cpp.o: Homework2/CMakeFiles/Homework2.dir/flags.make
Homework2/CMakeFiles/Homework2.dir/Src/test_Homework2.cpp.o: ../Homework2/Src/test_Homework2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chanzy/Desktop/FV-IV/Course/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Homework2/CMakeFiles/Homework2.dir/Src/test_Homework2.cpp.o"
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework2 && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Homework2.dir/Src/test_Homework2.cpp.o -c /Users/chanzy/Desktop/FV-IV/Course/Homework2/Src/test_Homework2.cpp

Homework2/CMakeFiles/Homework2.dir/Src/test_Homework2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Homework2.dir/Src/test_Homework2.cpp.i"
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework2 && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chanzy/Desktop/FV-IV/Course/Homework2/Src/test_Homework2.cpp > CMakeFiles/Homework2.dir/Src/test_Homework2.cpp.i

Homework2/CMakeFiles/Homework2.dir/Src/test_Homework2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Homework2.dir/Src/test_Homework2.cpp.s"
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework2 && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chanzy/Desktop/FV-IV/Course/Homework2/Src/test_Homework2.cpp -o CMakeFiles/Homework2.dir/Src/test_Homework2.cpp.s

# Object files for target Homework2
Homework2_OBJECTS = \
"CMakeFiles/Homework2.dir/Src/Homework2.cpp.o" \
"CMakeFiles/Homework2.dir/Src/test_Homework2.cpp.o"

# External object files for target Homework2
Homework2_EXTERNAL_OBJECTS =

Homework2/Homework2: Homework2/CMakeFiles/Homework2.dir/Src/Homework2.cpp.o
Homework2/Homework2: Homework2/CMakeFiles/Homework2.dir/Src/test_Homework2.cpp.o
Homework2/Homework2: Homework2/CMakeFiles/Homework2.dir/build.make
Homework2/Homework2: cfl/libcfl.a
Homework2/Homework2: test/libcfl_test.a
Homework2/Homework2: cfl/libcfl.a
Homework2/Homework2: Homework2/CMakeFiles/Homework2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/chanzy/Desktop/FV-IV/Course/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Homework2"
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework2 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Homework2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Homework2/CMakeFiles/Homework2.dir/build: Homework2/Homework2

.PHONY : Homework2/CMakeFiles/Homework2.dir/build

Homework2/CMakeFiles/Homework2.dir/clean:
	cd /Users/chanzy/Desktop/FV-IV/Course/build/Homework2 && $(CMAKE_COMMAND) -P CMakeFiles/Homework2.dir/cmake_clean.cmake
.PHONY : Homework2/CMakeFiles/Homework2.dir/clean

Homework2/CMakeFiles/Homework2.dir/depend:
	cd /Users/chanzy/Desktop/FV-IV/Course/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/chanzy/Desktop/FV-IV/Course /Users/chanzy/Desktop/FV-IV/Course/Homework2 /Users/chanzy/Desktop/FV-IV/Course/build /Users/chanzy/Desktop/FV-IV/Course/build/Homework2 /Users/chanzy/Desktop/FV-IV/Course/build/Homework2/CMakeFiles/Homework2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Homework2/CMakeFiles/Homework2.dir/depend

