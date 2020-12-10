/**
	@file		ntv2devicefeatures.hh
	@brief		Declares NTV2DeviceCanDo... and NTV2DeviceGetNum... functions.
				This module is included at compile time from 'ntv2devicefeatures.h'.
	@copyright	(C) 2004-2020 AJA Video Systems, Inc.	Proprietary and confidential information.
	@note		Generated by 'ajalibraries/ajantv2/sdkgen/ntv2sdkgen.py' on Fri Dec  4 12:56:47 2020.
**/
#ifndef NTV2DEVICEFEATURES_HH
#define NTV2DEVICEFEATURES_HH

#if defined(__cplusplus) && defined(NTV2_BUILDING_DRIVER)
extern "C"
{
#endif


/**
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanChangeEmbeddedAudioClock (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID does not have fixed frame buffer sizes.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanChangeFrameBufferSize (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has at least one UFC, and it can be disabled.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDisableUFC (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID supports 12g routing crosspoints.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDo12gRouting (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device supports 12G SDI
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDo12GSDI (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device supports SMPTE 2110
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDo2110 (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID is capable of handling 2Kx1556 video.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDo2KVideo (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can perform 3G level B to 3G level A conversion.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDo3GLevelConversion (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID supports SMPTE 425 mux control.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDo425Mux (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can handle 4K/UHD video.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDo4KVideo (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can handle 8K/UHD2 video.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDo8KVideo (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has at least one AES/EBU audio input.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoAESAudioIn (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has at least one analog audio input or output.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoAnalogAudio (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has at least one analog video input.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoAnalogVideoIn (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has at least one analog video output.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoAnalogVideoOut (const NTV2DeviceID inDeviceID);

/**
	@return		True if audio system(s) on the device having the given ID can be set to a 192kHz sample rate.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoAudio192K (const NTV2DeviceID inDeviceID);

/**
	@return		True if the audio system(s) on the device having the given ID can be configured to embed/de-embed only 2 audio channels.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoAudio2Channels (const NTV2DeviceID inDeviceID);

/**
	@return		True if the audio system(s) on the device having the given ID can be configured to embed/de-embed only 6 audio channels.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoAudio6Channels (const NTV2DeviceID inDeviceID);

/**
	@return		True if the audio system(s) on the device having the given ID can be configured to embed/de-embed only 8 audio channels.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoAudio8Channels (const NTV2DeviceID inDeviceID);

/**
	@return		True if audio system(s) on the device having the given ID can be set to a 96kHz sample rate.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoAudio96K (const NTV2DeviceID inDeviceID);

/**
	@return		True if audio system(s) on the device having the given ID have an adjustable delay.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoAudioDelay (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device supports an audio mixer (firmware dependent).
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoAudioMixer (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can be connected to an AJA breakout box.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoBreakoutBox (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can capture (ingest) video.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoCapture (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has at least one programmable LUT.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoColorCorrection (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID supports ANC insertion/extraction.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoCustomAnc (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has a mixer/keyer whose opacity is adjustable.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoDSKOpacity (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can input/output 10-bit RGB over 2-wire SDI.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoDualLink (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can squeeze/stretch between 1920x1080/1280x1080 and 1280x720/960x720.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoDVCProHD (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has enhanced color space converter capability.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoEnhancedCSC (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device supports frame pulse source independent of reference source
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoFramePulseSelect (const NTV2DeviceID inDeviceID);

/**
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoFrameStore1Display (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID supports HDMI HDR output.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoHDMIHDROut (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can handle 3D-stereo video output over HDMI.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoHDMIOutStereo (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can squeeze/stretch between 1920x1080 and 1440x1080.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoHDV (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can handle HD (High Definition) video.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoHDVideo (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device supports 1080p > 50 RGB rates
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoHFRRGB (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID ingests and/or plays video via TCP/IP (instead of SDI/HDMI/etc).
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoIP (const NTV2DeviceID inDeviceID);

/**
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoIsoConvert (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device supports the JPEG 2000 codec
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoJ2K (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can take in LTC (Linear TimeCode) from one of its inputs.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoLTC (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can read LTC (Linear TimeCode) from its reference input.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoLTCInOnRefPort (const NTV2DeviceID inDeviceID);

/**
	@return		True if the DMA hardware on the device having the given ID supports MSI (Message Signaled Interrupts).
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoMSI (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can simultaneously handle different video formats on more than one SDI input or output.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoMultiFormat (const NTV2DeviceID inDeviceID);

/**
	@return		True if the audio system(s) on the device having the given ID can set the per-audio-channel-pair non-PCM (Pulse Code Modulation) bits in outgoing embedded audio.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoPCMControl (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has per-audio-channel-pair PCM detection capabilities.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoPCMDetection (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID supports Programmed I/O.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoPIO (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can output (play) video.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoPlayback (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has at least one programmable color space converter widget.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoProgrammableCSC (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has at least one RS-422 serial port, and it (they) can be programmed (for baud rate, parity, etc.).
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoProgrammableRS422 (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can can accommodate Apple ProRes-compressed video in its frame buffers.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoProRes (const NTV2DeviceID inDeviceID);

/**
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoQREZ (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can handle quarter-sized frames (pixel-halving and line-halving during input, pixel-double and line-double during output).
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoQuarterExpand (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can do frame rate conversion.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoRateConvert (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can do RGB over 3G Level A.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoRGBLevelAConversion (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device with the given ID has CSCs capable of splitting the key (alpha) and YCbCr (fill) from RGB frame buffers that include alpha. Has nothing to do with RGB wire formats.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoRGBPlusAlphaOut (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can insert and/or extract RP-188/VITC.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoRP188 (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can perform SDI error checking.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoSDIErrorChecks (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can handle SD (Standard Definition) video.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoSDVideo (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID uses a 'stacked' arrangement of its audio buffers.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoStackedAudio (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID supports 3D video input over dual-stream SDI.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoStereoIn (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID supports 3D video output over dual-stream SDI.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoStereoOut (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID connects to the host using a Thunderbolt cable.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoThunderbolt (const NTV2DeviceID inDeviceID);

/**
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoVideoProcessing (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can insert and/or extract RP-188/VITC2.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoVITC2 (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device is capable of doing a warm boot rather than power cycling after firmware update.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanDoWarmBootFPGA (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can measure its temperature.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanMeasureTemperature (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can report if its "fail-safe" firmware is loaded and running.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanReportFailSafeLoaded (const NTV2DeviceID inDeviceID);

/**
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanReportFrameSize (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID can report its running (and not necessarily installed) firmware date.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanReportRunningFirmwareDate (const NTV2DeviceID inDeviceID);

/**
	@return		True if the fan on the device having the given ID can be thermostatically controlled.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceCanThermostat (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has a pair of unbalanced RCA audio monitor output jacks.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasAudioMonitorRCAJacks (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has a bi-directional analog audio connector.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasBiDirectionalAnalogAudio (const NTV2DeviceID inDeviceID);

/**
	@return		True if the SDI connectors on the device having the given ID are bi-directional.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasBiDirectionalSDI (const NTV2DeviceID inDeviceID);

/**
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasColorSpaceConverterOnChannel2 (const NTV2DeviceID inDeviceID);

/**
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasGenlockv2 (const NTV2DeviceID inDeviceID);

/**
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasGenlockv3 (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has a headphone jack.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasHeadphoneJack (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has an HEVC M30 encoder/decoder.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasHEVCM30 (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has an HEVC M31 encoder.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasHEVCM31 (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has LED audio meters.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasLEDAudioMeters (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has NorthWest Logic DMA hardware.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasNWL (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID supports 2nd-generation PCIe.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasPCIeGen2 (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device can be configured and controlled by the retail services and AJA Control Panel.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasRetailSupport (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID supports rotary encoder volume control
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasRotaryEncoder (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID has bypass relays on its SDI connectors.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasSDIRelays (const NTV2DeviceID inDeviceID);

/**
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasSPIFlash (const NTV2DeviceID inDeviceID);

/**
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasSPIFlashSerial (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID uses version 2 SPI hardware.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasSPIv2 (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID uses version 3 SPI hardware.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasSPIv3 (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID uses version 4 SPI hardware.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasSPIv4 (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID uses version 5 SPI hardware.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasSPIv5 (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device has Xilinx DMA engine
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceHasXilinxDMA (const NTV2DeviceID inDeviceID);

/**
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceIs64Bit (const NTV2DeviceID inDeviceID);

/**
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceIsDirectAddressable (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID connects to the host with a cable.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceIsExternalToHost (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID is supported by this SDK.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceIsSupported (const NTV2DeviceID inDeviceID);

/**
	@return		True if the routing for the device having the given ID can be queried and/or changed.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceNeedsRoutingSetup (const NTV2DeviceID inDeviceID);

/**
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport bool NTV2DeviceSoftwareCanChangeFrameBufferSize (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the size, in bytes, of the device's active RAM available for video and audio.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport ULWord NTV2DeviceGetActiveMemorySize (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the version number of the DAC on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetDACVersion (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the down-converter delay on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetDownConverterDelay (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the version number of the HDMI input or output hardware on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport ULWord NTV2DeviceGetHDMIVersion (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the version number of the LUT(s) on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport ULWord NTV2DeviceGetLUTVersion (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the maximum number of audio channels that a single audio system can support on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetMaxAudioChannels (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the highest register number for the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport ULWord NTV2DeviceGetMaxRegisterNumber (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the maximum number of 32-bit words that the DMA engine can move at a time on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport ULWord NTV2DeviceGetMaxTransferCount (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of 2022 channels configured on SFP 1 on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNum2022ChannelsSFP1 (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of 2022 channels configured on SFP 2 on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNum2022ChannelsSFP2 (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of quarter-size 4K/UHD down-converters on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNum4kQuarterSizeConverters (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of AES/EBU audio input channels on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumAESAudioInputChannels (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of AES/EBU audio output channels on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumAESAudioOutputChannels (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of analog audio input channels on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumAnalogAudioInputChannels (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of analog audio output channels on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumAnalogAudioOutputChannels (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of analog video inputs on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumAnalogVideoInputs (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of analog video outputs on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumAnalogVideoOutputs (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of independent audio systems on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumAudioSystems (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of cross-converters on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumCrossConverters (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of colorspace converter widgets on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumCSCs (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of DMA engines on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport ULWord NTV2DeviceGetNumDMAEngines (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of down-converters on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumDownConverters (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of SDI-embedded input audio channels supported by the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumEmbeddedAudioInputChannels (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of SDI-embedded output audio channels supported by the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumEmbeddedAudioOutputChannels (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of frame stores on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumFrameStores (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of frame sync widgets on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumFrameSyncs (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of HDMI audio input channels on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumHDMIAudioInputChannels (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of HDMI audio output channels on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumHDMIAudioOutputChannels (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of HDMI video inputs on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumHDMIVideoInputs (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of HDMI video outputs on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumHDMIVideoOutputs (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of input converter widgets on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumInputConverters (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of analog LTC inputs on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumLTCInputs (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of analog LTC outputs on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumLTCOutputs (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of LUT widgets on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumLUTs (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of mixer/keyer widgets on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumMixers (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of output converter widgets on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumOutputConverters (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of reference video inputs on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumReferenceVideoInputs (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of RS-422 serial ports on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumSerialPorts (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of up-converters on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumUpConverters (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of video channels supported on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport ULWord NTV2DeviceGetNumVideoChannels (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of SDI video inputs on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumVideoInputs (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the number of SDI video outputs on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport UWord NTV2DeviceGetNumVideoOutputs (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the highest bit number of the LED bits in the Global Control Register on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport ULWord NTV2DeviceGetPingLED (const NTV2DeviceID inDeviceID);

/**
	@return		Returns the version number of the UFC on the device having the given ID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
**/
AJAExport ULWord NTV2DeviceGetUFCVersion (const NTV2DeviceID inDeviceID);

/**
	@return		True if the device having the given ID supports the given NTV2ConversionMode.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
	@param[in]	inConversionMode	Specifies the NTV2ConversionMode.
**/
AJAExport bool NTV2DeviceCanDoConversionMode (const NTV2DeviceID inDeviceID, const NTV2ConversionMode inConversionMode);

/**
	@return		True if the device having the given ID supports the given NTV2DSKMode.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
	@param[in]	inDSKMode		Specifies the NTV2DSKMode.
**/
AJAExport bool NTV2DeviceCanDoDSKMode (const NTV2DeviceID inDeviceID, const NTV2DSKMode inDSKMode);

/**
	@return		True if the device having the given ID supports the given NTV2FrameBufferFormat.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
	@param[in]	inFBFormat		Specifies the NTV2FrameBufferFormat.
**/
AJAExport bool NTV2DeviceCanDoFrameBufferFormat (const NTV2DeviceID inDeviceID, const NTV2FrameBufferFormat inFBFormat);

/**
	@return		True if the device having the given ID supports the given NTV2InputSource.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
	@param[in]	inInputSource	Specifies the NTV2InputSource.
**/
AJAExport bool NTV2DeviceCanDoInputSource (const NTV2DeviceID inDeviceID, const NTV2InputSource inInputSource);

/**
	@return		True if the device having the given ID supports the given NTV2VideoFormat.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
	@param[in]	inVideoFormat	Specifies the NTV2VideoFormat.
**/
AJAExport bool NTV2DeviceCanDoVideoFormat (const NTV2DeviceID inDeviceID, const NTV2VideoFormat inVideoFormat);

/**
	@return		True if the device having the given ID supports the given NTV2WidgetID.
	@param[in]	inDeviceID		Specifies the NTV2DeviceID of interest.
	@param[in]	inWidgetID		Specifies the NTV2WidgetID.
**/
AJAExport bool NTV2DeviceCanDoWidget (const NTV2DeviceID inDeviceID, const NTV2WidgetID inWidgetID);
#define NTV2DEVICEFEATURES_HH

#if defined(__cplusplus) && defined(NTV2_BUILDING_DRIVER)
}
#endif



#endif	//	NTV2DEVICEFEATURES_HH
