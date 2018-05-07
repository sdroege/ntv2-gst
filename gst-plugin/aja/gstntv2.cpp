/**
    @file        ntv2encodehevc.cpp
    @brief        Implementation of NTV2EncodeHEVC class.
    @copyright    Copyright (C) 2015 AJA Video Systems, Inc.  All rights reserved.
                  Copyright (C) 2017 Sebastian Dröge <sebastian@centricular.com>
**/

#include <stdio.h>

#include "gstntv2.h"
#include "gstaja.h"
#include "ntv2utils.h"
#include "ntv2devicefeatures.h"
#include "ajabase/system/process.h"
#include "ajabase/system/systemtime.h"

GST_DEBUG_CATEGORY_STATIC (gst_ntv2_debug);
#define GST_CAT_DEFAULT gst_ntv2_debug

#define NTV2_AUDIOSIZE_MAX        (401 * 1024)

static void
_init_ntv2_debug (void)
{
#ifndef GST_DISABLE_GST_DEBUG
  static volatile gsize _init = 0;

  if (g_once_init_enter (&_init)) {
    GST_DEBUG_CATEGORY_INIT (gst_ntv2_debug, "ajantv2", 0, "AJA ntv2");
    g_once_init_leave (&_init, 1);
  }
#endif
}

NTV2GstAV::NTV2GstAV (const string inDeviceSpecifier,
    const NTV2Channel inChannel)

:



mACInputThread (NULL),
mCodecRawThread (NULL),
mCodecHevcThread (NULL),
mM31 (NULL),
mLock (new AJALock),
mDeviceID (DEVICE_ID_NOTFOUND),
mDeviceSpecifier (inDeviceSpecifier),
mInputChannel (inChannel),
mEncodeChannel (M31_CH0),
mInputSource (NTV2_INPUTSOURCE_SDI1),
mVideoFormat (NTV2_MAX_NUM_VIDEO_FORMATS),
mMultiStream (false),
mAudioSystem (NTV2_AUDIOSYSTEM_1),
mNumAudioChannels (0),
mLastFrame (false),
mLastFrameInput (false),
mLastFrameVideoOut (false),
mLastFrameHevc (false),
mLastFrameHevcOut (false),
mLastFrameAudioOut (false),
mGlobalQuit (false),
mStarted (false),
mVideoCallback (0),
mVideoCallbackRefcon (0),
mAudioCallback (0),
mAudioCallbackRefcon (0),
mAudioBufferPool (NULL),
mVideoBufferPool (NULL),
mVideoInputFrameCount (0),
mVideoOutFrameCount (0),
mCodecRawFrameCount (0),
mCodecHevcFrameCount (0), mHevcOutFrameCount (0), mAudioOutFrameCount (0)
{
  ::memset (mHevcInputBuffer, 0x0, sizeof (mHevcInputBuffer));

  _init_ntv2_debug ();
}                               //    constructor


NTV2GstAV::~NTV2GstAV ()
{
  //    Stop my capture and consumer threads, then destroy them...
  Quit ();

  if (mM31 != NULL) {
    delete mM31;
    mM31 = NULL;
  }
  // unsubscribe from input vertical event...
  mDevice.UnsubscribeInputVerticalEvent (mInputChannel);

  FreeHostBuffers ();

  delete mLock;
  mLock = NULL;


}                               // destructor


AJAStatus NTV2GstAV::Open (void)
{
  if (mDeviceID != DEVICE_ID_NOTFOUND)
    return AJA_STATUS_SUCCESS;

  //    Open the device...
  if (!CNTV2DeviceScanner::GetFirstDeviceFromArgument (mDeviceSpecifier,
          mDevice)) {
    GST_ERROR ("ERROR: Device not found");
    return AJA_STATUS_OPEN;
  }

  mDevice.SetEveryFrameServices (NTV2_OEM_TASKS);       //    Since this is an OEM app, use the OEM service level
  mDeviceID = mDevice.GetDeviceID ();   //    Keep the device ID handy, as it's used frequently

  // So we can configure each channel separately
  mDevice.SetMultiFormatMode(true);

  return AJA_STATUS_SUCCESS;
}


AJAStatus NTV2GstAV::Close (void)
{
  AJAStatus
  status (AJA_STATUS_SUCCESS);

  mDeviceID = DEVICE_ID_NOTFOUND;;

  return status;
}


AJAStatus
    NTV2GstAV::Init (const M31VideoPreset inPreset,
    const NTV2VideoFormat inVideoFormat,
    const NTV2InputSource inInputSource,
    const uint32_t inBitDepth,
    const bool inIs422,
    const bool inIsAuto,
    const bool inHevcOutput,
    const bool inQuadMode,
    const NTV2TCIndex inTimeCode,
    const bool inInfoData,
    const bool inCaptureTall,
    const bool inPassthrough)
{
  AJAStatus status (AJA_STATUS_SUCCESS);

  mPreset = inPreset;
  mVideoFormat = inVideoFormat;
  mVideoSource = inInputSource;
  mBitDepth = inBitDepth;
  mIs422 = inIs422;
  mIsAuto = inIsAuto;
  mHevcOutput = inHevcOutput;
  mQuad = inQuadMode;
  mTimecodeMode = inTimeCode;
  mWithInfo = inInfoData;
  mCaptureTall = inCaptureTall;
  mPassthrough = mPassthrough;

  if (mQuad) {
    if (mInputChannel != NTV2_CHANNEL1 && mInputChannel != NTV2_CHANNEL5) {
      GST_ERROR ("Quad mode requires channel 1 or 5");
      return AJA_STATUS_FAIL;
    }
  }

  //  If we are in auto mode then do nothing if we are already running, otherwise force raw, 422, 8 bit.
  //  This flag shoud only be driven by the audiosrc to either start a non running channel without having
  //  to know anything about the video or to latch onto an alreay running channel in the event it has been
  //  started by the videosrc or hevcsrc.
  if (mIsAuto) {
    if (mStarted)
      return AJA_STATUS_SUCCESS;

    //  Get SDI input format
    status = DetermineInputFormat (mInputChannel, mQuad, mVideoFormat);
    if (AJA_FAILURE (status))
      return status;

    mBitDepth = 8;
    mHevcOutput = false;
  }

  // Ensure that mCaptureTall is only set for formats that can
  // actually contain VANC and that we handle
  switch (mVideoFormat) {
    case NTV2_FORMAT_720p_5000:
    case NTV2_FORMAT_720p_5994:
    case NTV2_FORMAT_720p_6000:
    case NTV2_FORMAT_1080i_5000:
    case NTV2_FORMAT_1080i_5994:
    case NTV2_FORMAT_1080i_6000:
    case NTV2_FORMAT_1080p_2500:
    case NTV2_FORMAT_1080p_3000:
    case NTV2_FORMAT_1080p_5000_A:
    case NTV2_FORMAT_1080p_5994_A:
    case NTV2_FORMAT_1080p_6000_A:
      break;
    default:
      mCaptureTall = false;
      break;
    }

  //    Setup frame buffer
  status = SetupVideo ();
  if (AJA_FAILURE (status)) {
    GST_ERROR ("Video setup failure");
    return status;
  }

  //    Setup codec
  if (mHevcOutput) {
    status = SetupHEVC ();
    if (AJA_FAILURE (status)) {
      GST_ERROR ("Encoder setup failure");
      return status;
    }
  }

  return AJA_STATUS_SUCCESS;
}

AJAStatus
NTV2GstAV::InitAudio (const NTV2AudioSource inAudioSource, uint32_t * numAudioChannels)
{
  AJAStatus
  status (AJA_STATUS_SUCCESS);

  mAudioSource = inAudioSource;
  mNumAudioChannels = *numAudioChannels;

  //    Setup audio buffer
  status = SetupAudio ();
  if (AJA_FAILURE (status)) {
    GST_ERROR ("Audio setup failure");
    return status;
  }

  ULWord
      nchannels = -1;
  mDevice.GetNumberAudioChannels (nchannels, mAudioSystem);
  *numAudioChannels = nchannels;

  return status;
}

