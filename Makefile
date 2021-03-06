# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /mnt/c/Users/vovaa/Documents/bs-lab/bs-lab

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/vovaa/Documents/bs-lab/bs-lab

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/c/Users/vovaa/Documents/bs-lab/bs-lab/CMakeFiles /mnt/c/Users/vovaa/Documents/bs-lab/bs-lab/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/c/Users/vovaa/Documents/bs-lab/bs-lab/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named integrationtests

# Build rule for target.
integrationtests: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 integrationtests
.PHONY : integrationtests

# fast build rule for target.
integrationtests/fast:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/build
.PHONY : integrationtests/fast

#=============================================================================
# Target rules for targets named unittests

# Build rule for target.
unittests: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 unittests
.PHONY : unittests

# fast build rule for target.
unittests/fast:
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/build
.PHONY : unittests/fast

#=============================================================================
# Target rules for targets named mount.myfs

# Build rule for target.
mount.myfs: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 mount.myfs
.PHONY : mount.myfs

# fast build rule for target.
mount.myfs/fast:
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/build
.PHONY : mount.myfs/fast

src/DMAP.o: src/DMAP.cpp.o

.PHONY : src/DMAP.o

# target to build an object file
src/DMAP.cpp.o:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/DMAP.cpp.o
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/DMAP.cpp.o
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/DMAP.cpp.o
.PHONY : src/DMAP.cpp.o

src/DMAP.i: src/DMAP.cpp.i

.PHONY : src/DMAP.i

# target to preprocess a source file
src/DMAP.cpp.i:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/DMAP.cpp.i
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/DMAP.cpp.i
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/DMAP.cpp.i
.PHONY : src/DMAP.cpp.i

src/DMAP.s: src/DMAP.cpp.s

.PHONY : src/DMAP.s

# target to generate assembly for a file
src/DMAP.cpp.s:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/DMAP.cpp.s
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/DMAP.cpp.s
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/DMAP.cpp.s
.PHONY : src/DMAP.cpp.s

src/FAT.o: src/FAT.cpp.o

.PHONY : src/FAT.o

# target to build an object file
src/FAT.cpp.o:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/FAT.cpp.o
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/FAT.cpp.o
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/FAT.cpp.o
.PHONY : src/FAT.cpp.o

src/FAT.i: src/FAT.cpp.i

.PHONY : src/FAT.i

# target to preprocess a source file
src/FAT.cpp.i:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/FAT.cpp.i
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/FAT.cpp.i
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/FAT.cpp.i
.PHONY : src/FAT.cpp.i

src/FAT.s: src/FAT.cpp.s

.PHONY : src/FAT.s

# target to generate assembly for a file
src/FAT.cpp.s:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/FAT.cpp.s
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/FAT.cpp.s
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/FAT.cpp.s
.PHONY : src/FAT.cpp.s

src/RootDir.o: src/RootDir.cpp.o

.PHONY : src/RootDir.o

# target to build an object file
src/RootDir.cpp.o:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/RootDir.cpp.o
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/RootDir.cpp.o
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/RootDir.cpp.o
.PHONY : src/RootDir.cpp.o

src/RootDir.i: src/RootDir.cpp.i

.PHONY : src/RootDir.i

# target to preprocess a source file
src/RootDir.cpp.i:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/RootDir.cpp.i
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/RootDir.cpp.i
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/RootDir.cpp.i
.PHONY : src/RootDir.cpp.i

src/RootDir.s: src/RootDir.cpp.s

.PHONY : src/RootDir.s

# target to generate assembly for a file
src/RootDir.cpp.s:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/RootDir.cpp.s
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/RootDir.cpp.s
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/RootDir.cpp.s
.PHONY : src/RootDir.cpp.s

src/Utils.o: src/Utils.cpp.o

.PHONY : src/Utils.o

# target to build an object file
src/Utils.cpp.o:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/Utils.cpp.o
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/Utils.cpp.o
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/Utils.cpp.o
.PHONY : src/Utils.cpp.o

src/Utils.i: src/Utils.cpp.i

.PHONY : src/Utils.i

