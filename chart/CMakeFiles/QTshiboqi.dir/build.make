# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/Desktop/chart

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/Desktop/chart

# Include any dependencies generated for this target.
include CMakeFiles/QTshiboqi.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/QTshiboqi.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/QTshiboqi.dir/flags.make

CMakeFiles/QTshiboqi.dir/main.cpp.o: CMakeFiles/QTshiboqi.dir/flags.make
CMakeFiles/QTshiboqi.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Desktop/chart/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/QTshiboqi.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QTshiboqi.dir/main.cpp.o -c /home/pi/Desktop/chart/main.cpp

CMakeFiles/QTshiboqi.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QTshiboqi.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Desktop/chart/main.cpp > CMakeFiles/QTshiboqi.dir/main.cpp.i

CMakeFiles/QTshiboqi.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QTshiboqi.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Desktop/chart/main.cpp -o CMakeFiles/QTshiboqi.dir/main.cpp.s

CMakeFiles/QTshiboqi.dir/widget.cpp.o: CMakeFiles/QTshiboqi.dir/flags.make
CMakeFiles/QTshiboqi.dir/widget.cpp.o: widget.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Desktop/chart/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/QTshiboqi.dir/widget.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QTshiboqi.dir/widget.cpp.o -c /home/pi/Desktop/chart/widget.cpp

CMakeFiles/QTshiboqi.dir/widget.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QTshiboqi.dir/widget.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Desktop/chart/widget.cpp > CMakeFiles/QTshiboqi.dir/widget.cpp.i

CMakeFiles/QTshiboqi.dir/widget.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QTshiboqi.dir/widget.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Desktop/chart/widget.cpp -o CMakeFiles/QTshiboqi.dir/widget.cpp.s

CMakeFiles/QTshiboqi.dir/socketclient.cpp.o: CMakeFiles/QTshiboqi.dir/flags.make
CMakeFiles/QTshiboqi.dir/socketclient.cpp.o: socketclient.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Desktop/chart/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/QTshiboqi.dir/socketclient.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QTshiboqi.dir/socketclient.cpp.o -c /home/pi/Desktop/chart/socketclient.cpp

CMakeFiles/QTshiboqi.dir/socketclient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QTshiboqi.dir/socketclient.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Desktop/chart/socketclient.cpp > CMakeFiles/QTshiboqi.dir/socketclient.cpp.i

CMakeFiles/QTshiboqi.dir/socketclient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QTshiboqi.dir/socketclient.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Desktop/chart/socketclient.cpp -o CMakeFiles/QTshiboqi.dir/socketclient.cpp.s

CMakeFiles/QTshiboqi.dir/client.cpp.o: CMakeFiles/QTshiboqi.dir/flags.make
CMakeFiles/QTshiboqi.dir/client.cpp.o: client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Desktop/chart/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/QTshiboqi.dir/client.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QTshiboqi.dir/client.cpp.o -c /home/pi/Desktop/chart/client.cpp

CMakeFiles/QTshiboqi.dir/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QTshiboqi.dir/client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Desktop/chart/client.cpp > CMakeFiles/QTshiboqi.dir/client.cpp.i

CMakeFiles/QTshiboqi.dir/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QTshiboqi.dir/client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Desktop/chart/client.cpp -o CMakeFiles/QTshiboqi.dir/client.cpp.s

CMakeFiles/QTshiboqi.dir/QTshiboqi_autogen/mocs_compilation.cpp.o: CMakeFiles/QTshiboqi.dir/flags.make
CMakeFiles/QTshiboqi.dir/QTshiboqi_autogen/mocs_compilation.cpp.o: QTshiboqi_autogen/mocs_compilation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Desktop/chart/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/QTshiboqi.dir/QTshiboqi_autogen/mocs_compilation.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QTshiboqi.dir/QTshiboqi_autogen/mocs_compilation.cpp.o -c /home/pi/Desktop/chart/QTshiboqi_autogen/mocs_compilation.cpp

CMakeFiles/QTshiboqi.dir/QTshiboqi_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QTshiboqi.dir/QTshiboqi_autogen/mocs_compilation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Desktop/chart/QTshiboqi_autogen/mocs_compilation.cpp > CMakeFiles/QTshiboqi.dir/QTshiboqi_autogen/mocs_compilation.cpp.i

CMakeFiles/QTshiboqi.dir/QTshiboqi_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QTshiboqi.dir/QTshiboqi_autogen/mocs_compilation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Desktop/chart/QTshiboqi_autogen/mocs_compilation.cpp -o CMakeFiles/QTshiboqi.dir/QTshiboqi_autogen/mocs_compilation.cpp.s

# Object files for target QTshiboqi
QTshiboqi_OBJECTS = \
"CMakeFiles/QTshiboqi.dir/main.cpp.o" \
"CMakeFiles/QTshiboqi.dir/widget.cpp.o" \
"CMakeFiles/QTshiboqi.dir/socketclient.cpp.o" \
"CMakeFiles/QTshiboqi.dir/client.cpp.o" \
"CMakeFiles/QTshiboqi.dir/QTshiboqi_autogen/mocs_compilation.cpp.o"

# External object files for target QTshiboqi
QTshiboqi_EXTERNAL_OBJECTS =

QTshiboqi: CMakeFiles/QTshiboqi.dir/main.cpp.o
QTshiboqi: CMakeFiles/QTshiboqi.dir/widget.cpp.o
QTshiboqi: CMakeFiles/QTshiboqi.dir/socketclient.cpp.o
QTshiboqi: CMakeFiles/QTshiboqi.dir/client.cpp.o
QTshiboqi: CMakeFiles/QTshiboqi.dir/QTshiboqi_autogen/mocs_compilation.cpp.o
QTshiboqi: CMakeFiles/QTshiboqi.dir/build.make
QTshiboqi: /usr/lib/arm-linux-gnueabihf/libQt5Charts.so.5.15.2
QTshiboqi: /usr/lib/arm-linux-gnueabihf/libQt5Widgets.so.5.15.2
QTshiboqi: /usr/lib/arm-linux-gnueabihf/libQt5Gui.so.5.15.2
QTshiboqi: /usr/lib/arm-linux-gnueabihf/libQt5Core.so.5.15.2
QTshiboqi: CMakeFiles/QTshiboqi.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/Desktop/chart/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable QTshiboqi"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/QTshiboqi.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/QTshiboqi.dir/build: QTshiboqi

.PHONY : CMakeFiles/QTshiboqi.dir/build

CMakeFiles/QTshiboqi.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/QTshiboqi.dir/cmake_clean.cmake
.PHONY : CMakeFiles/QTshiboqi.dir/clean

CMakeFiles/QTshiboqi.dir/depend:
	cd /home/pi/Desktop/chart && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/Desktop/chart /home/pi/Desktop/chart /home/pi/Desktop/chart /home/pi/Desktop/chart /home/pi/Desktop/chart/CMakeFiles/QTshiboqi.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/QTshiboqi.dir/depend