void
NTV2GstAV::Quit (void)
{
  if (!mLastFrame && !mGlobalQuit) {
    //    Set the last frame flag to start the quit process
    mLastFrame = true;

    //    Wait for the last frame to be written to disk
    int i;
    int timeout = 300;
    for (i = 0; i < timeout; i++) {
      if (mHevcOutput) {
        if (mLastFrameHevcOut && mLastFrameAudioOut)
          break;
      } else {
        if (mLastFrameVideoOut && mLastFrameAudioOut)
          break;
      }
      AJATime::Sleep (10);
    }

    if (i == timeout)
      GST_ERROR ("ERROR: Wait for last frame timeout");

    if (mM31 && mHevcOutput) {
      //    Stop the encoder stream
      if (!mM31->ChangeEHState (Hevc_EhState_ReadyToStop, mEncodeChannel))
        GST_ERROR ("ERROR: ChangeEHState ready to stop failed");

      if (!mM31->ChangeEHState (Hevc_EhState_Stop, mEncodeChannel))
        GST_ERROR ("ERROR: ChangeEHState stop failed");

      // stop the video input stream
      if (!mM31->ChangeVInState (Hevc_VinState_Stop, mEncodeChannel))
        GST_ERROR ("ERROR: ChangeVInState stop failed");

      if (!mMultiStream) {
        //    Now go to the init state
        if (!mM31->ChangeMainState (Hevc_MainState_Init,
                Hevc_EncodeMode_Single))
          GST_ERROR ("ERROR: ChangeMainState to init failed");
      }
    }
  }
  //    Stop the worker threads
  mGlobalQuit = true;
  mStarted = false;

  StopACThread ();
  StopCodecRawThread ();
  StopCodecHevcThread ();
  FreeHostBuffers ();

  //  Stop video capture
  mDevice.SetMode (mInputChannel, NTV2_MODE_DISPLAY, false);
  if (mQuad) {
    mDevice.SetMode ((NTV2Channel) (mInputChannel + 1), NTV2_MODE_DISPLAY, false);
    mDevice.SetMode ((NTV2Channel) (mInputChannel + 2), NTV2_MODE_DISPLAY, false);
    mDevice.SetMode ((NTV2Channel) (mInputChannel + 3), NTV2_MODE_DISPLAY, false);
  }
}


AJAStatus NTV2GstAV::SetupHEVC (void)
{
  HevcMainState
      mainState;
  HevcEncodeMode
      encodeMode;
  HevcVinState
      vInState;
  HevcEhState
      ehState;

  // Allocate our M31 helper class
  mM31 = new CNTV2m31 (&mDevice);

  if (mMultiStream) {
    mM31->GetMainState (&mainState, &encodeMode);
    if ((mainState != Hevc_MainState_Encode)
        || (encodeMode != Hevc_EncodeMode_Multiple)) {
      // Here we need to start up the M31 so we reset the part then go into the init state
      if (!mM31->Reset ()) {
        GST_ERROR ("ERROR: Reset of M31 failed");
        return AJA_STATUS_INITIALIZE;
      }
      // After a reset we should be in the boot state so lets check this
      mM31->GetMainState (&mainState);
      if (mainState != Hevc_MainState_Boot) {
        GST_ERROR ("ERROR: Not in boot state after reset");
        return AJA_STATUS_INITIALIZE;
      }
      // Now go to the init state
      if (!mM31->ChangeMainState (Hevc_MainState_Init,
              Hevc_EncodeMode_Multiple)) {
        GST_ERROR ("ERROR: ChangeMainState to init failed");
        return AJA_STATUS_INITIALIZE;
      }

      mM31->GetMainState (&mainState);
      if (mainState != Hevc_MainState_Init) {
        GST_ERROR ("ERROR: Not in init state after change");
        return AJA_STATUS_INITIALIZE;
      }
      // Now lets configure the device for a given preset.  First we must clear out all of the params which
      // is necessary since the param space is basically uninitialized memory.
      mM31->ClearAllParams ();

      // Load and set common params for all channels
      if (!mM31->SetupCommonParams (mPreset, M31_CH0)) {
        GST_ERROR ("ERROR: SetCommonParams failed ch0");
        return AJA_STATUS_INITIALIZE;
      }
      // Change state to encode
      if (!mM31->ChangeMainState (Hevc_MainState_Encode,
              Hevc_EncodeMode_Multiple)) {
        GST_ERROR ("ERROR: ChangeMainState to encode failed");
        return AJA_STATUS_INITIALIZE;
      }

      mM31->GetMainState (&mainState);
      if (mainState != Hevc_MainState_Encode) {
        GST_ERROR ("ERROR: Not in encode state after change");
        return AJA_STATUS_INITIALIZE;
      }
    }
    // Write out stream params
    if (!mM31->SetupVIParams (mPreset, mEncodeChannel)) {
      GST_ERROR ("ERROR: SetupVIParams failed");
      return AJA_STATUS_INITIALIZE;
    }
    if (!mM31->SetupVInParams (mPreset, mEncodeChannel)) {
      GST_ERROR ("ERROR: SetupVinParams failed");
      return AJA_STATUS_INITIALIZE;
    }
    if (!mM31->SetupVAParams (mPreset, mEncodeChannel)) {
      GST_ERROR ("ERROR: SetupVAParams failed");
      return AJA_STATUS_INITIALIZE;
    }
    if (!mM31->SetupEHParams (mPreset, mEncodeChannel)) {
      GST_ERROR ("ERROR: SetupEHParams failed");
      return AJA_STATUS_INITIALIZE;
    }

    if (mWithInfo) {
      // Enable picture information
      if (!mM31->mpM31VInParam->SetPTSMode (M31_PTSModeHost,
              (M31VirtualChannel) mEncodeChannel)) {
        GST_ERROR ("ERROR: SetPTSMode failed");
        return AJA_STATUS_INITIALIZE;
      }
    }
    // Now that we have setup the M31 lets change the VIn and EH states for channel 0 to start
    if (!mM31->ChangeVInState (Hevc_VinState_Start, mEncodeChannel)) {
      GST_ERROR ("ERROR: ChangeVInState failed");
      return AJA_STATUS_INITIALIZE;
    }

    mM31->GetVInState (&vInState, mEncodeChannel);
    if (vInState != Hevc_VinState_Start) {
      GST_ERROR ("ERROR: VIn didn't start = %d", vInState);
      return AJA_STATUS_INITIALIZE;
    }

    if (!mM31->ChangeEHState (Hevc_EhState_Start, mEncodeChannel)) {
      GST_ERROR ("ERROR: ChangeEHState failed");
      return AJA_STATUS_INITIALIZE;
    }

    mM31->GetEHState (&ehState, mEncodeChannel);
    if (ehState != Hevc_EhState_Start) {
      GST_ERROR ("ERROR: EH didn't start = %d", ehState);
      return AJA_STATUS_INITIALIZE;
    }
  } else {
    // if we are in the init state assume that last stop was good
    // otherwise reset the codec
    mM31->GetMainState (&mainState, &encodeMode);
    if ((mainState != Hevc_MainState_Init)
        || (encodeMode != Hevc_EncodeMode_Single)) {
      // Here we need to start up the M31 so we reset the part then go into the init state
      if (!mM31->Reset ()) {
        GST_ERROR ("ERROR: Reset of M31 failed");
        return AJA_STATUS_INITIALIZE;
      }
      // After a reset we should be in the boot state so lets check this
      mM31->GetMainState (&mainState);
      if (mainState != Hevc_MainState_Boot) {
        GST_ERROR ("ERROR: Not in boot state after reset");
        return AJA_STATUS_INITIALIZE;
      }
      // Now go to the init state
      if (!mM31->ChangeMainState (Hevc_MainState_Init, Hevc_EncodeMode_Single)) {
        GST_ERROR ("ERROR: ChangeMainState to init failed");
        return AJA_STATUS_INITIALIZE;
      }

      mM31->GetMainState (&mainState);
      if (mainState != Hevc_MainState_Init) {
        GST_ERROR ("ERROR: Not in init state after change");
        return AJA_STATUS_INITIALIZE;
      }
    }
    // Now lets configure the device for a given preset.  First we must clear out all of the params which
    // is necessary since the param space is basically uninitialized memory.
    mM31->ClearAllParams ();

    // Now load params for M31 preset into local structures in CNTV2m31
    if (!mM31->LoadAllParams (mPreset)) {
      GST_ERROR ("ERROR: LoadAllPresets failed");
      return AJA_STATUS_INITIALIZE;
    }
    // Here is where you can alter params sent to the M31 because all of these structures are public

    // Write out all of the params to each of the 4 physical channels
    if (!mM31->SetAllParams (M31_CH0)) {
      GST_ERROR ("ERROR: SetVideoPreset failed ch0");
      return AJA_STATUS_INITIALIZE;
    }

    if (!mM31->SetAllParams (M31_CH1)) {
      GST_ERROR ("ERROR: SetVideoPreset failed ch1");
      return AJA_STATUS_INITIALIZE;
    }

    if (!mM31->SetAllParams (M31_CH2)) {
      GST_ERROR ("ERROR: SetVideoPreset failed ch2");
      return AJA_STATUS_INITIALIZE;
    }

    if (!mM31->SetAllParams (M31_CH3)) {
      GST_ERROR ("ERROR: SetVideoPreset failed ch3");
      return AJA_STATUS_INITIALIZE;
    }

    if (mWithInfo) {
      // Enable picture information
      if (!mM31->mpM31VInParam->SetPTSMode (M31_PTSModeHost,
              (M31VirtualChannel) M31_CH0)) {
        GST_ERROR ("ERROR: SetPTSMode failed");
        return AJA_STATUS_INITIALIZE;
      }
    }
    // Change state to encode
    if (!mM31->ChangeMainState (Hevc_MainState_Encode, Hevc_EncodeMode_Single)) {
      GST_ERROR ("ERROR: ChangeMainState to encode failed");
      return AJA_STATUS_INITIALIZE;
    }

    mM31->GetMainState (&mainState);
    if (mainState != Hevc_MainState_Encode) {
      GST_ERROR ("ERROR: Not in encode state after change");
      return AJA_STATUS_INITIALIZE;
    }
    // Now that we have setup the M31 lets change the VIn and EH states for channel 0 to start
    if (!mM31->ChangeVInState (Hevc_VinState_Start, 0x01)) {
      GST_ERROR ("ERROR: ChangeVInState failed");
      return AJA_STATUS_INITIALIZE;
    }

    mM31->GetVInState (&vInState, M31_CH0);
    if (vInState != Hevc_VinState_Start) {
      GST_ERROR ("ERROR: VIn didn't start = %d", vInState);
      return AJA_STATUS_INITIALIZE;
    }

    if (!mM31->ChangeEHState (Hevc_EhState_Start, 0x01)) {
      GST_ERROR ("ERROR: ChangeEHState failed");
      return AJA_STATUS_INITIALIZE;
    }

    mM31->GetEHState (&ehState, M31_CH0);
    if (ehState != Hevc_EhState_Start) {
      GST_ERROR ("ERROR: EH didn't start = %d", ehState);
      return AJA_STATUS_INITIALIZE;
    }
  }

  return AJA_STATUS_SUCCESS;
}