# target to preprocess a source file
src/Utils.cpp.i:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/Utils.cpp.i
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/Utils.cpp.i
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/Utils.cpp.i
.PHONY : src/Utils.cpp.i

src/Utils.s: src/Utils.cpp.s

.PHONY : src/Utils.s

# target to generate assembly for a file
src/Utils.cpp.s:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/Utils.cpp.s
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/Utils.cpp.s
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/Utils.cpp.s
.PHONY : src/Utils.cpp.s

src/blockdevice.o: src/blockdevice.cpp.o

.PHONY : src/blockdevice.o

# target to build an object file
src/blockdevice.cpp.o:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/blockdevice.cpp.o
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/blockdevice.cpp.o
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/blockdevice.cpp.o
.PHONY : src/blockdevice.cpp.o

src/blockdevice.i: src/blockdevice.cpp.i

.PHONY : src/blockdevice.i

# target to preprocess a source file
src/blockdevice.cpp.i:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/blockdevice.cpp.i
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/blockdevice.cpp.i
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/blockdevice.cpp.i
.PHONY : src/blockdevice.cpp.i

src/blockdevice.s: src/blockdevice.cpp.s

.PHONY : src/blockdevice.s

# target to generate assembly for a file
src/blockdevice.cpp.s:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/blockdevice.cpp.s
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/blockdevice.cpp.s
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/blockdevice.cpp.s
.PHONY : src/blockdevice.cpp.s

src/mount.myfs.o: src/mount.myfs.c.o

.PHONY : src/mount.myfs.o

# target to build an object file
src/mount.myfs.c.o:
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/mount.myfs.c.o
.PHONY : src/mount.myfs.c.o

src/mount.myfs.i: src/mount.myfs.c.i

.PHONY : src/mount.myfs.i

# target to preprocess a source file
src/mount.myfs.c.i:
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/mount.myfs.c.i
.PHONY : src/mount.myfs.c.i

src/mount.myfs.s: src/mount.myfs.c.s

.PHONY : src/mount.myfs.s

# target to generate assembly for a file
src/mount.myfs.c.s:
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/mount.myfs.c.s
.PHONY : src/mount.myfs.c.s

src/myfs.o: src/myfs.cpp.o

.PHONY : src/myfs.o

# target to build an object file
src/myfs.cpp.o:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/myfs.cpp.o
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/myfs.cpp.o
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/myfs.cpp.o
.PHONY : src/myfs.cpp.o

src/myfs.i: src/myfs.cpp.i

.PHONY : src/myfs.i

# target to preprocess a source file
src/myfs.cpp.i:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/myfs.cpp.i
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/myfs.cpp.i
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/myfs.cpp.i
.PHONY : src/myfs.cpp.i

src/myfs.s: src/myfs.cpp.s

.PHONY : src/myfs.s

# target to generate assembly for a file
src/myfs.cpp.s:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/myfs.cpp.s
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/myfs.cpp.s
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/myfs.cpp.s
.PHONY : src/myfs.cpp.s

src/myinmemoryfs.o: src/myinmemoryfs.cpp.o

.PHONY : src/myinmemoryfs.o

# target to build an object file
src/myinmemoryfs.cpp.o:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/myinmemoryfs.cpp.o
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/myinmemoryfs.cpp.o
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/myinmemoryfs.cpp.o
.PHONY : src/myinmemoryfs.cpp.o

src/myinmemoryfs.i: src/myinmemoryfs.cpp.i

.PHONY : src/myinmemoryfs.i

# target to preprocess a source file
src/myinmemoryfs.cpp.i:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/myinmemoryfs.cpp.i
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/myinmemoryfs.cpp.i
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/myinmemoryfs.cpp.i
.PHONY : src/myinmemoryfs.cpp.i

src/myinmemoryfs.s: src/myinmemoryfs.cpp.s

.PHONY : src/myinmemoryfs.s

# target to generate assembly for a file
src/myinmemoryfs.cpp.s:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/myinmemoryfs.cpp.s
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/myinmemoryfs.cpp.s
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/myinmemoryfs.cpp.s
.PHONY : src/myinmemoryfs.cpp.s

