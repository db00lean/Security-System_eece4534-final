# Download and install instructions for czmq

To install czmq for cross-compilation on the ZedBoards: https://github.com/zeromq/czmq#building-on-linux-and-macos

Scroll to the section "Building on Linux and macOS". We're gonna be following these directions, with some modifications. 

!!! Make sure to source the cross-compilation environment before starting! !!!

### libzmq download and install:

For `czmq`, you first need `libzmq`. `cd` into wherever you want to keep the source code (ex. your home directory). 

- `git clone git@github.com:zeromq/libzmq.git`
- `cd` into the repo
- `./autogen.sh`
  - Don't worry about any warnings that this might throw at you
- `./configure --target=arm-esl-linux-gnueabi --host=arm-esl-linux-gnueabi --build=x86_64-linux --prefix=$SDKTARGETSYSROOT/usr`
  - This tells `libzmq` that you wish to cross-compile, and lets it know that you want `libzmq` to be installed in the given `--prefix` option. 
  - I think the `--target` and `--build` options are unnecessary, but have not tested the build process without them. 
- `make`
- `make install`
  - I'm not sure the difference between `make` and `make install` here, but you need to run both. 
  - I think `make` physically builds the dependencies, and `make install` installs the top-level headers and libraries where the cross-compiler can find them. 

### czmq download and install

- `git clone git@github.com:zeromq/czmq.git`
- `cd` into the repo
- `./autogen.sh`
  - Again, don't worry about any warnings
- `./configure --target=arm-esl-linux-gnueabi --host=arm-esl-linux-gnueabi --build=x86_64-linux --prefix=$SDKTARGETSYSROOT/usr --with-libzmq=$SDKTARGETSYSROOT/usr`
  - Make sure to include the `--with-libzmq` option here! Otherwise, `czmq` will not be able to find the base `libzmq` files to build off of.
- `make`
- `make install`

Once you reach this point, you should be all set to start cross-compiling with czmq.

These steps have only been tested on WSL. Unsure about Mac/Linux people.
