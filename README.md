Pinakes
=======

Nothing to see here.

Just move along.

I'll reveal more one day.

Building
--------

1. Install [Retro68](https://github.com/autc04/Retro68);
2. Clone [MacintoshLibs](https://github.com/sentient06/MacintoshLibs) at the same directory;
3. Rename `MacintoshLibs` `libs`;
4. Go into any libs used and `make`, then `make install`;
5. Compile from one level above with the `build.sh` script:

```
if [ $# -ne 1 ] ; then
    echo 'Syntax: ./build.sh [project_name]'
    exit 1
fi

if [ ! -d "$1" ]; then
    echo 'The specified folder does not exist'
    exit 1
fi

cd "$1"
rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=/Users/$(whoami)/Development/Classic/Retro68-build/toolchain/m68k-apple-macos/cmake/retro68.toolchain.cmake
make
```