AJAStatus NTV2GstAV::SetupVideo (void)
{
  // Figure out frame buffer format
  if (mHevcOutput) {
    if (mBitDepth == 8) {
      if (mIs422)
        mPixelFormat = NTV2_FBF_8BIT_YCBCR_422PL2;
      else
        mPixelFormat = NTV2_FBF_8BIT_YCBCR_420PL2;
    } else {
      if (mIs422)
        mPixelFormat = NTV2_FBF_10BIT_YCBCR_422PL2;
      else
        mPixelFormat = NTV2_FBF_10BIT_YCBCR_420PL2;
    }
  } else {
    if (mBitDepth == 8)
      mPixelFormat = NTV2_FBF_8BIT_YCBCR;
    else
      mPixelFormat = NTV2_FBF_10BIT_YCBCR;
  }

  // Enable and subscribe to the interrupts for the channel to be used...
  mDevice.EnableOutputInterrupt ();
  mDevice.EnableInputInterrupt (mInputChannel);
  mDevice.SubscribeInputVerticalEvent (mInputChannel);

  // Enable input channel
  mDevice.SetMode (mInputChannel, NTV2_MODE_CAPTURE, false);
  mDevice.SetFrameBufferFormat (mInputChannel, mPixelFormat);

  mDevice.EnableChannel (mInputChannel);

  //    Setup frame buffer
  if (mQuad) {
    //    Set capture mode
    mDevice.SetMode ((NTV2Channel) (mInputChannel + 1), NTV2_MODE_CAPTURE, false);
    mDevice.SetMode ((NTV2Channel) (mInputChannel + 2), NTV2_MODE_CAPTURE, false);
    mDevice.SetMode ((NTV2Channel) (mInputChannel + 3), NTV2_MODE_CAPTURE, false);

    //    Set frame buffer format
    mDevice.SetFrameBufferFormat ((NTV2Channel) (mInputChannel + 1), mPixelFormat);
    mDevice.SetFrameBufferFormat ((NTV2Channel) (mInputChannel + 2), mPixelFormat);
    mDevice.SetFrameBufferFormat ((NTV2Channel) (mInputChannel + 3), mPixelFormat);

    //    Enable frame buffers
    mDevice.EnableChannel ((NTV2Channel) (mInputChannel + 1));
    mDevice.EnableChannel ((NTV2Channel) (mInputChannel + 2));
    mDevice.EnableChannel ((NTV2Channel) (mInputChannel + 3));

    if (::NTV2DeviceHasBiDirectionalSDI (mDeviceID)) {
      mDevice.SetSDITransmitEnable ((NTV2Channel) (mInputChannel), false);
      mDevice.SetSDITransmitEnable ((NTV2Channel) (mInputChannel + 1), false);
      mDevice.SetSDITransmitEnable ((NTV2Channel) (mInputChannel + 2), false);
      mDevice.SetSDITransmitEnable ((NTV2Channel) (mInputChannel + 3), false);
      mDevice.WaitForOutputVerticalInterrupt ();
      mDevice.WaitForOutputVerticalInterrupt ();
      mDevice.WaitForOutputVerticalInterrupt ();
    }
  }

  mDevice.SetVideoFormat(mVideoFormat, true, false, mInputChannel);

  mTimeBase.SetAJAFrameRate (GetAJAFrameRate (GetNTV2FrameRateFromVideoFormat
          (mVideoFormat)));

  // Set up routing

  // Select input channel based on mode
  NTV2CrosspointID inputIdentifier = NTV2_XptSDIIn1;
  switch (mVideoSource) {
    case NTV2_INPUTSOURCE_SDI1:
      // Select correct values based on channel
      switch (mInputChannel) {
         default:
         case NTV2_CHANNEL1:
            inputIdentifier = NTV2_XptSDIIn1;
            mInputSource    = NTV2_INPUTSOURCE_SDI1;
            break;
         case NTV2_CHANNEL2:
            inputIdentifier = NTV2_XptSDIIn2;
            mInputSource    = NTV2_INPUTSOURCE_SDI2;
            break;
         case NTV2_CHANNEL3:
            inputIdentifier = NTV2_XptSDIIn3;
            mInputSource    = NTV2_INPUTSOURCE_SDI3;
            break;
         case NTV2_CHANNEL4:
            inputIdentifier = NTV2_XptSDIIn4;
            mInputSource    = NTV2_INPUTSOURCE_SDI4;
            break;
         case NTV2_CHANNEL5:
            inputIdentifier = NTV2_XptSDIIn5;
            mInputSource    = NTV2_INPUTSOURCE_SDI5;
            break;
         case NTV2_CHANNEL6:
            inputIdentifier = NTV2_XptSDIIn6;
            mInputSource    = NTV2_INPUTSOURCE_SDI6;
            break;
         case NTV2_CHANNEL7:
            inputIdentifier = NTV2_XptSDIIn7;
            mInputSource    = NTV2_INPUTSOURCE_SDI7;
            break;
         case NTV2_CHANNEL8:
            inputIdentifier = NTV2_XptSDIIn8;
            mInputSource    = NTV2_INPUTSOURCE_SDI8;
            break;
      }

      if(!::NTV2BoardCanDoInputSource (mDeviceID, mInputSource))
        mInputSource = NTV2_INPUTSOURCE_SDI1;

      break;
    case NTV2_INPUTSOURCE_HDMI1:
      inputIdentifier = NTV2_XptHDMIIn;
      mInputSource = NTV2_INPUTSOURCE_HDMI;
      break;
    case NTV2_INPUTSOURCE_ANALOG1:
      inputIdentifier = NTV2_XptAnalogIn;
      mInputSource = NTV2_INPUTSOURCE_ANALOG;
      break;
    default:
      g_assert_not_reached ();
      break;
  }

  NTV2InputCrosspointID fbfInputSelect;
  CNTV2SignalRouter router;

  // Get corresponding input select entries for the channel
  switch (mInputChannel) {
    default:
    case NTV2_CHANNEL1:
      mEncodeChannel = M31_CH0;
      fbfInputSelect = NTV2_XptFrameBuffer1Input;
      break;
    case NTV2_CHANNEL2:
      mEncodeChannel = M31_CH1;
      fbfInputSelect = NTV2_XptFrameBuffer2Input;
      break;
    case NTV2_CHANNEL3:
      mEncodeChannel = M31_CH2;
      fbfInputSelect = NTV2_XptFrameBuffer3Input;
      break;
    case NTV2_CHANNEL4:
      mEncodeChannel = M31_CH3;
      fbfInputSelect = NTV2_XptFrameBuffer4Input;
      break;
    case NTV2_CHANNEL5:
      mEncodeChannel = M31_CH3; //FIXME
      fbfInputSelect = NTV2_XptFrameBuffer5Input;
      break;
    case NTV2_CHANNEL6:
      mEncodeChannel = M31_CH3; //FIXME
      fbfInputSelect = NTV2_XptFrameBuffer6Input;
      break;
    case NTV2_CHANNEL7:
      mEncodeChannel = M31_CH3; //FIXME
      fbfInputSelect = NTV2_XptFrameBuffer7Input;
      break;
    case NTV2_CHANNEL8:
      mEncodeChannel = M31_CH3; //FIXME
      fbfInputSelect = NTV2_XptFrameBuffer8Input;
      break;
  }

  // Add to the mapping to the router for this channel
  router.AddConnection (fbfInputSelect, inputIdentifier);

  // Disable SDI output from the SDI input being used,
  // but only if the device supports bi-directional SDI,
  // and only if the input being used is an SDI input
  if (::NTV2BoardHasBiDirectionalSDI (mDeviceID)) {
    mDevice.SetSDITransmitEnable(mInputChannel, false);
  } else {
    if (mInputSource == NTV2_INPUTSOURCE_HDMI) {
      // Enable HDMI passthrough
      router.AddConnection(NTV2_XptHDMIOutInput, NTV2_XptHDMIIn);
    } else {
      // enable SDI End to End mode for all AJA cards that don't support bidirectional SDI

      if (mInputChannel == NTV2_CHANNEL1) {
        router.AddConnection (NTV2_XptSDIOut1Input, NTV2_XptSDIIn1);
      } else if (mInputChannel == NTV2_CHANNEL2) {
        router.AddConnection (NTV2_XptSDIOut2Input, NTV2_XptSDIIn2);
      } else if (mInputChannel == NTV2_CHANNEL3) {
        router.AddConnection (NTV2_XptSDIOut3Input, NTV2_XptSDIIn3);
      } else if (mInputChannel == NTV2_CHANNEL4) {
        router.AddConnection (NTV2_XptSDIOut4Input, NTV2_XptSDIIn4);
      }
    }
  }

  // Enable UHD/4k quad mode
  if (mQuad) {
    if (mInputChannel == NTV2_CHANNEL1) {
      router.AddConnection(NTV2_XptFrameBuffer2Input, NTV2_XptSDIIn2);
      router.AddConnection(NTV2_XptFrameBuffer3Input, NTV2_XptSDIIn3);
      router.AddConnection(NTV2_XptFrameBuffer4Input, NTV2_XptSDIIn4);
    } else {
      router.AddConnection(NTV2_XptFrameBuffer6Input, NTV2_XptSDIIn6);
      router.AddConnection(NTV2_XptFrameBuffer7Input, NTV2_XptSDIIn7);
      router.AddConnection(NTV2_XptFrameBuffer8Input, NTV2_XptSDIIn8);
    }
    mOutputChannel = NTV2_CHANNEL5;
    mEncodeChannel = M31_CH0;
  }

  // Enable passthrough on bidirectional devices
  if (mPassthrough && ::NTV2DeviceHasBiDirectionalSDI(mDeviceID)) {
    int numVideoInputs = NTV2DeviceGetNumVideoOutputs (mDeviceID);

    mDevice.SetMode((NTV2Channel)(mInputChannel + numVideoInputs / 2), NTV2_MODE_DISPLAY);
    // Enable End to End mode for all AJA cards that don't support bidirectional SDI
    if (mInputChannel == NTV2_CHANNEL1) {
      router.AddConnection ((numVideoInputs == 8) ? NTV2_XptSDIOut5Input : NTV2_XptSDIOut3Input, NTV2_XptSDIIn1);
      mDevice.SetSDITransmitEnable ((numVideoInputs == 8) ? NTV2_CHANNEL5 : NTV2_CHANNEL3, true);
    } else if (mInputChannel == NTV2_CHANNEL2) {
      router.AddConnection ((numVideoInputs == 8) ? NTV2_XptSDIOut6Input : NTV2_XptSDIOut4Input, NTV2_XptSDIIn2);
      mDevice.SetSDITransmitEnable ((numVideoInputs == 8) ? NTV2_CHANNEL6 : NTV2_CHANNEL4 , true);
    } else if (mInputChannel == NTV2_CHANNEL3) {
      router.AddConnection ((numVideoInputs == 8) ? NTV2_XptSDIOut7Input : NTV2_XptSDIOut5Input, NTV2_XptSDIIn3);
      mDevice.SetSDITransmitEnable ((numVideoInputs == 8) ? NTV2_CHANNEL7 : NTV2_CHANNEL5 , true);
    } else if (mInputChannel == NTV2_CHANNEL4) {
      router.AddConnection ((numVideoInputs == 8) ? NTV2_XptSDIOut8Input : NTV2_XptSDIOut6Input, NTV2_XptSDIIn4);
      mDevice.SetSDITransmitEnable ((numVideoInputs == 8) ? NTV2_CHANNEL8 : NTV2_CHANNEL6 , true);
    }
  }

  // VANC handling
  if (mCaptureTall) {
    GST_DEBUG ("Asking to enable VANC Data");
    mDevice.SetEnableVANCData (true, false, mInputChannel);
    if (mPixelFormat == NTV2_FBF_8BIT_YCBCR) {
      GST_DEBUG ("8bit, asking to shift VANC");
      if (!mDevice.SetVANCShiftMode (mInputChannel,
              NTV2_VANCDATA_8BITSHIFT_ENABLE))
        GST_WARNING ("Failed to request 8bit VANC shift");
    }
  }

  // Enable routes
  mDevice.ApplySignalRoute (router, false);

  //    Set the device reference to the input...
  //    FIXME
//  if (mMultiStream) {
//    mDevice.SetReference (NTV2_REFERENCE_FREERUN);
//  } else {
    mDevice.SetReference (::NTV2InputSourceToReferenceSource (mInputSource));
//  }

#if 0
  //    When input is 3Gb convert to 3Ga for capture (no RGB support?)
  bool is3Gb = false;
  mDevice.GetSDIInput3GbPresent (is3Gb, mInputChannel);

  if (mQuad) {
    mDevice.SetSDIInLevelBtoLevelAConversion (NTV2_CHANNEL1, is3Gb);
    mDevice.SetSDIInLevelBtoLevelAConversion (NTV2_CHANNEL2, is3Gb);
    mDevice.SetSDIInLevelBtoLevelAConversion (NTV2_CHANNEL3, is3Gb);
    mDevice.SetSDIInLevelBtoLevelAConversion (NTV2_CHANNEL4, is3Gb);
    mDevice.SetSDIOutLevelAtoLevelBConversion (NTV2_CHANNEL5, false);
    mDevice.SetSDIOutLevelAtoLevelBConversion (NTV2_CHANNEL6, false);
    mDevice.SetSDIOutLevelAtoLevelBConversion (NTV2_CHANNEL7, false);
    mDevice.SetSDIOutLevelAtoLevelBConversion (NTV2_CHANNEL8, false);
  } else {
    mDevice.SetSDIInLevelBtoLevelAConversion (mInputChannel, is3Gb);
    mDevice.SetSDIOutLevelAtoLevelBConversion (mOutputChannel, false);
  }

  if (!mMultiStream)            //    If not doing multistream...
    mDevice.ClearRouting ();    //    ...replace existing routing

  //    Connect SDI output spigots to FB outputs...
  mDevice.Connect (NTV2_XptSDIOut5Input, NTV2_XptFrameBuffer5YUV);
  mDevice.Connect (NTV2_XptSDIOut6Input, NTV2_XptFrameBuffer6YUV);
  mDevice.Connect (NTV2_XptSDIOut7Input, NTV2_XptFrameBuffer7YUV);
  mDevice.Connect (NTV2_XptSDIOut8Input, NTV2_XptFrameBuffer8YUV);
#endif

  //    Give the device some time to lock to the input signal...
  mDevice.WaitForOutputVerticalInterrupt (mInputChannel, 8);


  return AJA_STATUS_SUCCESS;
}                               //    SetupAudio


