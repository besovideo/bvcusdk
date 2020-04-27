
#ifndef __SAVCOMMON_H__
#define __SAVCOMMON_H__

#ifdef _MSC_VER
	#ifdef LIBSAV_EXPORTS
		#define LIBSAV_API __declspec(dllexport)
	#else
		#define LIBSAV_API __declspec(dllimport)
	#endif
#else
	#define LIBSAV_API 
#endif/*_MSC_VER*/

#include "SAVTypes.h"
#include <stdint.h>

#define SAV_TIME_BASE 1000000 /*common time base, in us*/
#define SAV_PTS_VALUE_MIN    INT64_MIN
#define SAV_PTS_VALUE_MAX    INT64_MAX
#define SAV_PTS_VALUE_NOSET  (-0x7fffffffffffffff - 1)

/** 
 * Required number of additionally allocated bytes at the end of the input bitstream for decoding.
 * This is mainly needed because some optimized bitstream readers read
 * 32 or 64 bit at once and could read over the end.
 * Note: If the first 23 bits of the additional bytes are not 0, then damaged
 * MPEG bitstreams could cause overread and segfault.
 */
#define SAV_INPUT_BUFFER_PADDING_SIZE 64 /** Ô­16, ffmpeg 3.2.3 ÊÇ32, ffmpeg4.0.2ÊÇ64 */

typedef struct _SAV_Rational{
	SAV_TYPE_INT32 num; /*numerator*/
	SAV_TYPE_INT32 den; /*denominator*/
} SAV_Rational;

/*Return value of library API. Error: < 0; Succeed: >=0*/
typedef enum _SAV_Result{
	SAV_RESULT_E_FAILED = -0x10000, /*general error*/
	SAV_RESULT_E_INVALIDARG,        /*invalid argument*/
	SAV_RESULT_E_BADSTATE,          /*bad internal state*/
	SAV_RESULT_E_ALLOCMEMFAILED,    /*allocate memory failed*/
	SAV_RESULT_E_UNSUPPORTED,       /*unsupported functions*/
	SAV_RESULT_E_MEMALIGNMENT,      /*memory alignment is not satisfied*/
	SAV_RESULT_E_NOTFOUND,          /*not found*/
	SAV_RESULT_E_NOTALLOWED,        /*the requested access is not allowed*/
	SAV_RESULT_E_IO,                /*I/O error*/
	SAV_RESULT_E_EOF,               /*End of file*/
	SAV_RESULT_E_INVALIDDATA,       /*Invalid data found when processing input*/
	SAV_RESULT_E_NOTIMPL,           /*not implemented*/
	SAV_RESULT_E_BUFSMALL,          /*buffer is small*/
	SAV_RESULT_E_ABORTED,           /*aborted*/
	SAV_RESULT_E_NOTOPENED,         /*not opened*/
	

	SAV_RESULT_S_OK = 0,            /*succeed*/
	SAV_RESULT_S_IGNORE,            /*succeed ,but something can not handle is ignored.*/
	
}SAV_Result;
#define SAV_Result_SUCCEEDED(a) ( ((int)(a)) >= 0 )
#define SAV_Result_FAILED(a) ( ((int)(a)) < 0 )

/*media type*/
typedef enum{
	SAV_MEDIATYPE_UNKNOWN = -1,
	SAV_MEDIATYPE_VIDEO,
	SAV_MEDIATYPE_AUDIO,
	SAV_MEDIATYPE_DATA,
	SAV_MEDIATYPE_SUBTITLE,
	SAV_MEDIATYPE_ATTACHMENT,
	SAV_MEDIATYPE_NB
}SAV_MediaType;

