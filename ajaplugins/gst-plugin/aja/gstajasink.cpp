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
#include "gstajasinkmeta.h"

GST_DEBUG_CATEGORY_STATIC (gst_aja_sink_debug);
#define GST_CAT_DEFAULT gst_aja_sink_debug

#define DEFAULT_DEVICE_IDENTIFIER  ("0")
#define DEFAULT_CHANNEL            (::NTV2_CHANNEL1)
#define DEFAULT_QUEUE_SIZE         (16)

enum
{
  PROP_0,
  PROP_DEVICE_IDENTIFIER,
  PROP_CHANNEL,
  PROP_QUEUE_SIZE,
};

typedef enum {
  QUEUE_ITEM_TYPE_FRAME,
} QueueItemType;

typedef struct {
  QueueItemType type;
  GstVideoFrame frame;

  GstBuffer *audio_buffer;
  GstMapInfo audio_map;
} QueueItem;

static const NTV2VideoFormat supported_video_formats[] = {
	 NTV2_FORMAT_1080i_5000
	,NTV2_FORMAT_1080i_5994
	,NTV2_FORMAT_1080i_6000
	,NTV2_FORMAT_720p_5994
	,NTV2_FORMAT_720p_6000
	,NTV2_FORMAT_1080p_2997
	,NTV2_FORMAT_1080p_3000
	,NTV2_FORMAT_1080p_2500
	,NTV2_FORMAT_1080p_2398
	,NTV2_FORMAT_1080p_2400
	,NTV2_FORMAT_720p_5000
	,NTV2_FORMAT_720p_2398
	,NTV2_FORMAT_720p_2500
	,NTV2_FORMAT_1080p_5000_A
	,NTV2_FORMAT_1080p_5994_A
	,NTV2_FORMAT_1080p_6000_A
	,NTV2_FORMAT_625_5000
	,NTV2_FORMAT_525_5994
	,NTV2_FORMAT_525_2398
	,NTV2_FORMAT_525_2400
};

static GstCaps *
ntv2_video_format_to_caps (NTV2VideoFormat format)
{
  GstVideoInfo info;

  guint width = ::GetDisplayWidth (format);
  guint height = ::GetDisplayHeight (format);
  NTV2FrameRate fps = ::GetNTV2FrameRateFromVideoFormat (format);
  guint fps_n, fps_d;
  ::GetFramesPerSecond (fps, fps_n, fps_d);

  gst_video_info_set_format (&info, GST_VIDEO_FORMAT_v210, width, height);
  info.fps_n = fps_n;
  info.fps_d = fps_d;
  if (NTV2_IS_525_FORMAT (format)) {
    info.par_n = 10;
    info.par_d = 11;
  } else if (NTV2_IS_625_FORMAT (format)) {
    info.par_n = 12;
    info.par_d = 11;
  }
  info.interlace_mode = !::IsProgressiveTransport (format) ? GST_VIDEO_INTERLACE_MODE_INTERLEAVED : GST_VIDEO_INTERLACE_MODE_PROGRESSIVE;

  return gst_video_info_to_caps (&info);
}

static NTV2VideoFormat
ntv2_video_format_from_caps (GstCaps *caps)
{
  GstVideoInfo info;

  if (!gst_video_info_from_caps (&info, caps))
    return NTV2_FORMAT_UNKNOWN;

  for (gsize i = 0; i < G_N_ELEMENTS (supported_video_formats); i++) {
    NTV2VideoFormat format = supported_video_formats[i];

    guint width = ::GetDisplayWidth (format);
    guint height = ::GetDisplayHeight (format);
    NTV2FrameRate fps = ::GetNTV2FrameRateFromVideoFormat (format);
    guint fps_n, fps_d;
    ::GetFramesPerSecond (fps, fps_n, fps_d);

    if (width == (guint) info.width && height == (guint) info.height && (guint) info.fps_n == fps_n && (guint) info.fps_d == fps_d &&
        ((!::IsProgressiveTransport (format) && info.interlace_mode == GST_VIDEO_INTERLACE_MODE_INTERLEAVED) ||
        (::IsProgressiveTransport (format) && info.interlace_mode == GST_VIDEO_INTERLACE_MODE_PROGRESSIVE)))
      return format;
  }

  return NTV2_FORMAT_UNKNOWN;
}

