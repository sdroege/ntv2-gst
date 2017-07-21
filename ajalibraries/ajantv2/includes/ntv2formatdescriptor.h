/**
	@file		ntv2formatdescriptor.h
	@brief		Declares the NTV2FormatDescriptor class.
	@copyright	(C) 2016-2017 AJA Video Systems, Inc.	Proprietary and confidential information.
**/

#ifndef NTV2FORMATDESC_H
#define NTV2FORMATDESC_H

#include "ajaexport.h"
#include "ajatypes.h"
#include "ntv2enums.h"
#include "ntv2utils.h"
#include "videodefines.h"
#include "ntv2publicinterface.h"
#include <string>
#include <iostream>
#include <vector>
#if defined (AJALinux)
	#include <stdint.h>
#endif


/**
	@brief	This provides additional information about a video frame for a given video standard or format and pixel format,
			including the total number of lines, number of pixels per line, line pitch, and which line contains the start
			of active video.
**/
typedef struct NTV2FormatDescriptor
{
	/**
		@brief	My default constructor initializes me in an "invalid" state.
	**/
	explicit		NTV2FormatDescriptor ();	///< @brief	My default constructor

	/**
		@brief	Construct from line and pixel count, plus line pitch.
		@param[in]	inNumLines			Specifies the total number of lines.
		@param[in]	inNumPixels			Specifies the total number of pixels.
		@param[in]	inLinePitch			Specifies the line pitch as the number of 32-bit words per line.
		@param[in]	inFirstActiveLine	Optionally specifies the first active line of video, where zero is the first (top) line. Defaults to zero.
	**/
	explicit 		NTV2FormatDescriptor (	const ULWord inNumLines,
											const ULWord inNumPixels,
											const ULWord inLinePitch,
											const ULWord inFirstActiveLine = 0);
#if !defined (NTV2_DEPRECATE_13_0)
	/**
		@brief		Constructs me from the given video standard, pixel format, whether or not a 2K format is in use, and VANC settings.
		@param[in]	inVideoStandard			Specifies the video standard being used.
		@param[in]	inFrameBufferFormat		Specifies the pixel format of the frame buffer.
		@param[in]	inVANCenabled			Specifies if VANC is enabled or not. Defaults to false.
		@param[in]	in2Kby1080				Specifies if a 2K format is in use or not. Defaults to false.
		@param[in]	inWideVANC				Specifies if "taller VANC" is enabled or not. Defaults to false.
	**/
	explicit		NTV2FormatDescriptor (	const NTV2Standard			inVideoStandard,
											const NTV2FrameBufferFormat	inFrameBufferFormat,
											const bool					inVANCenabled	= false,
											const bool					in2Kby1080		= false,
											const bool					inWideVANC		= false);

	/**
		@brief		Constructs me from the given video format, pixel format and VANC settings.
		@param[in]	inVideoFormat			Specifies the video format being used.
		@param[in]	inFrameBufferFormat		Specifies the pixel format of the frame buffer.
		@param[in]	inVANCenabled			Specifies if VANC is enabled or not. Defaults to false.
		@param[in]	inWideVANC				Specifies if "taller VANC" is enabled or not. Defaults to false.
	**/
	explicit		NTV2FormatDescriptor (	const NTV2VideoFormat		inVideoFormat,
											const NTV2FrameBufferFormat	inFrameBufferFormat,
											const bool					inVANCenabled	= false,
											const bool					inWideVANC		= false);
#endif	//	!defined (NTV2_DEPRECATE_13_0)

	/**
		@brief		Constructs me from the given video standard, pixel format, and VANC settings.
		@param[in]	inStandard				Specifies the video standard being used.
		@param[in]	inFrameBufferFormat		Specifies the pixel format of the frame buffer.
		@param[in]	inVancMode				Specifies the VANC mode. Defaults to OFF.
	**/
	explicit		NTV2FormatDescriptor (	const NTV2Standard			inStandard,
											const NTV2FrameBufferFormat	inFrameBufferFormat,
											const NTV2VANCMode			inVancMode		= NTV2_VANCMODE_OFF);

	/**
		@brief		Constructs me from the given video format, pixel format and VANC settings.
		@param[in]	inVideoFormat			Specifies the video format being used.
		@param[in]	inFrameBufferFormat		Specifies the pixel format of the frame buffer.
		@param[in]	inVancMode				Specifies the VANC mode.
	**/
	explicit		NTV2FormatDescriptor (	const NTV2VideoFormat		inVideoFormat,
											const NTV2FrameBufferFormat	inFrameBufferFormat,
											const NTV2VANCMode			inVancMode
#if defined (NTV2_DEPRECATE_13_0)
											= NTV2_VANCMODE_OFF
#endif	//	NTV2_DEPRECATE_13_0
											);

	inline bool		IsValid (void) const				{return numLines && numPixels && mNumPlanes && mLinePitch[0];}	///< @return	True if valid;  otherwise false.
	inline bool		IsVANC (void) const					{return firstActiveLine > 0;}									///< @return	True if VANC geometry;  otherwise false.
	inline bool		IsPlanar (void) const				{return GetNumPlanes() > 1 || NTV2_IS_FBF_PLANAR (mPixelFormat);}	///< @return	True if planar format;  otherwise false.

	/**
		@return		The total number of bytes required to hold the raster.
		@note		To determine the byte count of all planes of a planar format, call GetNumPlanes,
					then sum the byte counts for each plane.
		@param[in]	inPlaneIndex0		Specifies the plane of interest. Defaults to zero.
	**/
	inline ULWord	GetTotalRasterBytes (const UWord inPlaneIndex0 = 0) const	{return inPlaneIndex0 < mNumPlanes ? numLines * mLinePitch[inPlaneIndex0] : 0;}

	/**
		@return		The total number of bytes required to hold the visible raster.
		@param[in]	inPlaneIndex0	Specifies the plane of interest. Defaults to zero.
	**/
	inline ULWord	GetVisibleRasterBytes (const UWord inPlaneIndex0 = 0) const	{return inPlaneIndex0 < mNumPlanes ? ((numLines - firstActiveLine) * mLinePitch[inPlaneIndex0]) : 0;}

	/**
		@return		The number of bytes per row/line of the raster.
		@param[in]	inPlaneIndex0	Specifies the plane of interest. Defaults to zero.
	**/
	inline ULWord	GetBytesPerRow (const UWord inPlaneIndex0 = 0) const	{return inPlaneIndex0 < mNumPlanes ? mLinePitch[inPlaneIndex0] : 0;}

	inline ULWord	GetRasterWidth (void) const			{return numPixels;}			///< @return	The width of the raster, in pixels.
	inline UWord	GetNumPlanes (void) const			{return mNumPlanes;}		///< @return	The number of planes in the raster.

	/**
		@return	The height of the raster, in lines.
		@param[in]	inVisibleOnly	Specify true to return just the visible height;  otherwise false (the default) to return the full height.
	**/
	inline ULWord	GetRasterHeight (const bool inVisibleOnly = false) const		{return inVisibleOnly ? numLines - firstActiveLine : numLines;}

	/**
		@return		The full height of the raster, in lines (including VANC, if any).
	**/
	inline ULWord	GetFullRasterHeight (void) const								{return numLines;}

	/**
		@return		The zero-based index number of the first active (visible) line in the raster. This will be zero for non-VANC rasters.
	**/
	inline ULWord	GetFirstActiveLine (void) const									{return firstActiveLine;}

	/**
		@return		The visible height of the raster, in lines (excluding VANC, if any).
	**/
	inline ULWord	GetVisibleRasterHeight (void) const								{return numLines - firstActiveLine;}

	/**
		@return		A pointer to the start of the given row in the given buffer, or NULL if row index is bad
					(using my description of the buffer contents).
		@param[in]	pInStartAddress		A pointer to the raster buffer.
		@param[in]	inRowIndex0			Specifies the row of interest in the buffer, where zero is the topmost row.
		@param[in]	inPlaneIndex0		Specifies the plane of interest. Defaults to zero.
	**/
	const void *	GetRowAddress (const void * pInStartAddress, const ULWord inRowIndex0, const UWord inPlaneIndex0 = 0) const;

	/**
		@return		A pointer to the start of the first visible row in the given buffer, or NULL if invalid
					(using my description of the buffer contents).
		@param[in]	pInStartAddress		A pointer to the raster buffer.
	**/
	inline UByte *					GetTopVisibleRowAddress (UByte * pInStartAddress) const				{return (UByte *) GetRowAddress (pInStartAddress, firstActiveLine);}

	/**
		@brief		Compares two buffers line-by-line (using my description of the buffer contents).
		@param[in]	pInStartAddress1		A valid, non-NULL pointer to the start of the first raster buffer.
		@param[in]	pInStartAddress2		A valid, non-NULL pointer to the start of the second raster buffer.
		@param[out]	outFirstChangedRowNum	Receives the zero-based row number of the first row that's different,
											or 0xFFFFFFFF if identical.
		@return		True if successful;  otherwise false.
	**/
	bool							GetFirstChangedRow (const void * pInStartAddress1, const void * pInStartAddress2, ULWord & outFirstChangedRowNum) const;

	/**
		@brief		Compares two buffers line-by-line (using my description of the buffer contents).
		@param[out]	outDiffs	Receives the ordered sequence of line offsets of the lines that differed.
								This will be empty if the two buffers are identical (or if an error occurs).
		@param[in]	pInBuffer1	Specifies the non-NULL address of the first memory buffer whose contents are to be compared.
		@param[in]	pInBuffer2	Specifies the non-NULL address of the second memory buffer whose contents are to be compared.
		@param[in]	inMaxLines	Optionally specifies the maximum number of lines to compare. If zero, all lines are compared.
								Defaults to zero (all lines).
		@return		True if successful;  otherwise false.
		@note		The buffers must be large enough to accommodate my video standard/format or else a memory access violation will occur.
	**/
	bool							GetChangedLines (NTV2RasterLineOffsets & outDiffs, const void * pInBuffer1, const void * pInBuffer2, const ULWord inMaxLines = 0) const;

	/**
		@return	The full-raster NTV2FrameDimensions (including VANC lines, if any).
	**/
	inline NTV2FrameDimensions		GetFullRasterDimensions (void) const					{return NTV2FrameDimensions (GetRasterWidth(), GetRasterHeight(false));}

	/**
		@return	The visible NTV2FrameDimensions (excluding VANC lines, if any).
	**/
	inline NTV2FrameDimensions		GetVisibleRasterDimensions (void) const					{return NTV2FrameDimensions (GetRasterWidth(), GetRasterHeight(true));}

	/**
		@brief		Answers with the equivalent SMPTE line number for the given line offset into the frame buffer I describe.
		@param[in]	inLineOffset	Specifies the zero-based line offset into the frame buffer that I describe.
		@param[out]	outSMPTELine	Receives the equivalent SMPTE line number.
		@param[out]	outIsField2		Receives true if the line number is associated with Field 2 (interlaced only); otherwise false.
		@return		True if successful;  otherwise false.
	**/
	bool							GetSMPTELineNumber (const ULWord inLineOffset, ULWord & outSMPTELine, bool & outIsField2) const;

	/**
		@return	True if I'm equal to the given NTV2FormatDescriptor.
		@param[in]	inRHS	The right-hand-side operand that I'll be compared with.
	**/
	bool							operator == (const NTV2FormatDescriptor & inRHS) const;

	/**
		@brief		Writes a human-readable description of me into the given output stream.
		@param		inOutStream		The output stream to be written into.
		@param[in]	inDetailed		If true (the default), writes a detailed description;  otherwise writes a brief one.
		@return		The output stream I was handed.
	**/
	std::ostream &					Print (std::ostream & inOutStream, const bool inDetailed = true) const;

	/**
		@brief		Writes the given frame buffer line offset as a formatted SMPTE line number into the given output stream.
		@param[in]	inLineOffset	Specifies the zero-based line offset in the frame buffer.
		@return		The output stream I was handed.
	**/
	std::ostream &					PrintSMPTELineNumber (std::ostream & inOutStream, const ULWord inLineOffset) const;

	inline NTV2Standard				GetVideoStandard (void) const	{return mStandard;}							///< @return	The video standard I was created with.
	inline NTV2VideoFormat			GetVideoFormat (void) const		{return mVideoFormat;}						///< @return	The video format I was created with.
	inline NTV2FrameBufferFormat	GetPixelFormat (void) const		{return mPixelFormat;}						///< @return	The pixel format I was created with.
	inline NTV2VANCMode				GetVANCMode (void) const		{return mVancMode;}							///< @return	The VANC mode I was created with.
	inline bool						Is2KFormat (void) const			{return m2Kby1080;}							///< @return	True if I was created with a 2Kx1080 video format.
	inline bool						IsQuadRaster (void) const		{return NTV2_IS_QUAD_STANDARD(mStandard) || NTV2_IS_4K_VIDEO_FORMAT(mVideoFormat);}	///< @return	True if I was created with a 4K/UHD video format or standard.
	inline bool						IsTallVanc (void) const			{return mVancMode == NTV2_VANCMODE_TALL;}	///< @return	True if I was created with just "tall" VANC.
	inline bool						IsTallerVanc (void) const		{return mVancMode == NTV2_VANCMODE_TALLER;}	///< @return	True if I was created with "taller" VANC.
	inline NTV2FrameGeometry		GetFrameGeometry (void) const	{return mFrameGeometry;}					///< @return	The frame geometry I was created with.
	void							MakeInvalid (void);				///< @brief	Resets me into an invalid (NULL) state.

	private:
		void						FinalizePlanar (void);			///< @brief	Finishes initialization for planar formats

	//	Member Data
	public:
		ULWord					numLines;			///< @brief	Height -- total number of lines
		ULWord					numPixels;			///< @brief	Width -- total number of pixels per line
		ULWord					linePitch;			///< @brief	Number of 32-bit words per line. Shadows mLinePitch[0] * sizeof(ULWord).
		ULWord					firstActiveLine;	///< @brief	First active line of video (0 if NTV2_VANCMODE_OFF)
	private:
		NTV2Standard			mStandard;			///< @brief	My originating video standard
		NTV2VideoFormat			mVideoFormat;		///< @brief	My originating video format (if known)
		NTV2FrameBufferFormat	mPixelFormat;		///< @brief	My originating frame buffer format
		NTV2VANCMode			mVancMode;			///< @brief	My originating VANC mode
		bool					m2Kby1080;			///< @brief	My originating 2Kx1080 setting
		ULWord					mLinePitch[4];		///< @brief	Number of bytes per row/line (per-plane)
		UWord					mNumPlanes;			///< @brief	Number of planes
		NTV2FrameGeometry		mFrameGeometry;		///< @brief My originating video geometry

} NTV2FormatDescriptor;


