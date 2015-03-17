#!/bin/sh
./regenerate.sh -mavericks true -mpi true -boost false
cmake . -DCMAKE_BUILD_TYPE=release
make 
