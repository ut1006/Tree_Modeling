# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kamada/work/Tree_Modeling/home_develop

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kamada/work/Tree_Modeling/home_develop/build

# Include any dependencies generated for this target.
include CMakeFiles/search_candidates.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/search_candidates.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/search_candidates.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/search_candidates.dir/flags.make

CMakeFiles/search_candidates.dir/src/main.cpp.o: CMakeFiles/search_candidates.dir/flags.make
CMakeFiles/search_candidates.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/search_candidates.dir/src/main.cpp.o: CMakeFiles/search_candidates.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kamada/work/Tree_Modeling/home_develop/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/search_candidates.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_candidates.dir/src/main.cpp.o -MF CMakeFiles/search_candidates.dir/src/main.cpp.o.d -o CMakeFiles/search_candidates.dir/src/main.cpp.o -c /home/kamada/work/Tree_Modeling/home_develop/src/main.cpp

CMakeFiles/search_candidates.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_candidates.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kamada/work/Tree_Modeling/home_develop/src/main.cpp > CMakeFiles/search_candidates.dir/src/main.cpp.i

CMakeFiles/search_candidates.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_candidates.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kamada/work/Tree_Modeling/home_develop/src/main.cpp -o CMakeFiles/search_candidates.dir/src/main.cpp.s

CMakeFiles/search_candidates.dir/src/source.cpp.o: CMakeFiles/search_candidates.dir/flags.make
CMakeFiles/search_candidates.dir/src/source.cpp.o: ../src/source.cpp
CMakeFiles/search_candidates.dir/src/source.cpp.o: CMakeFiles/search_candidates.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kamada/work/Tree_Modeling/home_develop/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/search_candidates.dir/src/source.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_candidates.dir/src/source.cpp.o -MF CMakeFiles/search_candidates.dir/src/source.cpp.o.d -o CMakeFiles/search_candidates.dir/src/source.cpp.o -c /home/kamada/work/Tree_Modeling/home_develop/src/source.cpp

CMakeFiles/search_candidates.dir/src/source.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_candidates.dir/src/source.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kamada/work/Tree_Modeling/home_develop/src/source.cpp > CMakeFiles/search_candidates.dir/src/source.cpp.i

CMakeFiles/search_candidates.dir/src/source.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_candidates.dir/src/source.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kamada/work/Tree_Modeling/home_develop/src/source.cpp -o CMakeFiles/search_candidates.dir/src/source.cpp.s

CMakeFiles/search_candidates.dir/src/callbacks.cpp.o: CMakeFiles/search_candidates.dir/flags.make
CMakeFiles/search_candidates.dir/src/callbacks.cpp.o: ../src/callbacks.cpp
CMakeFiles/search_candidates.dir/src/callbacks.cpp.o: CMakeFiles/search_candidates.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kamada/work/Tree_Modeling/home_develop/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/search_candidates.dir/src/callbacks.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_candidates.dir/src/callbacks.cpp.o -MF CMakeFiles/search_candidates.dir/src/callbacks.cpp.o.d -o CMakeFiles/search_candidates.dir/src/callbacks.cpp.o -c /home/kamada/work/Tree_Modeling/home_develop/src/callbacks.cpp

CMakeFiles/search_candidates.dir/src/callbacks.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_candidates.dir/src/callbacks.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kamada/work/Tree_Modeling/home_develop/src/callbacks.cpp > CMakeFiles/search_candidates.dir/src/callbacks.cpp.i

CMakeFiles/search_candidates.dir/src/callbacks.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_candidates.dir/src/callbacks.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kamada/work/Tree_Modeling/home_develop/src/callbacks.cpp -o CMakeFiles/search_candidates.dir/src/callbacks.cpp.s

CMakeFiles/search_candidates.dir/src/draw.cpp.o: CMakeFiles/search_candidates.dir/flags.make
CMakeFiles/search_candidates.dir/src/draw.cpp.o: ../src/draw.cpp
CMakeFiles/search_candidates.dir/src/draw.cpp.o: CMakeFiles/search_candidates.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kamada/work/Tree_Modeling/home_develop/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/search_candidates.dir/src/draw.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_candidates.dir/src/draw.cpp.o -MF CMakeFiles/search_candidates.dir/src/draw.cpp.o.d -o CMakeFiles/search_candidates.dir/src/draw.cpp.o -c /home/kamada/work/Tree_Modeling/home_develop/src/draw.cpp

CMakeFiles/search_candidates.dir/src/draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_candidates.dir/src/draw.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kamada/work/Tree_Modeling/home_develop/src/draw.cpp > CMakeFiles/search_candidates.dir/src/draw.cpp.i

CMakeFiles/search_candidates.dir/src/draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_candidates.dir/src/draw.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kamada/work/Tree_Modeling/home_develop/src/draw.cpp -o CMakeFiles/search_candidates.dir/src/draw.cpp.s

CMakeFiles/search_candidates.dir/src/branches.cpp.o: CMakeFiles/search_candidates.dir/flags.make
CMakeFiles/search_candidates.dir/src/branches.cpp.o: ../src/branches.cpp
CMakeFiles/search_candidates.dir/src/branches.cpp.o: CMakeFiles/search_candidates.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kamada/work/Tree_Modeling/home_develop/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/search_candidates.dir/src/branches.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_candidates.dir/src/branches.cpp.o -MF CMakeFiles/search_candidates.dir/src/branches.cpp.o.d -o CMakeFiles/search_candidates.dir/src/branches.cpp.o -c /home/kamada/work/Tree_Modeling/home_develop/src/branches.cpp

