#!/bin/bash -eu
# Supply build instructions
# Use the following environment variables to build the code
# $CXX:               c++ compiler
# $CC:                c compiler
# CFLAGS:             compiler flags for C files
# CXXFLAGS:           compiler flags for CPP files
# LIB_FUZZING_ENGINE: linker flag for fuzzing harnesses
mkdir build
cd build
cmake ../
make

# Copy all fuzzer executables to $OUT/
$CC $CFLAGS $LIB_FUZZING_ENGINE \
  $SRC/lwgps/.clusterfuzzlite/process_fuzzer.c \
  -o $OUT/process_fuzzer \
  $PWD/CMakeFiles/LwLibPROJECT.dir/lwgps/src/lwgps/lwgps.c.o \
  -I$SRC/lwgps/lwgps/src/include \
  -I$SRC/lwgps/dev
