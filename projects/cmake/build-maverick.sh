#!/bin/sh
./regenerate.sh -maverick true
cmake .
make -j 4