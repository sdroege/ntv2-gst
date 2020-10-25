/**
	@file		ntv2debug.cpp
	@brief		Implements the NTV2 debug output functions.
	@copyright	(C) 2004-2020 AJA Video Systems, Inc.	Proprietary and confidential information.
	@note		Because this module is compiled into the driver, it must remain straight ANSI 'C' -- no C++ or STL.
**/

#include "ntv2publicinterface.h"
#include "ntv2debug.h"

// Macro to simplify returning of strings for given enum
#define NTV2DEBUG_ENUM_CASE_RETURN_STR(enum_name) case(enum_name): return #enum_name;

#if !defined(NTV2_DEPRECATE_14_3)
const char * NTV2DeviceTypeString (NTV2DeviceType type)
{
	const char *result = "";

	switch (type)
	{
		case DEVICETYPE_NTV2:		result = "DEVICETYPE_NTV2";		break;
#if !defined (_DEBUG)
		default:
#endif
		case DEVICETYPE_UNKNOWN:	result = "DEVICETYPE_UNKNOWN";	break;
	}

	return (result);
}
#endif	//	!defined(NTV2_DEPRECATE_14_3)

const char * NTV2DeviceIDString (const NTV2DeviceID id)
{
	switch (id)
	{
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONALHI);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONALHIDVI);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_IOEXPRESS);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_CORVID1);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_CORVID22);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_CORVID3G);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONA3G);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONA3GQUAD);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONALHEPLUS);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_IOXT);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_CORVID24);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_TTAP);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_IO4K);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_IO4KUFC);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONA4);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONA4UFC);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_CORVID88);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_CORVID44);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_CORVIDHEVC);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_CORVIDHBR);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONAIP_2022);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONAIP_4CH_2SFP);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONAIP_1RX_1TX_1SFP_J2K);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONAIP_2TX_1SFP_J2K);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONAIP_1RX_1TX_2110);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONAIP_2110);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_IO4KPLUS);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_IOIP_2022);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_IOIP_2110);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONA1);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONAHDMI);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONA5);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONA5_8KMK);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONA5_8K);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONA5_2X4K);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_KONA5_3DLUT);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_CORVID44_8KMK);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_CORVID44_8K);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_CORVID44_2X4K);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_CORVID44_PLNR);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_TTAP_PRO);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(DEVICE_ID_NOTFOUND);
	}
	return "";
}


const char * NTV2DeviceString (const NTV2DeviceID id)
{
	switch (id)
	{
		case DEVICE_ID_KONALHI:						return "KonaLHi";
		case DEVICE_ID_KONALHIDVI:					return "KonaLHiDVI";
		case DEVICE_ID_IOEXPRESS:					return "IoExpress";
		case DEVICE_ID_CORVID1:						return "Corvid1";
		case DEVICE_ID_CORVID22:					return "Corvid22";
		case DEVICE_ID_CORVID3G:					return "Corvid3G";
		case DEVICE_ID_KONA3G:						return "Kona3G";
		case DEVICE_ID_KONA3GQUAD:					return "Kona3GQuad";
		case DEVICE_ID_KONALHEPLUS:					return "KonaLHePlus";
		case DEVICE_ID_IOXT:						return "IoXT";
		case DEVICE_ID_CORVID24:					return "Corvid24";
		case DEVICE_ID_TTAP:						return "TTap";
		case DEVICE_ID_IO4K:						return "Io4K";
		case DEVICE_ID_IO4KUFC:						return "Io4KUfc";
		case DEVICE_ID_KONA4:						return "Kona4";
		case DEVICE_ID_KONA4UFC:					return "Kona4Ufc";
		case DEVICE_ID_CORVID88:					return "Corvid88";
		case DEVICE_ID_CORVID44:					return "Corvid44";
		case DEVICE_ID_CORVIDHEVC:					return "CorvidHEVC";
		case DEVICE_ID_CORVIDHBR:					return "CorvidHBR";
		case DEVICE_ID_KONAIP_2022:					return "KonaIP_2022";
		case DEVICE_ID_KONAIP_4CH_2SFP:				return "KonaIP_4ch2SFP";
		case DEVICE_ID_KONAIP_1RX_1TX_1SFP_J2K:		return "KonaIP_1Rx1Tx1SFPJ2K";
		case DEVICE_ID_KONAIP_2TX_1SFP_J2K:			return "KonaIP_2Tx1SFPJ2K";
		case DEVICE_ID_KONAIP_1RX_1TX_2110:			return "KonaIP_1Rx1Tx2110";
		case DEVICE_ID_KONAIP_2110:					return "KonaIP_2110";
		case DEVICE_ID_IO4KPLUS:					return "DNxIV";
		case DEVICE_ID_IOIP_2022:					return "DNxIP_2022";
		case DEVICE_ID_IOIP_2110:					return "DNxIP_2110";
		case DEVICE_ID_KONA1:						return "Kona1";
		case DEVICE_ID_KONAHDMI:					return "KonaHDMI";
		case DEVICE_ID_KONA5:						return "Kona5";
		case DEVICE_ID_KONA5_8KMK:					return "Kona5_8KMK";
		case DEVICE_ID_KONA5_8K:					return "Kona5_8K";
		case DEVICE_ID_KONA5_2X4K:					return "Kona5_2X4K";
		case DEVICE_ID_KONA5_3DLUT:					return "Kona5_3DLUT";
		case DEVICE_ID_CORVID44_8KMK:				return "Corvid44_8KMK";
		case DEVICE_ID_CORVID44_8K:					return "Corvid44_8K";
		case DEVICE_ID_CORVID44_2X4K:				return "Corvid44_2X4K";
		case DEVICE_ID_CORVID44_PLNR:				return "Corvid44_PLNR";
		case DEVICE_ID_TTAP_PRO:					return "TTapPro";
		case DEVICE_ID_NOTFOUND:					return "Unknown";
	}

	return "";
}


