To install czmq: https://github.com/zeromq/czmq#building-on-linux-and-macos

Scroll to the section "Building on Linux and macOS"

Specifically,
```
git clone git://github.com/zeromq/libzmq.git
cd libzmq
./autogen.sh
./configure
make check
sudo make install
sudo ldconfig
cd ..

git clone git://github.com/zeromq/czmq.git
cd czmq
./autogen.sh
./configure
make check
sudo make install
sudo ldconfig
cd ..
```


