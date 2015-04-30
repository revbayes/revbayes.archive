#!/bin/sh
./regenerate.sh -mac_universal true
cmake . -DCMAKE_BUILD_TYPE=release "-DCMAKE_OSX_ARCHITECTURES=x86_64"
make -j 4
