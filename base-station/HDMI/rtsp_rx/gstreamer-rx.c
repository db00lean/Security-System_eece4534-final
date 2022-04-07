#include <stdio.h>

#include <gst/app/gstappsink.h>
#include <gst/gst.h>

#include "imagelib.h"
#include "gstreamer-rx.h"

struct camera_rx * init_rx_camera(char* uri) {

  struct camera_rx* cam = malloc(sizeof(struct camera_rx));
  // create structure
  if (cam == NULL) {
    exit(0); //TODO
  }

  /* Initialize GStreamer */
  // TODO do we need to do this every time?
  gst_init(NULL, NULL);

  // TODO handle ip address, setup rtsp stream

  // nice
  // if we want RGB, just change the format!
  // rtsp verison:
  // pipeline = gst_parse_launch(
  //     "rtspsrc location=\"rtsp://rtsp.stream/pattern\" ! rtph264depay ! "
  //     "h264parse ! decodebin ! videoconvert ! "
  //     "video/x-raw,format=ARGB,width=320,height=240 ! appsink name=sink",
  //     NULL);
  // alternative in case you don't have network or dave decides he's had enough:
  // (or maybe you just want faster development)
  cam->pipeline = gst_parse_launch(
      "videotestsrc ! videoconvert ! "
      "video/x-raw,format=ARGB,width=320,height=240 ! "
      "queue leaky=downstream max-size-time=250000000 ! " //.25 second queue
      "appsink name=sink",
      NULL);

  gst_element_set_state(cam->pipeline, GST_STATE_PLAYING);


  GstElement *sink = gst_bin_get_by_name(GST_BIN(cam->pipeline), "sink");
  if (!sink) {
    printf("sink is NULL\n");
    exit(1);
  }

  cam->appsink = GST_APP_SINK(sink);
  if (!cam->appsink) {
    printf("appsink is NULL\n");
    exit(1);
  }

  /* Start playing */
  GstStateChangeReturn ret = gst_element_set_state(cam->pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Unable to set the pipeline to the playing state.\n");
    gst_object_unref(cam->pipeline);
    exit(1);
  }

  return cam;
}

struct image * get_frame(struct camera_rx *cam, enum img_enc enc) {
  GstSample *sample = gst_app_sink_pull_sample(cam->appsink);
  if (!sample) {
    printf("sample is NULL\n");
    exit(1); //TODO
  }
  GstBuffer *buffer = gst_sample_get_buffer(sample);
  GstMapInfo map;
  gst_buffer_map(buffer, &map, GST_MAP_READ);

  struct image *img = create_image_size(IMGENC_ARGB, map.size);
  memcpy(img->buf, map.data, map.size);
  return img;
}

void cleanup_rx_camera(struct camera_rx * cam) {
  /* Free resources */
  gst_element_set_state(cam->pipeline, GST_STATE_NULL);
  gst_object_unref(cam->pipeline);
}
