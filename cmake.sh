#!/bin/sh

export PATH=${HOME}/cmake-3.24.2-linux-x86_64/bin:$PATH
export CMAKE_PREFIX_PATH=${HOME}/usr/local/lib/cmake

# *****************
# *** (0) Clean ***
# *****************

rm -rf build

# ***************************
# *** (1) Create Makefile ***
# ***************************

#rm -rf build
#mkdir build
#cd build
#cmake ..
#cd ..
cmake -S . -B build

# *****************
# *** (2) Build ***
# *****************

#cd build
#make
#cd ..
cmake --build build

# *******************
# *** (3) Install ***
# *******************

cd build
make DESTDIR=${HOME} install
cd ..
