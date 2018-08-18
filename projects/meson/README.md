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
# Create a directory `build` where the build will take place
meson build -Dprefix=$HOME/Applications/revbayes
# Run `ninja` in the `build` directory
ninja -C build
```

# Cross-build

## Linux to windows

To do a cross-build from linux to windows, we need to
* download windows libraries
* install the cross-compiler
* create a meson cross-file
* run meson

1. Let's put our windows libraries in a directory called `~/win_root`.  The libraries will end up in `~/win_root/mingw64/lib` with header files in `~/win_root/mingw64/include`.

   ``` sh
   mkdir ~/win_root
   cd ~/win_root

   for PKG in boost-1.67.0-2 \
              pango-1.42.3-1 \
              glib2-2.56.1-3 \
              cairo-1.15.8-1 \
              gdk-pixbuf2-2.36.9-1 \
              atk-2.28.1-1 \
              gtk2-2.24.32-1 \
              pcre-8.42-1 \
              fribidi-1.0.5-1 \
              pixman-0.34.0-3 \
              fontconfig-2.13.0-1 \
              freetype-2.9.1-1 \
              zlib-1.2.8-9 \
              bzip2-1.0.6-6 \
              libpng-1.6.35-1 \
              harfbuzz-1.8.5-1 \
              graphite2-1.3.9-1 \
              expat-2.2.5-1 \
              ; do
      wget http://repo.msys2.org/mingw/x86_64/mingw-w64-x86_64-${PKG}-any.pkg.tar.xz
      tar -Jxf mingw-w64-x86_64-${PKG}-any.pkg.tar.xz
   done
   # Tell pkg-config where to look for `*.pc` files.
   export PKG_CONFIG_PATH=$HOME//win_root/mingw64/lib/pkgconfig 
   # Check that GTK has been installed successfully
   pkg-config --libs gtk+-2.0
   ```

   If you just want to install BOOST, only the first package above is actually necessary.  All the remaining packages are dependencies of GTK.

2. We also need to have a cross-compiler installed.  The convention these days is that cross-compilers live in `/usr/bin/` and are prefixed with the description of the target architecture.  For example, `/usr/bin/x86_64-w64-mingw32-g++` is the compiler that targets 64-bit windows.

   ```
   apt-get install g++-mingw-w64
   apt-get install wine64
   ```

   I think on homebrew, you can do:
   ```
   brew install mingw
   brew install wine
   ```
   However, I have not tried this.

3. Look at the cross file `revbayes/projects/meson/mingw-64bit-cross.txt`. Change the string `/path/to/win_root/` to refer to `$HOME/win_root`.


4. Before running meson, we need to create `revbayes/src/meson.build`, which just has a long list of all the include directories and `*.cpp` files.
   ```
   cd revbayes/projects/meson
   ./generate.sh
   ```

   Then we can finally run meson.
   ```
   cd revbayes
   meson cross-build --cross-file=projects/meson/mingw-64bit-cross.txt -Dcmd-gtk=true
   ninja -C cross-build
   ```
