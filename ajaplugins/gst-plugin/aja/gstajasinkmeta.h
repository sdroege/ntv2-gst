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

#ifndef _GST_AJA_SINK_META_H_
#define _GST_AJA_SINK_META_H_

#include <gst/gst.h>
#include <gst/base/base.h>
#include <gst/video/video.h>
#include "gstaja.h"

G_BEGIN_DECLS

typedef struct {
  GstMeta meta;

  GstBuffer *buffer;
} GstAjaSinkAudioMeta;

G_GNUC_INTERNAL
GType gst_aja_sink_audio_meta_api_get_type (void);
#define GST_AJA_SINK_AUDIO_META_API_TYPE (gst_aja_sink_audio_meta_api_get_type())

G_GNUC_INTERNAL
const GstMetaInfo *gst_aja_sink_audio_meta_get_info (void);
#define GST_AJA_SINK_AUDIO_META_INFO (gst_aja_sink_audio_meta_get_info())

#define gst_buffer_get_aja_sink_audio_meta(b) \
        ((GstAjaSinkAudioMeta*)gst_buffer_get_meta((b),GST_AJA_SINK_AUDIO_META_API_TYPE))

G_GNUC_INTERNAL
GstAjaSinkAudioMeta *gst_buffer_add_aja_sink_audio_meta (GstBuffer * buffer, GstBuffer * audio_buffer);



G_END_DECLS

#endif /* _GST_AJA_SINK_META_H_ */
