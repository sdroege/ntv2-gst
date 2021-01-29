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

typedef enum {
  QUEUE_ITEM_TYPE_FRAME,
} QueueItemType;

typedef struct {
  QueueItemType type;
  GstVideoFrame frame;
} QueueItem;

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

  element_class->change_state =
      GST_DEBUG_FUNCPTR (gst_aja_sink_change_state);

  basesink_class->unlock = GST_DEBUG_FUNCPTR (gst_aja_sink_unlock);
  basesink_class->unlock_stop =
      GST_DEBUG_FUNCPTR (gst_aja_sink_unlock_stop);
  basesink_class->set_caps =
      GST_DEBUG_FUNCPTR (gst_aja_sink_set_caps);
  basesink_class->get_caps =
      GST_DEBUG_FUNCPTR (gst_aja_sink_get_caps);
  basesink_class->event =
      GST_DEBUG_FUNCPTR (gst_aja_sink_event);
  basesink_class->render =
      GST_DEBUG_FUNCPTR (gst_aja_sink_render);

  // TODO:
  // - Define a combined audio/video caps format
  // - Write a muxer and surrounding bin (with videorate and audiobuffersplit)
  //   to feed this sink properly
  // - Propose_allocation for providing an AJA buffer pool to upstream from
  //   the muxer: needs to be audio/video specific
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
gst_aja_sink_init (GstAjaSink * self)
{
  // TODO:
  // - We preroll 8 frames in the driver, should be configurable
  // - Our ringbuffer is twice the size
  gst_base_sink_set_render_delay (GST_BASE_SINK (self), 8 * GST_SECOND / 30);
  self->queue = gst_queue_array_new_for_struct (sizeof (QueueItem), 16);
}