/*video pixel format*/
#define SAV_IMG_COMPONENT 4
typedef enum {
	SAV_PIX_FMT_NONE = -1,
	SAV_PIX_FMT_YUV420P,   /*planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)*/
	SAV_PIX_FMT_YUYV422,   /*packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr*/
	SAV_PIX_FMT_RGB24,     /*packed RGB 8:8:8, 24bpp, RGBRGB...*/
	SAV_PIX_FMT_BGR24,     /*packed RGB 8:8:8, 24bpp, BGRBGR...*/
	SAV_PIX_FMT_YUV422P,   /*planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)*/
	SAV_PIX_FMT_YUV444P,   /*planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)*/
    SAV_PIX_FMT_YUV410P,   /*planar YUV 4:1:0,  9bpp, (1 Cr & Cb sample per 4x4 Y samples)*/
    SAV_PIX_FMT_YUV411P,   /*planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples)*/
    SAV_PIX_FMT_GRAY8,     /*       Y        ,  8bpp*/
    SAV_PIX_FMT_MONOWHITE, /*       Y        ,  1bpp, 0 is white, 1 is black, in each byte pixels are ordered from the msb to the lsb*/
    SAV_PIX_FMT_MONOBLACK, /*       Y        ,  1bpp, 0 is black, 1 is white, in each byte pixels are ordered from the msb to the lsb*/
    SAV_PIX_FMT_PAL8,      /*8 bit with PIX_FMT_RGB32 palette*/
    SAV_PIX_FMT_YUVJ420P,  /*planar YUV 4:2:0, 12bpp, full scale (JPEG), deprecated in favor of PIX_FMT_YUV420P and setting color_range*/
    SAV_PIX_FMT_YUVJ422P,  /*planar YUV 4:2:2, 16bpp, full scale (JPEG), deprecated in favor of PIX_FMT_YUV422P and setting color_range*/
    SAV_PIX_FMT_YUVJ444P,  /*planar YUV 4:4:4, 24bpp, full scale (JPEG), deprecated in favor of PIX_FMT_YUV444P and setting color_range*/
    
    SAV_PIX_FMT_UYVY422 = 17,   /*packed YUV 4:2:2, 16bpp, Cb Y0 Cr Y1*/
    SAV_PIX_FMT_UYYVYY411, /*packed YUV 4:1:1, 12bpp, Cb Y0 Y1 Cr Y2 Y3*/
    SAV_PIX_FMT_BGR8,      /*packed RGB 3:3:2,  8bpp, (msb)2B 3G 3R(lsb)*/
    SAV_PIX_FMT_BGR4,      /*packed RGB 1:2:1 bitstream,  4bpp, (msb)1B 2G 1R(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits*/
    SAV_PIX_FMT_BGR4_BYTE, /*packed RGB 1:2:1,  8bpp, (msb)1B 2G 1R(lsb)*/
    SAV_PIX_FMT_RGB8,      /*packed RGB 3:3:2,  8bpp, (msb)2R 3G 3B(lsb)*/
    SAV_PIX_FMT_RGB4,      /*packed RGB 1:2:1 bitstream,  4bpp, (msb)1R 2G 1B(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits*/
    SAV_PIX_FMT_RGB4_BYTE, /*packed RGB 1:2:1,  8bpp, (msb)1R 2G 1B(lsb)*/
    SAV_PIX_FMT_NV12,      /*planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V)*/
    SAV_PIX_FMT_NV21,      /*as above, but U and V bytes are swapped*/
    
    SAV_PIX_FMT_ARGB,      /*packed ARGB 8:8:8:8, 32bpp, ARGBARGB...*/
    SAV_PIX_FMT_RGBA,      /*packed RGBA 8:8:8:8, 32bpp, RGBARGBA...*/
    SAV_PIX_FMT_ABGR,      /*packed ABGR 8:8:8:8, 32bpp, ABGRABGR...*/
    SAV_PIX_FMT_BGRA,      /*packed BGRA 8:8:8:8, 32bpp, BGRABGRA...*/
    
    SAV_PIX_FMT_GRAY16BE,  /*       Y        , 16bpp, big-endian*/
    SAV_PIX_FMT_GRAY16LE,  /*       Y        , 16bpp, little-endian*/
    SAV_PIX_FMT_YUV440P,   /*planar YUV 4:4:0 (1 Cr & Cb sample per 1x2 Y samples)*/
    SAV_PIX_FMT_YUVJ440P,  /*planar YUV 4:4:0 full scale (JPEG), deprecated in favor of PIX_FMT_YUV440P and setting color_range*/
    SAV_PIX_FMT_YUVA420P,  /*planar YUV 4:2:0, 20bpp, (1 Cr & Cb sample per 2x2 Y & A samples)*/
        
    SAV_PIX_FMT_RGB565BE = 43,  /*packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), big-endian*/
    SAV_PIX_FMT_RGB565LE,  /*packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), little-endian*/
    SAV_PIX_FMT_RGB555BE,  /*packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), big-endian, most significant bit to 0*/
    SAV_PIX_FMT_RGB555LE,  /*packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), little-endian, most significant bit to 0*/
    
    SAV_PIX_FMT_BGR565BE,  /*packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), big-endian*/
    SAV_PIX_FMT_BGR565LE,  /*packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), little-endian*/
    SAV_PIX_FMT_BGR555BE,  /*packed BGR 5:5:5, 16bpp, (msb)1A 5B 5G 5R(lsb), big-endian, most significant bit to 1*/
    SAV_PIX_FMT_BGR555LE,  /*packed BGR 5:5:5, 16bpp, (msb)1A 5B 5G 5R(lsb), little-endian, most significant bit to 1*/
    
    SAV_PIX_FMT_NB,
}SAV_PixelFormat;

