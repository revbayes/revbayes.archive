#
# @author Sebastian Hoehna
# @author Marc A. Suchard
#

CMake preparation:
$ ./regenerate.sh

CMake build instructions:

$ cmake . cmake . <-DCMAKE_BUILD_TYPE=debugfull | cmake . -DCMAKE_BUILD_TYPE=release>
$ make
$ ./rb