void
gst_aja_sink_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstAjaSink *self = GST_AJA_SINK (object);

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
  GstAjaSink *self = GST_AJA_SINK (object);

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_aja_sink_finalize (GObject * object)
{
  GstAjaSink *self = GST_AJA_SINK (object);
  QueueItem *item;

  g_assert (self->device == NULL);

  while ((item = (QueueItem *) gst_queue_array_pop_head_struct (self->queue))) {
    // TODO: free item content
  }
  g_clear_pointer (&self->queue, gst_queue_array_free);

  delete self->output_thread;
  self->output_thread = NULL;

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static gboolean
gst_aja_sink_unlock (GstBaseSink * bsink)
{
  GstAjaSink *self = GST_AJA_SINK (bsink);

  return TRUE;
}

static gboolean
gst_aja_sink_unlock_stop (GstBaseSink * bsink)
{
  GstAjaSink *self = GST_AJA_SINK (bsink);

  return TRUE;
}

static gboolean
gst_aja_sink_open (GstAjaSink * self)
{
  GST_DEBUG_OBJECT (self, "Opening device");

  g_assert (self->device == NULL);

  self->device = new CNTV2Device ();

  // TODO: Make property
  if (!CNTV2DeviceScanner::GetFirstDeviceFromArgument ("0", *self->device)) {
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

  self->device->SetEveryFrameServices (NTV2_OEM_TASKS);
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

  self->device->SetMultiFormatMode(true);

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
  GST_OBJECT_UNLOCK (self);

  if (self->buffer_pool) {
    gst_buffer_pool_set_active (self->buffer_pool, FALSE);
    gst_clear_object (&self->buffer_pool);
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
  gst_caps_replace (&self->configured_caps, caps);
  GST_OBJECT_UNLOCK (self);

  // TODO: Parse caps

  g_assert (self->device != NULL);

  // FIXME: Needs global locking because the device state is global

  if (!::NTV2DeviceCanDoVideoFormat (self->device_id, ::NTV2_FORMAT_1080p_3000)) {
    GST_ERROR_OBJECT (self, "Device does not support mode");
    return FALSE;
  }
  self->device->SetVideoFormat (::NTV2_FORMAT_1080p_3000, false, false, ::NTV2_CHANNEL1);

  if (!::NTV2DeviceCanDoFrameBufferFormat (self->device_id, ::NTV2_FBF_10BIT_YCBCR)) {
    GST_ERROR_OBJECT (self, "Device does not support frame buffer format");
    return FALSE;
  }
  self->device->SetFrameBufferFormat (::NTV2_CHANNEL1, ::NTV2_FBF_10BIT_YCBCR);

  // TODO: Property to lock to SDI1, etc
  self->device->SetFramePulseReference (::NTV2_REFERENCE_FREERUN);

  self->device->EnableChannel (::NTV2_CHANNEL1);

  // TODO: Enable VANC as needed
  NTV2Standard standard;
  NTV2FrameGeometry geometry;
  self->device->GetStandard (standard);
  self->device->GetFrameGeometry (geometry);
  self->device->SetVANCMode(::NTV2_VANCMODE_OFF, standard, geometry, ::NTV2_CHANNEL1);

  // TODO: Configure audio

  // TODO: Do the buffer pool in the muxer for video, on demand. And also pass
  // it upstream in propose_allocation.

  // TODO: Handle VANC
  guint video_buffer_size = GetVideoActiveSize (::NTV2_FORMAT_1080p_3000, ::NTV2_FBF_10BIT_YCBCR, NTV2_VANCMODE_OFF);

  self->device->DMABufferAutoLock(false, true, 0);

  // TODO: No hardcoding
  GstAllocator *video_alloc = gst_aja_allocator_new(self->device, video_buffer_size, 16);

  self->buffer_pool = gst_aja_buffer_pool_new ();
  GstStructure *config = gst_buffer_pool_get_config (self->buffer_pool);
  gst_buffer_pool_config_set_params (config, NULL, video_buffer_size,
      16, 0);
  gst_buffer_pool_config_set_allocator (config, video_alloc, NULL);
  gst_structure_set (config, "is-video", G_TYPE_BOOLEAN, TRUE, NULL);
  gst_buffer_pool_set_config (self->buffer_pool, config);
  gst_buffer_pool_set_active (self->buffer_pool, TRUE);
  gst_object_unref (video_alloc);

  CNTV2SignalRouter router;

  self->device->GetRouting(router);

  if (::NTV2DeviceHasBiDirectionalSDI (self->device_id))
    self->device->SetSDITransmitEnable(::NTV2_CHANNEL1, true);

  const NTV2Standard outputStandard (::GetNTV2StandardFromVideoFormat(::NTV2_FORMAT_1080p_3000));
  self->device->SetSDIOutputStandard (::NTV2_CHANNEL1, outputStandard);

  router.AddConnection (::NTV2_XptSDIOut1Input, ::NTV2_XptFrameBuffer1YUV);

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
  if (self->configured_caps)
    caps = gst_caps_ref (self->configured_caps);
  else
    caps = gst_pad_get_pad_template_caps (GST_BASE_SINK_PAD (self));
  GST_OBJECT_UNLOCK (self);

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
  GstVideoFrame in_frame;
  GstBuffer *item_buffer;
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

  item.type = QUEUE_ITEM_TYPE_FRAME;
  gst_video_frame_map (&item.frame, &self->configured_info, item_buffer, GST_MAP_WRITE);
  gst_video_frame_copy (&item.frame, &in_frame);
  gst_video_frame_unmap (&in_frame);
  gst_buffer_unref (item_buffer);

  g_mutex_lock (&self->queue_lock);
  while (gst_queue_array_get_length (self->queue) >= 16) {
    QueueItem *tmp = (QueueItem *) gst_queue_array_pop_head_struct (self->queue);

    if (tmp->type == QUEUE_ITEM_TYPE_FRAME) {
      GST_WARNING_OBJECT (self, "Dropping old frame");
      gst_video_frame_unmap (&tmp->frame);
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
  while ((!self->playing && !self->shutdown) || (self->playing && gst_queue_array_get_length (self->queue) < 8 && !self->eos))
    g_cond_wait (&self->queue_cond, &self->queue_lock);
  if (self->shutdown) {
    GST_DEBUG_OBJECT (self, "Shutting down");
    g_mutex_unlock (&self->queue_lock);
    return;
  }

  GST_DEBUG_OBJECT (self, "Starting playing");
  g_mutex_unlock (&self->queue_lock);

  self->device->SubscribeOutputVerticalEvent (::NTV2_CHANNEL1);

  self->device->AutoCirculateStop (::NTV2_CHANNEL1);
  self->device->AutoCirculateInitForOutput (::NTV2_CHANNEL1,
                                            7,
                                            ::NTV2_AUDIOSYSTEM_INVALID,
                                            0,
                                            1,
                                            0, 7);
  self->device->AutoCirculateStart (::NTV2_CHANNEL1);

  g_mutex_lock (&self->queue_lock);
  while (self->playing && !self->shutdown) {
    AUTOCIRCULATE_STATUS status;

    self->device->AutoCirculateGetStatus (::NTV2_CHANNEL1, status);

    GST_DEBUG_OBJECT (self,
                      "Start frame %d "
                      "end frame %d "
                      "active frame %d "
                      "start time %" G_GUINT64_FORMAT " "
                      "current time %" G_GUINT64_FORMAT " "
                      "frames processed %u "
                      "frames dropped %u "
                      "buffer level %u "
                      "frames per second %f",
                      status.acStartFrame, status.acEndFrame,
                      status.acActiveFrame,
                      status.acRDTSCStartTime,
                      status.acRDTSCCurrentTime,
                      status.acFramesProcessed,
                      status.acFramesDropped,
                      status.acBufferLevel,
                      ((double)status.acFramesProcessed+status.acFramesDropped) / (((double)status.acRDTSCCurrentTime - status.acRDTSCStartTime) / 10000000.0)
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
                        "Video %p %" G_GSIZE_FORMAT,
                        GST_VIDEO_FRAME_PLANE_DATA (&item.frame, 0), GST_VIDEO_FRAME_SIZE (&item.frame));

      AUTOCIRCULATE_TRANSFER transfer;

      transfer.SetVideoBuffer ((guint*) GST_VIDEO_FRAME_PLANE_DATA (&item.frame, 0), GST_VIDEO_FRAME_SIZE (&item.frame));
      if (!self->device->AutoCirculateTransfer (::NTV2_CHANNEL1, transfer)) {
        GST_WARNING_OBJECT (self, "Failed to transfer frame");
      }

      gst_video_frame_unmap (&item.frame);

      g_mutex_lock (&self->queue_lock);
    } else {
      g_mutex_unlock (&self->queue_lock);

      self->device->WaitForOutputVerticalInterrupt (::NTV2_CHANNEL1);

      g_mutex_lock (&self->queue_lock);
    }
  }

  if (!self->playing && !self->shutdown)
    goto restart;
  g_mutex_unlock (&self->queue_lock);

  self->device->AutoCirculateStop (::NTV2_CHANNEL1);
  self->device->UnsubscribeOutputVerticalEvent (::NTV2_CHANNEL1);

  GST_DEBUG_OBJECT (self, "Stopped");
}