static void gst_aja_sink_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec);
static void gst_aja_sink_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec);
static void gst_aja_sink_finalize (GObject * object);

static gboolean gst_aja_sink_set_caps (GstBaseSink * bsink, GstCaps * caps);
static GstCaps * gst_aja_sink_get_caps (GstBaseSink * bsink, GstCaps * filter);
static gboolean gst_aja_sink_event (GstBaseSink * bsink, GstEvent * event);
static GstFlowReturn gst_aja_sink_render (GstBaseSink * bsink,
    GstBuffer * buffer);

static gboolean gst_aja_sink_open (GstAjaSink * sink);
static gboolean gst_aja_sink_close (GstAjaSink * sink);
static gboolean gst_aja_sink_start (GstAjaSink * sink);
static gboolean gst_aja_sink_stop (GstAjaSink * sink);

static GstStateChangeReturn gst_aja_sink_change_state (GstElement *
    element, GstStateChange transition);

static void output_thread_func (AJAThread * thread, void * data);

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

  g_object_class_install_property (gobject_class, PROP_DEVICE_IDENTIFIER,
      g_param_spec_string ("device-identifier",
          "Device identifier",
          "Input device instance to use",
          DEFAULT_DEVICE_IDENTIFIER,
          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
              G_PARAM_CONSTRUCT)));

  g_object_class_install_property (gobject_class, PROP_CHANNEL,
      g_param_spec_uint ("channel",
          "Channel",
          "Channel to use",
          0, NTV2_MAX_NUM_CHANNELS - 1, DEFAULT_CHANNEL,
          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
              G_PARAM_CONSTRUCT)));

  g_object_class_install_property (gobject_class, PROP_QUEUE_SIZE,
      g_param_spec_uint ("queue-size",
          "Queue Size",
          "Size of internal queue in number of video frames. "
          "Half of this is allocated as device buffers and equal to the latency.",
          1, G_MAXINT, DEFAULT_QUEUE_SIZE,
          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));

  element_class->change_state =
      GST_DEBUG_FUNCPTR (gst_aja_sink_change_state);

  basesink_class->set_caps =
      GST_DEBUG_FUNCPTR (gst_aja_sink_set_caps);
  basesink_class->get_caps =
      GST_DEBUG_FUNCPTR (gst_aja_sink_get_caps);
  basesink_class->event =
      GST_DEBUG_FUNCPTR (gst_aja_sink_event);
  basesink_class->render =
      GST_DEBUG_FUNCPTR (gst_aja_sink_render);

  templ_caps = gst_caps_new_empty ();
  for (gsize i = 0; i < G_N_ELEMENTS (supported_video_formats); i++) {
    NTV2VideoFormat format = supported_video_formats[i];
    GstCaps *format_caps = ntv2_video_format_to_caps (format);
    gst_caps_append (templ_caps, format_caps);
  }
  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS, templ_caps));
  gst_caps_unref (templ_caps);

  gst_element_class_set_static_metadata (element_class, "AJA Audio/video sink",
      "Audio/Video/Sink", "AJA Audio/video sink",
      "Sebastian Dröge <sebastian@centricular.com>");

  GST_DEBUG_CATEGORY_INIT (gst_aja_sink_debug, "ajavideosink", 0,
      "debug category for ajavideosink element");
}

static void
gst_aja_sink_init (GstAjaSink * self)
{
  self->device_identifier = g_strdup (DEFAULT_DEVICE_IDENTIFIER);
  self->channel = DEFAULT_CHANNEL;
  self->queue_size = DEFAULT_QUEUE_SIZE;

  gst_base_sink_set_render_delay (GST_BASE_SINK (self), (self->queue_size / 2) * GST_SECOND / 30);
  self->queue = gst_queue_array_new_for_struct (sizeof (QueueItem), self->queue_size);
}

