Building RevBayes with cmake
############################

You build RevBayes by running a build script appropriate for your
operating system:

Script              Operating system
==================  ==============================================
build.sh            Unix, Linux and Mac OS X previous to Mavericks
build-mavericks.sh  Mac OS X Mavericks
build-win.sh        Windows

Example for a Linux system:
$ ./build.sh

The build script produces 'rb', which is the basic console executable.
The 'rb-extended' binary includes advanced console features, like
command history and tab completion.

More Information
================
The build script invokes the regenerate script to build the boost
libraries and preparing things for cmake. Then it runs cmake, and
finally it uses make to compile a set of executables.

You may want to modify some of the flags in the build script:

For a development build, you might want to add the cmake flag
$ cmake . -DCMAKE_BUILD_TYPE=debugfull

When making a release, use this cmake flag instead:
-DCMAKE_BUILD_TYPE=release

To avoid using the parallel version of make, you might want to
delete the -j 4 flag:
$ make -j 4


