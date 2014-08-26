#!/bin/sh
./regenerate.sh -win true
cmake . -DCMAKE_BUILD_TYPE=release
make -j 4