/**
	@brief		Writes the given NTV2FormatDescriptor to the specified output stream.
	@param		inOutStream		Specifies the output stream to receive the human-readable representation of the NTV2FormatDescriptor.
	@param[in]	inFormatDesc	Specifies the NTV2FormatDescriptor instance to print to the output stream.
	@return	A non-constant reference to the specified output stream.
**/
inline std::ostream & operator << (std::ostream & inOutStream, const NTV2FormatDescriptor & inFormatDesc)	{return inFormatDesc.Print (inOutStream);}


//#if !defined (NTV2_DEPRECATE_12_6)
	AJAExport NTV2FormatDescriptor GetFormatDescriptor (const NTV2Standard			inVideoStandard,
														const NTV2FrameBufferFormat	inFrameBufferFormat,
														const bool					inVANCenabled	= false,
														const bool					in2Kby1080		= false,
														const bool					inWideVANC		= false);
//#endif	//	!defined (NTV2_DEPRECATE_12_6)


/**
	@brief		Returns a format descriptor that describes a video frame having the given video format and pixel format.
	@param[in]	inVideoFormat			Specifies the video format being used.
	@param[in]	inFrameBufferFormat		Specifies the pixel format of the frame buffer.
	@param[in]	inVANCenabled			Specifies if VANC is enabled or not. Defaults to false.
	@param[in]	inWideVANC				Specifies if "wide VANC" is enabled or not. Defaults to false.
	@return		A format descriptor that describes a video frame having the given video standard and pixel format.
**/
AJAExport NTV2FormatDescriptor GetFormatDescriptor (const NTV2VideoFormat			inVideoFormat,
													 const NTV2FrameBufferFormat	inFrameBufferFormat,
													 const bool						inVANCenabled	= false,
													 const bool						inWideVANC		= false);

