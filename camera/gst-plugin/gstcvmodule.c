/* GStreamer
 * Copyright (C) 2022 FIXME <fixme@example.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */
/**
 * SECTION:element-gstcvmodule
 *
 * The cvmodule element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! cvmodule ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideosink.h>
#include "gstcvmodule.h"

GST_DEBUG_CATEGORY_STATIC (gst_cvmodule_debug_category);
#define GST_CAT_DEFAULT gst_cvmodule_debug_category

/* prototypes */


static void gst_cvmodule_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_cvmodule_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_cvmodule_dispose (GObject * object);
static void gst_cvmodule_finalize (GObject * object);

static GstFlowReturn gst_cvmodule_show_frame (GstVideoSink * video_sink,
    GstBuffer * buf);

enum
{
  PROP_0
};

/* pad templates */

/* FIXME: add/remove formats you can handle */
#define VIDEO_SINK_CAPS \
    GST_VIDEO_CAPS_MAKE("{ I420, Y444, Y42B, UYVY, RGBA }")


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstCvModule, gst_cvmodule, GST_TYPE_VIDEO_SINK,
  GST_DEBUG_CATEGORY_INIT (gst_cvmodule_debug_category, "cvmodule", 0,
  "debug category for cvmodule element"));

static void
gst_cvmodule_class_init (GstCvModuleClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstVideoSinkClass *video_sink_class = GST_VIDEO_SINK_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_pad_template (GST_ELEMENT_CLASS(klass),
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
        gst_caps_from_string (VIDEO_SINK_CAPS)));

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS(klass),
      "FIXME Long name", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_cvmodule_set_property;
  gobject_class->get_property = gst_cvmodule_get_property;
  gobject_class->dispose = gst_cvmodule_dispose;
  gobject_class->finalize = gst_cvmodule_finalize;
  video_sink_class->show_frame = GST_DEBUG_FUNCPTR (gst_cvmodule_show_frame);

}

static void
gst_cvmodule_init (GstCvModule *cvmodule)
{
}

void
gst_cvmodule_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstCvModule *cvmodule = GST_CVMODULE (object);

  GST_DEBUG_OBJECT (cvmodule, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_cvmodule_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstCvModule *cvmodule = GST_CVMODULE (object);

  GST_DEBUG_OBJECT (cvmodule, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_cvmodule_dispose (GObject * object)
{
  GstCvModule *cvmodule = GST_CVMODULE (object);

  GST_DEBUG_OBJECT (cvmodule, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_cvmodule_parent_class)->dispose (object);
}

void
gst_cvmodule_finalize (GObject * object)
{
  GstCvModule *cvmodule = GST_CVMODULE (object);

  GST_DEBUG_OBJECT (cvmodule, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_cvmodule_parent_class)->finalize (object);
}

static GstFlowReturn
gst_cvmodule_show_frame (GstVideoSink * sink, GstBuffer * buf)
{
  GstCvModule *cvmodule = GST_CVMODULE (sink);

  GST_DEBUG_OBJECT (cvmodule, "show_frame");

  return GST_FLOW_OK;
}

static gboolean
plugin_init (GstPlugin * plugin)
{

  /* FIXME Remember to set the rank if it's an element that is meant
     to be autoplugged by decodebin. */
  return gst_element_register (plugin, "cvmodule", GST_RANK_NONE,
      GST_TYPE_CVMODULE);
}

/* FIXME: these are normally defined by the GStreamer build system.
   If you are creating an element to be included in gst-plugins-*,
   remove these, as they're always defined.  Otherwise, edit as
   appropriate for your external plugin package. */
#ifndef VERSION
#define VERSION "0.0.FIXME"
#endif
#ifndef PACKAGE
#define PACKAGE "FIXME_package"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "FIXME_package_name"
#endif
#ifndef GST_PACKAGE_ORIGIN
#define GST_PACKAGE_ORIGIN "http://FIXME.org/"
#endif

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    cvmodule,
    "FIXME plugin description",
    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)

