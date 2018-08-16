# Building with meson

# Install

On Mac:
``` sh
brew install meson
```

On Debian/Ubuntu Linux
``` sh
apt-get install meson
```

```
# First install ninja
python3 -m venv meson
source meson/bin/activate
pip3 install meson
```

# Build

```
cd projects/meson
./generate.sh
cd ../..
meson build -Dprefix=$HOME/Applications/revbayes
ninja -C meson
```

# Cross-build

Edit the `root` directory in `projects/meson/mingw-64bit-cross.txt`
be a directory that will be treated as the Windows root.

```
apt-get install g++-mingw-w64
apt-get install wine64
meson build --cross-file=projects/meson/mingw-64bit-cross.txt
```
