#!/bin/sh
./regenerate.sh $@
cmake . -DCMAKE_BUILD_TYPE=release
make -j 4