const char *NTV2StandardString (NTV2Standard std)
{
	switch (std)
	{
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_1080);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_720);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_525);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_625);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_1080p);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_2K);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_2Kx1080p);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_2Kx1080i);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_3840x2160p);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_4096x2160p);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_3840HFR);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_4096HFR);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_7680);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_8192);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_3840i);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_4096i);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_STANDARD_INVALID);
#if !defined (_DEBUG)
		default:						return "";
#endif
	}
	return "";
}


const char *NTV2FrameBufferFormatString (NTV2FrameBufferFormat fmt)
{
	switch (fmt)
	{
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_10BIT_YCBCR);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_8BIT_YCBCR);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_ARGB);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_RGBA);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_10BIT_RGB);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_8BIT_YCBCR_YUY2);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_ABGR);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_10BIT_DPX);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_10BIT_YCBCR_DPX);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_8BIT_DVCPRO);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_8BIT_YCBCR_420PL3);	//	was NTV2_FBF_8BIT_QREZ
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_8BIT_HDV);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_24BIT_RGB);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_24BIT_BGR);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_10BIT_YCBCRA);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_10BIT_DPX_LE);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_48BIT_RGB);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_12BIT_RGB_PACKED);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_PRORES_DVCPRO);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_PRORES_HDV);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_10BIT_RGB_PACKED);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_10BIT_ARGB);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_16BIT_ARGB);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_8BIT_YCBCR_422PL3);	//	was NTV2_FBF_UNUSED_23
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_10BIT_RAW_RGB);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_10BIT_RAW_YCBCR);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_10BIT_YCBCR_420PL3_LE);	//	was NTV2_FBF_UNUSED_26
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_10BIT_YCBCR_422PL3_LE);	//	was NTV2_FBF_UNUSED_27
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_10BIT_YCBCR_420PL2);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_10BIT_YCBCR_422PL2);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_8BIT_YCBCR_420PL2);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_8BIT_YCBCR_422PL2);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FBF_INVALID);
#if !defined (_DEBUG)
		default:						return "";
#endif
	}
	return "";
}


const char *NTV2FrameGeometryString (NTV2FrameGeometry geom)
{
	switch (geom)
	{
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_1920x1080);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_1280x720);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_720x486);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_720x576);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_720x508);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_720x598);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_1920x1112);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_1920x1114);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_1280x740);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_2048x1080);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_2048x1556);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_2048x1588);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_2048x1112);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_2048x1114);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_720x514);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_720x612);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_4x1920x1080);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_4x2048x1080);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_4x3840x2160);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_4x4096x2160);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FG_INVALID);
#if !defined (_DEBUG)
		default:						return "";
#endif
	}
	return "";
}


const char *NTV2FrameRateString (NTV2FrameRate rate)
{
	switch (rate)
	{
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_12000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_11988);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_6000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_5994);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_5000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_4800);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_4795);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_3000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_2997);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_1500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_1498);
#if !defined(NTV2_DEPRECATE_16_0)
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_1900);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_1898);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_1800);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_1798);
#endif	//!defined(NTV2_DEPRECATE_16_0)
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FRAMERATE_INVALID);
		case NTV2_NUM_FRAMERATES:		return "";  //special case

#if !defined (_DEBUG)
		default:						return "";
#endif
	}
	return "";
}


const char *NTV2VideoFormatString (NTV2VideoFormat fmt)
{
	switch (fmt)
	{
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080i_5000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080i_5994);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080i_6000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_720p_5994);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_720p_6000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080psf_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080psf_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2997);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_3000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2K_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2K_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2K_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2K_2997);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2K_3000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2K_4795_A);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2K_4800_A);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2K_5000_A);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2K_5994_A);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2K_6000_A);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2K_4795_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2K_4800_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2K_5000_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2K_5994_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_2K_6000_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080psf_2K_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080psf_2K_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080psf_2K_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080psf_2500_2);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080psf_2997_2);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080psf_3000_2);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_720p_5000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_5000_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_5994_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_6000_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_5000_A);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_5994_A);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_1080p_6000_A);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_720p_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_720p_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_525_5994);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_625_5000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_525_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_525_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_525psf_2997);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_625psf_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_2K_1498);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_2K_1500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_2K_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_2K_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_2K_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080psf_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080psf_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080psf_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080psf_2997);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080psf_3000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080p_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080p_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080p_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080p_2997);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080p_3000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080psf_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080psf_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080psf_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080psf_2997);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080psf_3000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_2997);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_3000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_4795);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_4800);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080p_5000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080p_5994);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080p_6000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_5000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_5994);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_6000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_11988);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_12000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160psf_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160psf_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160psf_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160p_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160p_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160p_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160p_2997);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160p_3000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160psf_2997);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160psf_3000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160p_5000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160p_5994);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160p_6000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160psf_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160psf_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160psf_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_2997);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_3000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160psf_2997);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160psf_3000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_4795);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_4800);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_5000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_5994);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_6000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_11988);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_12000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_4795_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_4800_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080p_5000_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080p_5994_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x1920x1080p_6000_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_5000_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_5994_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x2048x1080p_6000_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160p_5000_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160p_5994_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_3840x2160p_6000_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_4795_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_4800_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_5000_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_5994_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4096x2160p_6000_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x3840x2160p_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x3840x2160p_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x3840x2160p_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x3840x2160p_2997);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x3840x2160p_3000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x3840x2160p_5000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x3840x2160p_5994);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x3840x2160p_6000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x3840x2160p_5000_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x3840x2160p_5994_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x3840x2160p_6000_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x4096x2160p_2398);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x4096x2160p_2400);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x4096x2160p_2500);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x4096x2160p_2997);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x4096x2160p_3000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x4096x2160p_4795);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x4096x2160p_4800);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x4096x2160p_5000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x4096x2160p_5994);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x4096x2160p_6000);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x4096x2160p_4795_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x4096x2160p_4800_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x4096x2160p_5000_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x4096x2160p_5994_B);
		NTV2DEBUG_ENUM_CASE_RETURN_STR(NTV2_FORMAT_4x4096x2160p_6000_B);
		//special cases
		case NTV2_FORMAT_END_HIGH_DEF_FORMATS:		return "";
		case NTV2_FORMAT_END_STANDARD_DEF_FORMATS:	return "";
		case NTV2_FORMAT_END_2K_DEF_FORMATS:		return "";
		case NTV2_FORMAT_END_HIGH_DEF_FORMATS2:		return "";
		case NTV2_FORMAT_END_4K_TSI_DEF_FORMATS:	return "";
		case NTV2_FORMAT_END_4K_DEF_FORMATS2:		return "";
		case NTV2_FORMAT_END_UHD2_DEF_FORMATS:		return "";
		case NTV2_FORMAT_END_UHD2_FULL_DEF_FORMATS:	return "";
		case NTV2_FORMAT_UNKNOWN:					return "";
