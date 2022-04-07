#include <stdio.h>

#include <gst/app/gstappsink.h>
#include <gst/gst.h>

int main(int argc, char *argv[]) {
  /* Initialize GStreamer */
  gst_init(&argc, &argv);

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
  GstElement *pipeline = gst_parse_launch(
      "videotestsrc ! videoconvert ! "
      "video/x-raw,format=ARGB,width=320,height=240 ! "
      "queue leaky=downstream max-size-time=250000000 ! " //.25 second queue
      "appsink name=sink",
      NULL);

  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  GstBus *bus = gst_element_get_bus(pipeline);

  GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
  if (!sink) {
    printf("sink is NULL\n");
    exit(1);
  }

  GstAppSink *appsink = GST_APP_SINK(sink);
  if (!appsink) {
    printf("appsink is NULL\n");
    exit(1);
  }

  /* Start playing */
  GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Unable to set the pipeline to the playing state.\n");
    gst_object_unref(pipeline);
    return -1;
  }

  GstSample *sample = gst_app_sink_pull_sample(appsink);
  if (!sample) {
    printf("sample is NULL\n");
    exit(1);
  }

  GstBuffer *buffer = gst_sample_get_buffer(sample);
  GstMapInfo map;

  gst_buffer_map(buffer, &map, GST_MAP_READ);

  // Allocate appropriate buffer to store compressed image
  char *some_memory = malloc(map.size);
  // Copy image
  memcpy(some_memory, map.data, map.size);
  // free because we're nice
  free(some_memory);

  // TODO: put this into one of our nice structs

  //  /* Wait until error or EOS */
  // GstMessage *msg =
  //      gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
  //      GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
  //
  //  /* Parse message */
  //  if (msg != NULL) {
  //    GError *err;
  //    gchar *debug_info;
  //
  //    switch (GST_MESSAGE_TYPE (msg)) {
  //      case GST_MESSAGE_ERROR:
  //        gst_message_parse_error (msg, &err, &debug_info);
  //        g_printerr ("Error received from element %s: %s\n",
  //            GST_OBJECT_NAME (msg->src), err->message);
  //        g_printerr ("Debugging information: %s\n",
  //            debug_info ? debug_info : "none");
  //        g_clear_error (&err);
  //        g_free (debug_info);
  //        break;
  //      case GST_MESSAGE_EOS:
  //        g_print ("End-Of-Stream reached.\n");
  //        break;
  //      default:
  //        /* We should not reach here because we only asked for ERRORs and EOS
  //        */ g_printerr ("Unexpected message received.\n"); break;
  //    }
  //    gst_message_unref (msg);
  //  }

  printf("done\n");

  /* Free resources */
  gst_object_unref(bus);
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);
  return 0;
}
