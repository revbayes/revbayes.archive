#!/bin/sh
./regenerate.sh -win true
cmake .
make -j 4