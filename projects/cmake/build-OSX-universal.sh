#!/bin/sh
./regenerate.sh -mavericks true
cmake . -DCMAKE_BUILD_TYPE=release "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386"
make 