AJAStatus NTV2GstAV::SetupAudio (void)
{
  // Select audio system to use based on the channel
  switch (mInputChannel) {
    default:
    case NTV2_CHANNEL1:
      mAudioSystem = NTV2_AUDIOSYSTEM_1;
      break;
    case NTV2_CHANNEL2:
      mAudioSystem = NTV2_AUDIOSYSTEM_2;
      break;
    case NTV2_CHANNEL3:
      mAudioSystem = NTV2_AUDIOSYSTEM_3;
      break;
    case NTV2_CHANNEL4:
      mAudioSystem = NTV2_AUDIOSYSTEM_4;
      break;
    case NTV2_CHANNEL5:
      mAudioSystem = NTV2_AUDIOSYSTEM_5;
      break;
    case NTV2_CHANNEL6:
      mAudioSystem = NTV2_AUDIOSYSTEM_6;
      break;
    case NTV2_CHANNEL7:
      mAudioSystem = NTV2_AUDIOSYSTEM_7;
      break;
    case NTV2_CHANNEL8:
      mAudioSystem = NTV2_AUDIOSYSTEM_8;
      break;
  }

  // Then based on channel and/or mode, select the audio input
  switch (mAudioSource) {
    case NTV2_AUDIO_EMBEDDED:
      switch (mInputChannel) {
        default:
        case NTV2_CHANNEL1:
          mDevice.SetAudioSystemInputSource (mAudioSystem, NTV2_AUDIO_EMBEDDED, NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_1);
          mDevice.SetEmbeddedAudioInput(NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_1, mAudioSystem);
          break;
        case NTV2_CHANNEL2:
          mDevice.SetAudioSystemInputSource (mAudioSystem, NTV2_AUDIO_EMBEDDED, NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_2);
          mDevice.SetEmbeddedAudioInput(NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_2, mAudioSystem);
          break;
        case NTV2_CHANNEL3:
          mDevice.SetAudioSystemInputSource (mAudioSystem, NTV2_AUDIO_EMBEDDED, NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_3);
          mDevice.SetEmbeddedAudioInput(NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_3, mAudioSystem);
          break;
        case NTV2_CHANNEL4:
          mDevice.SetAudioSystemInputSource (mAudioSystem, NTV2_AUDIO_EMBEDDED, NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_4);
          mDevice.SetEmbeddedAudioInput(NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_4, mAudioSystem);
          break;
        case NTV2_CHANNEL5:
          mDevice.SetAudioSystemInputSource (mAudioSystem, NTV2_AUDIO_EMBEDDED, NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_5);
          mDevice.SetEmbeddedAudioInput(NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_5, mAudioSystem);
          break;
        case NTV2_CHANNEL6:
          mDevice.SetAudioSystemInputSource (mAudioSystem, NTV2_AUDIO_EMBEDDED, NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_6);
          mDevice.SetEmbeddedAudioInput(NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_6, mAudioSystem);
          break;
        case NTV2_CHANNEL7:
          mDevice.SetAudioSystemInputSource (mAudioSystem, NTV2_AUDIO_EMBEDDED, NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_7);
          mDevice.SetEmbeddedAudioInput(NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_7, mAudioSystem);
          break;
        case NTV2_CHANNEL8:
          mDevice.SetAudioSystemInputSource (mAudioSystem, NTV2_AUDIO_EMBEDDED, NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_8);
          mDevice.SetEmbeddedAudioInput(NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_8, mAudioSystem);
          break;
      }

      break;
    case NTV2_AUDIO_HDMI:
      mDevice.SetAudioSystemInputSource (mAudioSystem, NTV2_AUDIO_HDMI, NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_1);
      break;
    case NTV2_AUDIO_AES:
      mDevice.SetAudioSystemInputSource (mAudioSystem, NTV2_AUDIO_AES, NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_1);
      break;
    case NTV2_AUDIO_ANALOG:
      mDevice.SetAudioSystemInputSource (mAudioSystem, NTV2_AUDIO_ANALOG, NTV2_EMBEDDED_AUDIO_INPUT_VIDEO_1);
      break;
    default:
      g_assert_not_reached ();
      break;
  }

  if (mNumAudioChannels == 0)
    mNumAudioChannels =::NTV2DeviceGetMaxAudioChannels (mDeviceID);
  if (mNumAudioChannels >::NTV2DeviceGetMaxAudioChannels (mDeviceID))
    return AJA_STATUS_FAIL;

  // Setting channels or getting the maximum number of channels generally fails and we always
  // get all channels available to the card
  mDevice.SetNumberAudioChannels (mNumAudioChannels, mAudioSystem);
  mDevice.SetAudioRate (NTV2_AUDIO_48K, mAudioSystem);
  mDevice.SetEmbeddedAudioClock (NTV2_EMBEDDED_AUDIO_CLOCK_VIDEO_INPUT,
      mAudioSystem);

  //    The on-device audio buffer should be 4MB to work best across all devices & platforms...
  mDevice.SetAudioBufferSize (NTV2_AUDIO_BUFFER_BIG, mAudioSystem);

  mDevice.SetAudioLoopBack(NTV2_AUDIO_LOOPBACK_OFF, mAudioSystem);

  return AJA_STATUS_SUCCESS;

}                               //    SetupAudio


