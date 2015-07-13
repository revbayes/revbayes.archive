#!/bin/sh
./regenerate.sh -win true
cmake . -G "Unix Makefiles" -DCMAKE_C_COMPILER=GCC -DCMAKE_CXX_COMPILER=G++ -DCMAKE_BUILD_TYPE=release
make -j 4