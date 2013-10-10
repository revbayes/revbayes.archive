#!/bin/sh 
 # Shell script to find out all the files under a directory and 
 #its subdirectories. This also takes into consideration those files
 #or directories which have spaces or newlines in their names 

./createCMakeSingle.sh "../src/core" "core" > "../projects/cmake/core/CMakeLists.txt"
./createCMakeSingle.sh "../src/revlanguage" "revlanguage" > "../projects/cmake/revlanguage/CMakeLists.txt"