#if !defined (_DEBUG)
		default:									return "";
#endif
	}
	return "";

}	//	NTV2VideoFormatString


// indexed by RegisterNum - 2048
const char * ntv2RegStrings_SDI_RX_Status[] =
{
	"SDI 1 RX Status",								//   0	2048
	"SDI 1 RX CRC Error Count",						//	 1	2049
	"SDI 1 RX Frame Count Low",						//	 2	2050
	"SDI 1 RX Frame Count High",					//	 3	2051
	"SDI 1 RX Frame Ref Count Low",					//	 4	2052
	"SDI 1 RX Frame Ref Count High",				//	 5	2053
	"Unused SDI Register 2054",						//	 6	2054
	"Unused SDI Register 2055",						//	 7	2055
	"SDI 2 RX Status",								//   8	2056
	"SDI 2 RX CRC Error Count",						//	 9	2057
	"SDI 2 RX Frame Count Low",						//	10	2058
	"SDI 2 RX Frame Count High",					//	11	2059
	"SDI 2 RX Frame Ref Count Low",					//	12	2060
	"SDI 2 RX Frame Ref Count High",				//	13	2061
	"Unused SDI Register 2062",						//	14	2062
	"Unused SDI Register 2063",						//	15	2063
	"SDI 3 RX Status",								//  16	2064
	"SDI 3 RX CRC Error Count",						//	17	2065
	"SDI 3 RX Frame Count Low",						//	18	2066
	"SDI 3 RX Frame Count High",					//	19	2067
	"SDI 3 RX Frame Ref Count Low",					//	20	2068
	"SDI 3 RX Frame Ref Count High",				//	21	2069
	"Unused SDI Register 2070",						//	22	2070
	"Unused SDI Register 2071",						//	23	2071
	"SDI 4 RX Status",								//  24	2072
	"SDI 4 RX CRC Error Count",						//	25	2073
	"SDI 4 RX Frame Count Low",						//	26	2074
	"SDI 4 RX Frame Count High",					//	27	2075
	"SDI 4 RX Frame Ref Count Low",					//	28	2076
	"SDI 4 RX Frame Ref Count High",				//	29	2077
	"Unused SDI Register 2078",						//	30	2078
	"Unused SDI Register 2079",						//	31	2079
	"SDI 5 RX Status",								//  32	2080
	"SDI 5 RX CRC Error Count",						//	33	2081
	"SDI 5 RX Frame Count Low",						//	34	2082
	"SDI 5 RX Frame Count High",					//	35	2083
	"SDI 5 RX Frame Ref Count Low",					//	36	2084
	"SDI 5 RX Frame Ref Count High",				//	37	2085
	"Unused SDI Register 2086",						//	38	2086
	"Unused SDI Register 2087",						//	39	2087
	"SDI 6 RX Status",								//  40	2088
	"SDI 6 RX CRC Error Count",						//	41	2089
	"SDI 6 RX Frame Count Low",						//	42	2090
	"SDI 6 RX Frame Count High",					//	43	2091
	"SDI 6 RX Frame Ref Count Low",					//	44	2092
	"SDI 6 RX Frame Ref Count High",				//	45	2093
	"Unused SDI Register 2094",						//	46	2094
	"Unused SDI Register 2095",						//	47	2095
	"SDI 7 RX Status",								//  48	2096
	"SDI 7 RX CRC Error Count",						//	49	2097
	"SDI 7 RX Frame Count Low",						//	50	2098
	"SDI 7 RX Frame Count High",					//	51	2099
	"SDI 7 RX Frame Ref Count Low",					//	52	2100
	"SDI 7 RX Frame Ref Count High",				//	53	2101
	"Unused SDI Register 2102",						//	54	2102
	"Unused SDI Register 2103",						//	55	2103
	"SDI 8 RX Status",								//  56	2104
	"SDI 8 RX CRC Error Count",						//	57	2105
	"SDI 8 RX Frame Count Low",						//	58	2106
	"SDI 8 RX Frame Count High",					//	59	2107
	"SDI 8 RX Frame Ref Count Low",					//	60	2108
	"SDI 8 RX Frame Ref Count High",				//	61	2109
	"Unused SDI Register 2110",						//	62	2110
	"Unused SDI Register 2111",						//	63	2111
	"Free Running Clock Low",						//	64	2112
	"Free Running Clock High"						//	65	2113
};	//	ntv2RegStreings_SDI


