// Connor Northway
#include <stdio.h>

#include <gst/app/gstappsink.h>
#include <gst/video/video.h>
#include <gst/gst.h>

#include "../inc/imagelib.h"
#include "../inc/gstreamer-rx.h"

static const char* const ENC_LOOKUP[] = {
  [IMGENC_RGB] = "RGB",
  [IMGENC_ARGB] = "ARGB",
  [IMGENC_BGR] = "BGR",
  [IMGENC_BGRA] = "BGRA",
};


struct camera_rx * init_rx_camera(char* ip) {

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
  // rtsp with online source:
  // pipeline = gst_parse_launch(
  //     "rtspsrc location=\"rtsp://rtsp.stream/pattern\" ! rtph264depay ! "
  //     "h264parse ! decodebin ! videoconvert ! "
  //     "video/x-raw,format=ARGB,width=320,height=240 ! appsink name=sink",
  //     NULL);
  // alternative in case you don't have network or dave decides he's had enough:
  // (or maybe you just want faster development)
  //cam->pipeline = gst_parse_launch(
  //    "videotestsrc ! videoconvert ! "
  //    //"video/x-raw,format=ARGB,width=960,height=540 ! "
  //    "appsink name=sink max-buffers=10 drop=true",
  //    NULL);


  // jpeg / cam decoding
  // create pipeline string
  char* pipeline_string = calloc(sizeof(char), 200);
  snprintf(pipeline_string, 200, "rtspsrc location=rtsp://%s:8554/test ! rtpjpegdepay ! "
      "decodebin ! appsink name=sink max-buffers=10 drop=true", ip);

  printf("%s", pipeline_string);

  cam->pipeline = gst_parse_launch(pipeline_string, NULL);

  free(pipeline_string);

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
  GstSample *sample = gst_app_sink_try_pull_sample(cam->appsink, GST_FRAME_PULL_TIMEOUT_NS);

  if (!sample) {
    //printf("no sample provided\n");
    if (gst_app_sink_is_eos(cam->appsink)) {
      printf("stream is EOS\n");
    }
    return NULL;
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
    gst_caps_unref(caps);
    gst_sample_unref(sample);
    return NULL;
  }

  GstBuffer *buffer = gst_sample_get_buffer(converted_sample);
  GstMapInfo map;
  gst_buffer_map(buffer, &map, GST_MAP_READ);

  struct image *img = create_image_size(enc, map.size);
  if(!img) {
    printf("failed to allocate image\n");
  }
  memcpy(img->buf, map.data, map.size);
  img->buf_len = map.size;
  img->width=width;
  img->height=height;
  img->enc = enc;

  // cleanup memory
  gst_caps_unref(caps);
  gst_sample_unref(sample);
  gst_buffer_unmap(buffer, &map);
  gst_sample_unref(converted_sample);

  return img;
}

int play_stream(struct camera_rx * cam) {
  /* Start playing */
  GstStateChangeReturn ret = gst_element_set_state(cam->pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Unable to set the pipeline to the playing state.\n");
    return 1;
  }
  printf("playing stream\n");
  return 0;
}

int pause_stream(struct camera_rx * cam) {
  GstStateChangeReturn ret = gst_element_set_state(cam->pipeline, GST_STATE_PAUSED);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Unable to set the pipeline to the playing state.\n");
    return 1;
  }
  printf("paused stream\n");
  return 0;
}

void cleanup_rx_camera(struct camera_rx * cam) {
  /* Free resources */
  gst_element_set_state(cam->pipeline, GST_STATE_NULL);
  gst_object_unref(cam->pipeline);
  gst_object_unref(cam->appsink);
  free(cam);
}
