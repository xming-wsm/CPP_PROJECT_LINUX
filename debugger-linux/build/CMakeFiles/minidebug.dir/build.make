# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/build

# Include any dependencies generated for this target.
include CMakeFiles/minidebug.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/minidebug.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/minidebug.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/minidebug.dir/flags.make

CMakeFiles/minidebug.dir/main.cpp.o: CMakeFiles/minidebug.dir/flags.make
CMakeFiles/minidebug.dir/main.cpp.o: ../main.cpp
CMakeFiles/minidebug.dir/main.cpp.o: CMakeFiles/minidebug.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/minidebug.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/minidebug.dir/main.cpp.o -MF CMakeFiles/minidebug.dir/main.cpp.o.d -o CMakeFiles/minidebug.dir/main.cpp.o -c /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/main.cpp

CMakeFiles/minidebug.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/minidebug.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/main.cpp > CMakeFiles/minidebug.dir/main.cpp.i

CMakeFiles/minidebug.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/minidebug.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/main.cpp -o CMakeFiles/minidebug.dir/main.cpp.s

CMakeFiles/minidebug.dir/linenoise.c.o: CMakeFiles/minidebug.dir/flags.make
CMakeFiles/minidebug.dir/linenoise.c.o: ../linenoise.c
CMakeFiles/minidebug.dir/linenoise.c.o: CMakeFiles/minidebug.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/minidebug.dir/linenoise.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/minidebug.dir/linenoise.c.o -MF CMakeFiles/minidebug.dir/linenoise.c.o.d -o CMakeFiles/minidebug.dir/linenoise.c.o -c /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/linenoise.c

CMakeFiles/minidebug.dir/linenoise.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/minidebug.dir/linenoise.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/linenoise.c > CMakeFiles/minidebug.dir/linenoise.c.i

CMakeFiles/minidebug.dir/linenoise.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/minidebug.dir/linenoise.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/linenoise.c -o CMakeFiles/minidebug.dir/linenoise.c.s

CMakeFiles/minidebug.dir/debugger.cpp.o: CMakeFiles/minidebug.dir/flags.make
CMakeFiles/minidebug.dir/debugger.cpp.o: ../debugger.cpp
CMakeFiles/minidebug.dir/debugger.cpp.o: CMakeFiles/minidebug.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/minidebug.dir/debugger.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/minidebug.dir/debugger.cpp.o -MF CMakeFiles/minidebug.dir/debugger.cpp.o.d -o CMakeFiles/minidebug.dir/debugger.cpp.o -c /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/debugger.cpp

CMakeFiles/minidebug.dir/debugger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/minidebug.dir/debugger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/debugger.cpp > CMakeFiles/minidebug.dir/debugger.cpp.i

CMakeFiles/minidebug.dir/debugger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/minidebug.dir/debugger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/debugger.cpp -o CMakeFiles/minidebug.dir/debugger.cpp.s

CMakeFiles/minidebug.dir/breakpoint.cpp.o: CMakeFiles/minidebug.dir/flags.make
CMakeFiles/minidebug.dir/breakpoint.cpp.o: ../breakpoint.cpp
CMakeFiles/minidebug.dir/breakpoint.cpp.o: CMakeFiles/minidebug.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/minidebug.dir/breakpoint.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/minidebug.dir/breakpoint.cpp.o -MF CMakeFiles/minidebug.dir/breakpoint.cpp.o.d -o CMakeFiles/minidebug.dir/breakpoint.cpp.o -c /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/breakpoint.cpp

CMakeFiles/minidebug.dir/breakpoint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/minidebug.dir/breakpoint.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/breakpoint.cpp > CMakeFiles/minidebug.dir/breakpoint.cpp.i

CMakeFiles/minidebug.dir/breakpoint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/minidebug.dir/breakpoint.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/breakpoint.cpp -o CMakeFiles/minidebug.dir/breakpoint.cpp.s

CMakeFiles/minidebug.dir/register.cpp.o: CMakeFiles/minidebug.dir/flags.make
CMakeFiles/minidebug.dir/register.cpp.o: ../register.cpp
CMakeFiles/minidebug.dir/register.cpp.o: CMakeFiles/minidebug.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/minidebug.dir/register.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/minidebug.dir/register.cpp.o -MF CMakeFiles/minidebug.dir/register.cpp.o.d -o CMakeFiles/minidebug.dir/register.cpp.o -c /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/register.cpp

CMakeFiles/minidebug.dir/register.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/minidebug.dir/register.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/register.cpp > CMakeFiles/minidebug.dir/register.cpp.i

CMakeFiles/minidebug.dir/register.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/minidebug.dir/register.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/register.cpp -o CMakeFiles/minidebug.dir/register.cpp.s

# Object files for target minidebug
minidebug_OBJECTS = \
"CMakeFiles/minidebug.dir/main.cpp.o" \
"CMakeFiles/minidebug.dir/linenoise.c.o" \
"CMakeFiles/minidebug.dir/debugger.cpp.o" \
"CMakeFiles/minidebug.dir/breakpoint.cpp.o" \
"CMakeFiles/minidebug.dir/register.cpp.o"

# External object files for target minidebug
minidebug_EXTERNAL_OBJECTS =

minidebug: CMakeFiles/minidebug.dir/main.cpp.o
minidebug: CMakeFiles/minidebug.dir/linenoise.c.o
minidebug: CMakeFiles/minidebug.dir/debugger.cpp.o
minidebug: CMakeFiles/minidebug.dir/breakpoint.cpp.o
minidebug: CMakeFiles/minidebug.dir/register.cpp.o
minidebug: CMakeFiles/minidebug.dir/build.make
minidebug: CMakeFiles/minidebug.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable minidebug"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/minidebug.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/minidebug.dir/build: minidebug
.PHONY : CMakeFiles/minidebug.dir/build

CMakeFiles/minidebug.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/minidebug.dir/cmake_clean.cmake
.PHONY : CMakeFiles/minidebug.dir/clean

CMakeFiles/minidebug.dir/depend:
	cd /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/build /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/build /home/xming/Documents/CPP_PROJECT_LINUX/debugger-linux/build/CMakeFiles/minidebug.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/minidebug.dir/depend