// indexed by RegisterNum
const char * ntv2RegStrings[] =
{
	"kRegGlobalControl",							//   0
	"kRegCh1Control",								//   1
	"kRegCh1PCIAccessFrame",						//   2
	"kRegCh1OutputFrame",							//   3
	"kRegCh1InputFrame",							//   4
	"kRegCh2Control",								//   5
	"kRegCh2PCIAccessFrame",						//   6
	"kRegCh2OutputFrame",							//   7
	"kRegCh2InputFrame",							//   8
	"kRegVidProc1Control",							//   9
	"kRegVidProcXptControl",						//  10
	"kRegMixer1Coefficient",						//  11
	"kRegSplitControl",								//  12
	"kRegFlatMatteValue",							//  13
	"kRegOutputTimingControl",						//  14
	"kRegReserved15",								//  15
	"kRegReserved16",								//  16
	"kRegFlashProgramReg",							//  17
	"kRegLineCount",								//  18
	"kRegAud1Delay",								//  19
	"kRegVidIntControl",							//  20
	"kRegStatus",									//  21
	"kRegInputStatus",								//  22
	"kRegAud1Detect",								//  23
	"kRegAud1Control",								//  24
	"kRegAud1SourceSelect",							//  25
	"kRegAud1OutputLastAddr",						//  26
	"kRegAud1InputLastAddr",						//  27
	"kRegAud1Counter",								//  28
	"kRegRP188InOut1DBB",							//  29
	"kRegRP188InOut1Bits0_31",						//  30
	"kRegRP188InOut1Bits32_63",						//  31
	"kRegDMA1HostAddr",								//  32
	"kRegDMA1LocalAddr",							//  33
	"kRegDMA1XferCount",							//  34
	"kRegDMA1NextDesc",								//  35
	"kRegDMA2HostAddr",								//  36
	"kRegDMA2LocalAddr",							//  37
	"kRegDMA2XferCount",							//  38
	"kRegDMA2NextDesc",								//  39
	"kRegDMA3HostAddr",								//  40
	"kRegDMA3LocalAddr",							//  41
	"kRegDMA3XferCount",							//  42
	"kRegDMA3NextDesc",								//  43
	"kRegDMA4HostAddr",								//  44
	"kRegDMA4LocalAddr",							//  45
	"kRegDMA4XferCount",							//  46
	"kRegDMA4NextDesc",								//  47
	"kRegDMAControl",								//  48
	"kRegDMAIntControl",							//  49
	"kRegBoardID",									//  50
	"kRegReserved51",								//  51
	"kRegReserved52",								//  52
	"kRegReserved53",								//  53
	"kRegReserved54",								//  54
	"kRegReserved55",								//  55
	"kRegReserved56",								//  56
	"kRegReserved57",								//  57
	"kRegXenaxFlashControlStatus",					//  58
	"kRegXenaxFlashAddress",						//  59
	"kRegXenaxFlashDIN",							//  60
	"kRegXenaxFlashDOUT",							//  61
	"kRegReserved62",								//  62
	"kRegCPLDVersion",								//  63
	"kRegRP188InOut2DBB",							//  64
	"kRegRP188InOut2Bits0_31",						//  65
	"kRegRP188InOut2Bits32_63",						//  66
	"kRegCanDoStatus",								//  67
	"kRegCh1ColorCorrectionControl",				//  68
	"kRegCh2ColorCorrectionControl",				//  69
	"kRegRS422Transmit",							//  70
	"kRegRS422Receive",								//  71
	"kRegRS422Control",								//  72
	"kRegReserved73",								//  73
	"kRegReserved74",								//  74
	"kRegReserved75",								//  75
	"kRegReserved76",								//  76
	"kRegReserved77",								//  77
	"kRegReserved78",								//  78
	"kRegReserved79",								//  79
	"kRegReserved80",								//  80

	"kRegAnalogInputStatus",						//  81
	"kRegAnalogInputControl",						//	82
	"kRegReserved83",								//  83
	"kRegFS1ProcAmpC1Y_C1CB",						//  84
	"kRegFS1ProcAmpC1CR_C2CB",						//  85
	"kRegFS1ProcAmpC2CROffsetY",					//  86
	"kRegAud2Delay",								//  87
	"kRegBitfileDate",								//  88
	"kRegBitfileTime",								//  89

	"kRegFS1I2CControl",							//  90
	"kRegFS1I2C1Address",							//  91
	"kRegFS1I2C1Data",								//  92
	"kRegFS1I2C2Address",							//  93
	"kRegFS1I2C2Data",								//  94
	"kRegFS1ReferenceSelect",						//  95
	"kRegAverageAudioLevelChan1_2",					//  96
	"kRegAverageAudioLevelChan3_4",					//  97
	"kRegAverageAudioLevelChan5_6",					//  98
	"kRegAverageAudioLevelChan7_8",					//  99

	"kRegDMA1HostAddrHigh",							// 100
	"kRegDMA1NextDescHigh",							// 101
	"kRegDMA2HostAddrHigh",							// 102
	"kRegDMA2NextDescHigh",							// 103
	"kRegDMA3HostAddrHigh",							// 104
	"kRegDMA3NextDescHigh",							// 105
	"kRegDMA4HostAddrHigh",							// 106
	"kRegDMA4NextDescHigh",							// 107

	"kRegGlobalControl3",							// 108
	"kRegReserved109",								// 109
	"kRegLTCEmbeddedBits0_31",						// 110
	"kRegLTCEmbeddedBits32_63",						// 111
	"kRegLTCAnalogBits0_31",						// 112
	"kRegLTCAnalogBits32_63",						// 113

	"kRegReserved114",								// 114
	"kRegReserved115",								// 115
	"kRegSysmonControl",							// 116
	"kRegSysmonConfig1_0",							// 117
	"kRegSysmonConfig2",							// 118
	"kRegSysmonVccIntDieTemp",						// 119

	"kRegInternalExternalVoltage",					// 120
	"kRegFlashProgramReg2",							// 121
	"kRegHDMIOut3DStatus1",							// 122
	"kRegHDMIOut3DStatus2",							// 123
	"kRegHDMIOut3DControl",							// 124
	"kRegHDMIOutControl",							// 125
	"kRegHDMIInputStatus",							// 126
	"kRegHDMIInputControl",							// 127
	"kRegAnalogOutControl",							// 128
	"kRegSDIOut1Control",							// 129

	"kRegSDIOut2Control",							// 130
	"kRegConversionControl",						// 131
	"kRegFrameSync1Control",						// 132
	"kRegI2CWriteData",								// 133
	"kRegFrameSync2Control",						// 134
	"kRegI2CWriteControl",							// 135
	"kRegXptSelectGroup1",							// 136
	"kRegXptSelectGroup2",							// 137
	"kRegXptSelectGroup3",							// 138
	"kRegXptSelectGroup4",							// 139
	"kRegXptSelectGroup5",							// 140
	"kRegXptSelectGroup6",							// 141

	"kRegCSCoefficients1_2",						// 142
	"kRegCSCoefficients3_4",						// 143
	"kRegCSCoefficients5_6",						// 144
	"kRegCSCoefficients7_8",						// 145
	"kRegCSCoefficients9_10",						// 146

	"kRegCS2Coefficients1_2",						// 147
	"kRegCS2Coefficients3_4",						// 148
	"kRegCS2Coefficients5_6",						// 149
	"kRegCS2Coefficients7_8",						// 150
	"kRegCS2Coefficients9_10",						// 151

	"kRegField1Line21CaptionDecode",				// 152		//	OBSOLETE
	"kRegField2Line21CaptionDecode",				// 153		//	OBSOLETE
	"kRegField1Line21CaptionEncode",				// 154		//	OBSOLETE
	"kRegField2Line21CaptionEncode",				// 155		//	OBSOLETE
	"kRegVANCGrabberSetup",							// 156		//	OBSOLETE
	"kRegVANCGrabberStatus1",						// 157		//	OBSOLETE
	"kRegVANCGrabberStatus2",						// 158		//	OBSOLETE
	"kRegVANCGrabberDataBuffer",					// 159		//	OBSOLETE
	"kRegVANCInserterSetup1",						// 160		//	OBSOLETE
	"kRegVANCInserterSetup2",						// 161		//	OBSOLETE
	"kRegVANCInserterDataBuffer",					// 162		//	OBSOLETE

	"kRegXptSelectGroup7",							// 163
	"kRegXptSelectGroup8",							// 164
	"kRegCh1ControlExtended",						// 165
	"kRegCh2ControlExtended",						// 166
	"kRegAFDVANCGrabber",							// 167
	"kRegFS1DownConverter2Control",					// 168
	"kRegSDIOut3Control",							// 169
	"kRegSDIOut4Control",							// 170
	"kRegAFDVANCInserterSDI1",						// 171
	"kRegAFDVANCInserterSDI2",						// 172
	"kRegAudioChannelMappingCh1",					// 173
	"kRegXptSelectGroup36",							// 174
	"kRegAudioChannelMappingCh3",					// 175
	"kRegAudioChannelMappingCh4",					// 176
	"kRegAudioChannelMappingCh5",					// 177
	"kRegAudioChannelMappingCh6",					// 178
	"kRegAudioChannelMappingCh7",					// 179
	"kRegAudioChannelMappingCh8",					// 180

	"kRegReserved181",								// 181
	"kRegReserved182",								// 182
	"kRegReserved183",								// 183
	"kRegReserved184",								// 184
	"kRegReserved185",								// 185
	"kRegReserved186",								// 186
	"kRegReserved187",								// 187
	"kRegSDIIn1VPIDA",								// 188
	"kRegSDIIn1VPIDB",								// 189
	"kRegAudioOutputSourceMap",						// 190
	"kRegXptSelectGroup11",							// 191
	"kRegStereoCompressor",							// 192
	"kRegXptSelectGroup12",							// 193
	"kRegFrameApertureOffset",						// 194
	"kRegReserved195",								// 195
	"kRegReserved196",								// 196
	"kRegReserved197",								// 197
	"kRegReserved198",								// 198
	"kRegReserved199",								// 199
	"kRegReserved200",								// 200
	"kRegReserved201",								// 201
	"kRegRP188InOut1Bits0_31_2",					// 202
	"kRegRP188InOut1Bits32_63_2",					// 203
	"kRegRP188InOut2Bits0_31_2",					// 204
	"kRegRP188InOut2Bits32_63_2",					// 205
	"kRegRP188InOut3Bits0_31_2",					// 206
	"kRegRP188InOut3Bits32_63_2",					// 207
	"kRegRP188InOut4Bits0_31_2",					// 208
	"kRegRP188InOut4Bits32_63_2",					// 209
	"kRegRP188InOut5Bits0_31_2",					// 210
	"kRegRP188InOut5Bits32_63_2",					// 211
	"kRegRP188InOut6Bits0_31_2",					// 212
	"kRegRP188InOut6Bits32_63_2",					// 213
	"kRegRP188InOut7Bits0_31_2",					// 214
	"kRegRP188InOut7Bits32_63_2",					// 215
	"kRegRP188InOut8Bits0_31_2",					// 216
	"kRegRP188InOut8Bits32_63_2",					// 217
	"kRegReserved218",								// 218
	"kRegReserved219",								// 219
	"kRegReserved220",								// 220
	"kRegReserved221",								// 221
	"kRegReserved222",								// 222
	"kRegReserved223",								// 223
	"kRegReserved224",								// 224
	"kRegReserved225",								// 225
	"kRegReserved226",								// 226
	"kRegReserved227",								// 227
	"kRegReserved228",								// 228
	"kRegReserved229",								// 229
	"kRegReserved230",								// 230
	"kRegReserved231",								// 231
	"kRegSDIInput3GStatus",							// 232
	"kRegLTCStatusControl",							// 233
	"kRegSDIOut1VPIDA",								// 234
	"kRegSDIOut1VPIDB",								// 235
	"kRegSDIOut2VPIDA",								// 236
	"kRegSDIOut2VPIDB",								// 237
	"kRegSDIIn2VPIDA",								// 238
	"kRegSDIIn2VPIDB",								// 239
	"kRegAud2Control",								// 240
	"kRegAud2SourceSelect",							// 241
	"kRegAud2OutputLastAddr",						// 242
	"kRegAud2InputLastAddr",						// 243
	"kRegRS4222Transmit",							// 244
	"kRegRS4222Receive",							// 245
	"kRegRS4222Control",							// 246
	"kRegVidProc2Control",							// 247
	"kRegMixer2Coefficient",						// 248
	"kRegFlatMatte2Value",							// 249
	"kRegXptSelectGroup9",							// 250
	"kRegXptSelectGroup10",							// 251
	"kRegLTC2EmbeddedBits0_31",						// 252
	"kRegLTC2EmbeddedBits32_63",					// 253
	"kRegLTC2AnalogBits0_31",						// 254
	"kRegLTC2AnalogBits32_63",						// 255
	"kRegSDITransmitControl",						// 256
	"kRegCh3Control",								// 257
	"kRegCh3OutputFrame",							// 258
	"kRegCh3InputFrame",							// 259
	"kRegCh4Control",								// 260
	"kRegCh4OutputFrame",							// 261
	"kRegCh4InputFrame",							// 262
	"kRegXptSelectGroup13",							// 263
	"kRegXptSelectGroup14",							// 264
	"kRegStatus2",									// 265
	"kRegVidIntControl2",							// 266
	"kRegGlobalControl2",							// 267
	"kRegRP188InOut3DBB",							// 268
	"kRegRP188InOut3Bits0_31",						// 269
	"kRegRP188InOut3Bits32_63",						// 270
	"kRegSDIOut3VPIDA",								// 271
	"kRegSDIOut3VPIDB",								// 272
	"kRegRP188InOut4DBB",							// 273
	"kRegRP188InOut4Bits0_31",						// 274
	"kRegRP188InOut4Bits32_63",						// 275
	"kRegSDIOut4VPIDA",								// 276
	"kRegSDIOut4VPIDB",								// 277
	"kRegAud3Control",								// 278
	"kRegAud4Control",								// 279
	"kRegAud3SourceSelect",							// 280
	"kRegAud4SourceSelect",							// 281
	"kRegAudDetect2",								// 282
	"kRegAud3OutputLastAddr",						// 283
	"kRegAud3InputLastAddr",						// 284
	"kRegAud4OutputLastAddr",						// 285
	"kRegAud4InputLastAddr",						// 286
	"kRegSDIInput3GStatus2",						// 287
	"kRegInputStatus2",								// 288
	"kRegCh3PCIAccessFrame",						// 289
	"kRegCh4PCIAccessFrame",						// 290

	"kRegCS3Coefficients1_2",						// 291
	"kRegCS3Coefficients3_4",						// 292
	"kRegCS3Coefficients5_6",						// 293
	"kRegCS3Coefficients7_8",						// 294
	"kRegCS3Coefficients9_10",						// 295

	"kRegCS4Coefficients1_2",						// 296
	"kRegCS4Coefficients3_4",						// 297
	"kRegCS4Coefficients5_6",						// 298
	"kRegCS4Coefficients7_8",						// 299
	"kRegCS4Coefficients9_10",						// 300

	"kRegXptSelectGroup17",							// 301
	"kRegXptSelectGroup15",							// 302
	"kRegXptSelectGroup16",							// 303

	"kRegAud3Delay",								// 304
	"kRegAud4Delay",								// 305

	"kRegSDIIn3VPIDA",								// 306
	"kRegSDIIn3VPIDB",								// 307
	"kRegSDIIn4VPIDA",								// 308
	"kRegSDIIn4VPIDB",								// 309

	"kRegSDIWatchdogControlStatus",					// 310
	"kRegSDIWatchdogTimeout",						// 311
	"kRegSDIWatchdogKick1",							// 312
	"kRegSDIWatchdogKick2",							// 313
	"kRegReserved314",								// 314
	"kRegReserved315",								// 315

	"kRegLTC3EmbeddedBits0_31",						// 316
	"kRegLTC3EmbeddedBits32_63",					// 317

	"kRegLTC4EmbeddedBits0_31",						// 318
	"kRegLTC4EmbeddedBits32_63",					// 319

	"kRegReserved320",								// 320
	"kRegReserved321",								// 321
	"kRegReserved322",								// 322
	"kRegReserved323",								// 323
	"kRegReserved324",								// 324
	"kRegReserved325",								// 325
	"kRegReserved326",								// 326
	"kRegReserved327",								// 327
	"kRegReserved328",								// 328

	"kRegHDMITimeCode",								// 329

	//	HDMI HDR Registers
	"kRegHDMIHDRGreenPrimary",						// 330
	"kRegHDMIHDRBluePrimary",						// 331
	"kRegHDMIHDRRedPrimary",						// 332
	"kRegHDMIHDRWhitePoint",						// 333
	"kRegHDMIHDRMasteringLuminence",				// 334
	"kRegHDMIHDRLightLevel",						// 335
	"kRegHDMIHDRControl",							// 336

	"kRegSDIOut5Control",							// 337
	"kRegSDIOut5VPIDA",								// 338
	"kRegSDIOut5VPIDB",								// 339

	"kRegRP188InOut5Bits0_31",						// 340
	"kRegRP188InOut5Bits32_63",						// 341
	"kRegRP188InOut5DBB",							// 342

	"kRegReserved343",								// 343

	"kRegLTC5EmbeddedBits0_31",						// 344
	"kRegLTC5EmbeddedBits32_63",					// 345

	"kRegDL5Control",								// 346

	"kRegCS5Coefficients1_2",						// 347
	"kRegCS5Coefficients3_4",						// 348
	"kRegCS5Coefficients5_6",						// 349
	"kRegCS5Coefficients7_8",						// 350
	"kRegCS5Coefficients9_10",						// 351

	"kRegXptSelectGroup18",							// 352

	"kRegReserved353",								// 353

	"kRegDC1",										// 354
	"kRegDC2",										// 355
	"kRegXptSelectGroup19",							// 356

	"kRegXptSelectGroup20",							// 357
	"kRegRasterizerControl",						// 358

	//	HDMI V2 In Registers
	"kRegHDMIV2I2C1Control",						// 360
	"kRegHDMIV2I2C1Data",							// 361
	"kRegHDMIV2VideoSetup",							// 362
	"kRegHDMIV2HSyncDurationAndBackPorch",			// 363
	"kRegHDMIV2HActive",							// 364
	"kRegHDMIV2VSyncDurationAndBackPorchField1",	// 365
	"kRegHDMIV2VSyncDurationAndBackPorchField2",	// 366
	"kRegHDMIV2VActiveField1",						// 367
	"kRegHDMIV2VActiveField2",						// 368
	"kRegHDMIV2VideoStatus",						// 369
	"kRegHDMIV2HorizontalMeasurements",				// 370
	"kRegHDMIV2HBlankingMeasurements",				// 371
	"kRegHDMIV2HBlankingMeasurements1",				// 372
	"kRegHDMIV2VerticalMeasurementsField0",			// 373
	"kRegHDMIV2VerticalMeasurementsField1",			// 374
	"kRegHDMIV2i2c2Control",						// 375
	"kRegHDMIV2i2c2Data",							// 376

	"kRegLUTV2Control",								// 376

	//	Scott: New DAX/MultiChannel Registers
	"kRegGlobalControlCh2",							// 377
	"kRegGlobalControlCh3",							// 378
	"kRegGlobalControlCh4",							// 379
	"kRegGlobalControlCh5",							// 380
	"kRegGlobalControlCh6",							// 381
	"kRegGlobalControlCh7",							// 382
	"kRegGlobalControlCh8",							// 383

	"kRegCh5Control",								// 384
	"kRegCh5OutputFrame",							// 385
	"kRegCh5InputFrame",							// 386
	"kRegCh5PCIAccessFrame",						// 387

	"kRegCh6Control",								// 388
	"kRegCh6OutputFrame",							// 389
	"kRegCh6InputFrame",							// 390
	"kRegCh6PCIAccessFrame",						// 391

	"kRegCh7Control",								// 392
	"kRegCh7OutputFrame",							// 393
	"kRegCh7InputFrame",							// 394
	"kRegCh7PCIAccessFrame",						// 395

	"kRegCh8Control",								// 396
	"kRegCh8OutputFrame",							// 397
	"kRegCh8InputFrame",							// 398
	"kRegCh8PCIAccessFrame",						// 399

	"kRegXptSelectGroup21",							// 400
	"kRegXptSelectGroup22",							// 401
	"kRegXptSelectGroup30",							// 402
	"kRegXptSelectGroup23",							// 403
	"kRegXptSelectGroup24",							// 404
	"kRegXptSelectGroup25",							// 405
	"kRegXptSelectGroup26",							// 406
	"kRegXptSelectGroup27",							// 407
	"kRegXptSelectGroup28",							// 408
	"kRegXptSelectGroup29",							// 409

	"kRegSDIIn5VPIDA",								// 410
	"kRegSDIIn5VPIDB",								// 411

	"kRegSDIIn6VPIDA",								// 412
	"kRegSDIIn6VPIDB",								// 413
	"kRegSDIOut6VPIDA",								// 414
	"kRegSDIOut6VPIDB",								// 415
	"kRegRP188InOut6Bits0_31",						// 416
	"kRegRP188InOut6Bits32_63",						// 417
	"kRegRP188InOut6DBB",							// 418
	"kRegLTC6EmbeddedBits0_31",						// 419
	"kRegLTC6EmbeddedBits32_63",					// 420

	"kRegSDIIn7VPIDA",								// 421
	"kRegSDIIn7VPIDB",								// 422
	"kRegSDIOut7VPIDA",								// 423
	"kRegSDIOut7VPIDB",								// 424
	"kRegRP188InOut7Bits0_31",						// 425
	"kRegRP188InOut7Bits32_63",						// 426
	"kRegRP188InOut7DBB",							// 427
	"kRegLTC7EmbeddedBits0_31",						// 428
	"kRegLTC7EmbeddedBits32_63",					// 429

	"kRegSDIIn8VPIDA",								// 430
	"kRegSDIIn8VPIDB",								// 431
	"kRegSDIOut8VPIDA",								// 432
	"kRegSDIOut8VPIDB",								// 433
	"kRegRP188InOut8Bits0_31",						// 434
	"kRegRP188InOut8Bits32_63",						// 435
	"kRegRP188InOut8DBB",							// 436
	"kRegLTC8EmbeddedBits0_31",						// 437
	"kRegLTC8EmbeddedBits32_63",					// 438

	"kRegXptSelectGroup31",							// 439

	"kRegAud5Control",								// 440
	"kRegAud5SourceSelect",							// 441
	"kRegAud5OutputLastAddr",						// 442
	"kRegAud5InputLastAddr",						// 443

	"kRegAud6Control",								// 444
	"kRegAud6SourceSelect",							// 445
	"kRegAud6OutputLastAddr",						// 446
	"kRegAud6InputLastAddr",						// 447

	"kRegAud7Control",								// 448
	"kRegAud7SourceSelect",							// 449
	"kRegAud7OutputLastAddr",						// 450
	"kRegAud7InputLastAddr",						// 451

	"kRegAud8Control",								// 452
	"kRegAud8SourceSelect",							// 453
	"kRegAud8OutputLastAddr",						// 454
	"kRegAud8InputLastAddr",						// 455

	"kRegAudioDetect5678",							// 456

	"kRegSDI5678Input3GStatus",						// 457

	"kRegInput56Status",							// 458
	"kRegInput78Status",							// 459

	"kRegCS6Coefficients1_2",						// 460
	"kRegCS6Coefficients3_4",						// 461
	"kRegCS6Coefficients5_6",						// 462
	"kRegCS6Coefficients7_8",						// 463
	"kRegCS6Coefficients9_10",						// 464

	"kRegCS7Coefficients1_2",						// 465
	"kRegCS7Coefficients3_4",						// 466
	"kRegCS7Coefficients5_6",						// 467
	"kRegCS7Coefficients7_8",						// 468
	"kRegCS7Coefficients9_10",						// 469

	"kRegCS8Coefficients1_2",						// 470
	"kRegCS8Coefficients3_4",						// 471
	"kRegCS8Coefficients5_6",						// 472
	"kRegCS8Coefficients7_8",						// 473
	"kRegCS8Coefficients9_10",						// 474

	"kRegSDIOut6Control",							// 475
	"kRegSDIOut7Control",							// 476
	"kRegSDIOut8Control",							// 477

	"kRegOutputTimingControlch2",					// 478
	"kRegOutputTimingControlch3",					// 479
	"kRegOutputTimingControlch4",					// 480
	"kRegOutputTimingControlch5",					// 481
	"kRegOutputTimingControlch6",					// 482
	"kRegOutputTimingControlch7",					// 483
	"kRegOutputTimingControlch8",					// 484

	"kRegVidProc3Control",							// 485
	"kRegMixer3Coefficient",						// 486
	"kRegFlatMatte3Value",							// 487

	"kRegVidProc4Control",							// 488
	"kRegMixer4Coefficient",						// 489
	"kRegFlatMatte4Value",							// 490

	"kRegTRSErrorStatus",							// 491

	"kRegAud5Delay",								// 492
	"kRegAud6Delay",								// 493
	"kRegAud7Delay",								// 494
	"kRegAud8Delay",								// 495

	"kRegPCMControl4321",							// 496
	"kRegPCMControl8765",							// 497

	"kRegCh1Control2MFrame",						// 498
	"kRegCh2Control2MFrame",						// 499
	"kRegCh3Control2MFrame",						// 500
	"kRegCh4Control2MFrame",						// 501
	"kRegCh5Control2MFrame",						// 502
	"kRegCh6Control2MFrame",						// 503
	"kRegCh7Control2MFrame",						// 504
	"kRegCh8Control2MFrame",						// 505

	"kRegXptSelectGroup32",							// 506
	"kRegXptSelectGroup33",							// 507
	"kRegXptSelectGroup34",							// 508
	"kRegXptSelectGroup35",							// 509

	"kRegReserved510",								// 510
	"kRegReserved511"								// 511

};	//	ntv2RegStrings