/**
	@brief		Unpacks a line of NTV2_FBF_10BIT_YCBCR video into 16-bit-per-component YUV data.
	@param[in]	pIn10BitYUVLine		A valid, non-NULL pointer to the start of the line that contains the NTV2_FBF_10BIT_YCBCR data
									to be converted.
	@param[out]	out16BitYUVLine		Receives the unpacked 16-bit-per-component YUV data. The sequence is cleared before filling.
									The UWord sequence will be Cb0, Y0, Cr0, Y1, Cb1, Y2, Cr1, Y3, Cb2, Y4, Cr2, Y5, . . .
	@param[in]	inNumPixels			Specifies the width of the line to be converted, in pixels.
	@return		True if successful;  otherwise false.
**/
AJAExport bool		UnpackLine_10BitYUVtoUWordSequence (const void * pIn10BitYUVLine, const NTV2FormatDescriptor & inFormatDesc, UWordSequence & out16BitYUVLine);


#if !defined (NTV2_DEPRECATE)
	extern AJAExport const NTV2FormatDescriptor formatDescriptorTable [NTV2_NUM_STANDARDS] [NTV2_FBF_NUMFRAMEBUFFERFORMATS];
#endif	//	!defined (NTV2_DEPRECATE)


#endif	//	NTV2FORMATDESC_H