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

## Preparing to run meson
Before building with meson, we first need to generate the files `src/meson.build`, `src/libs/meson.build`,
`src/revlanguage/meson.build`, `src/core/meson.build`, and `src/cmd/meson.build`.
```
cd projects/meson
./generate.sh
```
The script `generate.sh`
* scans the directories for `*.cpp` and `*.c` to find the source files in each directory.
* scans the directories for `*.h` files to find the directories that need to be in the include path.

## Configuring

To configure the build, we run:
```
cd revbayes
meson build -Dprefix=$HOME/Applications/revbayes
```
This creates a `revbayes/build` where the build will take place.  If you want to change configuration options, you can run `meson configure` in the `build` directory.  For to change the install prefix to `/usr/local`,
```
cd build
meson configure -Dprefix=/usr/local
```
If there are errors in the configure step, you can look in `build-gtk/meson-logs/meson-log.txt` for log messages to help diagnose the problem.

## Perform the build
You won't need to re-run meson, since `ninja` will do that automatically if any changes are detected.
```
# Run `ninja` in the `build` directory
ninja -C build
ninja -C build install
```

## RevStudio

To build `RevStudio` in addition to `rb`, you need to enable the `studio` flag when running meson:
```
meson build-gtk -Dstudio=true
```
However, if GTK2 is not installed or not usable, this will produce an error message.
Meson uses `pkg-config` to find most dependencies, and to find out what compiler and linker flags they need.
If meson cannot find GTK2, you can:
* run `pkg-config gtk+-2.0 --cflags` to see if pkg-config can find GTK2.
* look in `build-gtk/meson-logs/meson-log.txt` for log messages to help diagnose the problem.

### Installing GTK2
To install GTK2 on Debian & Ubuntu:
```
apt-get install libgtk+-2.0-dev
```

To install GTK2 on Mac:
```
brew install gtk2
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
              gettext-0.19.4-4 \
              ; do
      wget http://repo.msys2.org/mingw/x86_64/mingw-w64-x86_64-${PKG}-any.pkg.tar.xz
      tar -Jxf mingw-w64-x86_64-${PKG}-any.pkg.tar.xz
   done
   # Tell pkg-config where to look for `*.pc` files.
   export PKG_CONFIG_PATH=$HOME/win_root/mingw64/lib/pkgconfig
   # Check that GTK has been installed successfully
   pkg-config --cflags --libs gtk+-2.0
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

   Let's check that we have defined `PKG_CONFIG_PATH` so that the windows GTK2 libraries are available:
   ```
   echo $PKG_CONFIG_PATH
   pkg-config --cflags --libs gtk+-2.0
   ```
   If pkg-config can't find GTK2, then meson will give an error message.

   Then we can finally run meson.
   ```
   cd revbayes
   meson cross-build --cross-file=projects/meson/mingw-64bit-cross.txt -Dstudio=true
   ninja -C cross-build
   ```
