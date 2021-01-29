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
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef _GST_AJA_SINK_H_
#define _GST_AJA_SINK_H_

#include <gst/gst.h>
#include <gst/base/base.h>
#include <gst/video/video.h>
#include "gstaja.h"

G_BEGIN_DECLS

#define GST_TYPE_AJA_SINK          (gst_aja_sink_get_type())
#define GST_AJA_SINK(obj)          (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_AJA_SINK,GstAjaSink))
#define GST_AJA_SINK_CAST(obj)     ((GstAjaSink*)obj)
#define GST_AJA_SINK_CLASS(klass)  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_AJA_SINK,GstAjaSinkClass))
#define GST_IS_AJA_SINK(obj)       (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_AJA_SINK))
#define GST_IS_AJA_SINK_CLASS(obj) (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_AJA_SINK))

typedef struct _GstAjaSink GstAjaSink;
typedef struct _GstAjaSinkClass GstAjaSinkClass;

struct _GstAjaSink
{
  GstBaseSink parent;

  GMutex queue_lock;
  GCond queue_cond;
  GstQueueArray *queue;
  gboolean eos;
  gboolean playing;
  gboolean shutdown;

  GstBufferPool *buffer_pool;

  CNTV2Card *device;
  NTV2DeviceID device_id;

  // TODO: Properties
  gchar *device_specifier;
  NTV2Channel output_channel;

  GstCaps *configured_caps;
  GstVideoInfo configured_info;

  AJAThread *output_thread;
};

struct _GstAjaSinkClass
{
  GstBaseSinkClass parent_class;
};

GType gst_aja_sink_get_type (void);

G_END_DECLS

#endif /* _GST_AJA_SINK_H_ */