/* GStreamer
 * Copyright (C) 2015 PSM <philm@aja.com>
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

#ifndef _GST_AJA_VIDEO_SINK_H_
#define _GST_AJA_VIDEO_SINK_H_

#include <gst/video/video.h>
#include <gst/video/gstvideosink.h>

G_BEGIN_DECLS

#define GST_TYPE_AJA_VIDEO_SINK   (gst_aja_video_sink_get_type())
#define GST_AJA_VIDEO_SINK(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_AJA_VIDEO_SINK,GstAjavideosink))
#define GST_AJA_VIDEO_SINK_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_AJA_VIDEO_SINK,GstAjavideosinkClass))
#define GST_IS_AJA_VIDEO_SINK(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_AJA_VIDEO_SINK))
#define GST_IS_AJA_VIDEO_SINK_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_AJA_VIDEO_SINK))

typedef struct _GstAjavideosink GstAjavideosink;
typedef struct _GstAjavideosinkClass GstAjavideosinkClass;

struct _GstAjavideosink
{
  GstVideoSink base_aja_video_sink;

};

struct _GstAjavideosinkClass
{
  GstVideoSinkClass base_aja_video_sink_class;
};

GType gst_aja_video_sink_get_type (void);

G_END_DECLS

#endif