src/myondiskfs.o: src/myondiskfs.cpp.o

.PHONY : src/myondiskfs.o

# target to build an object file
src/myondiskfs.cpp.o:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/myondiskfs.cpp.o
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/myondiskfs.cpp.o
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/myondiskfs.cpp.o
.PHONY : src/myondiskfs.cpp.o

src/myondiskfs.i: src/myondiskfs.cpp.i

.PHONY : src/myondiskfs.i

# target to preprocess a source file
src/myondiskfs.cpp.i:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/myondiskfs.cpp.i
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/myondiskfs.cpp.i
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/myondiskfs.cpp.i
.PHONY : src/myondiskfs.cpp.i

src/myondiskfs.s: src/myondiskfs.cpp.s

.PHONY : src/myondiskfs.s

# target to generate assembly for a file
src/myondiskfs.cpp.s:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/src/myondiskfs.cpp.s
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/src/myondiskfs.cpp.s
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/myondiskfs.cpp.s
.PHONY : src/myondiskfs.cpp.s

src/wrap.o: src/wrap.cpp.o

.PHONY : src/wrap.o

# target to build an object file
src/wrap.cpp.o:
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/wrap.cpp.o
.PHONY : src/wrap.cpp.o

src/wrap.i: src/wrap.cpp.i

.PHONY : src/wrap.i

# target to preprocess a source file
src/wrap.cpp.i:
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/wrap.cpp.i
.PHONY : src/wrap.cpp.i

src/wrap.s: src/wrap.cpp.s

.PHONY : src/wrap.s

# target to generate assembly for a file
src/wrap.cpp.s:
	$(MAKE) -f CMakeFiles/mount.myfs.dir/build.make CMakeFiles/mount.myfs.dir/src/wrap.cpp.s
.PHONY : src/wrap.cpp.s

testing/itest.o: testing/itest.cpp.o

.PHONY : testing/itest.o

# target to build an object file
testing/itest.cpp.o:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/testing/itest.cpp.o
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/testing/itest.cpp.o
.PHONY : testing/itest.cpp.o

testing/itest.i: testing/itest.cpp.i

.PHONY : testing/itest.i

# target to preprocess a source file
testing/itest.cpp.i:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/testing/itest.cpp.i
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/testing/itest.cpp.i
.PHONY : testing/itest.cpp.i

testing/itest.s: testing/itest.cpp.s

.PHONY : testing/itest.s

# target to generate assembly for a file
testing/itest.cpp.s:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/testing/itest.cpp.s
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/testing/itest.cpp.s
.PHONY : testing/itest.cpp.s

testing/main.o: testing/main.cpp.o

.PHONY : testing/main.o

# target to build an object file
testing/main.cpp.o:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/testing/main.cpp.o
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/testing/main.cpp.o
.PHONY : testing/main.cpp.o

testing/main.i: testing/main.cpp.i

.PHONY : testing/main.i

# target to preprocess a source file
testing/main.cpp.i:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/testing/main.cpp.i
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/testing/main.cpp.i
.PHONY : testing/main.cpp.i

testing/main.s: testing/main.cpp.s

.PHONY : testing/main.s

# target to generate assembly for a file
testing/main.cpp.s:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/testing/main.cpp.s
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/testing/main.cpp.s
.PHONY : testing/main.cpp.s

testing/tools.o: testing/tools.cpp.o

.PHONY : testing/tools.o

# target to build an object file
testing/tools.cpp.o:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/testing/tools.cpp.o
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/testing/tools.cpp.o
.PHONY : testing/tools.cpp.o

testing/tools.i: testing/tools.cpp.i

.PHONY : testing/tools.i

# target to preprocess a source file
testing/tools.cpp.i:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/testing/tools.cpp.i
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/testing/tools.cpp.i
.PHONY : testing/tools.cpp.i

testing/tools.s: testing/tools.cpp.s

.PHONY : testing/tools.s