/*Audio Sample format*/
typedef enum  {
	SAV_SAMPLE_FMT_NONE = -1,
	SAV_SAMPLE_FMT_U8,       /*unsigned 8 bits*/
	SAV_SAMPLE_FMT_S16,      /*signed 16 bits*/
	SAV_SAMPLE_FMT_S32,      /*signed 32 bits*/
	SAV_SAMPLE_FMT_FLT,      /*float*/
	SAV_SAMPLE_FMT_DBL,      /*double*/
	
	SAV_SAMPLE_FMT_U8P,      /*unsigned 8 bits, planar*/
    SAV_SAMPLE_FMT_S16P,     /*signed 16 bits, planar*/
    SAV_SAMPLE_FMT_S32P,     /*signed 32 bits, planar*/
    SAV_SAMPLE_FMT_FLTP,     /*float, planar*/
    SAV_SAMPLE_FMT_DBLP,     /*double, planar*/
    SAV_SAMPLE_FMT_S64,       /*signed 64 bits*/
    SAV_SAMPLE_FMT_S64P,      /*signed 64 bits, planar*/
    
	SAV_SAMPLE_FMT_NB        /*Number of sample formats. DO NOT USE if linking dynamically*/
}SAV_SampleFormat;

/*video encoded picture type*/
typedef enum  {
	SAV_PICTURE_TYPE_NONE = 0,/*Undefined*/
	SAV_PICTURE_TYPE_I,       /*Intra*/
	SAV_PICTURE_TYPE_P,       /*Predicted*/
	SAV_PICTURE_TYPE_B,       /*Bi-dir predicted*/
	SAV_PICTURE_TYPE_S,       /*S(GMC)-VOP MPEG4*/
	SAV_PICTURE_TYPE_SI,      /*Switching Intra*/
	SAV_PICTURE_TYPE_SP,      /*Switching Predicted*/
	SAV_PICTURE_TYPE_BI,      /*BI type*/
}SAV_PictureType;

/*flags*/
enum {
	SAV_PKT_FLAG_KEY = 1 /*key frame flag*/
};

typedef struct _SAV_VideoParam{
	/**
	* Pixel format, see SAV_PIX_FMT_xxx.	
	* - encode:set by user.
	* - decode:set by library.
	* - mux/demux: unused
	*/
	SAV_PixelFormat ePixelFmt;

	/**
	* picture width / height.
	* - mux/encode: MUST be set by user.
	* - demux/decode:set by library.
	*/
	SAV_TYPE_INT32 iWidth;
	SAV_TYPE_INT32 iHeight;

	/**
	* the number of pictures in a group of pictures, or 0 for intra_only
	* - encode:set by user.
	* - mux/demux/decode: unused
	*/
	SAV_TYPE_INT32 iGOP;

	/**
	* maximum number of B-frames between non-B-frames
	* Note: The output will be delayed by iMaxBFrames+1 relative to the input.
	* - encode:set by user.
	* - mux/demux/decode: unused
	*/
	SAV_TYPE_INT32 iMaxBFrames;

}SAV_VideoParam;

