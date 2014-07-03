#!/bin/sh
./regenerate.sh -mavericks true
cmake .
make -j 4