void
NTV2GstAV::SetupHostBuffers (void)
{
  mVideoBufferSize =
      GetVideoActiveSize (mVideoFormat, mPixelFormat,
      mCaptureTall ? NTV2_VANCMODE_TALL : NTV2_VANCMODE_OFF);
  mPicInfoBufferSize = sizeof (HevcPictureInfo) * 2;
  mEncInfoBufferSize = sizeof (HevcEncodedInfo) * 2;
  mAudioBufferSize = NTV2_AUDIOSIZE_MAX;

  if (mHevcOutput) {
    mHevcInputCircularBuffer.SetAbortFlag (&mGlobalQuit);
    for (unsigned bufferNdx = 0; bufferNdx < VIDEO_RING_SIZE; bufferNdx++) {
      memset (&mHevcInputBuffer[bufferNdx], 0, sizeof (AjaVideoBuff));
      mHevcInputBuffer[bufferNdx].pVideoBuffer =
          new uint32_t[mVideoBufferSize / 4];
      mHevcInputBuffer[bufferNdx].videoBufferSize = mVideoBufferSize;
      mHevcInputBuffer[bufferNdx].videoDataSize = 0;
      mHevcInputBuffer[bufferNdx].pInfoBuffer =
          new uint32_t[mPicInfoBufferSize / 4];
      mHevcInputBuffer[bufferNdx].infoBufferSize = mPicInfoBufferSize;
      mHevcInputBuffer[bufferNdx].infoDataSize = 0;
      mHevcInputCircularBuffer.Add (&mHevcInputBuffer[bufferNdx]);
    }
  }
  // These video buffers are actually passed out of this class so we need to assign them unique numbers
  // so they can be tracked and also they have a state
  mVideoBufferPool = gst_aja_buffer_pool_new ();
  GstStructure *config = gst_buffer_pool_get_config (mVideoBufferPool);
  gst_buffer_pool_config_set_params (config, NULL, mVideoBufferSize,
      VIDEO_ARRAY_SIZE, 0);
  gst_structure_set (config, "is-video", G_TYPE_BOOLEAN, TRUE, "is-hevc",
      G_TYPE_BOOLEAN, mHevcOutput, NULL);
  gst_buffer_pool_set_config (mVideoBufferPool, config);
  gst_buffer_pool_set_active (mVideoBufferPool, TRUE);

  mAudioBufferPool = gst_aja_buffer_pool_new ();
  config = gst_buffer_pool_get_config (mAudioBufferPool);
  gst_buffer_pool_config_set_params (config, NULL, mAudioBufferSize,
      AUDIO_ARRAY_SIZE, 0);
  gst_structure_set (config, "is-video", G_TYPE_BOOLEAN, FALSE, "is-hevc",
      G_TYPE_BOOLEAN, FALSE, NULL);
  gst_buffer_pool_set_config (mAudioBufferPool, config);
  gst_buffer_pool_set_active (mAudioBufferPool, TRUE);

}                               //    SetupHostBuffers


void
NTV2GstAV::FreeHostBuffers (void)
{
  if (mHevcOutput) {
    for (unsigned bufferNdx = 0; bufferNdx < VIDEO_RING_SIZE; bufferNdx++) {
      if (mHevcInputBuffer[bufferNdx].pVideoBuffer) {
        delete[]mHevcInputBuffer[bufferNdx].pVideoBuffer;
        mHevcInputBuffer[bufferNdx].pVideoBuffer = NULL;
      }
      if (mHevcInputBuffer[bufferNdx].pInfoBuffer) {
        delete[]mHevcInputBuffer[bufferNdx].pInfoBuffer;
        mHevcInputBuffer[bufferNdx].pInfoBuffer = NULL;
      }
    }
    mHevcInputCircularBuffer.Clear ();
  }

  if (mVideoBufferPool) {
    gst_buffer_pool_set_active (mVideoBufferPool, FALSE);
    gst_object_unref (mVideoBufferPool);
    mVideoBufferPool = NULL;
  }

  if (mAudioBufferPool) {
    gst_buffer_pool_set_active (mAudioBufferPool, FALSE);
    gst_object_unref (mAudioBufferPool);
    mAudioBufferPool = NULL;
  }
}

void
NTV2GstAV::SetupAutoCirculate (void)
{
  //    Tell capture AutoCirculate to use 8 frame buffers on the device...
  mInputTransferStruct.Clear ();
  mInputTransferStruct.acFrameBufferFormat = mPixelFormat;

  mDevice.AutoCirculateStop (mInputChannel);
  mDevice.AutoCirculateInitForInput (mInputChannel, 8,  //    Frames to circulate
      mAudioSystem,             //    Which audio system
      AUTOCIRCULATE_WITH_RP188);        //    With RP188?
}


