# Building with meson

[Meson](https://mesonbuild.com/) ([git](https://github.com/mesonbuild/meson)) is a new build system that aims to replace autotools and compete with CMake.  It is written in Python and works on Windows, Mac, and Linux.

See the conference talk [Making build systems not suck](https://www.youtube.com/watch?v=KPi0AuVpxLI)

# Install

On Mac:
``` sh
brew install meson
```

On Debian/Ubuntu Linux
``` sh
apt-get install meson
```

On Windows, first, install ninja.  For example, you could [download ninja](https://github.com/ninja-build/ninja/releases) and modify your PATH variable so the windows `cmd` shell can find it.  Second,
```
python3 -m venv meson
source meson/bin/activate
pip3 install meson
```

## Install BOOST

This `README.md` assumes that you've already installed BOOST, and that it is installed as a system library.
On mac:
```
brew install boost
```
On Debian/Ubuntu Linux:
```
apt-get install libboost-dev
```
I think on RedHat-based Linux, you can do:
```
dnf install boost-devel
```
On Windows, you can [download boost](https://www.boost.org/users/download/)

# Build

## Preparing to run meson
Before building with meson, we first need to generate the files `src/meson.build`, `src/libs/meson.build`,
`src/revlanguage/meson.build`, `src/core/meson.build`, and `src/cmd/meson.build`.
```
cd revbayes/projects/meson
./generate.sh
```
The script `generate.sh`
* scans the directories for `*.cpp` and `*.c` files to find the source files in each directory.
* scans the directories for `*.h` files to find the directories that need to be in the include path.

## Configuring

To configure the build, we run the following command from the `revbayes` directory:
```
meson build -Dprefix=$HOME/Applications/revbayes
```
This creates a `revbayes/build` directory where the build will take place, and says that the executables will eventually be installed in `$HOME/Applications/revbayes`.  If you want to install someplace else, you can use a different "prefix".

If there are errors in the configure step, you can look in `build/meson-logs/meson-log.txt` for log messages to help diagnose the problem.

## Perform the build
Next, run `ninja` while targetting the `build` directory
```
ninja -C build
ninja -C build install
```
You won't need to re-run meson if your `meson.build` files have changed, since  `ninja` will do that automatically.

## Altering the configuration

If you want to change configuration options after you have run `meson`, you can run `meson configure` in the `build` directory to view the current settings.  Then you can alter the settings by running `meson configure -Doption=value`.  For example, to change the install prefix to `/usr/local`,
```
cd build
meson configure -Dprefix=/usr/local
```

## Option: MPI

To build with MPI, set the `mpi` option:
```
meson build-mpi -Dmpi=true -Dprefix=$HOME/Applications/revbayes-mpi
ninja -C build-mpi install
```

## Option: Jupyter Kernel

To build the jupyter kernel, set the `jupyter` option:
```
meson build-jupyter -Djupyter=true -Dprefix=$HOME/Applications/revbayes-jupyter
ninja -C build-jupyter install
```

## Option: RevStudio

To build the `RevStudio` GUI in addition to `rb`, you need to enable the `studio` flag when running meson:
```
meson build-gtk -Dstudio=true
ninja -C build-gtk install
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
* tell pkg-config where to find them
* install the cross-compiler
* create a meson cross-file
* run meson

1. Let's put our windows libraries in a directory called `~/win_root`.  The libraries will end up in `~/win_root/mingw64/lib` with header files in `~/win_root/mingw64/include`.

   ``` sh
   mkdir ~/win_root
   cd ~/win_root

   for PKG in boost-1.67.0-2 \
              pango-1.42.4-2 \
              glib2-2.58.1-1 \
              cairo-1.16.0-1 \
              gdk-pixbuf2-2.38.0-2 \
              atk-2.30.0-1 \
              gtk2-2.24.32-3 \
              pcre-8.42-1 \
              fribidi-1.0.5-1 \
              pixman-0.36.0-1 \
              fontconfig-2.13.1-1 \
              freetype-2.9.1-1 \
              zlib-1.2.11-5 \
              bzip2-1.0.6-6 \
              libpng-1.6.36-1 \
              harfbuzz-2.2.0-1 \
              graphite2-1.3.12-1 \
              expat-2.2.6-1 \
              gettext-0.19.8.1-7 \
              libtiff-4.0.9-2 \
              jasper-2.0.14-1 \
              libjpeg-turbo-2.0.1-1 \
              xz-5.2.4-1 \
              libiconv-1.15-3 \
              libffi-3.2.1-3 \
              ; do
      wget http://repo.msys2.org/mingw/x86_64/mingw-w64-x86_64-${PKG}-any.pkg.tar.xz
      tar -Jxf mingw-w64-x86_64-${PKG}-any.pkg.tar.xz
   done
   ```

   If you just want to install BOOST, only the first package above is actually necessary.  All the remaining packages are dependencies of GTK.

2. We also need to tell pkg-config that our libraries live in a different place than they would if this was really windows.

   ```
   # Tell pkg-config where to look for `*.pc` files.
   export PKG_CONFIG_PATH=$HOME/win_root/mingw64/lib/pkgconfig
   # Tell pkg-config to prefix -I/-L paths with this
   export PKG_CONFIG_SYSROOT_DIR=$HOME/win_root
   # Check that GTK has been installed successfully
   pkg-config --cflags --libs gtk+-2.0
   ```

   If pkg-config cannot find `gtk+-2.0`, then the configuring with meson won't succeed.  If don't tell it about the sysroot, running meson will succeed, but the compiler won't be able to find GTK2.

3. We also need to have a cross-compiler installed.  The convention these days is that cross-compilers live in `/usr/bin/` and are prefixed with the description of the target architecture.  For example, `/usr/bin/x86_64-w64-mingw32-g++` is the compiler that targets 64-bit windows.

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

4. Look at the cross file `revbayes/projects/meson/mingw-64bit-cross.txt`. Change the string `/path/to/win_root/` to refer to `$HOME/win_root`.


5. Now we can finally run meson:

   ```
   cd revbayes
   ( cd projects/meson ; ./generate.sh )
   meson cross-build --cross-file=projects/meson/mingw-64bit-cross.txt -Dstudio=true -Dstatic=false
   ninja -C cross-build
   ```

   It seems that statically linking the gtk libraries doesn't work, at least
   when using the libraries from MSYS2.