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
