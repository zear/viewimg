A simple SDL image viewer for GCW Zero.

Installation:
gcc -O2 viewimg.c -lSDL -lSDL_image -I/usr/include/SDL -o viewimg
strip viewimg

/opt/gcw0-toolchain/usr/bin/mipsel-linux-gcc -O2 viewimg.c -I/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/include/SDL/ -lSDL -lSDL_image -o viewimg
/opt/gcw0-toolchain/usr/bin/mipsel-linux-strip viewimg

Usage:
viewimg image.png

License:
Public Domain