AJAStatus NTV2GstAV::Run ()
{
  mVideoInputFrameCount = 0;
  mVideoOutFrameCount = 0;
  mCodecRawFrameCount = 0;
  mCodecHevcFrameCount = 0;
  mHevcOutFrameCount = 0;
  mAudioOutFrameCount = 0;
  mLastFrame = false;
  mLastFrameInput = false;
  mLastFrameVideoOut = false;
  mLastFrameHevc = false;
  mLastFrameHevcOut = false;
  mLastFrameAudioOut = false;
  mGlobalQuit = false;


  //    Setup to capture video/audio/anc input
  SetupAutoCirculate ();

  //    Setup the circular buffers
  SetupHostBuffers ();

  if (mDevice.GetInputVideoFormat (mInputSource) == NTV2_FORMAT_UNKNOWN)
    GST_WARNING ("No video signal present on the input connector");

  // always start the AC thread
  StartACThread ();

  // if doing hevc output then start the hevc threads
  if (mHevcOutput) {
    StartCodecRawThread ();
    StartCodecHevcThread ();
  }

  mStarted = true;
  return AJA_STATUS_SUCCESS;
}


// This is where we will start the AC thread
void
NTV2GstAV::StartACThread (void)
{
  mACInputThread = new AJAThread ();
  mACInputThread->Attach (ACInputThreadStatic, this);
  mACInputThread->SetPriority (AJA_ThreadPriority_High);
  mACInputThread->Start ();
}


// This is where we will stop the AC thread
void
NTV2GstAV::StopACThread (void)
{
  if (mACInputThread) {
    while (mACInputThread->Active ())
      AJATime::Sleep (10);

    delete mACInputThread;
    mACInputThread = NULL;
  }
}


// The video input thread static callback
void
NTV2GstAV::ACInputThreadStatic (AJAThread * pThread, void *pContext)
{
  (void) pThread;

  NTV2GstAV *pApp (reinterpret_cast < NTV2GstAV * >(pContext));
  pApp->ACInputWorker ();
}


