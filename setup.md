# Dependencies to Cross-Compile

## Installing extended SDK

The extended SDK includes gstreamer.

Install needed packages:
`sudo apt install build-essential gdb-multiarch git libyaml-dev`

Install extended SDK:
```
SDK_INST=esl-glibc-x86_64-esl-eece4534-gstreamer-image-cortexa9t2hf-neon-zedboard-esl-v2-toolchain-2022.1.sh

wget https://www1.coe.neu.edu/~esl/EECE4534/$SDK_INST

bash ./$SDK_INST -d ~/eece4534sdk
```

Recommend creating command alias `setup-4534` to source the environment:
`echo "alias setup-4534='source ~/eece4534sdk/environment-setup-cortexa9t2hf-neon-esl-linux-gnueabi && PS1=\"(4534) \$PS1\"'" >> ~/.bashrc`


Optionally download qemu zedboard image:
```
cd ~/eece4534sdk/sysroots/cortexa9t2hf-neon-esl-linux-gnueabi/

wget https://www1.coe.neu.edu/~esl/EECE4534/esl-eece4534-image-zedboard-esl.wic
```

## Installing czmq dependencies to be able to cross-compile

Install czmq for cross-compilation on the ZedBoards: https://github.com/zeromq/czmq#building-on-linux-and-macos

Scroll to the section "Building on Linux and macOS". We're gonna be following these directions, with some modifications. 

!!! Make sure to source the cross-compilation environment before starting! !!!

### libzmq download and install:

For `czmq`, you first need `libzmq`. `cd` into wherever you want to keep the source code (ex. your home directory). 

- `git clone https://github.com/zeromq/libzmq.git`
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

## Install opencv for cross-compilation

TODO

## HDMI dependencies

TODO

# Run-time dependencies

## Camera Module

### Gstreamer

`dnf install gstreamer1.0-dev libgstrtsp-1.0 gstreamer1.0-rtsp-server gstreamer1.0-plugins-good gstreamer1.0-plugins-base gstreamer1.0-plugins-ugly-x264`

### OpenCV

TODO

## Base Station

TODO

