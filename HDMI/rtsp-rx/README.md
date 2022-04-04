# gstreamer frame receiver

## image library
Trying to standardize on raw byte-based images. (Like PPM)

See imagelib.h for functions.

Ability to allocate and free 'images'. You get a pointer to the
'image' struct. Do whatever you want with it, but the important
part is that the raw image data is in the `buf` member.

Images can be encoded in different formats, the type of which is in the
`enc` member.

See
[http://davis.lbl.gov/Manuals/NETPBM/doc/ppm.html](http://davis.lbl.gov/Manuals/NETPBM/doc/ppm.html)
for notes on the PPM format, which uses a 1-byte RGB layout.


### test program:
Allocates and creates an image (RGB), writes a gradient to it, and writes the
image data to a file for viewing. Open it with your favorite image viewer, but
it's just a gradient.

## GStreamer

gstreamer testing program provided in repository `gstreamer-rx.c`.
the things I've been playing with are mostly commented.

it sets up the pipeline, grabs one frame using the `appsink` element, and
copies it to a data pointer. the program currently exits immediately. I used
`gdb` to debug it and ensure the data arrived in the pointer.

### other notes about gstreamer:
easy debugging with `GSTREAMER_DEBUG=1` env variable

### gstreamer commandline examples:
These aren't necessary anymore, but keeping them around because it's decent
examples of creating pipelines
Use `identity eos-after=2` to only save one frame of the stream. Without this
gstreamer will continue writing 'ppm frames' to the end of the file!

`gst-launch-1.0 videotestsrc pattern=0 ! videoconvert ! avenc_ppm ! identity
eos-after=2 filesink location=pattern.ppm`

`gst-launch-1.0 rtspsrc location=rtsp://rtsp.stream/pattern protocols=tcp !
rtph264depay ! h264parse ! decodebin ! videoconvert ! avenc_ppm ! identity
eos-after=2 ! filesink location=pattern.ppm`

ffmpeg too, easy RX with `ffplay rtsp://rtsp.stream/pattern`, at least on linux


#### Acknowledgements
thank you dave@rtsp.stream !
