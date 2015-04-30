#!/bin/sh
./regenerate.sh -mavericks true
cmake . -DCMAKE_BUILD_TYPE=release
make -j 4
