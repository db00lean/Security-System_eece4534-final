# gstreamer frame receiver

### image library
Trying to standardize on raw byte-based images. (Like PPM)

See imagelib.h for functions.

Ability to allocate and free 'images'. You get a pointer to the
'image' struct. Do whatever you want with it, but the important
part is that the raw image data is in the `buf` member.

This can be accessed by any code that wants to read the image.

Data is formatted as a series of pixels, `R, G, B` one byte each.
They are arranged from top left to bottom right, in rows.
See
[http://davis.lbl.gov/Manuals/NETPBM/doc/ppm.html](http://davis.lbl.gov/Manuals/NETPBM/doc/ppm.html).


### test program:
Allocates and creates an image, writes a gradient to it, and
writes the image data to a file for viewing.

Open it with your favorite image viewer, but it's just a gradient.

### gstreamer:
Unimplemented. Libraries link, but no gstreamer calls yet.
likely difficult, very powerful

commandline incantation that works:
`gst-launch-1.0 videotestsrc pattern=0 ! videoconvert ! avenc_ppm ! filesink location=pattern.ppm`

let's make it work for rtsp::
....getting there....
`gst-launch-1.0 rtspsrc location=rtsp://rtsp.stream/pattern protocols=tcp ! rtph264depay ! h264parse ! decodebin ! videoconvert ! avenc_ppm ! filesink location=pattern.ppm`

note: both of these will keep appending new frames to the end of the 'ppm' file.
be careful, you may end up with 500M of ""ppm image"" if you let it keep going!

use `identity error-after=2` to bail after the first frame.
`gst-launch-1.0 rtspsrc location=rtsp://rtsp.stream/pattern protocols=tcp !
rtph264depay ! h264parse ! decodebin ! videoconvert ! avenc_ppm ! identity
error-after=2 ! filesink location=pattern.ppm`

### ffmpeg:
may be easier. has a C library / api too but check this out:
`ffmpeg -y -rtsp_transport tcp -i rtsp://rtsp.stream/pattern -frames:v 1 out.ppm`
proof that we can do what we want with it, as long as I can understand the API
can even do rescaling stuff if we really need using ffmpeg

thank you dave@rtsp.stream !

(though, this has a REALLY slow startup time. would assume it's faster if
you're persistently connected and just grabbing new frames)

easy RX with `ffplay rtsp://rtsp.stream/pattern`, at least on linux
