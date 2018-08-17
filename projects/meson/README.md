# Building with meson

[Meson](https://mesonbuild.com/) ([git](https://github.com/mesonbuild/meson)) is a new build system that aims to replace autotools and compete with CMake.  It is written in Python and works on Windows, Mac, and Linux.

# Install

On Mac:
``` sh
brew install meson
```

On Debian/Ubuntu Linux
``` sh
apt-get install meson
```

On Windows or other Linux distributions:

```
# First install ninja
python3 -m venv meson
source meson/bin/activate
pip3 install meson
```

# Build

We first have to generate the list of include directories, and the list of `*.cpp` and `*.c` files.  These could easily be split up into libraries, but I haven't done that yet:

```
cd projects/meson
./generate.sh
cd ../..
```

Then we need to run meson (which is like `cmake`) and `ninja` (which is like `make`).

```
meson build -Dprefix=$HOME/Applications/revbayes
ninja -C meson
```

# Cross-build

To do a cross-build with meson, we need to supply a "cross-file" that says which compiler and assembler and such to use.

We also need to have a cross-compiler installed.  The convention these days is that cross-compilers live in `/usr/bin/` and are prefixed with the description of the target architecture.  For example, `/usr/bin/x86_64-w64-mingw32-g++` is the compiler that targets 64-bit windows.

There are some cross-files in `projects/meson`.  Before using these, you need to edit the `root` that acts like a filesystem for the target architecture.  When cross-compiler to Mac, the root contains the SDK.  When cross-compiling to Windows, the root would contain other libraries you need, such as GTK, in native windows format.

```
apt-get install g++-mingw-w64
apt-get install wine64
meson build --cross-file=projects/meson/mingw-64bit-cross.txt
```
