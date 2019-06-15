# Building with meson

[Meson](https://mesonbuild.com/) ([git](https://github.com/mesonbuild/meson)) is a new build system that aims to replace autotools and fills the same role as CMake.

Meson makes it easy to cross-compile to Windows, Macintosh, and ARM systems from Linux (see [Cross-Compile.md](Cross-Compile.md)).

## Install tools

Before compiling revbayes, you need to install C++ compiler.  Then you need to install meson and BOOST.

### Mac:
- First install XCode and homebrew
``` sh
brew install meson boost
```

### Debian/Ubuntu Linux
``` sh
apt-get install g++ meson libboost-dev
```

### Redhat Linux
``` sh
dnf install gcc-c++ meson boost-devel
```

### Other

You can [download ninja](https://github.com/ninja-build/ninja/releases) and install meson using python:
``` sh
python3 -m venv meson
source meson/bin/activate
pip3 install meson
```

## Build RevBayes

1. Download RevBayes from our github repository. Clone the repository using git by running the following command in the terminal:

    ``` sh
    git clone https://github.com/revbayes/revbayes.git revbayes
    ```

1. Configure and compile Revbayes:
   
    ``` sh
    ( cd revbayes/ ; git checkout development )               # Probably you want the development branch
    ( cd revbayes/projects/meson ; ./generate.sh )
    meson build revbayes --prefix=$HOME/Applications/revbayes
    ninja -C build install
    ```

    This creates a `build` directory where the build will take place, and says that the executables will eventually be installed in `$HOME/Applications/revbayes`.

    If there are errors in the configure step, you can look in `build/meson-logs/meson-log.txt` error log messages to help diagnose the problem.

1. For the MPI version, add `-Dmpi=true` to the `meson` command.

    ``` sh
    meson build revbayes -Dmpi=true --prefix=$HOME/Applications/revbayes
    ```

## Configuration

Setting such as `-Dmpi=true` can be changed after the build directory is created by running the following in the `build` directory:
``` sh
meson configure -Doption=value
```
You can examine current options by running
``` sh
meson configure
```

For example, to change the install prefix to `/usr/local`,
```
cd build
meson configure -Dprefix=/usr/local
```

### Option: MPI

To build with MPI, set the `mpi` option:
```
meson build-mpi -Dmpi=true -Dprefix=$HOME/Applications/revbayes-mpi
ninja -C build-mpi install
```

### Option: Jupyter Kernel

To build the jupyter kernel, set the `jupyter` option:
```
meson build-jupyter -Djupyter=true -Dprefix=$HOME/Applications/revbayes-jupyter
ninja -C build-jupyter install
```

### Option: RevStudio

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

