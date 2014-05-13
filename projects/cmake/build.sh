#!/bin/sh
./regenerate
cmake .
make -j 4