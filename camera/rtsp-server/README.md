# Setup SDK with Gstreamer
Set up new SDK and environment. I simply deleted my old one (`rm -rf ~/eece4534sdk`)

Install the new SDK by running:

`SDK_INST=esl-glibc-x86_64-esl-eece4534-gstreamer-image-cortexa9t2hf-neon-zedboard-esl-v2-toolchain-2022.1.sh`

`wget https://www1.coe.neu.edu/~esl/EECE4534/$SDK_INST`

`bash ./$SDK_INST -d ~/eece4534sdk`

For qemu, use the same image as before:

`cd ~/eece4534sdk/sysroots/cortexa9t2hf-neon-esl-linux-gnueabi/`

`wget https://www1.coe.neu.edu/~esl/EECE4534/esl-eece4534-image-zedboard-esl.wic`

# Compile test launch program

With SDK sourced, compile code using gstreamer, for example

`${CC} test-launch.c -o test-launch $(pkg-config --cflags --libs gstreamer-1.0 gstreamer-rtsp-server-1.0)`

If you want to compile for x86, make sure you don't have the SDK sourced and simply use gcc instead of `${CC}`. See gstreamer
installation instructions and make sure you also install `gstreamer-rtsp-server-1.0` installed.

# Running test launch on zedboard

Install gstreamer libraries on zedboard before running. On zedboard or in qemu (must have internet connection):
`dnf install gstreamer1.0-dev libgstrtsp-1.0 gstreamer1.0-rtsp-server gstreamer1.0-plugins-good gstreamer1.0-plugins-base gstreamer1.0-plugins-ugly-x264`

If above command does not work, you must edit `/etc/yum.repos.d/oe-remote-repo-~esl-EECE4534-rpm.repo`
to remove the tilde (~) characters, so that the file looks like
```
[oe-remote-repo-esl-EECE4534-rpm]
name=OE Remote Repo: esl EECE4534 rpm
baseurl=http://www1.coe.neu.edu/~esl/EECE4534/rpm
gpgcheck=0
```

Run test launch:

`./test-launch "( videotestsrc ! x264enc ! rtph264pay name=pay0 pt=96 )"`

Currently this runs on the zedboard but a client is not able to connect, but this seems to be a network issue.

If compiled and run on an x86 machine, the server works and clients are able to connect (tested with VLC and ffplay)

