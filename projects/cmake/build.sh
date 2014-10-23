#!/bin/sh
./regenerate.sh $@
cmake . -DCMAKE_BUILD_TYPE=debugfull
make -j 2