void
gst_aja_sink_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstAjaSink *self = GST_AJA_SINK (object);

  switch (property_id) {
    case PROP_DEVICE_IDENTIFIER:
      g_free (self->device_identifier);
      self->device_identifier = g_value_dup_string (value);
      break;
    case PROP_CHANNEL:
      self->channel = (NTV2Channel) g_value_get_uint (value);
      break;
    case PROP_QUEUE_SIZE:
      self->queue_size = g_value_get_uint (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_aja_sink_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstAjaSink *self = GST_AJA_SINK (object);

  switch (property_id) {
    case PROP_DEVICE_IDENTIFIER:
      g_value_set_string (value, self->device_identifier);
      break;
    case PROP_CHANNEL:
      g_value_set_uint (value, self->channel);
      break;
    case PROP_QUEUE_SIZE:
      g_value_set_uint (value, self->queue_size);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_aja_sink_finalize (GObject * object)
{
  GstAjaSink *self = GST_AJA_SINK (object);

  g_assert (self->device == NULL);
  g_assert (gst_queue_array_get_length (self->queue) == 0);
  g_clear_pointer (&self->queue, gst_queue_array_free);

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static gboolean
gst_aja_sink_open (GstAjaSink * self)
{
  GST_DEBUG_OBJECT (self, "Opening device");

  g_assert (self->device == NULL);

  self->device = new CNTV2Device ();

  if (!CNTV2DeviceScanner::GetFirstDeviceFromArgument (self->device_identifier, *self->device)) {
    GST_ERROR_OBJECT (self, "Failed to open device");
    delete self->device;
    self->device = NULL;
    return FALSE;
  }

  if (!self->device->IsDeviceReady (false)) {
    delete self->device;
    self->device = NULL;
    return FALSE;
  }

  self->device->SetEveryFrameServices (::NTV2_OEM_TASKS);
  self->device_id = self->device->GetDeviceID ();

  std::string serial_number;
  if (!self->device->GetSerialNumberString (serial_number))
    serial_number = "none";

  GST_DEBUG_OBJECT (self, "Opened device with ID %d (%s, version %s, serial number %s)",
      self->device_id, self->device->GetDisplayName ().c_str (),
      self->device->GetDeviceVersionString ().c_str (), serial_number.c_str ());

  GST_DEBUG_OBJECT (self, "Using SDK version %d.%d.%d.%d (%s) and driver version %s",
      AJA_NTV2_SDK_VERSION_MAJOR, AJA_NTV2_SDK_VERSION_MINOR,
      AJA_NTV2_SDK_VERSION_POINT, AJA_NTV2_SDK_BUILD_NUMBER,
      AJA_NTV2_SDK_BUILD_DATETIME, self->device->GetDriverVersionString ().c_str ());

  self->device->SetMultiFormatMode (true);

  GST_DEBUG_OBJECT (self, "Opened device");

  return TRUE;
}

static gboolean
gst_aja_sink_close (GstAjaSink * self)
{
  if (self->device) {
    delete self->device;
    self->device = NULL;
  }

  GST_DEBUG_OBJECT (self, "Closed device");

  return TRUE;
}

static gboolean
gst_aja_sink_start (GstAjaSink * self)
{
  GST_DEBUG_OBJECT (self, "Starting");
  self->output_thread = new AJAThread();
  self->output_thread->Attach (output_thread_func, self);
  self->output_thread->SetPriority(AJA_ThreadPriority_High);
  self->output_thread->Start();
  g_mutex_lock (&self->queue_lock);
  self->shutdown = FALSE;
  self->playing = FALSE;
  self->eos = FALSE;
  g_cond_signal (&self->queue_cond);
  g_mutex_unlock (&self->queue_lock);

  return TRUE;
}

static gboolean
gst_aja_sink_stop (GstAjaSink * self)
{
  QueueItem *item;

  GST_DEBUG_OBJECT (self, "Stopping");

  g_mutex_lock (&self->queue_lock);
  self->shutdown = TRUE;
  self->playing = FALSE;
  g_cond_signal (&self->queue_cond);
  g_mutex_unlock (&self->queue_lock);

  if (self->output_thread) {
    self->output_thread->Stop();
    delete self->output_thread;
    self->output_thread = NULL;
  }

  GST_OBJECT_LOCK (self);
  gst_clear_caps (&self->configured_caps);
  self->configured_audio_channels = 0;
  GST_OBJECT_UNLOCK (self);

  while ((item = (QueueItem *) gst_queue_array_pop_head_struct (self->queue))) {
    if (item->type == QUEUE_ITEM_TYPE_FRAME) {
      gst_video_frame_unmap (&item->frame);
      if (item->audio_buffer) {
        gst_buffer_unmap (item->audio_buffer, &item->audio_map);
        gst_buffer_unref (item->audio_buffer);
      }
    }
  }

  if (self->buffer_pool) {
    gst_buffer_pool_set_active (self->buffer_pool, FALSE);
    gst_clear_object (&self->buffer_pool);
  }

  if (self->audio_buffer_pool) {
    gst_buffer_pool_set_active (self->audio_buffer_pool, FALSE);
    gst_clear_object (&self->audio_buffer_pool);
  }

  GST_DEBUG_OBJECT (self, "Stopped");

  return TRUE;
}

static GstStateChangeReturn
gst_aja_sink_change_state (GstElement * element, GstStateChange transition)
{
  GstAjaSink *self = GST_AJA_SINK (element);
  GstStateChangeReturn ret;

  switch (transition) {
    case GST_STATE_CHANGE_NULL_TO_READY:
      if (!gst_aja_sink_open (self))
        return GST_STATE_CHANGE_FAILURE;
      break;
    case GST_STATE_CHANGE_READY_TO_PAUSED:
      if (!gst_aja_sink_start (self))
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
      g_mutex_lock (&self->queue_lock);
      self->playing = FALSE;
      g_cond_signal (&self->queue_cond);
      g_mutex_unlock (&self->queue_lock);
      break;
    case GST_STATE_CHANGE_PAUSED_TO_PLAYING:
      g_mutex_lock (&self->queue_lock);
      self->playing = TRUE;
      g_cond_signal (&self->queue_cond);
      g_mutex_unlock (&self->queue_lock);
      break;
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      if (!gst_aja_sink_stop (self))
        return GST_STATE_CHANGE_FAILURE;
      break;
    case GST_STATE_CHANGE_READY_TO_NULL:
      if (!gst_aja_sink_close (self))
        return GST_STATE_CHANGE_FAILURE;
      break;
    default:
      break;
  }

  return ret;
}

static gboolean
gst_aja_sink_set_caps (GstBaseSink * bsink, GstCaps * caps)
{
  GstAjaSink *self = GST_AJA_SINK (bsink);
  const GstStructure *s;
  NTV2VideoFormat video_format = ::NTV2_FORMAT_UNKNOWN;

  GST_DEBUG_OBJECT (self, "Configuring caps %" GST_PTR_FORMAT, caps);

  GST_OBJECT_LOCK (self);
  if (self->configured_caps && !gst_caps_can_intersect (self->configured_caps, caps)) {
    GST_OBJECT_UNLOCK (self);
    GST_FIXME_OBJECT (self, "Reconfiguring caps not supported yet");
    return FALSE;
  }

  if (!gst_video_info_from_caps (&self->configured_info, caps)) {
    GST_OBJECT_UNLOCK (self);
    GST_FIXME_OBJECT (self, "Failed to parse caps");
    return FALSE;
  }

  self->configured_audio_channels = 0;
  s = gst_caps_get_structure (caps, 0);
  gst_structure_get_int (s, "audio-channels", &self->configured_audio_channels);

  gst_caps_replace (&self->configured_caps, caps);
  GST_OBJECT_UNLOCK (self);

  video_format = ntv2_video_format_from_caps (caps);
  if (video_format == NTV2_FORMAT_UNKNOWN) {
    GST_ERROR_OBJECT (self, "Unsupported caps %" GST_PTR_FORMAT, caps);
    return FALSE;
  }

  // Configure render delay based on the framerate and queue size
  gst_base_sink_set_render_delay (GST_BASE_SINK (self),
                                  gst_util_uint64_scale (self->queue_size / 2,
                                                         self->configured_info.fps_d * GST_SECOND,
                                                         self->configured_info.fps_n
                                ));

  g_assert (self->device != NULL);

  // FIXME: Needs global locking because the device state is global

  if (!::NTV2DeviceCanDoVideoFormat (self->device_id, video_format)) {
    GST_ERROR_OBJECT (self, "Device does not support mode %d", (int) video_format);
    return FALSE;
  }
  GST_DEBUG_OBJECT (self, "Configuring video format %d on channel %d", (int) video_format, (int) self->channel);
  self->device->SetVideoFormat (video_format, false, false, self->channel);

  if (!::NTV2DeviceCanDoFrameBufferFormat (self->device_id, ::NTV2_FBF_10BIT_YCBCR)) {
    GST_ERROR_OBJECT (self, "Device does not support frame buffer format %d", (int) ::NTV2_FBF_10BIT_YCBCR);
    return FALSE;
  }
  self->device->SetFrameBufferFormat (self->channel, ::NTV2_FBF_10BIT_YCBCR);

  // TODO: Property to lock to SDI1, etc
  self->device->SetFramePulseReference (::NTV2_REFERENCE_FREERUN);

  if (!self->device->EnableChannel (self->channel)) {
    GST_ERROR_OBJECT (self, "Failed to enable channel");
    return FALSE;
  }

  // TODO: Enable VANC as needed
  NTV2Standard standard;
  NTV2FrameGeometry geometry;
  self->device->GetStandard (standard);
  self->device->GetFrameGeometry (geometry);
  self->device->SetVANCMode(::NTV2_VANCMODE_OFF, standard, geometry, self->channel);

  // TODO: Do the buffer pool in the muxer for video, on demand. And also pass
  // it upstream in propose_allocation.

  // TODO: Handle VANC
  guint video_buffer_size = GetVideoActiveSize (video_format, ::NTV2_FBF_10BIT_YCBCR, ::NTV2_VANCMODE_OFF);

  self->device->DMABufferAutoLock(false, true, 0);

  GstAllocator *video_alloc = gst_aja_allocator_new(self->device, video_buffer_size, self->queue_size);

  self->buffer_pool = gst_aja_buffer_pool_new ();
  GstStructure *config = gst_buffer_pool_get_config (self->buffer_pool);
  gst_buffer_pool_config_set_params (config, NULL, video_buffer_size, self->queue_size, 0);
  gst_buffer_pool_config_set_allocator (config, video_alloc, NULL);
  gst_structure_set (config, "is-video", G_TYPE_BOOLEAN, TRUE, NULL);
  gst_buffer_pool_set_config (self->buffer_pool, config);
  gst_buffer_pool_set_active (self->buffer_pool, TRUE);
  gst_object_unref (video_alloc);

  if (self->configured_audio_channels) {
    guint audio_buffer_size = 1UL*1024UL*1024UL;
    GstAllocator *audio_alloc = gst_aja_allocator_new(self->device, audio_buffer_size, self->queue_size);

    self->audio_buffer_pool = gst_aja_buffer_pool_new ();
    config = gst_buffer_pool_get_config (self->audio_buffer_pool);
    gst_buffer_pool_config_set_params (config, NULL, audio_buffer_size,
        self->queue_size, 0);
    gst_buffer_pool_config_set_allocator (config, audio_alloc, NULL);
    gst_structure_set (config, "is-video", G_TYPE_BOOLEAN, FALSE, NULL);
    gst_buffer_pool_set_config (self->audio_buffer_pool, config);
    gst_buffer_pool_set_active (self->audio_buffer_pool, TRUE);
    gst_object_unref (audio_alloc);

    self->audio_system = ::NTV2_AUDIOSYSTEM_1;
    if (::NTV2DeviceGetNumAudioSystems(self->device_id) > 1)
      self->audio_system = ::NTV2ChannelToAudioSystem(self->channel);
    if (!::NTV2DeviceCanDoFrameStore1Display(self->device_id))
      self->audio_system = ::NTV2_AUDIOSYSTEM_1;

    GST_DEBUG_OBJECT (self, "Using audio system %d", self->audio_system);

    self->device->SetNumberAudioChannels(self->configured_audio_channels, self->audio_system);
    self->device->SetAudioRate(::NTV2_AUDIO_48K, self->audio_system);
    self->device->SetAudioBufferSize(::NTV2_AUDIO_BUFFER_BIG, self->audio_system);
    self->device->SetSDIOutputAudioSystem(self->channel, self->audio_system);
    self->device->SetSDIOutputDS2AudioSystem(self->channel, self->audio_system);
    self->device->SetAudioLoopBack(::NTV2_AUDIO_LOOPBACK_OFF, self->audio_system);
  } else {
    self->audio_system = ::NTV2_AUDIOSYSTEM_INVALID;
  }

  CNTV2SignalRouter router;

  self->device->GetRouting(router);

  if (::NTV2DeviceHasBiDirectionalSDI (self->device_id))
    self->device->SetSDITransmitEnable(self->channel, true);

  const NTV2Standard outputStandard (::GetNTV2StandardFromVideoFormat(video_format));
  self->device->SetSDIOutputStandard (self->channel, outputStandard);

  const NTV2OutputCrosspointID framebuffer_id = ::GetFrameBufferOutputXptFromChannel(self->channel, false, false);
  const NTV2InputCrosspointID sdi_output_id = ::GetSDIOutputInputXpt (self->channel, false);

  // Need to remove old routes for the output and framebuffer we're going to use
  NTV2ActualConnections connections = router.GetConnections();

  for (NTV2ActualConnectionsConstIter iter = connections.begin(); iter != connections.end(); iter++) {
    if (iter->first == sdi_output_id ||
        iter->second == framebuffer_id)
      router.RemoveConnection(iter->first, iter->second);
  }

  GST_DEBUG_OBJECT (self, "Creating connection %d - %d", sdi_output_id, framebuffer_id);
  router.AddConnection (sdi_output_id, framebuffer_id);

  {
    std::stringstream os;
    CNTV2SignalRouter oldRouter;
    self->device->GetRouting(oldRouter);
    oldRouter.Print(os);
    GST_DEBUG_OBJECT (self, "Previous routing:\n%s", os.str().c_str());
  }
  self->device->ApplySignalRoute (router, true);
  {
    std::stringstream os;
    CNTV2SignalRouter currentRouter;
    self->device->GetRouting(currentRouter);
    currentRouter.Print(os);
    GST_DEBUG_OBJECT (self, "New routing:\n%s", os.str().c_str());
  }

  return TRUE;
}

static GstCaps *
gst_aja_sink_get_caps (GstBaseSink * bsink, GstCaps * filter)
{
  GstAjaSink *self = GST_AJA_SINK (bsink);
  GstCaps *caps;

  GST_OBJECT_LOCK (self);
  if (self->configured_caps) {
    caps = gst_caps_ref (self->configured_caps);
    GST_OBJECT_UNLOCK (self);
  } else if (self->device) {
    GST_OBJECT_UNLOCK (self);

    caps = gst_caps_new_empty ();

    for (gsize i = 0; i < G_N_ELEMENTS (supported_video_formats); i++) {
      NTV2VideoFormat format = supported_video_formats[i];

      if (::NTV2DeviceCanDoVideoFormat (self->device_id, format)) {
        gst_caps_append (caps, ntv2_video_format_to_caps (format));
      }
    }
  } else {
    GST_OBJECT_UNLOCK (self);
    caps = gst_pad_get_pad_template_caps (GST_BASE_SINK_PAD (self));
  }

  if (filter) {
    GstCaps *tmp = gst_caps_intersect_full (filter, caps, GST_CAPS_INTERSECT_FIRST);
    gst_caps_unref (caps);
    caps = tmp;
  }

  return caps;
}

static gboolean
gst_aja_sink_event (GstBaseSink * bsink, GstEvent * event)
{
  GstAjaSink *self = GST_AJA_SINK (bsink);

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_EOS: {
      GST_DEBUG_OBJECT (self, "Signalling EOS");

      g_mutex_lock (&self->queue_lock);
      self->eos = TRUE;
      g_cond_signal (&self->queue_cond);
      g_mutex_unlock (&self->queue_lock);

      break;
    }
    default:
      break;
  }

  return GST_BASE_SINK_CLASS (parent_class)->event (bsink, event);
}

static GstFlowReturn
gst_aja_sink_render (GstBaseSink * bsink, GstBuffer * buffer)
{
  GstAjaSink *self = GST_AJA_SINK (bsink);
  GstFlowReturn flow_ret = GST_FLOW_OK;
  GstAjaSinkAudioMeta *meta;
  GstVideoFrame in_frame;
  GstMapInfo audio_map;
  GstBuffer *item_buffer, *item_audio_buffer;
  QueueItem item;

  if (!gst_video_frame_map (&in_frame, &self->configured_info, buffer, GST_MAP_READ)) {
    GST_ERROR_OBJECT (self, "Failed to map buffer");
    return GST_FLOW_ERROR;
  }

  flow_ret = gst_buffer_pool_acquire_buffer (self->buffer_pool, &item_buffer, NULL);
  if (flow_ret != GST_FLOW_OK) {
    gst_video_frame_unmap (&in_frame);
    return flow_ret;
  }

  meta = gst_buffer_get_aja_sink_audio_meta (buffer);
  if (meta) {
    flow_ret = gst_buffer_pool_acquire_buffer (self->audio_buffer_pool, &item_audio_buffer, NULL);
    if (flow_ret != GST_FLOW_OK) {
      gst_video_frame_unmap (&in_frame);
      gst_buffer_unref (item_buffer);
      return flow_ret;
    }
  }

  item.type = QUEUE_ITEM_TYPE_FRAME;
  gst_video_frame_map (&item.frame, &self->configured_info, item_buffer, GST_MAP_WRITE);
  gst_video_frame_copy (&item.frame, &in_frame);
  gst_video_frame_unmap (&in_frame);
  gst_buffer_unref (item_buffer);

  if (meta) {
    gst_buffer_map (meta->buffer, &audio_map, GST_MAP_READ);
    gst_buffer_fill (item_audio_buffer, 0, audio_map.data, audio_map.size);
    gst_buffer_unmap (meta->buffer, &audio_map);
    gst_buffer_set_size (item_audio_buffer, audio_map.size);

    gst_buffer_map (item_audio_buffer, &item.audio_map, GST_MAP_READ);
    item.audio_buffer = item_audio_buffer;
  } else {
    item.audio_buffer = NULL;
  }

  g_mutex_lock (&self->queue_lock);
  while (gst_queue_array_get_length (self->queue) >= 16) {
    QueueItem *tmp = (QueueItem *) gst_queue_array_pop_head_struct (self->queue);

    if (tmp->type == QUEUE_ITEM_TYPE_FRAME) {
      GST_WARNING_OBJECT (self, "Dropping old frame");
      gst_video_frame_unmap (&tmp->frame);
      if (tmp->audio_buffer) {
        gst_buffer_unmap (tmp->audio_buffer, &tmp->audio_map);
        gst_buffer_unref (tmp->audio_buffer);
      }
    }
  }

  GST_DEBUG_OBJECT (self, "Queuing frame %p", GST_VIDEO_FRAME_PLANE_DATA (&item.frame, 0));
  gst_queue_array_push_tail_struct (self->queue, &item);
  GST_DEBUG_OBJECT (self, "%u frames queued", gst_queue_array_get_length (self->queue));
  g_cond_signal (&self->queue_cond);
  g_mutex_unlock (&self->queue_lock);

  return flow_ret;
}

static void
output_thread_func (AJAThread * thread, void * data)
{
  GstAjaSink *self = GST_AJA_SINK (data);

  g_mutex_lock (&self->queue_lock);
restart:
  GST_DEBUG_OBJECT (self, "Waiting for playing or shutdown");
  while ((!self->playing && !self->shutdown) || (self->playing && gst_queue_array_get_length (self->queue) < self->queue_size / 2 && !self->eos))
    g_cond_wait (&self->queue_cond, &self->queue_lock);
  if (self->shutdown) {
    GST_DEBUG_OBJECT (self, "Shutting down");
    g_mutex_unlock (&self->queue_lock);
    return;
  }

  GST_DEBUG_OBJECT (self, "Starting playing");
  g_mutex_unlock (&self->queue_lock);

  self->device->AutoCirculateStop (self->channel);

  // FIXME: Needs global locking because the device state is global
  self->device->SubscribeOutputVerticalEvent (self->channel);
  if (!self->device->AutoCirculateInitForOutput (self->channel,
                                                 self->queue_size / 2,
                                                 self->audio_system,
                                                 0, // TODO RP188 etc
                                                 1)) {
    GST_ELEMENT_ERROR (self, STREAM, FAILED, (NULL), ("Failed to initialize autocirculate"));
    goto out;
  }
  self->device->AutoCirculateStart (self->channel);

  g_mutex_lock (&self->queue_lock);
  while (self->playing && !self->shutdown) {
    AUTOCIRCULATE_STATUS status;

    self->device->AutoCirculateGetStatus (self->channel, status);

    GST_DEBUG_OBJECT (self,
                      "Start frame %d "
                      "end frame %d "
                      "active frame %d "
                      "start time %" G_GUINT64_FORMAT " "
                      "current time %" G_GUINT64_FORMAT " "
                      "frames processed %u "
                      "frames dropped %u "
                      "buffer level %u",
                      status.acStartFrame, status.acEndFrame,
                      status.acActiveFrame,
                      status.acRDTSCStartTime,
                      status.acRDTSCCurrentTime,
                      status.acFramesProcessed,
                      status.acFramesDropped,
                      status.acBufferLevel
    );

    if (status.GetNumAvailableOutputFrames() > 1) {
      QueueItem item, *item_p;

      while ((item_p = (QueueItem *) gst_queue_array_pop_head_struct (self->queue)) == NULL && self->playing && !self->shutdown) {
        GST_DEBUG_OBJECT (self, "Waiting for more frames or shutdown");
        g_cond_wait (&self->queue_cond, &self->queue_lock);
      }

      if (!self->playing || self->shutdown) {
        if (item_p && item_p->type == QUEUE_ITEM_TYPE_FRAME) {
          gst_video_frame_unmap (&item_p->frame);
          if (item_p->audio_buffer) {
            gst_buffer_unmap (item_p->audio_buffer, &item_p->audio_map);
            gst_buffer_unref (item_p->audio_buffer);
          }
        }
        break;
      }

      if (item_p && item_p->type != QUEUE_ITEM_TYPE_FRAME) {
        continue;
      }

      GST_DEBUG_OBJECT (self, "%u frames queued", gst_queue_array_get_length (self->queue));

      item = *item_p;
      g_mutex_unlock (&self->queue_lock);

      GST_DEBUG_OBJECT (self, "Transferring frame: "
                        "Video %p %" G_GSIZE_FORMAT " "
                        "Audio %p %" G_GSIZE_FORMAT,
                        GST_VIDEO_FRAME_PLANE_DATA (&item.frame, 0), GST_VIDEO_FRAME_SIZE (&item.frame),
                        item.audio_buffer ? item.audio_map.data : NULL, item.audio_buffer ? item.audio_map.size : 0);

      AUTOCIRCULATE_TRANSFER transfer;

      transfer.SetVideoBuffer ((guint*) GST_VIDEO_FRAME_PLANE_DATA (&item.frame, 0), GST_VIDEO_FRAME_SIZE (&item.frame));
      if (item.audio_buffer) {
        transfer.SetAudioBuffer((guint *) item.audio_map.data, item.audio_map.size);
      }

      if (!self->device->AutoCirculateTransfer (self->channel, transfer)) {
        GST_WARNING_OBJECT (self, "Failed to transfer frame");
      }

      gst_video_frame_unmap (&item.frame);

      if (item.audio_buffer) {
        gst_buffer_unmap (item.audio_buffer, &item.audio_map);
        gst_buffer_unref (item.audio_buffer);
      }

      g_mutex_lock (&self->queue_lock);
    } else {
      g_mutex_unlock (&self->queue_lock);

      self->device->WaitForOutputVerticalInterrupt (self->channel);

      g_mutex_lock (&self->queue_lock);
    }
  }

  if (!self->playing && !self->shutdown)
    goto restart;
  g_mutex_unlock (&self->queue_lock);

out:
  // FIXME: Needs global locking because the device state is global
  self->device->AutoCirculateStop (self->channel);
  self->device->UnsubscribeOutputVerticalEvent (self->channel);

  GST_DEBUG_OBJECT (self, "Stopped");
}
