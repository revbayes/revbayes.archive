#
# @author Sebastian Hoehna
# @author Marc A. Suchard
#

CMake preparation:
$ ./regenerate.sh

CMake build instructions for a development build:
$ cmake . -DCMAKE_BUILD_TYPE=debugfull
$ make -j 4
$ ./rb

When making a release, use instead this cmake flag:
-DCMAKE_BUILD_TYPE=release
