# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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
CMAKE_COMMAND = "/Applications/CMake 2.8-4.app/Contents/bin/cmake"

# The command to remove a file.
RM = "/Applications/CMake 2.8-4.app/Contents/bin/cmake" -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = "/Applications/CMake 2.8-4.app/Contents/bin/ccmake"

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/hobu/hg/liblas-dist

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/hobu/hg/liblas-dist

# Include any dependencies generated for this target.
include src/CMakeFiles/las_c.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/las_c.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/las_c.dir/flags.make

src/CMakeFiles/las_c.dir/c_api.cpp.o: src/CMakeFiles/las_c.dir/flags.make
src/CMakeFiles/las_c.dir/c_api.cpp.o: src/c_api.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/hobu/hg/liblas-dist/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/las_c.dir/c_api.cpp.o"
	cd /Users/hobu/hg/liblas-dist/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/las_c.dir/c_api.cpp.o -c /Users/hobu/hg/liblas-dist/src/c_api.cpp

src/CMakeFiles/las_c.dir/c_api.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/las_c.dir/c_api.cpp.i"
	cd /Users/hobu/hg/liblas-dist/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/hobu/hg/liblas-dist/src/c_api.cpp > CMakeFiles/las_c.dir/c_api.cpp.i

src/CMakeFiles/las_c.dir/c_api.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/las_c.dir/c_api.cpp.s"
	cd /Users/hobu/hg/liblas-dist/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/hobu/hg/liblas-dist/src/c_api.cpp -o CMakeFiles/las_c.dir/c_api.cpp.s

src/CMakeFiles/las_c.dir/c_api.cpp.o.requires:
.PHONY : src/CMakeFiles/las_c.dir/c_api.cpp.o.requires

src/CMakeFiles/las_c.dir/c_api.cpp.o.provides: src/CMakeFiles/las_c.dir/c_api.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/las_c.dir/build.make src/CMakeFiles/las_c.dir/c_api.cpp.o.provides.build
.PHONY : src/CMakeFiles/las_c.dir/c_api.cpp.o.provides

src/CMakeFiles/las_c.dir/c_api.cpp.o.provides.build: src/CMakeFiles/las_c.dir/c_api.cpp.o

# Object files for target las_c
las_c_OBJECTS = \
"CMakeFiles/las_c.dir/c_api.cpp.o"

# External object files for target las_c
las_c_EXTERNAL_OBJECTS =

bin/Release/liblas_c.2.0.0.dylib: src/CMakeFiles/las_c.dir/c_api.cpp.o
bin/Release/liblas_c.2.0.0.dylib: bin/Release/liblas.2.0.0.dylib
bin/Release/liblas_c.2.0.0.dylib: /usr/local/lib/libboost_program_options.a
bin/Release/liblas_c.2.0.0.dylib: /usr/local/lib/libboost_thread.a
bin/Release/liblas_c.2.0.0.dylib: src/CMakeFiles/las_c.dir/build.make
bin/Release/liblas_c.2.0.0.dylib: src/CMakeFiles/las_c.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library ../bin/Release/liblas_c.dylib"
	cd /Users/hobu/hg/liblas-dist/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/las_c.dir/link.txt --verbose=$(VERBOSE)
	cd /Users/hobu/hg/liblas-dist/src && $(CMAKE_COMMAND) -E cmake_symlink_library ../bin/Release/liblas_c.2.0.0.dylib ../bin/Release/liblas_c.2.0.0.dylib ../bin/Release/liblas_c.dylib

bin/Release/liblas_c.dylib: bin/Release/liblas_c.2.0.0.dylib

# Rule to build all files generated by this target.
src/CMakeFiles/las_c.dir/build: bin/Release/liblas_c.dylib
.PHONY : src/CMakeFiles/las_c.dir/build

src/CMakeFiles/las_c.dir/requires: src/CMakeFiles/las_c.dir/c_api.cpp.o.requires
.PHONY : src/CMakeFiles/las_c.dir/requires

src/CMakeFiles/las_c.dir/clean:
	cd /Users/hobu/hg/liblas-dist/src && $(CMAKE_COMMAND) -P CMakeFiles/las_c.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/las_c.dir/clean

src/CMakeFiles/las_c.dir/depend:
	cd /Users/hobu/hg/liblas-dist && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hobu/hg/liblas-dist /Users/hobu/hg/liblas-dist/src /Users/hobu/hg/liblas-dist /Users/hobu/hg/liblas-dist/src /Users/hobu/hg/liblas-dist/src/CMakeFiles/las_c.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/las_c.dir/depend

