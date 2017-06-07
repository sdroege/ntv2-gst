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
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */
/**
 * SECTION:element-gstaja_audio_src
 *
 * The aja_audio_src element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v fakesrc ! aja_audio_src ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/audio/gstaudiosrc.h>
#include "gstajaaudiosrc.h"
#include "gstajavideosrc.h"

GST_DEBUG_CATEGORY_STATIC (gst_aja_audio_src_debug);
#define GST_CAT_DEFAULT gst_aja_audio_src_debug

#define DEFAULT_CONNECTION      (GST_AJA_AUDIO_CONNECTION_AUTO)
#define DEFAULT_DEVICE_NUMBER   (0)
#define DEFAULT_INPUT_CHANNEL   (0)
#define DEFAULT_CHANNELS        (8)
#define DEFAULT_QUEUE_SIZE      (5)

#define DEFAULT_ALIGNMENT_THRESHOLD   (40 * GST_MSECOND)
#define DEFAULT_DISCONT_WAIT          (1 * GST_SECOND)

enum
{
    PROP_0,
    PROP_DEVICE_NUMBER,
    PROP_INPUT_CHANNEL,
    PROP_CHANNELS,
    PROP_ALIGNMENT_THRESHOLD,
    PROP_DISCONT_WAIT,
    PROP_QUEUE_SIZE,
};

static GstStaticPadTemplate gst_aja_audio_src_template = GST_STATIC_PAD_TEMPLATE ("src",
                                                                                  GST_PAD_SRC,
                                                                                  GST_PAD_ALWAYS,
                                                                                  GST_STATIC_CAPS
                                                                                  ("audio/x-raw, format=S32LE, channels=[1,2], rate=48000, "
                                                                                   "layout=interleaved;"
                                                                                   "audio/x-raw, format=S32LE, channels=[3,16], rate=48000, "
                                                                                   "channel-mask = (bitmask) 0, layout=interleaved;")
                                                                                  );


typedef struct
{
    GstAjaAudioSrc              *audio_src;
    AjaAudioBuff                *audio_buff;
    GstClockTime                capture_time;
    GstClockTime                stream_time;
} AjaCaptureAudioPacket;

static void
aja_capture_audio_packet_free (void *data)
{
    AjaCaptureAudioPacket *packet = (AjaCaptureAudioPacket *) data;
        
    if ((packet->audio_src->input) && (packet->audio_src->input->ntv2AVHevc))
        packet->audio_src->input->ntv2AVHevc->ReleaseAudioBuffer(packet->audio_buff);
    g_free (packet);
}

typedef struct
{
    GstAjaAudioSrc              *audio_src;
    AjaAudioBuff                *audio_buff;
} AjaAudioPacket;

static void
aja_audio_packet_free (void *data)
{
    AjaAudioPacket *packet = (AjaAudioPacket *) data;
    
    if ((packet->audio_src->input) && (packet->audio_src->input->ntv2AVHevc))
        packet->audio_src->input->ntv2AVHevc->ReleaseAudioBuffer(packet->audio_buff);
    g_free (packet);
}

static void gst_aja_audio_src_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_aja_audio_src_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec);

static void gst_aja_audio_src_finalize (GObject * object);


static gboolean gst_aja_audio_src_set_caps (GstBaseSrc * bsrc, GstCaps * caps);
static GstCaps *gst_aja_audio_src_get_caps (GstBaseSrc * bsrc, GstCaps * filter);
static gboolean gst_aja_audio_src_unlock (GstBaseSrc * bsrc);
static gboolean gst_aja_audio_src_unlock_stop (GstBaseSrc * bsrc);
static gboolean gst_aja_audio_src_query (GstBaseSrc * bsrc, GstQuery * query);

static GstFlowReturn gst_aja_audio_src_create (GstPushSrc * psrc, GstBuffer ** buffer);

static gboolean gst_aja_audio_src_open (GstAjaAudioSrc * src);
static gboolean gst_aja_audio_src_close (GstAjaAudioSrc * src);

static gboolean gst_aja_audio_src_stop (GstAjaAudioSrc * src);

static GstStateChangeReturn gst_aja_audio_src_change_state (GstElement * element, GstStateChange transition);

static gboolean gst_aja_audio_src_audio_callback (int64_t refcon, int64_t msg);

#define parent_class gst_aja_audio_src_parent_class
G_DEFINE_TYPE (GstAjaAudioSrc, gst_aja_audio_src, GST_TYPE_PUSH_SRC);


static void
gst_aja_audio_src_class_init (GstAjaAudioSrcClass * klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
    GstBaseSrcClass *basesrc_class = GST_BASE_SRC_CLASS (klass);
    GstPushSrcClass *pushsrc_class = GST_PUSH_SRC_CLASS (klass);
    
    gobject_class->set_property = gst_aja_audio_src_set_property;
    gobject_class->get_property = gst_aja_audio_src_get_property;
    gobject_class->finalize = gst_aja_audio_src_finalize;
    
    element_class->change_state = GST_DEBUG_FUNCPTR (gst_aja_audio_src_change_state);
    
    basesrc_class->get_caps = GST_DEBUG_FUNCPTR (gst_aja_audio_src_get_caps);
    basesrc_class->set_caps = GST_DEBUG_FUNCPTR (gst_aja_audio_src_set_caps);
    basesrc_class->query = GST_DEBUG_FUNCPTR (gst_aja_audio_src_query);
    basesrc_class->unlock = GST_DEBUG_FUNCPTR (gst_aja_audio_src_unlock);
    basesrc_class->unlock_stop = GST_DEBUG_FUNCPTR (gst_aja_audio_src_unlock_stop);
    
    pushsrc_class->create = GST_DEBUG_FUNCPTR (gst_aja_audio_src_create);
    
    g_object_class_install_property (gobject_class, PROP_DEVICE_NUMBER,
                                     g_param_spec_uint ("device-number",
                                                        "Device number",
                                                        "Input device instance to use",
                                                        0, G_MAXINT, DEFAULT_DEVICE_NUMBER,
                                                        (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT)));
    
    g_object_class_install_property (gobject_class, PROP_INPUT_CHANNEL,
                                     g_param_spec_uint ("input-channel",
                                                        "Input channel",
                                                        "Input channel to use",
                                                        0, G_MAXINT, DEFAULT_INPUT_CHANNEL,
                                                        (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT)));

    g_object_class_install_property (gobject_class, PROP_CHANNELS,
                                     g_param_spec_uint ("channels",
                                                        "Channels",
                                                        "Number of audio channels",
                                                        2, 16, DEFAULT_CHANNELS,
                                                        (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT)));
    

    g_object_class_install_property (gobject_class, PROP_ALIGNMENT_THRESHOLD,
                                     g_param_spec_uint64 ("alignment-threshold",
                                                          "Alignment Threshold",
                                                          "Timestamp alignment threshold in nanoseconds",
                                                          0, G_MAXUINT64 - 1, DEFAULT_ALIGNMENT_THRESHOLD,
                                                          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));

    g_object_class_install_property (gobject_class, PROP_DISCONT_WAIT,
                                     g_param_spec_uint64 ("discont-wait",
                                                          "Discont Wait",
                                                          "Window of time in nanoseconds to wait before "
                                                          "creating a discontinuity",
                                                          0, G_MAXUINT64 - 1, DEFAULT_DISCONT_WAIT,
                                                          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));

    g_object_class_install_property (gobject_class, PROP_QUEUE_SIZE,
                                     g_param_spec_uint ("queue-size",
                                                        "Queue Size",
                                                        "Size of internal queue in number of video frames",
                                                        1, G_MAXINT, DEFAULT_QUEUE_SIZE,
                                                        (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));
    
    
    gst_element_class_add_pad_template (element_class, gst_static_pad_template_get (&gst_aja_audio_src_template));
    
    gst_element_class_set_static_metadata (element_class, "Aja Audio Source", "Audio/Src", "Aja RT Audio Source", "PSM <philm@aja.com>");
    
    GST_DEBUG_CATEGORY_INIT (gst_aja_audio_src_debug, "ajaaudiosrc", 0, "debug category for ajaaudiosrc element");
}

static void
gst_aja_audio_src_init (GstAjaAudioSrc *src)
{
    GST_DEBUG_OBJECT (src, "init");
    
    src->input_channel = DEFAULT_INPUT_CHANNEL;
    src->device_number = DEFAULT_DEVICE_NUMBER;
    src->channels = DEFAULT_CHANNELS;
    src->queue_size = DEFAULT_QUEUE_SIZE;
    src->alignment_threshold = DEFAULT_ALIGNMENT_THRESHOLD;
    src->discont_wait = DEFAULT_DISCONT_WAIT;
    
    gst_base_src_set_live (GST_BASE_SRC (src), TRUE);
    gst_base_src_set_format (GST_BASE_SRC (src), GST_FORMAT_TIME);
    
    g_mutex_init (&src->lock);
    g_cond_init (&src->cond);
    
    g_queue_init (&src->current_packets);
}

void
gst_aja_audio_src_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec)
{
    GstAjaAudioSrc *src = GST_AJA_AUDIO_SRC (object);
    GST_DEBUG_OBJECT (src, "set_property");
    
    switch (property_id)
    {
        case PROP_DEVICE_NUMBER:
            src->device_number = g_value_get_uint (value);
            break;
            
        case PROP_INPUT_CHANNEL:
            src->input_channel = g_value_get_uint (value);
            break;

        case PROP_CHANNELS:
            src->channels = g_value_get_uint (value);
            break;
            
        case PROP_ALIGNMENT_THRESHOLD:
            src->alignment_threshold = g_value_get_uint64 (value);
            break;
            
        case PROP_DISCONT_WAIT:
            src->discont_wait = g_value_get_uint64 (value);
            break;

        case PROP_QUEUE_SIZE:
            src->queue_size = g_value_get_uint (value);
            break;
            
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            break;
    }
}

void
gst_aja_audio_src_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec)
{
    GstAjaAudioSrc *src = GST_AJA_AUDIO_SRC (object);
    GST_DEBUG_OBJECT (src, "get_property");
    
    switch (property_id)
    {
        case PROP_DEVICE_NUMBER:
            g_value_set_uint (value, src->device_number);
            break;
            
        case PROP_INPUT_CHANNEL:
            g_value_set_uint (value, src->input_channel);
            break;

        case PROP_CHANNELS:
            g_value_set_uint (value, src->channels);
            break;
            
        case PROP_ALIGNMENT_THRESHOLD:
            g_value_set_uint64 (value, src->alignment_threshold);
            break;
            
        case PROP_DISCONT_WAIT:
            g_value_set_uint64 (value, src->discont_wait);
            break;
            
        case PROP_QUEUE_SIZE:
            g_value_set_uint (value, src->queue_size);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            break;
    }
}

void
gst_aja_audio_src_finalize (GObject * object)
{
    GstAjaAudioSrc *src = GST_AJA_AUDIO_SRC (object);
    GST_DEBUG_OBJECT (src, "finalize");

    g_queue_foreach (&src->current_packets, (GFunc) aja_capture_audio_packet_free, NULL);
    g_queue_clear (&src->current_packets);

    g_mutex_clear (&src->lock);
    g_cond_clear (&src->cond);

    G_OBJECT_CLASS (gst_aja_audio_src_parent_class)->finalize (object);
}

static gboolean
gst_aja_audio_src_set_caps (GstBaseSrc * bsrc, GstCaps * caps)
{
    GstAjaAudioSrc *src = GST_AJA_AUDIO_SRC (bsrc);
    GST_DEBUG_OBJECT (src, "set_caps");

    GstCaps *current_caps;
    
    if ((current_caps = gst_pad_get_current_caps (GST_BASE_SRC_PAD (bsrc))))
    {
        GST_DEBUG_OBJECT (src, "Pad already has caps %" GST_PTR_FORMAT, caps);
        
        if (!gst_caps_is_equal (caps, current_caps))
        {
            GST_ERROR_OBJECT (src, "New caps are not equal to old caps");
            gst_caps_unref (current_caps);
            return FALSE;
        }
        else
        {
            gst_caps_unref (current_caps);
            return TRUE;
        }
    }
    
    if (!gst_audio_info_from_caps (&src->info, caps))
        return FALSE;

    // FIXME do something to actually enable audio? is it always enabled?
    // FIXME Is there anything we can actually configure based on the caps?
    
    g_mutex_lock (&src->input->lock);
    src->input->audio_enabled = TRUE;
    if (src->input->start_streams && src->input->videosrc)
        src->input->start_streams (src->input->videosrc);
    g_mutex_unlock (&src->input->lock);
    
    return TRUE;
}

static GstCaps *
gst_aja_audio_src_get_caps (GstBaseSrc * bsrc, GstCaps * filter)
{
    GstAjaAudioSrc *src = GST_AJA_AUDIO_SRC (bsrc);
    GST_DEBUG_OBJECT (src, "get_caps");
    
    // We don't support renegotiation
    GstCaps *caps = gst_pad_get_current_caps (GST_BASE_SRC_PAD (bsrc));
    
    if (!caps) {
        caps = gst_pad_get_pad_template_caps (GST_BASE_SRC_PAD (bsrc));
        if (src->channels) {
          GstCaps *tmp, *channel_filter;

          channel_filter = gst_caps_new_simple ("audio/x-raw", "channels", G_TYPE_INT, src->channels, NULL);
          tmp = gst_caps_intersect (caps, channel_filter);
          gst_caps_unref (caps);
          gst_caps_unref (channel_filter);
          caps = tmp;
        }
    }
    
    if (filter)
    {
        GstCaps *tmp = gst_caps_intersect_full (filter, caps, GST_CAPS_INTERSECT_FIRST);
        gst_caps_unref (caps);
        caps = tmp;
    }
    
    return caps;
}

static gboolean
gst_aja_audio_src_query (GstBaseSrc * bsrc, GstQuery * query)
{
    GstAjaAudioSrc *src = GST_AJA_AUDIO_SRC (bsrc);
    GST_DEBUG_OBJECT (src, "query");

    gboolean ret = TRUE;
    
    switch (GST_QUERY_TYPE (query))
    {
        case GST_QUERY_LATENCY:
        {
            if (src->input)
            {
                g_mutex_lock (&src->input->lock);
                if (src->input->mode)
                {
                    GstClockTime min, max;
                    
                    min = gst_util_uint64_scale_ceil (GST_SECOND, src->input->mode->fps_d, src->input->mode->fps_n);
                    max = src->queue_size * min;
                    
                    gst_query_set_latency (query, TRUE, min, max);
                    ret = TRUE;
                }
                else
                {
                    ret = FALSE;
                }
                g_mutex_unlock (&src->input->lock);
            }
            else
            {
                ret = FALSE;
            }
            break;
        }
            
        default:
            ret = GST_BASE_SRC_CLASS (parent_class)->query (bsrc, query);
            break;
    }
    return ret;
}

static gboolean
gst_aja_audio_src_unlock (GstBaseSrc * bsrc)
{
    GstAjaAudioSrc *src = GST_AJA_AUDIO_SRC (bsrc);
    GST_DEBUG_OBJECT (src, "unlock");
  
    g_mutex_lock (&src->lock);
    src->flushing = TRUE;
    g_cond_signal (&src->cond);
    g_mutex_unlock (&src->lock);
    
    return TRUE;
}

// Clear any pending unlock request, as we succeeded in unlocking
static gboolean
gst_aja_audio_src_unlock_stop (GstBaseSrc * bsrc)
{
    GstAjaAudioSrc *src = GST_AJA_AUDIO_SRC (bsrc);
    GST_DEBUG_OBJECT (src, "unlock_stop");
    
    g_mutex_lock (&src->lock);
    src->flushing = FALSE;
    g_queue_foreach (&src->current_packets, (GFunc) aja_capture_audio_packet_free, NULL);
    g_queue_clear (&src->current_packets);
    g_mutex_unlock (&src->lock);
    
    return TRUE;
}

static gboolean
gst_aja_audio_src_open (GstAjaAudioSrc * src)
{
    AJAStatus           status;
    GST_DEBUG_OBJECT (src, "open");
    
    src->input = gst_aja_acquire_input (src->device_number, src->input_channel, GST_ELEMENT_CAST (src), TRUE, FALSE);
    if (!src->input)
    {
        GST_ERROR_OBJECT (src, "Failed to acquire input");
        return FALSE;
    }

    g_mutex_lock (&src->input->lock);
    status = src->input->ntv2AVHevc->Open ();
    if (!AJA_SUCCESS (status))
    {
        GST_ERROR_OBJECT (src, "Failed to open input");
        g_mutex_unlock (&src->input->lock);
        return FALSE;
    }

    status = src->input->ntv2AVHevc->InitAudio (&src->channels);
    if (status != AJA_STATUS_SUCCESS) {
        GST_ERROR_OBJECT (src, "Failed to initialize audio");
        g_mutex_unlock (&src->input->lock);
        return FALSE;
    }
    
    g_mutex_unlock (&src->input->lock);

    return TRUE;
}

static gboolean
gst_aja_audio_src_close (GstAjaAudioSrc * src)
{
    GST_DEBUG_OBJECT (src, "close");
    
    if (src->input)
    {
        // The real shutdown will happen by the videosrc
        g_mutex_lock (&src->input->lock);
        src->input->audio_enabled = FALSE;
        gst_object_unref (src->input->audiosrc);
        src->input->audiosrc = NULL;
        g_mutex_unlock (&src->input->lock);
        src->input = NULL;
    }
    
    return TRUE;
}

static gboolean
gst_aja_audio_src_stop (GstAjaAudioSrc * src)
{
    GST_DEBUG_OBJECT (src, "stop");
    
    g_queue_foreach (&src->current_packets, (GFunc) aja_capture_audio_packet_free, NULL);
    g_queue_clear (&src->current_packets);
    
    if (src->input && src->input->audio_enabled)
    {
        g_mutex_lock (&src->input->lock);
        src->input->audio_enabled = FALSE;
        src->input->ntv2AVHevc->SetCallback(AUDIO_CALLBACK, 0, 0);
        g_mutex_unlock (&src->input->lock);
    }
    
    return TRUE;
}

static GstStateChangeReturn
gst_aja_audio_src_change_state (GstElement * element, GstStateChange transition)
{
    GstAjaAudioSrc *src = GST_AJA_AUDIO_SRC (element);
    GST_DEBUG_OBJECT (src, "change_state");

    GstStateChangeReturn ret;
    
    switch (transition)
    {
        case GST_STATE_CHANGE_NULL_TO_READY:
            if (!gst_aja_audio_src_open (src))
            {
                ret = GST_STATE_CHANGE_FAILURE;
                goto out;
            }
            break;
            
        case GST_STATE_CHANGE_READY_TO_PAUSED:
        {
            GstElement *videosrc = NULL;
            
            // Check if there is a video src for this input too and if it
            // is actually in the same pipeline
            g_mutex_lock (&src->input->lock);
            if (src->input->videosrc)
                videosrc = GST_ELEMENT_CAST (gst_object_ref (src->input->videosrc));
            src->input->ntv2AVHevc->SetCallback(AUDIO_CALLBACK, (int64_t)&gst_aja_audio_src_audio_callback, (int64_t)src);
            g_mutex_unlock (&src->input->lock);
            
            if (!videosrc)
            {
                GST_ELEMENT_ERROR (src, STREAM, FAILED, (NULL), ("Audio src needs a video src for its operation"));
                ret = GST_STATE_CHANGE_FAILURE;
                goto out;
            }
            // FIXME: This causes deadlocks sometimes
#if 0
            else if (!in_same_pipeline (GST_ELEMENT_CAST (src), videosrc))
            {
                GST_ELEMENT_ERROR (src, STREAM, FAILED, (NULL), ("Audio src and video src need to be in the same pipeline"));
                ret = GST_STATE_CHANGE_FAILURE;
                gst_object_unref (videosrc);
                goto out;
            }
#endif
            if (videosrc)
                gst_object_unref (videosrc);
            
            src->flushing = FALSE;
            src->next_offset = -1;
            break;
        }
            
        default:
            break;
    }
    
    ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);
    if (ret == GST_STATE_CHANGE_FAILURE)
        return ret;
    
    switch (transition)
    {
        case GST_STATE_CHANGE_PAUSED_TO_READY:
            gst_aja_audio_src_stop (src);
            break;
            
        case GST_STATE_CHANGE_READY_TO_NULL:
            gst_aja_audio_src_close (src);
            break;
            
        default:
            break;
    }
out:
    
    return ret;
}

static void
gst_aja_audio_src_got_packet (GstAjaAudioSrc * src, AjaAudioBuff * audioBuff)
{
    GstAjaVideoSrc *videosrc = NULL;
    GstClockTime stream_time, timestamp;

    stream_time = gst_util_uint64_scale (audioBuff->frameNumber, src->input->mode->fps_d * GST_SECOND, src->input->mode->fps_n);

    //GST_ERROR_OBJECT (src, "Got audio packet at %" GST_TIME_FORMAT, GST_TIME_ARGS (capture_time));
    
    g_mutex_lock (&src->input->lock);
    if (src->input->videosrc)
        videosrc = GST_AJA_VIDEO_SRC_CAST (gst_object_ref (src->input->videosrc));
    g_mutex_unlock (&src->input->lock);
    
    if (videosrc)
    {
        // FIXME: This is not a problem anymore after switching to bufferpools
        // and a single layer of copying in AJA: we will always get video
        // first then
        while (!videosrc->current_time_mapping.b)
          g_usleep (10);
        g_mutex_lock (&videosrc->lock);

        if (videosrc->first_time == GST_CLOCK_TIME_NONE)
            videosrc->first_time = stream_time;

        if (videosrc->skip_first_time > 0
            && stream_time - videosrc->first_time < videosrc->skip_first_time) {
            GST_DEBUG_OBJECT (src,
                "Skipping frame as requested: %" GST_TIME_FORMAT " < %"
                GST_TIME_FORMAT, GST_TIME_ARGS (stream_time),
                GST_TIME_ARGS (videosrc->skip_first_time + videosrc->first_time));
            g_mutex_unlock (&videosrc->lock);
            src->input->ntv2AVHevc->ReleaseAudioBuffer(audioBuff);
            return;
        }

        if (videosrc->output_stream_time)
            timestamp = stream_time;
        else
            timestamp = gst_clock_adjust_with_calibration (NULL, stream_time,
                videosrc->current_time_mapping.xbase,
                videosrc->current_time_mapping.b, videosrc->current_time_mapping.num,
                videosrc->current_time_mapping.den);
        g_mutex_unlock (&videosrc->lock);
        gst_object_unref (videosrc);
        //GST_LOG_OBJECT (src, "Actual timestamp %" GST_TIME_FORMAT, GST_TIME_ARGS (capture_time));
    } else {
      timestamp = GST_CLOCK_TIME_NONE;
    }
    
    g_mutex_lock (&src->lock);
    if (!src->flushing)
    {
        AjaCaptureAudioPacket *f;
        
        while (g_queue_get_length (&src->current_packets) >= src->queue_size)
        {
            f = (AjaCaptureAudioPacket *) g_queue_pop_head (&src->current_packets);
            GST_WARNING_OBJECT (src, "Dropping old packet at %" GST_TIME_FORMAT, GST_TIME_ARGS (f->capture_time));
            aja_capture_audio_packet_free (f);
        }
        
        f = (AjaCaptureAudioPacket *) g_malloc0 (sizeof (AjaCaptureAudioPacket));
        f->audio_src = src;
        f->audio_buff = audioBuff;
        f->capture_time = timestamp;
        f->stream_time = stream_time;
        
        g_queue_push_tail (&src->current_packets, f);
        g_cond_signal (&src->cond);
    }
    g_mutex_unlock (&src->lock);
}

static GstFlowReturn
gst_aja_audio_src_create (GstPushSrc * bsrc, GstBuffer ** buffer)
{
    GstAjaAudioSrc *src = GST_AJA_AUDIO_SRC (bsrc);
    //GST_DEBUG_OBJECT (src, "create");

    GstFlowReturn flow_ret = GST_FLOW_OK;
    
    
    
    const guint8 *data;
    glong sample_count;
    gsize data_size;
    AjaAudioPacket *ap;
    AjaCaptureAudioPacket *p;
    
    GstClockTime timestamp, duration;
    GstClockTime start_time, end_time;
    guint64 start_offset, end_offset;
    gboolean discont = FALSE;
    static GstStaticCaps stream_reference =
      GST_STATIC_CAPS ("timestamp/x-aja-stream");
    
    g_mutex_lock (&src->lock);
    while (g_queue_is_empty (&src->current_packets) && !src->flushing)
    {
        g_cond_wait (&src->cond, &src->lock);
    }
    
    p = (AjaCaptureAudioPacket *) g_queue_pop_head (&src->current_packets);
    g_mutex_unlock (&src->lock);
    
    if (src->flushing)
    {
        if (p)
            aja_capture_audio_packet_free (p);
        GST_DEBUG_OBJECT (src, "Flushing");
        return GST_FLOW_FLUSHING;
    }
    
    data = (const guint8 *)p->audio_buff->pAudioBuffer;
    data_size = (gsize)p->audio_buff->audioDataSize;
    sample_count = data_size / src->info.bpf;
    
    ap = (AjaAudioPacket *) g_malloc0 (sizeof (AjaAudioPacket));

    *buffer = gst_buffer_new_wrapped_full ((GstMemoryFlags) GST_MEMORY_FLAG_READONLY,
                                           (gpointer) data, data_size, 0, data_size, ap,
                                           (GDestroyNotify) aja_audio_packet_free);
    
    
    ap->audio_src = p->audio_src;
    ap->audio_buff = p->audio_buff;
    src->input->ntv2AVHevc->AddRefAudioBuffer(ap->audio_buff);
 

    timestamp = p->capture_time;
    
    // Jitter and discontinuity handling, based on audiobasesrc
    start_time = timestamp;
    
    // Convert to the sample numbers
    start_offset = gst_util_uint64_scale (start_time, src->info.rate, GST_SECOND);
    
    end_offset = start_offset + sample_count;
    end_time = gst_util_uint64_scale_int (end_offset, GST_SECOND, src->info.rate);
    
    duration = end_time - start_time;
    
    if (src->next_offset == (guint64) - 1)
    {
        discont = TRUE;
    } else
    {
        guint64 diff, max_sample_diff;
        
        // Check discont
        if (start_offset <= src->next_offset)
            diff = src->next_offset - start_offset;
        else
            diff = start_offset - src->next_offset;
        
        max_sample_diff = gst_util_uint64_scale_int (src->alignment_threshold, src->info.rate, GST_SECOND);
        
        // Discont!
        if (G_UNLIKELY (diff >= max_sample_diff))
        {
            if (src->discont_wait > 0)
            {
                if (src->discont_time == GST_CLOCK_TIME_NONE)
                {
                    src->discont_time = start_time;
                }
                else if (start_time - src->discont_time >= src->discont_wait)
                {
                    discont = TRUE;
                    src->discont_time = GST_CLOCK_TIME_NONE;
                }
            }
            else
            {
                discont = TRUE;
            }
        }
        else if (G_UNLIKELY (src->discont_time != GST_CLOCK_TIME_NONE))
        {
            // we have had a discont, but are now back on track!
            src->discont_time = GST_CLOCK_TIME_NONE;
        }
    }
    
    if (discont)
    {
        // Have discont, need resync and use the capture timestamps
        if (src->next_offset != (guint64) - 1)
            GST_INFO_OBJECT (src, "Have discont. Expected %"
                             G_GUINT64_FORMAT ", got %" G_GUINT64_FORMAT,
                             src->next_offset, start_offset);
        GST_BUFFER_FLAG_SET (*buffer, GST_BUFFER_FLAG_DISCONT);
        src->next_offset = end_offset;
        src->discont_time = GST_CLOCK_TIME_NONE;
    }
    else
    {
        // No discont, just keep counting
        timestamp = gst_util_uint64_scale (src->next_offset, GST_SECOND, src->info.rate);
        src->next_offset += sample_count;
        duration = gst_util_uint64_scale (src->next_offset, GST_SECOND, src->info.rate) - timestamp;
    }
    
    GST_BUFFER_TIMESTAMP (*buffer) = timestamp;
    GST_BUFFER_DURATION (*buffer) = duration;

#if GST_CHECK_VERSION (1, 13, 0)
    gst_buffer_add_reference_timestamp_meta (*buffer,
        gst_static_caps_get (&stream_reference), p->stream_time,
        NULL);
#endif
    
#if 0
    GST_DEBUG_OBJECT (src,
                      "Outputting buffer %p with timestamp %" GST_TIME_FORMAT " and duration %"
                      GST_TIME_FORMAT, *buffer, GST_TIME_ARGS (GST_BUFFER_TIMESTAMP (*buffer)),
                      GST_TIME_ARGS (GST_BUFFER_DURATION (*buffer)));
#endif
    
    aja_capture_audio_packet_free (p);
    
    return flow_ret;
}

static gboolean
gst_aja_audio_src_audio_callback(int64_t refcon, int64_t msg)
{
    GstAjaAudioSrc *src = (GstAjaAudioSrc *) refcon;
    AjaAudioBuff *audioBuffer = (AjaAudioBuff *) msg;
    
    if (src->input->audio_enabled == FALSE)
        return FALSE;
    
    gst_aja_audio_src_got_packet(src, audioBuffer);
    return TRUE;
}


