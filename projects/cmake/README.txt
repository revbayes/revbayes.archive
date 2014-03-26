#
# @author Sebastian Hoehna
# @author Marc A. Suchard
#

CMake preparation:
$ ./regenerate.sh

CMake build instructions for a development build:
$ cmake . -DCMAKE_BUILD_TYPE=debugfull
$ make
$ ./rb

When making a release, use the cmake flag:
-DCMAKE_BUILD_TYPE=release
