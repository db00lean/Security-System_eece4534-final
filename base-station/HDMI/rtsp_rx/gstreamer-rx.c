#include <stdio.h>

#include <gst/app/gstappsink.h>
#include <gst/video/video.h>
#include <gst/gst.h>

#include "imagelib.h"
#include "gstreamer-rx.h"

static const char* const ENC_LOOKUP[] = {
  [IMGENC_RGB] = "RGB",
  [IMGENC_ARGB] = "ARGB",
  [IMGENC_BGR] = "BGR",
};


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
      "appsink name=sink max-buffers=120,drop=true",
      NULL);

  gst_element_set_state(cam->pipeline, GST_STATE_PLAYING);


  cam->appsink = GST_APP_SINK_CAST(gst_bin_get_by_name(GST_BIN(cam->pipeline), "sink"));

  /* Start playing */
  GstStateChangeReturn ret = gst_element_set_state(cam->pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Unable to set the pipeline to the playing state.\n");
    gst_object_unref(cam->pipeline);
    exit(1);
  }

  return cam;
}

struct image * get_frame(struct camera_rx *cam, enum img_enc enc, int width, int height) {
  GstSample *sample = gst_app_sink_pull_sample(cam->appsink);

  if (!sample) {
    printf("sample is NULL\n");
    exit(1); //TODO
  }

  GstCaps *caps = gst_caps_new_simple ("video/x-raw",
     "format", G_TYPE_STRING, ENC_LOOKUP[enc],
     "width", G_TYPE_INT, width,
     "height", G_TYPE_INT, height,
     NULL);

  // 0.5 second timeout. is ok?
  GstSample *converted_sample = gst_video_convert_sample(sample, caps, 500000000, NULL);


  if (!converted_sample) {
    printf("sample converted to NULL\n");
    exit(1); //TODO
  }

  GstBuffer *buffer = gst_sample_get_buffer(converted_sample);
  GstMapInfo map;
  gst_buffer_map(buffer, &map, GST_MAP_READ);

  struct image *img = create_image_size(enc, map.size);
  memcpy(img->buf, map.data, map.size);
  img->buf_len = map.size;
  img->width=width;
  img->height=height;
  img->enc = enc;

  // cleanup memory
  gst_buffer_unmap(buffer, &map);
  gst_sample_unref(sample);
  gst_sample_unref(converted_sample);
  gst_caps_unref(caps);

  return img;
}

void cleanup_rx_camera(struct camera_rx * cam) {
  /* Free resources */
  gst_element_set_state(cam->pipeline, GST_STATE_NULL);
  gst_object_unref(cam->pipeline);
  gst_object_unref(cam->appsink);
  free(cam);
  gst_deinit();
}
