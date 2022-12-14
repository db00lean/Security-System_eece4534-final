// Connor Northway
#ifndef GSTREAMER_RX_H
#define GSTREAMER_RX_H

#include <gst/app/gstappsink.h>
#include <gst/gst.h>

#include "imagelib.h"

#define GST_FRAME_PULL_TIMEOUT_NS 1000000

struct camera_rx {
  GstElement *pipeline;
  GstAppSink *appsink;
};

struct image * get_frame(struct camera_rx* cam, enum img_enc enc, int width, int height);

void free_rx_camera(struct camera_rx* cam);

struct camera_rx * init_rx_camera(char* uri);

void cleanup_rx_camera(struct camera_rx * cam);

int pause_stream(struct camera_rx * cam);

int play_stream(struct camera_rx * cam);

#endif