# target to generate assembly for a file
testing/tools.cpp.s:
	$(MAKE) -f CMakeFiles/integrationtests.dir/build.make CMakeFiles/integrationtests.dir/testing/tools.cpp.s
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/testing/tools.cpp.s
.PHONY : testing/tools.cpp.s

testing/utest-blockdevice.o: testing/utest-blockdevice.cpp.o

.PHONY : testing/utest-blockdevice.o

# target to build an object file
testing/utest-blockdevice.cpp.o:
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/testing/utest-blockdevice.cpp.o
.PHONY : testing/utest-blockdevice.cpp.o

testing/utest-blockdevice.i: testing/utest-blockdevice.cpp.i

.PHONY : testing/utest-blockdevice.i

# target to preprocess a source file
testing/utest-blockdevice.cpp.i:
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/testing/utest-blockdevice.cpp.i
.PHONY : testing/utest-blockdevice.cpp.i

testing/utest-blockdevice.s: testing/utest-blockdevice.cpp.s

.PHONY : testing/utest-blockdevice.s

# target to generate assembly for a file
testing/utest-blockdevice.cpp.s:
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/testing/utest-blockdevice.cpp.s
.PHONY : testing/utest-blockdevice.cpp.s

testing/utest-myfs.o: testing/utest-myfs.cpp.o

.PHONY : testing/utest-myfs.o

# target to build an object file
testing/utest-myfs.cpp.o:
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/testing/utest-myfs.cpp.o
.PHONY : testing/utest-myfs.cpp.o

testing/utest-myfs.i: testing/utest-myfs.cpp.i

.PHONY : testing/utest-myfs.i

# target to preprocess a source file
testing/utest-myfs.cpp.i:
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/testing/utest-myfs.cpp.i
.PHONY : testing/utest-myfs.cpp.i

testing/utest-myfs.s: testing/utest-myfs.cpp.s

.PHONY : testing/utest-myfs.s

# target to generate assembly for a file
testing/utest-myfs.cpp.s:
	$(MAKE) -f CMakeFiles/unittests.dir/build.make CMakeFiles/unittests.dir/testing/utest-myfs.cpp.s
.PHONY : testing/utest-myfs.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... integrationtests"
	@echo "... unittests"
	@echo "... mount.myfs"
	@echo "... src/DMAP.o"
	@echo "... src/DMAP.i"
	@echo "... src/DMAP.s"
	@echo "... src/FAT.o"
	@echo "... src/FAT.i"
	@echo "... src/FAT.s"
	@echo "... src/RootDir.o"
	@echo "... src/RootDir.i"
	@echo "... src/RootDir.s"
	@echo "... src/Utils.o"
	@echo "... src/Utils.i"
	@echo "... src/Utils.s"
	@echo "... src/blockdevice.o"
	@echo "... src/blockdevice.i"
	@echo "... src/blockdevice.s"
	@echo "... src/mount.myfs.o"
	@echo "... src/mount.myfs.i"
	@echo "... src/mount.myfs.s"
	@echo "... src/myfs.o"
	@echo "... src/myfs.i"
	@echo "... src/myfs.s"
	@echo "... src/myinmemoryfs.o"
	@echo "... src/myinmemoryfs.i"
	@echo "... src/myinmemoryfs.s"
	@echo "... src/myondiskfs.o"
	@echo "... src/myondiskfs.i"
	@echo "... src/myondiskfs.s"
	@echo "... src/wrap.o"
	@echo "... src/wrap.i"
	@echo "... src/wrap.s"
	@echo "... testing/itest.o"
	@echo "... testing/itest.i"
	@echo "... testing/itest.s"
	@echo "... testing/main.o"
	@echo "... testing/main.i"
	@echo "... testing/main.s"
	@echo "... testing/tools.o"
	@echo "... testing/tools.i"
	@echo "... testing/tools.s"
	@echo "... testing/utest-blockdevice.o"
	@echo "... testing/utest-blockdevice.i"
	@echo "... testing/utest-blockdevice.s"
	@echo "... testing/utest-myfs.o"
	@echo "... testing/utest-myfs.i"
	@echo "... testing/utest-myfs.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