void
NTV2GstAV::ACInputWorker (void)
{
  guint64 timeoutStart = GST_CLOCK_TIME_NONE;

  // Choose timecode source
  NTV2TCIndex tcIndex;

  if (mTimecodeMode == NTV2_TCINDEX_LTC1 || mTimecodeMode == NTV2_TCINDEX_LTC2) {
    tcIndex = mTimecodeMode;
  } else {
    switch (mInputChannel) {
      default:
      case NTV2_CHANNEL1:
        if (mTimecodeMode == NTV2_TCINDEX_SDI1)
          tcIndex = NTV2_TCINDEX_SDI1_LTC;
        else if (mTimecodeMode == NTV2_TCINDEX_SDI1_LTC)
          tcIndex = NTV2_TCINDEX_SDI1_LTC;
        else
          tcIndex = NTV2_TCINDEX_SDI1_2;
        break;
      case NTV2_CHANNEL2:
        if (mTimecodeMode == NTV2_TCINDEX_SDI1)
          tcIndex = NTV2_TCINDEX_SDI2_LTC;
        else if (mTimecodeMode == NTV2_TCINDEX_SDI1_LTC)
          tcIndex = NTV2_TCINDEX_SDI2_LTC;
        else
          tcIndex = NTV2_TCINDEX_SDI2_2;
        break;
      case NTV2_CHANNEL3:
        if (mTimecodeMode == NTV2_TCINDEX_SDI1)
          tcIndex = NTV2_TCINDEX_SDI3_LTC;
        else if (mTimecodeMode == NTV2_TCINDEX_SDI1_LTC)
          tcIndex = NTV2_TCINDEX_SDI3_LTC;
        else
          tcIndex = NTV2_TCINDEX_SDI3_2;
        break;
      case NTV2_CHANNEL4:
        if (mTimecodeMode == NTV2_TCINDEX_SDI1)
          tcIndex = NTV2_TCINDEX_SDI4_LTC;
        else if (mTimecodeMode == NTV2_TCINDEX_SDI1_LTC)
          tcIndex = NTV2_TCINDEX_SDI4_LTC;
        else
          tcIndex = NTV2_TCINDEX_SDI4_2;
        break;
      case NTV2_CHANNEL5:
        if (mTimecodeMode == NTV2_TCINDEX_SDI1)
          tcIndex = NTV2_TCINDEX_SDI5_LTC;
        else if (mTimecodeMode == NTV2_TCINDEX_SDI1_LTC)
          tcIndex = NTV2_TCINDEX_SDI5_LTC;
        else
          tcIndex = NTV2_TCINDEX_SDI5_2;
        break;
      case NTV2_CHANNEL6:
        if (mTimecodeMode == NTV2_TCINDEX_SDI1)
          tcIndex = NTV2_TCINDEX_SDI6_LTC;
        else if (mTimecodeMode == NTV2_TCINDEX_SDI1_LTC)
          tcIndex = NTV2_TCINDEX_SDI6_LTC;
        else
          tcIndex = NTV2_TCINDEX_SDI6_2;
        break;
      case NTV2_CHANNEL7:
        if (mTimecodeMode == NTV2_TCINDEX_SDI1)
          tcIndex = NTV2_TCINDEX_SDI7_LTC;
        else if (mTimecodeMode == NTV2_TCINDEX_SDI1_LTC)
          tcIndex = NTV2_TCINDEX_SDI7_LTC;
        else
          tcIndex = NTV2_TCINDEX_SDI7_2;
        break;
      case NTV2_CHANNEL8:
        if (mTimecodeMode == NTV2_TCINDEX_SDI1)
          tcIndex = NTV2_TCINDEX_SDI8_LTC;
        else if (mTimecodeMode == NTV2_TCINDEX_SDI1_LTC)
          tcIndex = NTV2_TCINDEX_SDI8_LTC;
        else
          tcIndex = NTV2_TCINDEX_SDI8_2;
        break;
    }
  }

  // start AutoCirculate running...
  mDevice.AutoCirculateStart (mInputChannel);

  while (!mGlobalQuit) {
    AUTOCIRCULATE_STATUS acStatus;
    mDevice.AutoCirculateGetStatus (mInputChannel, acStatus);

    // wait for captured frame
    if (acStatus.acState == NTV2_AUTOCIRCULATE_RUNNING
        && acStatus.acBufferLevel > 1) {
      // At this point, there's at least one fully-formed frame available in the device's
      // frame buffer to transfer to the host. Reserve an AvaDataBuffer to "produce", and
      // use it in the next transfer from the device...
      AjaVideoBuff *pVideoData (mHevcOutput ?
          mHevcInputCircularBuffer.StartProduceNextBuffer () :
          AcquireVideoBuffer ());
      GstMapInfo video_map, audio_map;

      NTV2VideoFormat inputVideoFormat = mDevice.GetInputVideoFormat(mInputSource);

      // For quad mode, we will get the format of a single input
      NTV2VideoFormat effectiveVideoFormat = mVideoFormat;
      switch (mVideoFormat) {
        case NTV2_FORMAT_4x1920x1080p_2500:
          effectiveVideoFormat = NTV2_FORMAT_1080p_2500;
          break;
        case NTV2_FORMAT_4x1920x1080p_3000:
          effectiveVideoFormat = NTV2_FORMAT_1080p_3000;
          break;
        case NTV2_FORMAT_4x1920x1080p_5000:
          effectiveVideoFormat = NTV2_FORMAT_1080p_5000_A;
          break;
        case NTV2_FORMAT_4x1920x1080p_5994:
          effectiveVideoFormat = NTV2_FORMAT_1080p_5994_A;
          break;
        case NTV2_FORMAT_4x1920x1080p_6000:
          effectiveVideoFormat = NTV2_FORMAT_1080p_6000_A;
          break;
        case NTV2_FORMAT_4x2048x1080p_2500:
          effectiveVideoFormat = NTV2_FORMAT_1080p_2K_2500;
          break;
        case NTV2_FORMAT_4x2048x1080p_3000:
          effectiveVideoFormat = NTV2_FORMAT_1080p_2K_3000;
          break;
        case NTV2_FORMAT_4x2048x1080p_5000:
          effectiveVideoFormat = NTV2_FORMAT_1080p_2K_5000_A;
          break;
        case NTV2_FORMAT_4x2048x1080p_5994:
          effectiveVideoFormat = NTV2_FORMAT_1080p_2K_5994_A;
          break;
        case NTV2_FORMAT_4x2048x1080p_6000:
          effectiveVideoFormat = NTV2_FORMAT_1080p_2K_6000_A;
          break;
        default:
          break;
      }

      pVideoData->haveSignal = (effectiveVideoFormat == inputVideoFormat);
      if (!pVideoData->haveSignal && timeoutStart == GST_CLOCK_TIME_NONE) {
        timeoutStart = gst_util_get_timestamp ();
      }

      if (pVideoData->buffer) {
        gst_buffer_map (pVideoData->buffer, &video_map, GST_MAP_READWRITE);
        pVideoData->pVideoBuffer = (uint32_t *) video_map.data;
        pVideoData->videoBufferSize = video_map.size;
      }
      mInputTransferStruct.SetVideoBuffer (pVideoData->pVideoBuffer,
          pVideoData->videoBufferSize);

      AjaAudioBuff *pAudioData = AcquireAudioBuffer ();
      pAudioData->haveSignal = pVideoData->haveSignal;
      if (pAudioData->buffer) {
        gst_buffer_map (pAudioData->buffer, &audio_map, GST_MAP_READWRITE);
        pAudioData->pAudioBuffer = (uint32_t *) audio_map.data;
        pAudioData->audioBufferSize = audio_map.size;
      }
      mInputTransferStruct.SetAudioBuffer (pAudioData->pAudioBuffer,
          pAudioData->audioBufferSize);

      // do the transfer from the device into our host AvaDataBuffer...
      mDevice.AutoCirculateTransfer (mInputChannel, mInputTransferStruct);

      // get the video data size
      pVideoData->videoDataSize = pVideoData->videoBufferSize;
      if (pVideoData->buffer) {
        bool validVanc = false;
        NTV2FrameGeometry currentGeometry;
        gsize offset = 0;       // Offset in number of lines

        mDevice.GetFrameGeometry (&currentGeometry);
        switch (currentGeometry) {
          case NTV2_FG_1920x1112:
            // 30 line offset (or 32?)
            // FIXME : Remove hardcording once gstntv2 is gstvideoformat aware
            if (mBitDepth == 8)
              offset = 30 * 1920 * 2;
            else
              offset = 30 * 1920 * 16 / 6;
            validVanc = true;
            break;
          case NTV2_FG_1280x740:
            // 19 line offset (or 20 ?)
            // FIXME : Remove hardcording once gstntv2 is gstvideoformat aware
            if (mBitDepth == 8)
              offset = 19 * 1280 * 2;
            else
              offset = 19 * 1296 * 16 / 6;
            validVanc = true;
            break;
          default:
            if (mCaptureTall)
              GST_ERROR ("UNKNOWN GEOMETRY !");
            break;
        }
        GST_DEBUG ("offset %" G_GSIZE_FORMAT, offset);
        GST_DEBUG ("videoDataSize %u", pVideoData->videoDataSize);
        gst_buffer_unmap (pVideoData->buffer, &video_map);
        gst_buffer_resize (pVideoData->buffer, offset,
            pVideoData->videoDataSize - offset);
        pVideoData->pAncillaryData = validVanc ? pVideoData->pVideoBuffer : NULL;
        pVideoData->pVideoBuffer = NULL;
      }
      pVideoData->lastFrame = mLastFrame;

      // get the audio data size
      pAudioData->audioDataSize =
          mInputTransferStruct.acTransferStatus.acAudioTransferSize;
      if (pAudioData->buffer) {
        gst_buffer_unmap (pAudioData->buffer, &audio_map);
        gst_buffer_resize (pAudioData->buffer, 0, pAudioData->audioDataSize);
        pAudioData->pAudioBuffer = NULL;
      }
      pAudioData->lastFrame = mLastFrame;

      // FIXME: this should actually use acAudioClockTimeStamp but
      // it does not actually seem to be based on a 48kHz clock
      pVideoData->timeStamp =
          mInputTransferStruct.acTransferStatus.acFrameStamp.acFrameTime;
      pAudioData->timeStamp =
          mInputTransferStruct.acTransferStatus.acFrameStamp.acFrameTime;

      pVideoData->fieldCount =
          mInputTransferStruct.acTransferStatus.
          acFrameStamp.acCurrentFieldCount;

      pVideoData->frameNumber = mVideoInputFrameCount;
      pAudioData->frameNumber = mVideoInputFrameCount;

      pVideoData->timeCodeValid = false;
      NTV2_RP188 timeCode;
      if (mInputTransferStruct.acTransferStatus.
          acFrameStamp.GetInputTimeCode (timeCode, tcIndex)) {
        // get the sdi input anc data
        pVideoData->timeCodeDBB = timeCode.fDBB;
        pVideoData->timeCodeLow = timeCode.fLo;
        pVideoData->timeCodeHigh = timeCode.fHi;
        pVideoData->timeCodeValid = true;
      }

      if (mWithInfo) {
        // get picture and additional data pointers
        HevcPictureInfo *pInfo = (HevcPictureInfo *) pVideoData->pInfoBuffer;
        HevcPictureData *pPicData = &pInfo->pictureData;

        // initialize info buffer to 0
        memset (pInfo, 0, pVideoData->infoBufferSize);

        // calculate pts based on 90 Khz clock tick
        uint64_t pts =
            (uint64_t) mTimeBase.FramesToMicroseconds (mVideoInputFrameCount) *
            90000 / 1000000;

        // set serial number, pts and picture number
        pPicData->serialNumber = mVideoInputFrameCount; // can be anything
        pPicData->ptsValueLow = (uint32_t) (pts & 0xffffffff);
        pPicData->ptsValueHigh = (uint32_t) (pts >> 32);
        pPicData->pictureNumber = mVideoInputFrameCount + 1;    // must count starting with 1

        // set info data size
        pVideoData->infoDataSize = sizeof (HevcPictureData);
      }

      if (pVideoData->lastFrame && !mLastFrameInput) {
        GST_INFO ("Capture last frame number %d", mVideoInputFrameCount);
        mLastFrameInput = true;
      }

      mVideoInputFrameCount++;

      if (mHevcOutput) {
        mHevcInputCircularBuffer.EndProduceNextBuffer ();
      } else {
        // Possible callbacks are not setup yet so make sure we release the buffer if
        // no one is there to catch them
        if (!DoCallback (VIDEO_CALLBACK, pVideoData))
          ReleaseVideoBuffer (pVideoData);

        if (pVideoData->lastFrame) {
          GST_INFO ("Video out last frame number %d", mVideoOutFrameCount);
          mLastFrameVideoOut = true;
        }

        mVideoOutFrameCount++;
      }

      // Possible callbacks are not setup yet so make sure we release the buffer if
      // no one is there to catch them
      if (!DoCallback (AUDIO_CALLBACK, pAudioData))
        ReleaseAudioBuffer (pAudioData);

      if (pAudioData->lastFrame) {
        GST_INFO ("Audio out last frame number %d", mAudioOutFrameCount);
        mLastFrameAudioOut = true;
      }
      mAudioOutFrameCount++;
    } else {
      // Either AutoCirculate is not running, or there were no frames available on the device to transfer.
      // Rather than waste CPU cycles spinning, waiting until a frame becomes available, it's far more
      // efficient to wait for the next input vertical interrupt event to get signaled...
      if (mLastFrame) {
          mLastFrameVideoOut = true;
          mLastFrameAudioOut = true;
          break;
      } else {
        if (!mDevice.WaitForInputVerticalInterrupt (mInputChannel)) {
          if (timeoutStart == GST_CLOCK_TIME_NONE) {
            timeoutStart = gst_util_get_timestamp ();
          } else if (gst_util_get_timestamp() - timeoutStart >= 100 * GST_MSECOND) {
            // Only report missing frames a) every 100ms and b) if there
            // actually was no frame for 100ms
            DoCallback (VIDEO_CALLBACK, NULL);
            DoCallback (AUDIO_CALLBACK, NULL);
            timeoutStart = GST_CLOCK_TIME_NONE;
          }
        }
      }
    }
  }                             // loop til quit signaled

  // Stop AutoCirculate...
  mDevice.AutoCirculateStop (mInputChannel);
}

// This is where we start the codec raw thread
void
NTV2GstAV::StartCodecRawThread (void)
{
  mCodecRawThread = new AJAThread ();
  mCodecRawThread->Attach (CodecRawThreadStatic, this);
  mCodecRawThread->SetPriority (AJA_ThreadPriority_High);
  mCodecRawThread->Start ();
}


// This is where we stop the codec raw thread
void
NTV2GstAV::StopCodecRawThread (void)
{
  if (mCodecRawThread) {
    while (mCodecRawThread->Active ())
      AJATime::Sleep (10);

    delete mCodecRawThread;
    mCodecRawThread = NULL;
  }
}


// The codec raw static callback
void
NTV2GstAV::CodecRawThreadStatic (AJAThread * pThread, void *pContext)
{
  (void) pThread;

  NTV2GstAV *pApp (reinterpret_cast < NTV2GstAV * >(pContext));
  pApp->CodecRawWorker ();
}


