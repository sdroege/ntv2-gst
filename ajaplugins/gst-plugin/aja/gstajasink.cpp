/* GStreamer
 * Copyright (C) 2021 Sebastian Dröge <sebastian@centricular.com>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gstajasink.h"

GST_DEBUG_CATEGORY_STATIC (gst_aja_sink_debug);
#define GST_CAT_DEFAULT gst_aja_sink_debug

enum
{
  PROP_0,

};

static void gst_aja_sink_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec);
static void gst_aja_sink_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec);
static void gst_aja_sink_finalize (GObject * object);

static gboolean gst_aja_sink_unlock (GstBaseSink * bsink);
static gboolean gst_aja_sink_unlock_stop (GstBaseSink * bsink);
static GstFlowReturn gst_aja_sink_render (GstBaseSink * bsink,
    GstBuffer * buffer);

static gboolean gst_aja_sink_open (GstAjaSink * sink);
static gboolean gst_aja_sink_close (GstAjaSink * sink);
static gboolean gst_aja_sink_start (GstAjaSink * sink);
static gboolean gst_aja_sink_stop (GstAjaSink * sink);

static GstStateChangeReturn gst_aja_sink_change_state (GstElement *
    element, GstStateChange transition);

#define parent_class gst_aja_sink_parent_class
G_DEFINE_TYPE (GstAjaSink, gst_aja_sink, GST_TYPE_BASE_SINK);

static void
gst_aja_sink_class_init (GstAjaSinkClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
  GstBaseSinkClass *basesink_class = GST_BASE_SINK_CLASS (klass);
  GstCaps *templ_caps;

  gobject_class->set_property = gst_aja_sink_set_property;
  gobject_class->get_property = gst_aja_sink_get_property;
  gobject_class->finalize = gst_aja_sink_finalize;

  element_class->change_state =
      GST_DEBUG_FUNCPTR (gst_aja_sink_change_state);

  basesink_class->unlock = GST_DEBUG_FUNCPTR (gst_aja_sink_unlock);
  basesink_class->unlock_stop =
      GST_DEBUG_FUNCPTR (gst_aja_sink_unlock_stop);
  basesink_class->render =
      GST_DEBUG_FUNCPTR (gst_aja_sink_render);

  templ_caps = gst_caps_new_simple ("video/x-raw",
      "format", G_TYPE_STRING, "v210",
      "width", G_TYPE_INT, 1920,
      "height", G_TYPE_INT, 1080,
      "framerate", GST_TYPE_FRACTION, 30, 1,
      "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
      NULL);
  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS, templ_caps));
  gst_caps_unref (templ_caps);

  gst_element_class_set_static_metadata (element_class, "AJA Audio/video sink",
      "Audio/Video/Sink", "AJA Audio/video sink", "Sebastian Dröge <sebastian@centricular.com>");

  GST_DEBUG_CATEGORY_INIT (gst_aja_sink_debug, "ajavideosink", 0,
      "debug category for ajavideosink element");
}

static void
gst_aja_sink_init (GstAjaSink * sink)
{
}

void
gst_aja_sink_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstAjaSink *sink = GST_AJA_SINK (object);

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_aja_sink_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstAjaSink *sink = GST_AJA_SINK (object);

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_aja_sink_finalize (GObject * object)
{
  GstAjaSink *sink = GST_AJA_SINK (object);

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static gboolean
gst_aja_sink_unlock (GstBaseSink * bsink)
{
  GstAjaSink *sink = GST_AJA_SINK (bsink);

  return TRUE;
}

static gboolean
gst_aja_sink_unlock_stop (GstBaseSink * bsink)
{
  GstAjaSink *sink = GST_AJA_SINK (bsink);

  return TRUE;
}

static gboolean
gst_aja_sink_open (GstAjaSink * sink)
{
  return TRUE;
}

static gboolean
gst_aja_sink_close (GstAjaSink * sink)
{
  return TRUE;
}


static gboolean
gst_aja_sink_start (GstAjaSink *sink)
{
  return TRUE;
}

static gboolean
gst_aja_sink_stop (GstAjaSink * sink)
{
  return TRUE;
}

static GstStateChangeReturn
gst_aja_sink_change_state (GstElement * element, GstStateChange transition)
{
  GstAjaSink *sink = GST_AJA_SINK (element);
  GstStateChangeReturn ret;

  switch (transition) {
    case GST_STATE_CHANGE_NULL_TO_READY:
      if (!gst_aja_sink_open (sink))
        return GST_STATE_CHANGE_FAILURE;
      break;
    case GST_STATE_CHANGE_READY_TO_PAUSED:
      if (!gst_aja_sink_start (sink))
        return GST_STATE_CHANGE_FAILURE;
      break;
    case GST_STATE_CHANGE_PAUSED_TO_PLAYING:
      break;
    default:
      break;
  }

  ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);
  if (ret == GST_STATE_CHANGE_FAILURE)
    return ret;

  switch (transition) {
    case GST_STATE_CHANGE_PLAYING_TO_PAUSED:
      break;
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      if (!gst_aja_sink_stop (sink))
        return GST_STATE_CHANGE_FAILURE;
      break;
    case GST_STATE_CHANGE_READY_TO_NULL:
      if (!gst_aja_sink_close (sink))
        return GST_STATE_CHANGE_FAILURE;
      break;
    default:
      break;
  }
out:

  return ret;
}

static GstFlowReturn
gst_aja_sink_render (GstBaseSink * bsink, GstBuffer * buffer)
{
  GstAjaSink *sink = GST_AJA_SINK (bsink);
  GstFlowReturn flow_ret = GST_FLOW_OK;

  return flow_ret;
}
