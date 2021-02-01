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

#include "gstajasinkmeta.h"

GType
gst_aja_sink_audio_meta_api_get_type (void)
{
  static volatile GType type;

  if (g_once_init_enter (&type)) {
    static const gchar *tags[] = { NULL };
    GType _type = gst_meta_api_type_register ("GstAjaSinkAudioMetaAPI", tags);
    GST_INFO ("registering");
    g_once_init_leave (&type, _type);
  }
  return type;
}


static gboolean
gst_aja_sink_audio_meta_transform (GstBuffer * dest, GstMeta * meta,
    GstBuffer * buffer, GQuark type, gpointer data)
{
  GstAjaSinkAudioMeta *dmeta, *smeta;

  if (GST_META_TRANSFORM_IS_COPY (type)) {
    smeta = (GstAjaSinkAudioMeta *) meta;

    GST_DEBUG ("copy AJA sink audio metadata");
    dmeta = gst_buffer_add_aja_sink_audio_meta (dest, smeta->buffer);
    if (!dmeta)
      return FALSE;
  } else {
    /* return FALSE, if transform type is not supported */
    return FALSE;
  }
  return TRUE;
}

static gboolean
gst_aja_sink_audio_meta_init (GstMeta * meta, gpointer params, GstBuffer * buffer)
{
  GstAjaSinkAudioMeta *emeta = (GstAjaSinkAudioMeta *) meta;

  emeta->buffer = NULL;

  return TRUE;
}

static void
gst_aja_sink_audio_meta_free (GstMeta * meta, GstBuffer * buffer)
{
  GstAjaSinkAudioMeta *emeta = (GstAjaSinkAudioMeta *) meta;

  gst_buffer_replace (&emeta->buffer, NULL);
}

const GstMetaInfo *
gst_aja_sink_audio_meta_get_info (void)
{
  static const GstMetaInfo *meta_info = NULL;

  if (g_once_init_enter ((GstMetaInfo **) & meta_info)) {
    const GstMetaInfo *mi = gst_meta_register (GST_AJA_SINK_AUDIO_META_API_TYPE,
        "GstAjaSinkAudioMeta",
        sizeof (GstAjaSinkAudioMeta),
        gst_aja_sink_audio_meta_init,
        gst_aja_sink_audio_meta_free,
        gst_aja_sink_audio_meta_transform);
    g_once_init_leave ((GstMetaInfo **) & meta_info, (GstMetaInfo *) mi);
  }
  return meta_info;
}

GstAjaSinkAudioMeta *
gst_buffer_add_aja_sink_audio_meta (GstBuffer * buffer, GstBuffer * audio_buffer)
{
  GstAjaSinkAudioMeta *meta;

  g_return_val_if_fail (buffer != NULL, NULL);
  g_return_val_if_fail (audio_buffer != NULL, NULL);

  meta = (GstAjaSinkAudioMeta *) gst_buffer_add_meta (buffer,
      GST_AJA_SINK_AUDIO_META_INFO, NULL);

  meta->buffer = gst_buffer_ref (audio_buffer);

  return meta;
}
