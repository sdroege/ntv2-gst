/**
	@file		ntv2virtualregisters.h
	@copyright	Copyright (C) 2011-2019 AJA Video Systems, Inc.All rights reserved.
	@brief		Declares enums for virtual registers used in all platform drivers and the SDK.
**/

#ifndef NTV2VIRTUALREGISTERS_H
#define NTV2VIRTUALREGISTERS_H

#define VIRTUALREG_START			10000	//	Virtual registers start at register number 10000
#define	MAX_NUM_VIRTUAL_REGISTERS	1024	//	Starting in SDK 12.6, there's room for 1024 virtual registers

/**
	@brief	Virtual registers are used to pass 32-bit values to/from the device driver, and aren't always
			associated with hardware registers.

	@note	Drivers after SDK 12.5.x all allocate a 4K page for storing an array of 1024 x 4-byte integers.
			OEM applications can store values in slots kVRegFirstOEM thru kVRegLast, inclusive.
			AJA recommends storing at kVRegLast, kVRegLast-1, kVRegLast-2, etc., being careful
			to never store anything below kVRegFirstOEM.

	@note	AJA does not reserve virtual registers for OEMs, and thus, collisions may occur with other OEM applications.
**/
typedef enum
{
	// Common to all platforms
	kVRegDriverVersion						= VIRTUALREG_START,			///< @brief			Packed driver version -- use NTV2DriverVersionEncode, NTV2DriverVersionDecode* macros to encode/decode

	// Windows platform custom section
	kVRegAudioRecordPinDelay				= VIRTUALREG_START+1,		// Audio Delay relative to video, for Windows Media capture
	kVRegRelativeVideoPlaybackDelay			= VIRTUALREG_START+2,		// Video Delay relative to audio, for Windows Media playback
	kVRegGlobalAudioPlaybackMode			= VIRTUALREG_START+3,		// Shared with Linux, but not Mac
	kVRegFlashProgramKey					= VIRTUALREG_START+4,
	kVRegStrictTiming						= VIRTUALREG_START+5,		// Drift Correction requires Strict Frame Timing for Windows Media playback;Required for BackHaul;Correlate Presentation Time Stamp with Graph Clock;Turn off (default) to allow Playback even when Graph Manager gives us a Bogus Clcok!

	// COMMON_VIRTUAL_REGS_KONA2
	kVRegInputSelect						= VIRTUALREG_START+20,		/* Input 1, Input 2, DualLink	*/
	kVRegSecondaryFormatSelect				= VIRTUALREG_START+21,		/* NTV2VideoFormats				*/
	kVRegDigitalOutput1Select				= VIRTUALREG_START+22,		/* Primary, Secondary			*/
	kVRegDigitalOutput2Select				= VIRTUALREG_START+23,		/* Primary, Secondary, DualLink */
	kVRegAnalogOutputSelect					= VIRTUALREG_START+24,		/* Primary, Secondary			*/
	kVRegAnalogOutputType					= VIRTUALREG_START+25,		/* Analog output type			*/
	kVRegAnalogOutBlackLevel				= VIRTUALREG_START+26,		/* Analog output black level	*/

	// COMMON_VIRTUAL_REGS_MISC
	kVRegVideoOutPauseMode					= VIRTUALREG_START+40,		/* whether we pause on a frame or a field */
	kVRegPulldownPattern					= VIRTUALREG_START+41,		/* which 3:2 pulldown pattern to use */
	kVRegColorSpaceMode						= VIRTUALREG_START+42,		/* which color space matrix (Rec601, Rec709, ...) to use */
	kVRegGammaMode							= VIRTUALREG_START+43,		/* which gamma LUT (Rec601, Rec709, ...) to use */
	kVRegLUTType							= VIRTUALREG_START+44,		/* the current LUT function loaded into hardware */
	kVRegRGB10Range							= VIRTUALREG_START+45,		/* the user-selected 10-bit RGB range (SMPTE 64-940 or Full 0-1023) */
	kVRegRGB10Endian						= VIRTUALREG_START+46,		/* the user selected 10-bit RGB endian */

	// Windows platform custom section
	kVRegBitFileDownload					= VIRTUALREG_START+50,		// NTV2BitfileType
	kVRegSaveRegistersToRegistry			= VIRTUALREG_START+51,		// no argument				
	kVRegRecallRegistersFromRegistry		= VIRTUALREG_START+52,		// same address as above, on purpose
	kVRegClearAllSubscriptions				= VIRTUALREG_START+53,		// NTV2BitfileType
	kVRegRestoreHardwareProcampRegisters	= VIRTUALREG_START+54,
	kVRegAcquireReferenceCount				= VIRTUALREG_START+55,		// Acquire the board with a reference count on acquire
	kVRegReleaseReferenceCount				= VIRTUALREG_START+56,

	kVRegDTAudioMux0						= VIRTUALREG_START+60,		// Firmware desired
	kVRegDTAudioMux1						= VIRTUALREG_START+61,		// Firmware desired
	kVRegDTAudioMux2						= VIRTUALREG_START+62,		// Firmware desired
	kVRegDTFirmware							= VIRTUALREG_START+63,		// Firmware desired
	kVRegDTVersionAja						= VIRTUALREG_START+64,		// Driver version (Aja)
	kVRegDTVersionDurian					= VIRTUALREG_START+65,		// Driver version (Durian)
	kVRegDTAudioCapturePinConnected			= VIRTUALREG_START+66,		// Audio Capture Pin Connected ?

	kVRegTimeStampMode						= VIRTUALREG_START+70,		// 0-Scaled timestamps(100ns), 1- Raw PerformanceCounter
	kVRegTimeStampLastOutputVerticalLo		= VIRTUALREG_START+71,		// Lower 32 bits.
	kVRegTimeStampLastOutputVerticalHi		= VIRTUALREG_START+72,		// Hi 32 bits.
	kVRegTimeStampLastInput1VerticalLo		= VIRTUALREG_START+73,
	kVRegTimeStampLastInput1VerticalHi		= VIRTUALREG_START+74,
	kVRegTimeStampLastInput2VerticalLo		= VIRTUALREG_START+75,
	kVRegTimeStampLastInput2VerticalHi		= VIRTUALREG_START+76,
	kVRegNumberVideoMappingRegisters		= VIRTUALREG_START+77,
	kVRegNumberAudioMappingRegisters		= VIRTUALREG_START+78,
	kVRegAudioSyncTolerance					= VIRTUALREG_START+79,
	kVRegDmaSerialize						= VIRTUALREG_START+80,
	kVRegSyncChannel						= VIRTUALREG_START+81,		// Mac name
	kVRegSyncChannels						= VIRTUALREG_START+81,		// Windows and Linux name for the same thing
	kVRegSoftwareUartFifo					= VIRTUALREG_START+82,
	kVRegTimeCodeCh1Delay					= VIRTUALREG_START+83,
	kVRegTimeCodeCh2Delay					= VIRTUALREG_START+84,
	kVRegTimeCodeIn1Delay					= VIRTUALREG_START+85,
	kVRegTimeCodeIn2Delay					= VIRTUALREG_START+86,
	kVRegTimeCodeCh3Delay					= VIRTUALREG_START+87,
	kVRegTimeCodeCh4Delay					= VIRTUALREG_START+88,
	kVRegTimeCodeIn3Delay					= VIRTUALREG_START+89,
	kVRegTimeCodeIn4Delay					= VIRTUALREG_START+90,
	kVRegTimeCodeCh5Delay					= VIRTUALREG_START+91,
	kVRegTimeCodeIn5Delay					= VIRTUALREG_START+92,
	kVRegTimeCodeCh6Delay					= VIRTUALREG_START+93,
	kVRegTimeCodeIn6Delay					= VIRTUALREG_START+94,
	kVRegTimeCodeCh7Delay					= VIRTUALREG_START+95,
	kVRegTimeCodeIn7Delay					= VIRTUALREG_START+96,
	kVRegTimeCodeCh8Delay					= VIRTUALREG_START+97,
	kVRegTimeCodeIn8Delay					= VIRTUALREG_START+98,

	kVRegDebug1								= VIRTUALREG_START+100,		// general debug register

	// Control Panel virtual registers
	kVRegDisplayReferenceSelect				= VIRTUALREG_START+120,
	kVRegVANCMode							= VIRTUALREG_START+121,
	kVRegDualStreamTransportType			= VIRTUALREG_START+122,
	kVRegSDIOut1TransportType				= kVRegDualStreamTransportType,
	kVRegDSKMode							= VIRTUALREG_START+123,
	kVRegIsoConvertEnable					= VIRTUALREG_START+124,
	kVRegDSKAudioMode						= VIRTUALREG_START+125,
	kVRegDSKForegroundMode					= VIRTUALREG_START+126,
	kVRegDSKForegroundFade					= VIRTUALREG_START+127,
	kVRegCaptureReferenceSelect				= VIRTUALREG_START+128,

	kVReg2XTransferMode						= VIRTUALREG_START+130,
	kVRegSDIOutput1RGBRange					= VIRTUALREG_START+131,
	kVRegSDIInput1FormatSelect				= VIRTUALREG_START+132,
	kVRegSDIInput2FormatSelect				= VIRTUALREG_START+133,
	kVRegSDIInput1RGBRange					= VIRTUALREG_START+134,
	kVRegSDIInput2RGBRange					= VIRTUALREG_START+135,
	kVRegSDIInput1Stereo3DMode				= VIRTUALREG_START+136,
	kVRegSDIInput2Stereo3DMode				= VIRTUALREG_START+137,
	kVRegFrameBuffer1RGBRange				= VIRTUALREG_START+138,
	kVRegFrameBuffer1Stereo3DMode			= VIRTUALREG_START+139,

	kVRegHDMIInRgbRange						= VIRTUALREG_START+140,
	kVRegHDMIOutRgbRange					= VIRTUALREG_START+141,
	kVRegAnalogInBlackLevel					= VIRTUALREG_START+142,
	kVRegAnalogInputType					= VIRTUALREG_START+143,
	kVRegHDMIOutColorSpaceModeCtrl			= VIRTUALREG_START+144,
	kVRegHDMIOutProtocolMode				= VIRTUALREG_START+145,
	kVRegHDMIOutStereoSelect				= VIRTUALREG_START+146,
	kVRegHDMIOutStereoCodecSelect			= VIRTUALREG_START+147,
	kVRegReversePulldownOffset				= VIRTUALREG_START+148,
	kVRegSDIInput1ColorSpaceMode			= VIRTUALREG_START+149,

	kVRegSDIInput2ColorSpaceMode			= VIRTUALREG_START+150,
	kVRegSDIOutput2RGBRange					= VIRTUALREG_START+151,
	kVRegSDIOutput1Stereo3DMode				= VIRTUALREG_START+152,
	kVRegSDIOutput2Stereo3DMode				= VIRTUALREG_START+153,
	kVRegFrameBuffer2RGBRange				= VIRTUALREG_START+154,
	kVRegFrameBuffer2Stereo3DMode			= VIRTUALREG_START+155,
	kVRegAudioGainDisable					= VIRTUALREG_START+156,
	kVRegDBLAudioEnable						= VIRTUALREG_START+157,
	kVRegActiveVideoOutFilter				= VIRTUALREG_START+158,
	kVRegAudioInputMapSelect				= VIRTUALREG_START+159,

	kVRegAudioInputDelay					= VIRTUALREG_START+160,
	kVRegDSKGraphicFileIndex				= VIRTUALREG_START+161,
	kVRegTimecodeBurnInMode					= VIRTUALREG_START+162,
	kVRegUseQTTimecode						= VIRTUALREG_START+163,
	kVRegAvailable164						= VIRTUALREG_START+164,
	kVRegRP188SourceSelect					= VIRTUALREG_START+165,
	kVRegQTCodecModeDebug					= VIRTUALREG_START+166,
	kVRegHDMIOutColorSpaceModeStatus		= VIRTUALREG_START+167,		// deprecated
	kVRegDeviceOnline						= VIRTUALREG_START+168,
	kVRegIsDefaultDevice					= VIRTUALREG_START+169,

	kVRegDesktopFrameBufferStatus			= VIRTUALREG_START+170,
	kVRegSDIOutput1ColorSpaceMode			= VIRTUALREG_START+171,
	kVRegSDIOutput2ColorSpaceMode			= VIRTUALREG_START+172,
	kVRegAudioOutputDelay					= VIRTUALREG_START+173,
	kVRegTimelapseEnable					= VIRTUALREG_START+174,
	kVRegTimelapseCaptureValue				= VIRTUALREG_START+175,
	kVRegTimelapseCaptureUnits				= VIRTUALREG_START+176,
	kVRegTimelapseIntervalValue				= VIRTUALREG_START+177,
	kVRegTimelapseIntervalUnits				= VIRTUALREG_START+178,
	kVRegFrameBufferInstalled				= VIRTUALREG_START+179,

	kVRegAnalogInStandard					= VIRTUALREG_START+180,
	kVRegOutputTimecodeOffset				= VIRTUALREG_START+181,		// deprecated
	kVRegOutputTimecodeType					= VIRTUALREG_START+182,		// deprecated
	kVRegQuicktimeUsingBoard				= VIRTUALREG_START+183,		// Not used in Mac
	kVRegApplicationPID						= VIRTUALREG_START+184,		// The rest of this section handled by IOCTL in Mac
	kVRegApplicationCode					= VIRTUALREG_START+185,
	kVRegReleaseApplication					= VIRTUALREG_START+186,
	kVRegForceApplicationPID				= VIRTUALREG_START+187,
	kVRegForceApplicationCode				= VIRTUALREG_START+188,
	kVRegIpConfigStreamRefresh				= VIRTUALREG_START+189,
	kVRegSDIInput1Raster					= VIRTUALREG_START+190,
	kVRegInputChangedCount					= VIRTUALREG_START+191,

	// COMMON_VIRTUAL_REGS_PROCAMP_CONTROLS
	kVRegProcAmpSDRegsInitialized			= VIRTUALREG_START+200,
	kVRegProcAmpStandardDefBrightness		= VIRTUALREG_START+201,
	kVRegProcAmpStandardDefContrast			= VIRTUALREG_START+202,
	kVRegProcAmpStandardDefSaturation		= VIRTUALREG_START+203,
	kVRegProcAmpStandardDefHue				= VIRTUALREG_START+204,
	kVRegProcAmpStandardDefCbOffset			= VIRTUALREG_START+205,
	kVRegProcAmpStandardDefCrOffset			= VIRTUALREG_START+206,
	kVRegProcAmpEndStandardDefRange			= VIRTUALREG_START+207,

	kVRegProcAmpHDRegsInitialized			= VIRTUALREG_START+220,
	kVRegProcAmpHighDefBrightness			= VIRTUALREG_START+221,
	kVRegProcAmpHighDefContrast				= VIRTUALREG_START+222,
	kVRegProcAmpHighDefSaturationCb			= VIRTUALREG_START+223,
	kVRegProcAmpHighDefSaturationCr			= VIRTUALREG_START+224,
	kVRegProcAmpHighDefHue					= VIRTUALREG_START+225,
	kVRegProcAmpHighDefCbOffset				= VIRTUALREG_START+226,
	kVRegProcAmpHighDefCrOffset				= VIRTUALREG_START+227,
	kVRegProcAmpEndHighDefRange				= VIRTUALREG_START+228,

	// COMMON_VIRTUAL_REGS_USERSPACE_BUFFLEVEL
	kVRegChannel1UserBufferLevel			= VIRTUALREG_START+240,
	kVRegChannel2UserBufferLevel			= VIRTUALREG_START+241,
	kVRegInput1UserBufferLevel				= VIRTUALREG_START+242,
	kVRegInput2UserBufferLevel				= VIRTUALREG_START+243,

	// COMMON_VIRTUAL_REGS_EX
	kVRegProgressivePicture					= VIRTUALREG_START+260,
	kVRegLUT2Type							= VIRTUALREG_START+261,
	kVRegLUT3Type							= VIRTUALREG_START+262,
	kVRegLUT4Type							= VIRTUALREG_START+263,
	kVRegDigitalOutput3Select				= VIRTUALREG_START+264,
	kVRegDigitalOutput4Select				= VIRTUALREG_START+265,
	kVRegHDMIOutputSelect					= VIRTUALREG_START+266,
	kVRegRGBRangeConverterLUTType			= VIRTUALREG_START+267,
	kVRegTestPatternChoice					= VIRTUALREG_START+268,
	kVRegTestPatternFormat					= VIRTUALREG_START+269,
	kVRegEveryFrameTaskFilter				= VIRTUALREG_START+270,
	kVRegDefaultInput						= VIRTUALREG_START+271,
	kVRegDefaultVideoOutMode				= VIRTUALREG_START+272,
	kVRegDefaultVideoFormat					= VIRTUALREG_START+273,
	kVRegDigitalOutput5Select				= VIRTUALREG_START+274,
	kVRegLUT5Type							= VIRTUALREG_START+275,

	// Macintosh platform custom section
	kVRegMacUserModeDebugLevel				= VIRTUALREG_START+300,
	kVRegMacKernelModeDebugLevel			= VIRTUALREG_START+301,
	kVRegMacUserModePingLevel				= VIRTUALREG_START+302,
	kVRegMacKernelModePingLevel				= VIRTUALREG_START+303,
	kVRegLatencyTimerValue					= VIRTUALREG_START+304,
	kVRegAudioAVSyncEnable					= VIRTUALREG_START+305,
	kVRegAudioInputSelect					= VIRTUALREG_START+306,
	kVRegSerialSuspended					= VIRTUALREG_START+307,
	kVRegXilinxProgramming					= VIRTUALREG_START+308,
	kVRegETTDiagLastSerialTimestamp			= VIRTUALREG_START+309,
	kVRegETTDiagLastSerialTimecode			= VIRTUALREG_START+310,
	kVRegStartupStatusFlags					= VIRTUALREG_START+311,
	kVRegRGBRangeMode						= VIRTUALREG_START+312,
	kVRegEnableQueuedDMAs					= VIRTUALREG_START+313,		//	If non-zero, enables queued DMAs on multi-engine devices (Mac only)

	// Linux platform custom section
	kVRegBA0MemorySize						= VIRTUALREG_START+320,		// Memory-mapped register (BAR0) window size in bytes.
	kVRegBA1MemorySize						= VIRTUALREG_START+321,		// Memory-mapped framebuffer window size in bytes.
	kVRegBA4MemorySize						= VIRTUALREG_START+322,
	kVRegNumDmaDriverBuffers				= VIRTUALREG_START+323,		// Number of bigphysarea frames available (Read only).
	kVRegDMADriverBufferPhysicalAddress		= VIRTUALREG_START+324,		// Physical address of bigphysarea buffer
	kVRegBA2MemorySize						= VIRTUALREG_START+325,
	kVRegAcquireLinuxReferenceCount			= VIRTUALREG_START+326,		// Acquire the board with a reference count on acquire
	kVRegReleaseLinuxReferenceCount			= VIRTUALREG_START+327,

	// IoHD virtual registers
	kVRegAdvancedIndexing					= VIRTUALREG_START+340,		//	OBSOLETE after 12.4
	kVRegTimeStampLastInput3VerticalLo		= VIRTUALREG_START+341,
	kVRegTimeStampLastInput3VerticalHi		= VIRTUALREG_START+342,
	kVRegTimeStampLastInput4VerticalLo		= VIRTUALREG_START+343,
	kVRegTimeStampLastInput4VerticalHi		= VIRTUALREG_START+344,
	kVRegTimeStampLastInput5VerticalLo		= VIRTUALREG_START+345,
	kVRegTimeStampLastInput5VerticalHi		= VIRTUALREG_START+346,
	kVRegTimeStampLastInput6VerticalLo		= VIRTUALREG_START+347,
	kVRegTimeStampLastInput6VerticalHi		= VIRTUALREG_START+348,
	kVRegTimeStampLastInput7VerticalLo		= VIRTUALREG_START+349,
	kVRegTimeStampLastInput7VerticalHi		= VIRTUALREG_START+350,
	kVRegTimeStampLastInput8VerticalLo		= VIRTUALREG_START+351,
	kVRegTimeStampLastInput8VerticalHi		= VIRTUALREG_START+352,

	kVRegTimeStampLastOutput2VerticalLo		= VIRTUALREG_START+353,
	kVRegTimeStampLastOutput2VerticalHi		= VIRTUALREG_START+354,
	
	kVRegTimeStampLastOutput3VerticalLo		= VIRTUALREG_START+355,
	kVRegTimeStampLastOutput3VerticalHi		= VIRTUALREG_START+356,
	kVRegTimeStampLastOutput4VerticalLo		= VIRTUALREG_START+357,
	kVRegTimeStampLastOutput4VerticalHi		= VIRTUALREG_START+358,
	
	kVRegTimeStampLastOutput5VerticalLo		= VIRTUALREG_START+359,
	
	kVRegTimeStampLastOutput5VerticalHi		= VIRTUALREG_START+360,
	kVRegTimeStampLastOutput6VerticalLo		= VIRTUALREG_START+361,
	kVRegTimeStampLastOutput6VerticalHi		= VIRTUALREG_START+362,
	kVRegTimeStampLastOutput7VerticalLo		= VIRTUALREG_START+363,
	kVRegTimeStampLastOutput7VerticalHi		= VIRTUALREG_START+364,
	kVRegTimeStampLastOutput8VerticalLo		= VIRTUALREG_START+365,
	
	kVRegResetCycleCount					= VIRTUALREG_START+366,		// counts the number of device resets caused by plug-and-play or sleep, increments each time
	kVRegUseProgressive						= VIRTUALREG_START+367,		// when an option (e.g. Avid MC) choose P over PSF formats
	
	kVRegFlashSize							= VIRTUALREG_START+368,		// size of the flash partition for flash status
	kVRegFlashStatus						= VIRTUALREG_START+369,		// progress of flash patition
	kVRegFlashState							= VIRTUALREG_START+370,		// state status of flash

	kVRegPCIDeviceID						= VIRTUALREG_START+371,		// set by driver (read only)

	kVRegUartRxFifoSize						= VIRTUALREG_START+372,
	
	kVRegEFTNeedsUpdating					= VIRTUALREG_START+373,		// set when any retail virtual register has been changed
	
	kVRegSuspendSystemAudio					= VIRTUALREG_START+374,     // set when app wants to use AC audio and disable host audio (e.g., CoreAudio on MacOS)
	kVRegAcquireReferenceCounter			= VIRTUALREG_START+375,

	kVRegTimeStampLastOutput8VerticalHi		= VIRTUALREG_START+376,
	
	kVRegFramesPerVertical					= VIRTUALREG_START+377,
	kVRegServicesInitialized				= VIRTUALREG_START+378,		// set true when device is initialized by services

	kVRegFrameBufferGangCount				= VIRTUALREG_START+379,

	kVRegChannelCrosspointFirst				= VIRTUALREG_START+380,
                                                                        //	kVRegChannelCrosspointFirst+1
                                                                        //	kVRegChannelCrosspointFirst+2
                                                                        //	kVRegChannelCrosspointFirst+3
                                                                        //	kVRegChannelCrosspointFirst+4
                                                                        //	kVRegChannelCrosspointFirst+5
                                                                        //	kVRegChannelCrosspointFirst+6
	kVRegChannelCrosspointLast				= VIRTUALREG_START+387,		//	kVRegChannelCrosspointFirst+7

//	kVRegDriverVersionMajor					= VIRTUALREG_START+388,		///< @deprecated	Obsolete starting in SDK 13.0, replaced by kVRegDriverVersion
//	kVRegDriverVersionMinor					= VIRTUALREG_START+389,		///< @deprecated	Obsolete starting in SDK 13.0, replaced by kVRegDriverVersion
//	kVRegDriverVersionPoint					= VIRTUALREG_START+390,		///< @deprecated	Obsolete starting in SDK 13.0, replaced by kVRegDriverVersion
	kVRegFollowInputFormat					= VIRTUALREG_START+391,

	kVRegAncField1Offset					= VIRTUALREG_START+392,		///< @brief	How many bytes to subtract from the end of a frame buffer for field 1 ANC
	kVRegAncField2Offset					= VIRTUALREG_START+393,		///< @brief	How many bytes to subtract from the end of a frame buffer for field 2 ANC
	kVRegAgentCheck							= VIRTUALREG_START+394,
	kVRegUnused_2							= VIRTUALREG_START+395,
	
	kVReg4kOutputTransportSelection			= VIRTUALREG_START+396,
	kVRegCustomAncInputSelect				= VIRTUALREG_START+397,
	kVRegUseThermostat						= VIRTUALREG_START+398,
	kVRegThermalSamplingRate				= VIRTUALREG_START+399,
	kVRegFanSpeed							= VIRTUALREG_START+400,

	kVRegVideoFormatCh1						= VIRTUALREG_START+401,
	kVRegVideoFormatCh2						= VIRTUALREG_START+402,
	kVRegVideoFormatCh3						= VIRTUALREG_START+403,
	kVRegVideoFormatCh4						= VIRTUALREG_START+404,
	kVRegVideoFormatCh5						= VIRTUALREG_START+405,
	kVRegVideoFormatCh6						= VIRTUALREG_START+406,
	kVRegVideoFormatCh7						= VIRTUALREG_START+407,
	kVRegVideoFormatCh8						= VIRTUALREG_START+408,

	// Sarek VOIP section
	kVRegIPAddrEth0							= VIRTUALREG_START+409,
	kVRegSubnetEth0							= VIRTUALREG_START+410,
	kVRegGatewayEth0						= VIRTUALREG_START+411,

	kVRegIPAddrEth1							= VIRTUALREG_START+412,
	kVRegSubnetEth1							= VIRTUALREG_START+413,
	kVRegGatewayEth1						= VIRTUALREG_START+414,
	
	kVRegRxcEnable1							= VIRTUALREG_START+415,
    kVRegRxcSfp1RxMatch1					= VIRTUALREG_START+416,
    kVRegRxcSfp1SourceIp1                   = VIRTUALREG_START+417,
    kVRegRxcSfp1DestIp1                     = VIRTUALREG_START+418,
    kVRegRxcSfp1SourcePort1                 = VIRTUALREG_START+419,
    kVRegRxcSfp1DestPort1                   = VIRTUALREG_START+420,
    kVRegRxcSfp1Vlan1                       = VIRTUALREG_START+421,
    kVRegRxcSfp2RxMatch1                    = VIRTUALREG_START+422,
    kVRegRxcSfp2SourceIp1                   = VIRTUALREG_START+423,
    kVRegRxcSfp2DestIp1                     = VIRTUALREG_START+424,
    kVRegRxcSfp2SourcePort1                 = VIRTUALREG_START+425,
    kVRegRxcSfp2DestPort1                   = VIRTUALREG_START+426,
    kVRegRxcSfp2Vlan1                       = VIRTUALREG_START+427,
	kVRegRxcSsrc1							= VIRTUALREG_START+428,
	kVRegRxcPlayoutDelay1					= VIRTUALREG_START+429,

	kVRegRxcEnable2							= VIRTUALREG_START+430,
    kVRegRxcSfp1RxMatch2					= VIRTUALREG_START+431,
    kVRegRxcSfp1SourceIp2                   = VIRTUALREG_START+432,
    kVRegRxcSfp1DestIp2                     = VIRTUALREG_START+433,
    kVRegRxcSfp1SourcePort2                 = VIRTUALREG_START+434,
    kVRegRxcSfp1DestPort2                   = VIRTUALREG_START+435,
    kVRegRxcSfp1Vlan2                       = VIRTUALREG_START+436,
    kVRegRxcSfp2RxMatch2                    = VIRTUALREG_START+437,
    kVRegRxcSfp2SourceIp2                   = VIRTUALREG_START+438,
    kVRegRxcSfp2DestIp2                     = VIRTUALREG_START+439,
    kVRegRxcSfp2SourcePort2                 = VIRTUALREG_START+440,
    kVRegRxcSfp2DestPort2                   = VIRTUALREG_START+441,
    kVRegRxcSfp2Vlan2                       = VIRTUALREG_START+442,
	kVRegRxcSsrc2							= VIRTUALREG_START+443,
	kVRegRxcPlayoutDelay2					= VIRTUALREG_START+444,

	kVRegTxcEnable3							= VIRTUALREG_START+445,
    kVRegTxcSfp1LocalPort3                  = VIRTUALREG_START+446,
    kVRegTxcSfp1RemoteIp3                   = VIRTUALREG_START+447,
    kVRegTxcSfp1RemotePort3                 = VIRTUALREG_START+448,
    kVRegTxcSfp2LocalPort3                  = VIRTUALREG_START+449,
    kVRegTxcSfp2RemoteIp3                   = VIRTUALREG_START+450,
    kVRegTxcSfp2RemotePort3                 = VIRTUALREG_START+451,

	kVRegTxcEnable4							= VIRTUALREG_START+452,
    kVRegTxcSfp1LocalPort4                  = VIRTUALREG_START+453,
    kVRegTxcSfp1RemoteIp4                   = VIRTUALREG_START+454,
    kVRegTxcSfp1RemotePort4                 = VIRTUALREG_START+455,
    kVRegTxcSfp2LocalPort4                  = VIRTUALREG_START+456,
    kVRegTxcSfp2RemoteIp4                   = VIRTUALREG_START+457,
    kVRegTxcSfp2RemotePort4                 = VIRTUALREG_START+458,
	
	kVRegMailBoxAcquire						= VIRTUALREG_START+459,
	kVRegMailBoxRelease						= VIRTUALREG_START+460,
	kVRegMailBoxAbort						= VIRTUALREG_START+461,
	kVRegMailBoxTimeoutNS					= VIRTUALREG_START+462,		//	Units are 100 ns, not nanoseconds!

    kVRegRxc_2DecodeSelectionMode1          = VIRTUALREG_START+463,
    kVRegRxc_2DecodeProgramNumber1          = VIRTUALREG_START+464,
    kVRegRxc_2DecodeProgramPID1             = VIRTUALREG_START+465,
    kVRegRxc_2DecodeAudioNumber1            = VIRTUALREG_START+466,

    kVRegRxc_2DecodeSelectionMode2          = VIRTUALREG_START+467,
    kVRegRxc_2DecodeProgramNumber2          = VIRTUALREG_START+468,
    kVRegRxc_2DecodeProgramPID2             = VIRTUALREG_START+469,
    kVRegRxc_2DecodeAudioNumber2            = VIRTUALREG_START+470,

    kVRegTxc_2EncodeVideoFormat1            = VIRTUALREG_START+471,
    kVRegTxc_2EncodeUllMode1				= VIRTUALREG_START+472,
    kVRegTxc_2EncodeBitDepth1               = VIRTUALREG_START+473,
    kVRegTxc_2EncodeChromaSubSamp1          = VIRTUALREG_START+474,
    kVRegTxc_2EncodeMbps1                   = VIRTUALREG_START+475,
	kVRegTxc_2EncodeAudioChannels1			= VIRTUALREG_START+476,
	kVRegTxc_2EncodeStreamType1             = VIRTUALREG_START+477,
    kVRegTxc_2EncodeProgramPid1             = VIRTUALREG_START+478,
    kVRegTxc_2EncodeVideoPid1               = VIRTUALREG_START+479,
    kVRegTxc_2EncodePcrPid1					= VIRTUALREG_START+480,
    kVRegTxc_2EncodeAudio1Pid1              = VIRTUALREG_START+481,
    
    kVRegTxc_2EncodeVideoFormat2            = VIRTUALREG_START+482,
    kVRegTxc_2EncodeUllMode2				= VIRTUALREG_START+483,
    kVRegTxc_2EncodeBitDepth2               = VIRTUALREG_START+484,
    kVRegTxc_2EncodeChromaSubSamp2          = VIRTUALREG_START+485,
    kVRegTxc_2EncodeMbps2                   = VIRTUALREG_START+486,
	kVRegTxc_2EncodeAudioChannels2			= VIRTUALREG_START+487,
    kVRegTxc_2EncodeStreamType2             = VIRTUALREG_START+488,
    kVRegTxc_2EncodeProgramPid2             = VIRTUALREG_START+489,
    kVRegTxc_2EncodeVideoPid2               = VIRTUALREG_START+490,
    kVRegTxc_2EncodePcrPid2					= VIRTUALREG_START+491,
    kVRegTxc_2EncodeAudio1Pid2              = VIRTUALREG_START+492,

    kVReg2022_7Enable						= VIRTUALREG_START+493,
    kVReg2022_7NetworkPathDiff              = VIRTUALREG_START+494,
    
    kVRegKIPRxCfgError                      = VIRTUALREG_START+495,
    kVRegKIPTxCfgError                      = VIRTUALREG_START+496,
    kVRegKIPEncCfgError                     = VIRTUALREG_START+497,
    kVRegKIPDecCfgError                     = VIRTUALREG_START+498,
    kVRegKIPNetCfgError                     = VIRTUALREG_START+499,
    kVRegUseHDMI420Mode                     = VIRTUALREG_START+500,
    kVRegUnused501                          = VIRTUALREG_START+501,
    
    kVRegUserDefinedDBB						= VIRTUALREG_START+502,
    
    kVRegHDMIOutAudioChannels				= VIRTUALREG_START+503,
    kVRegHDMIOutRGBRange					= VIRTUALREG_START+504,
    kVRegZeroHostAncPostCapture				= VIRTUALREG_START+505,
    kVRegZeroDeviceAncPostCapture			= VIRTUALREG_START+506,
    kVRegAudioMonitorChannelSelect          = VIRTUALREG_START+507,
	kVRegAudioMixerOverrideState            = VIRTUALREG_START+508,
    kVRegAudioMixerSourceMainEnable         = VIRTUALREG_START+509,
    kVRegAudioMixerSourceAux1Enable         = VIRTUALREG_START+510,
    kVRegAudioMixerSourceAux2Enable         = VIRTUALREG_START+511,
    kVRegAudioMixerSourceMainGain           = VIRTUALREG_START+512,
    kVRegAudioMixerSourceAux1Gain           = VIRTUALREG_START+513,
    kVRegAudioMixerSourceAux2Gain           = VIRTUALREG_START+514,
    kVRegAudioCapMixerSourceMainEnable      = VIRTUALREG_START+515,
    kVRegAudioCapMixerSourceAux1Enable      = VIRTUALREG_START+516,
    kVRegAudioCapMixerSourceAux2Enable      = VIRTUALREG_START+517,
    kVRegAudioCapMixerSourceMainGain        = VIRTUALREG_START+518,
    kVRegAudioCapMixerSourceAux1Gain        = VIRTUALREG_START+519,
    kVRegAudioCapMixerSourceAux2Gain        = VIRTUALREG_START+520,
	
	kVRegSwizzle4kInput						= VIRTUALREG_START+521,
	kVRegSwizzle4kOutput					= VIRTUALREG_START+522,

	kVRegAnalogAudioIOConfiguration			= VIRTUALREG_START+523,
	kVRegHdmiHdrOutChanged					= VIRTUALREG_START+524,

	kVRegDisableAutoVPID					= VIRTUALREG_START+525,
	kVRegEnableBT2020						= VIRTUALREG_START+526,
	kVRegHdmiHdrOutMode						= VIRTUALREG_START+527,

    kVRegServicesForceInit                  = VIRTUALREG_START+528,		// set true when power state changes
    kVRegServicesModeFinal               	= VIRTUALREG_START+529,	

    kVRegLastAJA							= VIRTUALREG_START+530,		///< @brief	The last AJA virtual register slot
	kVRegFirstOEM							= kVRegLastAJA + 1,			///< @brief	The first virtual register slot available for general use
	kVRegLast								= VIRTUALREG_START + MAX_NUM_VIRTUAL_REGISTERS - 1	///< @brief	Last virtual register slot

} VirtualRegisterNum;