void
NTV2GstAV::CodecRawWorker (void)
{
  while (!mGlobalQuit) {
    // wait for the next raw video frame
    AjaVideoBuff
        * pFrameData (mHevcInputCircularBuffer.StartConsumeNextBuffer ());
    if (pFrameData) {
      if (!mLastFrameVideoOut) {
        // transfer the raw video frame to the codec
        if (mWithInfo) {
          mM31->RawTransfer (mEncodeChannel,
              (uint8_t *) pFrameData->pVideoBuffer,
              pFrameData->videoDataSize,
              (uint8_t *) pFrameData->pInfoBuffer,
              pFrameData->infoDataSize, pFrameData->lastFrame);
        } else {
          mM31->RawTransfer (mEncodeChannel,
              (uint8_t *) pFrameData->pVideoBuffer,
              pFrameData->videoDataSize, pFrameData->lastFrame);
        }
        if (pFrameData->lastFrame) {
          mLastFrameVideoOut = true;
        }

        mCodecRawFrameCount++;
      }
      // release the raw video frame
      mHevcInputCircularBuffer.EndConsumeNextBuffer ();
    }
  }                             // loop til quit signaled
}


// This is where we will start the codec hevc thread
void
NTV2GstAV::StartCodecHevcThread (void)
{
  mCodecHevcThread = new AJAThread ();
  mCodecHevcThread->Attach (CodecHevcThreadStatic, this);
  mCodecHevcThread->SetPriority (AJA_ThreadPriority_High);
  mCodecHevcThread->Start ();
}

// This is where we will stop the codec hevc thread
void
NTV2GstAV::StopCodecHevcThread (void)
{
  if (mCodecHevcThread) {
    while (mCodecHevcThread->Active ())
      AJATime::Sleep (10);

    delete mCodecHevcThread;
    mCodecHevcThread = NULL;
  }
}


// The codec hevc static callback
void
NTV2GstAV::CodecHevcThreadStatic (AJAThread * pThread, void *pContext)
{
  (void) pThread;

  NTV2GstAV *pApp (reinterpret_cast < NTV2GstAV * >(pContext));
  pApp->CodecHevcWorker ();
}


void
NTV2GstAV::CodecHevcWorker (void)
{
  while (!mGlobalQuit) {

    if (!mLastFrameHevcOut) {
      AjaVideoBuff *pDstFrame = AcquireVideoBuffer ();
      if (pDstFrame) {

        // transfer an hevc frame from the codec including encoded information
        mM31->EncTransfer (mEncodeChannel,
            (uint8_t *) pDstFrame->pVideoBuffer,
            pDstFrame->videoBufferSize,
            (uint8_t *) pDstFrame->pInfoBuffer,
            pDstFrame->infoBufferSize,
            pDstFrame->videoDataSize,
            pDstFrame->infoDataSize, pDstFrame->lastFrame);

        // FIXME: these are not passed properly from AC thread to here
        // pDstFrame->frameNumber = pFrameData->frameNumber;
        // pDstFrame->timeCodeDBB = pFrameData->timeCodeDBB;
        // pDstFrame->timeCodeLow = pFrameData->timeCodeLow;
        // pDstFrame->timeCodeHigh = pFrameData->timeCodeHigh;
        // pDstFrame->timeStamp = pFrameData->timeStamp;

        // Possible callbacks are not setup yet so make sure we release the buffer if
        // no one is there to catch them
        if (!DoCallback (VIDEO_CALLBACK, pDstFrame))
          ReleaseVideoBuffer (pDstFrame);
      }

      if (pDstFrame->lastFrame) {
        GST_INFO ("Hevc out last frame number %d", mHevcOutFrameCount);
        mLastFrameHevcOut = true;
      }

      mHevcOutFrameCount++;
    }
  }                             //    loop til quit signaled
}

void
NTV2GstAV::SetCallback (CallBackType cbType, NTV2Callback callback,
    void *callbackRefcon)
{
  if (cbType == VIDEO_CALLBACK) {
    mVideoCallback = callback;
    mVideoCallbackRefcon = callbackRefcon;
  } else if (cbType == AUDIO_CALLBACK) {
    mAudioCallback = callback;
    mAudioCallbackRefcon = callbackRefcon;
  }
}

AjaVideoBuff *
NTV2GstAV::AcquireVideoBuffer ()
{
  GstBuffer *buffer;
  AjaVideoBuff *videoBuff;

  if (gst_buffer_pool_acquire_buffer (mVideoBufferPool, &buffer,
          NULL) != GST_FLOW_OK)
    return NULL;

  videoBuff = gst_aja_buffer_get_video_buff (buffer);
  return videoBuff;
}

AjaAudioBuff *
NTV2GstAV::AcquireAudioBuffer ()
{
  GstBuffer *buffer;
  AjaAudioBuff *audioBuff;

  if (gst_buffer_pool_acquire_buffer (mAudioBufferPool, &buffer,
          NULL) != GST_FLOW_OK)
    return NULL;

  audioBuff = gst_aja_buffer_get_audio_buff (buffer);
  return audioBuff;
}


void
NTV2GstAV::ReleaseVideoBuffer (AjaVideoBuff * videoBuffer)
{
  if (videoBuffer->buffer)
    gst_buffer_unref (videoBuffer->buffer);
}


void
NTV2GstAV::ReleaseAudioBuffer (AjaAudioBuff * audioBuffer)
{
  if (audioBuffer->buffer)
    gst_buffer_unref (audioBuffer->buffer);
}


void
NTV2GstAV::AddRefVideoBuffer (AjaVideoBuff * videoBuffer)
{
  if (videoBuffer->buffer)
    gst_buffer_ref (videoBuffer->buffer);
}


void
NTV2GstAV::AddRefAudioBuffer (AjaAudioBuff * audioBuffer)
{
  if (audioBuffer->buffer)
    gst_buffer_ref (audioBuffer->buffer);
}


bool NTV2GstAV::GetHardwareClock (uint64_t desiredTimeScale, uint64_t * time)
{
  uint32_t
  audioCounter (0);

  bool
      status = mDevice.ReadRegister (kRegAud1Counter, &audioCounter);
  *time = (audioCounter * desiredTimeScale) / 48000;
  return status;
}


AJAStatus
    NTV2GstAV::DetermineInputFormat (NTV2Channel inputChannel, bool quad,
    NTV2VideoFormat & videoFormat)
{
  NTV2VideoFormat sdiFormat = mDevice.GetSDIInputVideoFormat (inputChannel);
  if (sdiFormat == NTV2_FORMAT_UNKNOWN)
    return AJA_STATUS_FAIL;

  switch (sdiFormat) {
    case NTV2_FORMAT_1080p_5000_A:
    case NTV2_FORMAT_1080p_5000_B:
      videoFormat = NTV2_FORMAT_1080p_5000_A;
      if (quad)
        videoFormat = NTV2_FORMAT_4x1920x1080p_5000;
      break;
    case NTV2_FORMAT_1080p_5994_A:
    case NTV2_FORMAT_1080p_5994_B:
      videoFormat = NTV2_FORMAT_1080p_5994_A;
      if (quad)
        videoFormat = NTV2_FORMAT_4x1920x1080p_5994;
      break;
    case NTV2_FORMAT_1080p_6000_A:
    case NTV2_FORMAT_1080p_6000_B:
      videoFormat = NTV2_FORMAT_1080p_6000_A;
      if (quad)
        videoFormat = NTV2_FORMAT_4x1920x1080p_6000;
      break;
    default:
      videoFormat = sdiFormat;
      break;
  }

  return AJA_STATUS_SUCCESS;
}


AJA_FrameRate NTV2GstAV::GetAJAFrameRate (NTV2FrameRate frameRate)
{
  switch (frameRate) {
    case NTV2_FRAMERATE_2398:
      return AJA_FrameRate_2398;
    case NTV2_FRAMERATE_2400:
      return AJA_FrameRate_2400;
    case NTV2_FRAMERATE_2500:
      return AJA_FrameRate_2500;
    case NTV2_FRAMERATE_2997:
      return AJA_FrameRate_2997;
    case NTV2_FRAMERATE_3000:
      return AJA_FrameRate_3000;
    case NTV2_FRAMERATE_4795:
      return AJA_FrameRate_4795;
    case NTV2_FRAMERATE_4800:
      return AJA_FrameRate_4800;
    case NTV2_FRAMERATE_5000:
      return AJA_FrameRate_5000;
    case NTV2_FRAMERATE_5994:
      return AJA_FrameRate_5994;
    case NTV2_FRAMERATE_6000:
      return AJA_FrameRate_6000;
    default:
      break;
  }

  return AJA_FrameRate_Unknown;
}


bool NTV2GstAV::DoCallback (CallBackType type, void *msg)
{
  if (type == VIDEO_CALLBACK) {
    if (mVideoCallback) {
      return mVideoCallback (mVideoCallbackRefcon, msg);
    }
  } else if (type == AUDIO_CALLBACK) {
    if (mAudioCallback) {
      return mAudioCallback (mAudioCallbackRefcon, msg);
    }
  }
  return false;
}
