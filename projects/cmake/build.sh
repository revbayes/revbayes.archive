#!/bin/sh
./regenerate.sh
cmake .
make -j 4