#if !defined(NTV2_DEPRECATE_15_0)
	#define	kVRegLinuxDriverVersion				VIRTUALREG_START		///< @deprecated	Obsolete in SDK 15.0, use kVRegDriverVersion instead
#endif

#if !defined (NTV2_DEPRECATE_12_7)
	//	The old virtual register names will be deprecated sometime after SDK 13.0.0
	#define	kRegLinuxDriverVersion				kVRegLinuxDriverVersion
	#define	kRegRelativeVideoPlaybackDelay		kVRegRelativeVideoPlaybackDelay
	#define	kRegAudioRecordPinDelay				kVRegAudioRecordPinDelay
	#define	kRegDriverVersion					kVRegDriverVersion
	#define	kRegGlobalAudioPlaybackMode			kVRegGlobalAudioPlaybackMode
	#define	kRegFlashProgramKey					kVRegFlashProgramKey
	#define	kRegStrictTiming					kVRegStrictTiming
	#define	kK2RegInputSelect					kVRegInputSelect
	#define	kK2RegSecondaryFormatSelect			kVRegSecondaryFormatSelect
	#define	kK2RegDigitalOutput1Select			kVRegDigitalOutput1Select
	#define	kK2RegDigitalOutput2Select			kVRegDigitalOutput2Select
	#define	kK2RegAnalogOutputSelect			kVRegAnalogOutputSelect
	#define	kK2RegAnalogOutputType				kVRegAnalogOutputType
	#define	kK2RegAnalogOutBlackLevel			kVRegAnalogOutBlackLevel
	#define	kVideoOutPauseMode					kVRegVideoOutPauseMode
	#define	kPulldownPattern					kVRegPulldownPattern
	#define	kColorSpaceMode						kVRegColorSpaceMode
	#define	kGammaMode							kVRegGammaMode
	#define	kLUTType							kVRegLUTType
	#define	kRGB10Range							kVRegRGB10Range
	#define	kRGB10Endian						kVRegRGB10Endian
	#define	kRegBitFileDownload					kVRegBitFileDownload
	#define	kRegSaveRegistersToRegistry			kVRegSaveRegistersToRegistry
	#define	kRegRecallRegistersFromRegistry		kVRegRecallRegistersFromRegistry
	#define	kRegClearAllSubscriptions			kVRegClearAllSubscriptions
	#define	kRegRestoreHardwareProcampRegisters	kVRegRestoreHardwareProcampRegisters
	#define	kRegAcquireReferenceCount			kVRegAcquireReferenceCount
	#define	kRegReleaseReferenceCount			kVRegReleaseReferenceCount
	#define	kRegDTAudioMux0						kVRegDTAudioMux0
	#define	kRegDTAudioMux1						kVRegDTAudioMux1
	#define	kRegDTAudioMux2						kVRegDTAudioMux2
	#define	kRegDTFirmware						kVRegDTFirmware
	#define	kRegDTVersionAja					kVRegDTVersionAja
	#define	kRegDTVersionDurian					kVRegDTVersionDurian
	#define	kRegDTAudioCapturePinConnected		kVRegDTAudioCapturePinConnected
	#define	kRegTimeStampMode					kVRegTimeStampMode
	#define	kRegTimeStampLastOutputVerticalLo	kVRegTimeStampLastOutputVerticalLo
	#define	kRegTimeStampLastOutputVerticalHi	kVRegTimeStampLastOutputVerticalHi
	#define	kRegTimeStampLastInput1VerticalLo	kVRegTimeStampLastInput1VerticalLo
	#define	kRegTimeStampLastInput1VerticalHi	kVRegTimeStampLastInput1VerticalHi
	#define	kRegTimeStampLastInput2VerticalLo	kVRegTimeStampLastInput2VerticalLo
	#define	kRegTimeStampLastInput2VerticalHi	kVRegTimeStampLastInput2VerticalHi
	#define	kRegNumberVideoMappingRegisters		kVRegNumberVideoMappingRegisters
	#define	kRegNumberAudioMappingRegisters		kVRegNumberAudioMappingRegisters
	#define	kRegAudioSyncTolerance				kVRegAudioSyncTolerance
	#define	kRegDmaSerialize					kVRegDmaSerialize
	#define	kRegSyncChannel						kVRegSyncChannel
	#define	kRegSyncChannels					kVRegSyncChannels
	#define	kRegSoftwareUartFifo				kVRegSoftwareUartFifo
	#define	kRegTimeCodeCh1Delay				kVRegTimeCodeCh1Delay
	#define	kRegTimeCodeCh2Delay				kVRegTimeCodeCh2Delay
	#define	kRegTimeCodeIn1Delay				kVRegTimeCodeIn1Delay
	#define	kRegTimeCodeIn2Delay				kVRegTimeCodeIn2Delay
	#define	kRegTimeCodeCh3Delay				kVRegTimeCodeCh3Delay
	#define	kRegTimeCodeCh4Delay				kVRegTimeCodeCh4Delay
	#define	kRegTimeCodeIn3Delay				kVRegTimeCodeIn3Delay
	#define	kRegTimeCodeIn4Delay				kVRegTimeCodeIn4Delay
	#define	kRegTimeCodeCh5Delay				kVRegTimeCodeCh5Delay
	#define	kRegTimeCodeIn5Delay				kVRegTimeCodeIn5Delay
	#define	kRegTimeCodeCh6Delay				kVRegTimeCodeCh6Delay
	#define	kRegTimeCodeIn6Delay				kVRegTimeCodeIn6Delay
	#define	kRegTimeCodeCh7Delay				kVRegTimeCodeCh7Delay
	#define	kRegTimeCodeIn7Delay				kVRegTimeCodeIn7Delay
	#define	kRegTimeCodeCh8Delay				kVRegTimeCodeCh8Delay
	#define	kRegTimeCodeIn8Delay				kVRegTimeCodeIn8Delay
	#define	kRegDebug1							kVRegDebug1
	#define	kDisplayReferenceSelect				kVRegDisplayReferenceSelect
	#define	kVANCMode							kVRegVANCMode
	#define	kRegDualStreamTransportType			kVRegDualStreamTransportType
	#define	kSDIOut1TransportType				kVRegSDIOut1TransportType
	#define	kDSKMode							kVRegDSKMode
	#define	kIsoConvertEnable					kVRegIsoConvertEnable
	#define	kDSKAudioMode						kVRegDSKAudioMode
	#define	kDSKForegroundMode					kVRegDSKForegroundMode
	#define	kDSKForegroundFade					kVRegDSKForegroundFade
	#define	kCaptureReferenceSelect				kVRegCaptureReferenceSelect
	#define	kPanMode							kVRegPanMode
	#define	kReg2XTransferMode					kVReg2XTransferMode
	#define	kRegSDIOutput1RGBRange				kVRegSDIOutput1RGBRange
	#define	kRegSDIInput1FormatSelect			kVRegSDIInput1FormatSelect
	#define	kRegSDIInput2FormatSelect			kVRegSDIInput2FormatSelect
	#define	kRegSDIInput1RGBRange				kVRegSDIInput1RGBRange
	#define	kRegSDIInput2RGBRange				kVRegSDIInput2RGBRange
	#define	kRegSDIInput1Stereo3DMode			kVRegSDIInput1Stereo3DMode
	#define	kRegSDIInput2Stereo3DMode			kVRegSDIInput2Stereo3DMode
	#define	kRegFrameBuffer1RGBRange			kVRegFrameBuffer1RGBRange
	#define	kRegFrameBuffer1Stereo3DMode		kVRegFrameBuffer1Stereo3DMode
	#define	kPanModeOffsetH						kVRegPanModeOffsetH
	#define	kPanModeOffsetV						kVRegPanModeOffsetV
	#define	kK2RegAnalogInBlackLevel			kVRegAnalogInBlackLevel
	#define	kK2RegAnalogInputType				kVRegAnalogInputType
	#define	kRegHDMIOutColorSpaceModeCtrl		kVRegHDMIOutColorSpaceModeCtrl
	#define	kHDMIOutProtocolMode				kVRegHDMIOutProtocolMode
	#define	kRegHDMIOutStereoSelect				kVRegHDMIOutStereoSelect
	#define	kRegHDMIOutStereoCodecSelect		kVRegHDMIOutStereoCodecSelect
	#define	kReversePulldownOffset				kVRegReversePulldownOffset
	#define	kRegSDIInput1ColorSpaceMode			kVRegSDIInput1ColorSpaceMode
	#define	kRegSDIInput2ColorSpaceMode			kVRegSDIInput2ColorSpaceMode
	#define	kRegSDIOutput2RGBRange				kVRegSDIOutput2RGBRange
	#define	kRegSDIOutput1Stereo3DMode			kVRegSDIOutput1Stereo3DMode
	#define	kRegSDIOutput2Stereo3DMode			kVRegSDIOutput2Stereo3DMode
	#define	kRegFrameBuffer2RGBRange			kVRegFrameBuffer2RGBRange
	#define	kRegFrameBuffer2Stereo3DMode		kVRegFrameBuffer2Stereo3DMode
	#define	kRegAudioGainDisable				kVRegAudioGainDisable
	#define	kDBLAudioEnable						kVRegDBLAudioEnable
	#define	kActiveVideoOutFilter				kVRegActiveVideoOutFilter
	#define	kRegAudioInputMapSelect				kVRegAudioInputMapSelect
	#define	kAudioInputDelay					kVRegAudioInputDelay
	#define	kDSKGraphicFileIndex				kVRegDSKGraphicFileIndex
	#define	kTimecodeBurnInMode					kVRegTimecodeBurnInMode
	#define	kUseQTTimecode						kVRegUseQTTimecode
	#define	kRegAvailable164					kVRegAvailable164
	#define	kRP188SourceSelect					kVRegRP188SourceSelect
	#define	kQTCodecModeDebug					kVRegQTCodecModeDebug
	#define	kRegHDMIOutColorSpaceModeStatus		kVRegHDMIOutColorSpaceModeStatus
	#define	kDeviceOnline						kVRegDeviceOnline
	#define	kIsDefaultDevice					kVRegIsDefaultDevice
	#define	kRegDesktopFrameBufferStatus		kVRegDesktopFrameBufferStatus
	#define	kRegSDIOutput1ColorSpaceMode		kVRegSDIOutput1ColorSpaceMode
	#define	kRegSDIOutput2ColorSpaceMode		kVRegSDIOutput2ColorSpaceMode
	#define	kAudioOutputDelay					kVRegAudioOutputDelay
	#define	kTimelapseEnable					kVRegTimelapseEnable
	#define	kTimelapseCaptureValue				kVRegTimelapseCaptureValue
	#define	kTimelapseCaptureUnits				kVRegTimelapseCaptureUnits
	#define	kTimelapseIntervalValue				kVRegTimelapseIntervalValue
	#define	kTimelapseIntervalUnits				kVRegTimelapseIntervalUnits
	#define	kFrameBufferInstalled				kVRegFrameBufferInstalled
	#define	kK2RegAnalogInStandard				kVRegAnalogInStandard
	#define	kRegOutputTimecodeOffset			kVRegOutputTimecodeOffset
	#define	kRegOutputTimecodeType				kVRegOutputTimecodeType
	#define	kRegQuicktimeUsingBoard				kVRegQuicktimeUsingBoard
	#define	kRegApplicationPID					kVRegApplicationPID
	#define	kRegApplicationCode					kVRegApplicationCode
	#define	kRegReleaseApplication				kVRegReleaseApplication
	#define	kRegForceApplicationPID				kVRegForceApplicationPID
	#define	kRegForceApplicationCode			kVRegForceApplicationCode
	#define	kRegProcAmpSDRegsInitialized		kVRegProcAmpSDRegsInitialized
	#define	kRegProcAmpStandardDefBrightness	kVRegProcAmpStandardDefBrightness
	#define	kRegProcAmpStandardDefContrast		kVRegProcAmpStandardDefContrast
	#define	kRegProcAmpStandardDefSaturation	kVRegProcAmpStandardDefSaturation
	#define	kRegProcAmpStandardDefHue			kVRegProcAmpStandardDefHue
	#define	kRegProcAmpStandardDefCbOffset		kVRegProcAmpStandardDefCbOffset
	#define	kRegProcAmpStandardDefCrOffset		kVRegProcAmpStandardDefCrOffset
	#define	kRegProcAmpEndStandardDefRange		kVRegProcAmpEndStandardDefRange
	#define	kRegProcAmpHDRegsInitialized		kVRegProcAmpHDRegsInitialized
	#define	kRegProcAmpHighDefBrightness		kVRegProcAmpHighDefBrightness
	#define	kRegProcAmpHighDefContrast			kVRegProcAmpHighDefContrast
	#define	kRegProcAmpHighDefSaturationCb		kVRegProcAmpHighDefSaturationCb
	#define	kRegProcAmpHighDefSaturationCr		kVRegProcAmpHighDefSaturationCr
	#define	kRegProcAmpHighDefHue				kVRegProcAmpHighDefHue
	#define	kRegProcAmpHighDefCbOffset			kVRegProcAmpHighDefCbOffset
	#define	kRegProcAmpHighDefCrOffset			kVRegProcAmpHighDefCrOffset
	#define	kRegProcAmpEndHighDefRange			kVRegProcAmpEndHighDefRange
	#define	kRegChannel1UserBufferLevel			kVRegChannel1UserBufferLevel
	#define	kRegChannel2UserBufferLevel			kVRegChannel2UserBufferLevel
	#define	kRegInput1UserBufferLevel			kVRegInput1UserBufferLevel
	#define	kRegInput2UserBufferLevel			kVRegInput2UserBufferLevel
	#define	kRegProgressivePicture				kVRegProgressivePicture
	#define	kRegLUT2Type						kVRegLUT2Type
	#define	kRegLUT3Type						kVRegLUT3Type
	#define	kRegLUT4Type						kVRegLUT4Type
	#define	kK2RegDigitalOutput3Select			kVRegDigitalOutput3Select
	#define	kK2RegDigitalOutput4Select			kVRegDigitalOutput4Select
	#define	kK2RegHDMIOutputSelect				kVRegHDMIOutputSelect
	#define	kK2RegRGBRangeConverterLUTType		kVRegRGBRangeConverterLUTType
	#define	kRegTestPatternChoice				kVRegTestPatternChoice
	#define	kRegTestPatternFormat				kVRegTestPatternFormat
	#define	kRegEveryFrameTaskFilter			kVRegEveryFrameTaskFilter
	#define	kRegDefaultInput					kVRegDefaultInput
	#define	kRegDefaultVideoOutMode				kVRegDefaultVideoOutMode
	#define	kRegDefaultVideoFormat				kVRegDefaultVideoFormat
	#define	kK2RegDigitalOutput5Select			kVRegDigitalOutput5Select
	#define	kRegLUT5Type						kVRegLUT5Type
	#define	kRegMacUserModeDebugLevel			kVRegMacUserModeDebugLevel
	#define	kRegMacKernelModeDebugLevel			kVRegMacKernelModeDebugLevel
	#define	kRegMacUserModePingLevel			kVRegMacUserModePingLevel
	#define	kRegMacKernelModePingLevel			kVRegMacKernelModePingLevel
	#define	kRegLatencyTimerValue				kVRegLatencyTimerValue
	#define	kRegAudioAVSyncEnable				kVRegAudioAVSyncEnable
	#define	kRegAudioInputSelect				kVRegAudioInputSelect
	#define	kSerialSuspended					kVRegSerialSuspended
	#define	kXilinxProgramming					kVRegXilinxProgramming
	#define	kETTDiagLastSerialTimestamp			kVRegETTDiagLastSerialTimestamp
	#define	kETTDiagLastSerialTimecode			kVRegETTDiagLastSerialTimecode
	#define	kStartupStatusFlags					kVRegStartupStatusFlags
	#define	kRegRGBRangeMode					kVRegRGBRangeMode
	#define	kRegEnableQueuedDMAs				kVRegEnableQueuedDMAs
	#define	kRegBA0MemorySize					kVRegBA0MemorySize
	#define	kRegBA1MemorySize					kVRegBA1MemorySize
	#define	kRegBA4MemorySize					kVRegBA4MemorySize
	#define	kRegNumDmaDriverBuffers				kVRegNumDmaDriverBuffers
	#define	kRegDMADriverBufferPhysicalAddress	kVRegDMADriverBufferPhysicalAddress
	#define	kRegBA2MemorySize					kVRegBA2MemorySize
	#define	kRegAcquireLinuxReferenceCount		kVRegAcquireLinuxReferenceCount
	#define	kRegReleaseLinuxReferenceCount		kVRegReleaseLinuxReferenceCount
	#define	kRegAdvancedIndexing				kVRegAdvancedIndexing
	#define	kRegTimeStampLastInput3VerticalLo	kVRegTimeStampLastInput3VerticalLo
	#define	kRegTimeStampLastInput3VerticalHi	kVRegTimeStampLastInput3VerticalHi
	#define	kRegTimeStampLastInput4VerticalLo	kVRegTimeStampLastInput4VerticalLo
	#define	kRegTimeStampLastInput4VerticalHi	kVRegTimeStampLastInput4VerticalHi
	#define	kRegTimeStampLastInput5VerticalLo	kVRegTimeStampLastInput5VerticalLo
	#define	kRegTimeStampLastInput5VerticalHi	kVRegTimeStampLastInput5VerticalHi
	#define	kRegTimeStampLastInput6VerticalLo	kVRegTimeStampLastInput6VerticalLo
	#define	kRegTimeStampLastInput6VerticalHi	kVRegTimeStampLastInput6VerticalHi
	#define	kRegTimeStampLastInput7VerticalLo	kVRegTimeStampLastInput7VerticalLo
	#define	kRegTimeStampLastInput7VerticalHi	kVRegTimeStampLastInput7VerticalHi
	#define	kRegTimeStampLastInput8VerticalLo	kVRegTimeStampLastInput8VerticalLo
	#define	kRegTimeStampLastInput8VerticalHi	kVRegTimeStampLastInput8VerticalHi
	#define	kRegTimeStampLastOutput2VerticalLo	kVRegTimeStampLastOutput2VerticalLo
	#define	kRegTimeStampLastOutput2VerticalHi	kVRegTimeStampLastOutput2VerticalHi
	#define	kRegTimeStampLastOutput3VerticalLo	kVRegTimeStampLastOutput3VerticalLo
	#define	kRegTimeStampLastOutput3VerticalHi	kVRegTimeStampLastOutput3VerticalHi
	#define	kRegTimeStampLastOutput4VerticalLo	kVRegTimeStampLastOutput4VerticalLo
	#define	kRegTimeStampLastOutput4VerticalHi	kVRegTimeStampLastOutput4VerticalHi
	#define	kRegTimeStampLastOutput5VerticalLo	kVRegTimeStampLastOutput5VerticalLo
	#define	kRegTimeStampLastOutput5VerticalHi	kVRegTimeStampLastOutput5VerticalHi
	#define	kRegTimeStampLastOutput6VerticalLo	kVRegTimeStampLastOutput6VerticalLo
	#define	kRegTimeStampLastOutput6VerticalHi	kVRegTimeStampLastOutput6VerticalHi
	#define	kRegTimeStampLastOutput7VerticalLo	kVRegTimeStampLastOutput7VerticalLo
	#define	kRegTimeStampLastOutput7VerticalHi	kVRegTimeStampLastOutput7VerticalHi
	#define	kRegTimeStampLastOutput8VerticalLo	kVRegTimeStampLastOutput8VerticalLo
	#define	kRegResetCycleCount					kVRegResetCycleCount
	#define	kRegUseProgressive					kVRegUseProgressive
	#define	kRegFlashSize						kVRegFlashSize
	#define	kRegFlashStatus						kVRegFlashStatus
	#define	kRegFlashState						kVRegFlashState
	#define	kRegPCIDeviceID						kVRegPCIDeviceID
	#define	kRegUartRxFifoSize					kVRegUartRxFifoSize
	#define	kRegEFTNeedsUpdating				kVRegEFTNeedsUpdating
	#define	kRegSuspendSystemAudio				kVRegSuspendSystemAudio
	#define	kRegAcquireReferenceCounter			kVRegAcquireReferenceCounter
	#define	kRegTimeStampLastOutput8VerticalHi	kVRegTimeStampLastOutput8VerticalHi
	#define	kRegFramesPerVertical				kVRegFramesPerVertical
	#define	kRegServicesInitialized				kVRegServicesInitialized
	#define	kRegFrameBufferGangCount			kVRegFrameBufferGangCount
	#define	kRegChannelCrosspointFirst			kVRegChannelCrosspointFirst
	#define	kRegChannelCrosspointLast			kVRegChannelCrosspointLast
	#define	kRegDriverVersionMajor				kVRegDriverVersionMajor
	#define	kRegDriverVersionMinor				kVRegDriverVersionMinor
	#define	kRegDriverVersionPoint				kVRegDriverVersionPoint
	#define	kRegFollowInputFormat				kVRegFollowInputFormat
	#define	kRegAncField1Offset					kVRegAncField1Offset
	#define	kRegAncField2Offset					kVRegAncField2Offset
	#define	kRegUnused_1						kVRegUnused_1
	#define	kRegUnused_2						kVRegUnused_2
	#define	kReg4kOutputTransportSelection		kVReg4kOutputTransportSelection
	#define	kRegCustomAncInputSelect			kVRegCustomAncInputSelect
	#define	kRegUseThermostat					kVRegUseThermostat
	#define	kRegThermalSamplingRate				kVRegThermalSamplingRate
	#define	kRegFanSpeed						kVRegFanSpeed
	#define	kRegVideoFormatCh1					kVRegVideoFormatCh1
	#define	kRegVideoFormatCh2					kVRegVideoFormatCh2
	#define	kRegVideoFormatCh3					kVRegVideoFormatCh3
	#define	kRegVideoFormatCh4					kVRegVideoFormatCh4
	#define	kRegVideoFormatCh5					kVRegVideoFormatCh5
	#define	kRegVideoFormatCh6					kVRegVideoFormatCh6
	#define	kRegVideoFormatCh7					kVRegVideoFormatCh7
	#define	kRegVideoFormatCh8					kVRegVideoFormatCh8
	#define	kRegUserDefinedDBB					kVRegUserDefinedDBB
	#define	kRegHDMIOutAudioChannels			kVRegHDMIOutAudioChannels
	#define	kRegHDMIOutRGBRange					kVRegHDMIOutRGBRange
	#define	kRegLastAJA							kVRegLastAJA
	#define	kRegFirstOEM						kVRegFirstOEM
	#define	kRegLast							kVRegLast
#endif	//	NTV2_DEPRECATE_12_7

#endif// NTV2VIRTUALREGISTERS_H