typedef struct _SAV_AudioParam{
	/**
	* samples per second
	* - mux/encode: set by user.
	* - demux/decode: Depends on codec, MAYBE set by user/library. for G.726/AAC, set by library
	*/
	SAV_TYPE_INT32 iSampleRate;//

	/**
	* number of audio channels
	* - mux/encode: set by user.
	* - demux/decode: set by library.
	*/
	SAV_TYPE_INT32 iChannelCount;

	/**
	* audio sample format
	* - encode: set by user.
	* - decode: set by library.
	* - mux/demux: unused
	*/
	SAV_SampleFormat eSampleFormat; //

	/**
	* Samples per packet.A packet is a group of samples encoded into stream by SAVCodec_Process()
	* - encode:set by user,but SAVCodec_Open()/SAVContainer_Open may change it to another value.
	* - mux/demux/decode: not used.
	*/
	SAV_TYPE_INT32 iFrameSize;

}SAV_AudioParam;

/*video/audio frame: describe the raw A/V data.*/
typedef struct _SAV_Frame{
	/*struct size.must be initialized to sizeof(SAV_Frame) by user*/
	SAV_TYPE_INT32 iSize;

	/**
	* Presentation timestamp in 1/SAV_TIME_BASE unit. SAV_PTS_VALUE_NOSET if no set.
	*  - encode:set by user.
	*  - decode:set by library.
	*/
	SAV_TYPE_INT64 iPTS;

	/**
	* video:pointer to the picture planes and the stride of each plane line.
	* audio:pointer to the audio data and length.Only pData[0]/iDataSize[0] is used
	* data:pointer to the data and length.Only pData[0]/iDataSize[0] is used
	* - encode:set by user. The memory is allocated/freed by user. 
	* - decode:set by library. Video:The memory is allocated/freed by library. Audio:The memory is allocated/freed by user
	*/
	SAV_TYPE_UINT8* ppData[SAV_IMG_COMPONENT];
	SAV_TYPE_INT32 iDataSize[SAV_IMG_COMPONENT];

	/**
	* Picture type of the frame, see SAV_PICTURE_TYPE_* .
	* - encode:set by user for expected picture type.
	* - decode:set by library.
	*/
	SAV_PictureType ePictType;

	/**
	* Flags. See SAV_PKT_FLAG_*
	* - encode:set by User. User set SAV_PKT_FLAG_KEY to inform codec to encode the current frame is into a key frame.
	* - decode:set by library.
	*/
	SAV_TYPE_INT32  iFlags;
}SAV_Frame;

/*video/audio packet: describe the encoded A/V data*/
typedef struct _SAV_Packet{
	//struct size.must be initialized to sizeof(SAV_Packet) by user
	SAV_TYPE_INT32 iSize;

	/**
	* Presentation timestamp in 1/SAV_TIME_BASE unit. SAV_PTS_VALUE_NOSET if no set.
	*  - demux/encode:set by library
	*  - mux/decode:set by user
	*/
	SAV_TYPE_INT64 iPTS;

	/**
	* Duration of this packet in 1/SAV_TIME_BASE unit, 0 if unknown.
	* Equals next_iPTS - this_iPTS in presentation order.
	* - mux/encode: set by library.
	* - demux/decode: set by library.If 0,not set
	*/
	SAV_TYPE_INT64 iDuration;

	/**
	*  pointers to the encoded stream. The memory is allocated/freed by user.
	*  - mux/encode: allocated/set/freed by user.see SAVCodec_Process
	*  - demux: allocated/set/freed by library. see SAVContainer_Process
	*  - decode: set by user.see SAV_INPUT_BUFFER_PADDING_SIZE for more information.
	*/
	SAV_TYPE_UINT8* pData;

	/**
	*  - encode: set by user.The available encoded stream buffer length
	*  - mux/decode: set by user.The encoded stream length.see SAV_INPUT_BUFFER_PADDING_SIZE for more information.
	*  - demux: set by library. the stream length read in.
	*/
	SAV_TYPE_INT32  iDataSize;

	/**
	* the index of the stream.
	* -encode/decode: not used
	* -mux: set by user.
	* -demux: set by library
	*/
	SAV_TYPE_INT32 iStreamIndex;

	/**
	* Flags. See SAV_PKT_FLAG_*
	* - mux: set by user
	* - demux/encode: set by library. if SAV_PKT_FLAG_KEY is set, the frame is a key frame
	* - decode: not used.
	*/
	SAV_TYPE_INT32  iFlags;

	/**
	* Picture type of the frame, see SAV_PICTURE_TYPE_* .
	* - encode: set by library. the encoded picture type by library
	* - demux/decode: not used.
	* - mux: not used
	*/
	SAV_PictureType ePictType;


}SAV_Packet;

#endif
