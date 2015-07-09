#!/bin/sh
./regenerate.sh -win true
cmake . -G "MSYS Makefiles" -DCMAKE_C_COMPILER=GCC -DCMAKE_COMPILER=G++ -DCMAKE_BUILD_TYPE=release
make -j 4