const char * NTV2RegisterNameString (const ULWord inRegNum)
{
	static const char *	sEmpty	("");
	if (inRegNum < kRegNumRegisters)
		return ntv2RegStrings [inRegNum];
	return sEmpty;
}


const char * NTV2InterruptEnumString (const unsigned inInterruptEnum)
{
	static const char *	sNullString = "";
	static const char * sInterruptEnumStrings[] = {	"eOutput1",					//	0
													"eInterruptMask",			//	1
													"eInput1",					//	2
													"eInput2",					//	3
													"eAudio",					//	4
													"eAudioInWrap",				//	5
													"eAudioOutWrap",			//	6
													"eDMA1",					//	7
													"eDMA2",					//	8
													"eDMA3",					//	9
													"eDMA4",					//	10
													"eChangeEvent",				//	11
													"eGetIntCount",				//	12
													"eWrapRate",				//	13
													"eUart1Tx",					//	14
													"eUart1Rx",					//	15
													"eAuxVerticalInterrupt",	//	16
													"ePushButtonChange",		//	17
													"eLowPower",				//	18
													"eDisplayFIFO",				//	19
													"eSATAChange",				//	20
													"eTemp1High",				//	21
													"eTemp2High",				//	22
													"ePowerButtonChange",		//	23
													"eInput3",					//	24
													"eInput4",					//	25
													"eUart2Tx",					//	26
													"eUart2Rx",					//	27
													"eHDMIRxV2HotplugDetect",	//	28
													"eInput5",					//	29
													"eInput6",					//	30
													"eInput7",					//	31
													"eInput8",					//	32
													"eInterruptMask2",			//	33
													"eOutput2",					//	34
													"eOutput3",					//	35
													"eOutput4",					//	36
													"eOutput5",					//	37
													"eOutput6",					//	38
													"eOutput7",					//	39
													"eOutput8"	};				//	40
#if !defined(NTV2_BUILDING_DRIVER)
	NTV2_ASSERT(sizeof(sInterruptEnumStrings) / sizeof(void*) == (unsigned)eNumInterruptTypes);
#endif
	if (inInterruptEnum < (unsigned)eNumInterruptTypes)
		return sInterruptEnumStrings[inInterruptEnum];
	return sNullString;
}	//	NTV2InterruptEnumString

#ifdef MSWindows

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

void __cdecl odprintf(const char *format, ...)
{
	//#ifdef _DEBUG
	char	buf[4096], *p = buf;
	va_list	args;

	va_start(args, format);
	p += _vsnprintf_s(buf, sizeof buf - 1, format, args);
	va_end(args);

	while ( p > buf  &&  isspace(p[-1]) )
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p   = '\0';

	::OutputDebugStringA(buf);
//#endif
}
#endif

#if !defined (NTV2_DEPRECATE)
	const char * NTV2BoardTypeString (NTV2BoardType type)	{return NTV2DeviceTypeString (type);}
	const char * NTV2BoardIDString (NTV2DeviceID id)		{return NTV2DeviceIDString (id);}
#endif	//	!defined (NTV2_DEPRECATE)