CMakeFiles/search_candidates.dir/src/branches.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_candidates.dir/src/branches.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kamada/work/Tree_Modeling/home_develop/src/branches.cpp > CMakeFiles/search_candidates.dir/src/branches.cpp.i

CMakeFiles/search_candidates.dir/src/branches.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_candidates.dir/src/branches.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kamada/work/Tree_Modeling/home_develop/src/branches.cpp -o CMakeFiles/search_candidates.dir/src/branches.cpp.s

CMakeFiles/search_candidates.dir/src/globals.cpp.o: CMakeFiles/search_candidates.dir/flags.make
CMakeFiles/search_candidates.dir/src/globals.cpp.o: ../src/globals.cpp
CMakeFiles/search_candidates.dir/src/globals.cpp.o: CMakeFiles/search_candidates.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kamada/work/Tree_Modeling/home_develop/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/search_candidates.dir/src/globals.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_candidates.dir/src/globals.cpp.o -MF CMakeFiles/search_candidates.dir/src/globals.cpp.o.d -o CMakeFiles/search_candidates.dir/src/globals.cpp.o -c /home/kamada/work/Tree_Modeling/home_develop/src/globals.cpp

CMakeFiles/search_candidates.dir/src/globals.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_candidates.dir/src/globals.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kamada/work/Tree_Modeling/home_develop/src/globals.cpp > CMakeFiles/search_candidates.dir/src/globals.cpp.i

CMakeFiles/search_candidates.dir/src/globals.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_candidates.dir/src/globals.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kamada/work/Tree_Modeling/home_develop/src/globals.cpp -o CMakeFiles/search_candidates.dir/src/globals.cpp.s

CMakeFiles/search_candidates.dir/src/decide_candidates.cpp.o: CMakeFiles/search_candidates.dir/flags.make
CMakeFiles/search_candidates.dir/src/decide_candidates.cpp.o: ../src/decide_candidates.cpp
CMakeFiles/search_candidates.dir/src/decide_candidates.cpp.o: CMakeFiles/search_candidates.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kamada/work/Tree_Modeling/home_develop/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/search_candidates.dir/src/decide_candidates.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_candidates.dir/src/decide_candidates.cpp.o -MF CMakeFiles/search_candidates.dir/src/decide_candidates.cpp.o.d -o CMakeFiles/search_candidates.dir/src/decide_candidates.cpp.o -c /home/kamada/work/Tree_Modeling/home_develop/src/decide_candidates.cpp

CMakeFiles/search_candidates.dir/src/decide_candidates.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_candidates.dir/src/decide_candidates.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kamada/work/Tree_Modeling/home_develop/src/decide_candidates.cpp > CMakeFiles/search_candidates.dir/src/decide_candidates.cpp.i

CMakeFiles/search_candidates.dir/src/decide_candidates.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_candidates.dir/src/decide_candidates.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kamada/work/Tree_Modeling/home_develop/src/decide_candidates.cpp -o CMakeFiles/search_candidates.dir/src/decide_candidates.cpp.s

# Object files for target search_candidates
search_candidates_OBJECTS = \
"CMakeFiles/search_candidates.dir/src/main.cpp.o" \
"CMakeFiles/search_candidates.dir/src/source.cpp.o" \
"CMakeFiles/search_candidates.dir/src/callbacks.cpp.o" \
"CMakeFiles/search_candidates.dir/src/draw.cpp.o" \
"CMakeFiles/search_candidates.dir/src/branches.cpp.o" \
"CMakeFiles/search_candidates.dir/src/globals.cpp.o" \
"CMakeFiles/search_candidates.dir/src/decide_candidates.cpp.o"

# External object files for target search_candidates
search_candidates_EXTERNAL_OBJECTS =

search_candidates: CMakeFiles/search_candidates.dir/src/main.cpp.o
search_candidates: CMakeFiles/search_candidates.dir/src/source.cpp.o
search_candidates: CMakeFiles/search_candidates.dir/src/callbacks.cpp.o
search_candidates: CMakeFiles/search_candidates.dir/src/draw.cpp.o
search_candidates: CMakeFiles/search_candidates.dir/src/branches.cpp.o
search_candidates: CMakeFiles/search_candidates.dir/src/globals.cpp.o
search_candidates: CMakeFiles/search_candidates.dir/src/decide_candidates.cpp.o
search_candidates: CMakeFiles/search_candidates.dir/build.make
search_candidates: /usr/lib/x86_64-linux-gnu/libglfw.so.3.3
search_candidates: /usr/lib/x86_64-linux-gnu/libGL.so
search_candidates: /usr/lib/x86_64-linux-gnu/libGLEW.so
search_candidates: CMakeFiles/search_candidates.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kamada/work/Tree_Modeling/home_develop/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable search_candidates"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/search_candidates.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/search_candidates.dir/build: search_candidates
.PHONY : CMakeFiles/search_candidates.dir/build

CMakeFiles/search_candidates.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/search_candidates.dir/cmake_clean.cmake
.PHONY : CMakeFiles/search_candidates.dir/clean

CMakeFiles/search_candidates.dir/depend:
	cd /home/kamada/work/Tree_Modeling/home_develop/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kamada/work/Tree_Modeling/home_develop /home/kamada/work/Tree_Modeling/home_develop /home/kamada/work/Tree_Modeling/home_develop/build /home/kamada/work/Tree_Modeling/home_develop/build /home/kamada/work/Tree_Modeling/home_develop/build/CMakeFiles/search_candidates.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/search_candidates.dir/depend

