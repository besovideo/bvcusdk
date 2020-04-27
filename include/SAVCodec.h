#ifndef __LIBSAVCODEC_H__
#define __LIBSAVCODEC_H__
#ifdef  __cplusplus
extern "C" {
#endif

#include "SAVCommon.h"
#include "SAVUtil.h"
/*
NOTE: The version of this library can be retrieved from VERSIONINFO resource in DLL.
*/
/*codec id*/
typedef enum  {
    SAVCODEC_ID_NONE ,
    /*video*/
    SAVCODEC_ID_FIRST_VIDEO = 0x00001,
    SAVCODEC_ID_MJPEG  = 0x00008,
    SAVCODEC_ID_H264  = 0x0001C,
    SAVCODEC_ID_HEVC = 0x000AE, // from ffmpeg libavcodec/avcodec.h AVCodecID.AV_CODEC_ID_HEVC
    SAVCODEC_ID_H265 = SAVCODEC_ID_HEVC,
    /*audio*/
    SAVCODEC_ID_FIRST_AUDIO = 0x10000, // PCM
    SAVCODEC_ID_G726  = 0x1100B,
    SAVCODEC_ID_AAC   = 0x15002,
	SAVCODEC_ID_G711A  = 0x10007,
    SAVCODEC_ID_MP3 = 0x15001, // 2019/01/30 add
    /*subtile*/
    SAVCODEC_ID_FIRST_SUBTITLE = 0x17000,
    SAVCODEC_ID_TEXT  = 0x17002,
    
    /*custom data*/
    SAVCODEC_ID_FIRST_CUSTOMDATA = 0xFFFF0000,
    SAVCODEC_ID_CUSTOMDATA1 =  SAVCODEC_ID_FIRST_CUSTOMDATA,
}SAVCodec_ID;

/**Each control command has a parameter SAVCodec_ControlParam* */
typedef enum {
	SAVCODEC_CONTROLCMD_FLUSH = 1, /*Flush buffers.Should be called when seeking. parameter: SAVCodec_ControlParam*/
	SAVCODEC_CONTROLCMD_SETQUALITY, /*Set encoder quality, Only available for SAVCODEC_ID_MJPEG now. parameter: SAVCodec_ControlParam_SetQuality*/
}SAVCodec_ControlCmd;

typedef struct _SAVCodec* SAVCodecHandle;

typedef struct _SAVCodec_Context{
	/*struct size.must be initialized to sizeof(SAVCodec_Context)*/
	SAV_TYPE_INT32 iSize; 
	/**
	* internal data used by library. User must NOT change it.
	* - mux/demux/encode/decode:set by library
	*/	
	SAVCodecHandle pCodec;
	/**
	*codec id. see SAVCodec_CODEC_ID_*
	* - mux/encode/decode:set by user.
    * - demux:set by library.
	*/
	SAVCodec_ID eCodecID;
	/**
	*media type. see SAV_MediaType
	* - mux/encode:set by user.
    * - demux/decode:set by library.
	*/
	SAV_MediaType eMediaType;
	
	union{
	SAV_VideoParam stVideoParam;/*video prarmeters*/
	SAV_AudioParam stAudioParam;/*audio prarmeters*/
	};
	/**
     * This is the fundamental unit of time (in seconds) in terms
     * of which frame timestamps are represented. For fixed-fps content,
     * timebase should be 1/framerate and timestamp increments should be
     * identically 1.For variable-fps content, timebase is invalid.
     * - mux/encode: MUST be set by user.
     * - demux/decode:set by library.
     */    
	SAV_Rational   TimeBase;
	
	/**
	* Encode or decode.SAVCodec_BOOL_TRUE: encode; SAVCodec_BOOL_FALSE: decode 
	* -encode/decode; set by user.
	*/
	SAV_TYPE_BOOL bEncode;
	
 	/**
     * the average bitrate.0 if bitrate info is unavailable
     * - mux/encode: set by user
     * - decode: set by library or user depending on codec. for G.726, set by user.
     * - demux: set by library.
     */
	SAV_TYPE_INT32 iBitRate;
	
	
	SAV_TYPE_INT32 iFlags; //reserved now.must be 0
	SAV_TYPE_INT32 iFlags2; //reserved now. must be 0
	
	/**
	 * some codecs need / can use extradata. Set to NULL/0 if not used
     * The allocated memory size should be at least iExtraDataSize + SAV_INPUT_BUFFER_PADDING_SIZE bytes.
	 * - demux/encode: allocated/set/freed by library.
     * - mux/decode: allocated/set/freed by user
     */
	SAV_TYPE_UINT8* pExtraData;
	SAV_TYPE_INT32  iExtraDataSize;
	
	/**
	 * A dictionary filled with SAVCodec_Context and codec-private options.
     * User may call SAVDict_Free after SAVCodec_Open to free memory. If user doesn't free the dictionary,
     * SAVCodec_Close() will call SAVDict_Free to free it.
     * pOptions should be initialized to NULL before call SAVDict_Set.
	 * - encode/decode: set by user.
	 * - mux/demux: not used.
	 */
	SAVDictionary* pOptions;
}SAVCodec_Context;

/*parameters for SAVCodec_Control*/
typedef struct _SAVCodec_ControlParam{
	/*struct size.*/
	SAV_TYPE_INT32 iSize;	
}SAVCodec_ControlParam;

typedef struct _SAVCodec_ControlParam_SetQuality{
	SAVCodec_ControlParam cp;/*cp.iSize.must be initialized to sizeof(SAVCodec_ControlParam_SetQuality)*/
	SAV_TYPE_INT32 iQuality;/*1~100*/
}SAVCodec_ControlParam_SetQuality;
/**
 * Open codec.
 * 
 * @param[in,out] pContext: the codec context
 * @return: see SAV_Result.
 */
LIBSAV_API SAV_Result SAVCodec_Open(SAVCodec_Context* pContext);


/**
 * Encode an audio/video frame from SAVCodec_Frame into SAVCodec_Packet, or
 * decode an audio/video packet from SAVCodec_Packet into SAVCodec_Frame.
 *
 * @param[in,out] pContext: the codec context. Some field in pContext may be changed during decoding. See comments of pFrame/pPacket
 * @param[in,out] pFrame: 
 * - encode: input of raw multimedia data.
 *   --encode audio: input data is in pFrame->ppData[0], data length is in pFrame->iDataSize[0],
 *            data length should >= pContext->stAudioParam.iFrameSize * pContext->stAudioParam.iChannelCount*bytes_per_single_channel_sample.
 *			  Each call of SAVCodec_Process() consumes pContext->stAudioParam.iFrameSize samples.
 *     NOTE: pFrame->ppData[0] MUST be 32 bytes aligned since some codecs are require special memory aligment.
 *   --encode video:input data is in pFrame->ppData[*], video stride is in pFrame->iDataSize[*].The expected encoded picture type
 *        is set in pFrame->ePictType(If don't want to specify picture type, set to SAVCodec_PICTURE_TYPE_NONE).If expect key frame,
 *        set pFrame->iFlags with SAVCodec_PKT_FLAG_KEY.
 * - decode: output of raw multimedia data.
 *   --decode audio: user should supply pFrame->ppData[0]. pFrame->iDataSize[0] is set to the length of pFrame->ppData[0]. Decoded
 *        audio data is outputed to pFrame->ppData[0].
 *     NOTE: pFrame->ppData[0] MUST be 32 bytes aligned for some codecs require special memory aligment
 *			iSampleRate/iChannelCount/eSampleFormat of pContext->stAudioParam is set by library.
 *   --decode video:  pFrame->ppData[*]/pFrame->iDataSize[*] is set by library, user can read the picture raw data from  pFrame->ppData[*].
 *          pFrame->ePictType/pFrame->iFlags/pContext->stVideoParam.iWidth/pContext->stVideoParam.iHeight/pContxt->stVideoParam.ePixFmt is set by library.
 * @param[in,out] pPacket: pPacket->pData should be at least 4 bytes aligned,32 bytes alignment is prefered. See SAVCodec_INPUT_BUFFER_PADDING_SIZE
 *    for more about pPacket->iDataSize.
 * - encode: output of encoded multimedia data.
 *   --encode audio: user should supply pPacket->pData. pPacket->iDataSize is set to the length of pPacket->pData.
 *        Encoded  data is outputed to  pPacket->pData.
 *   --encode video:user should supply pPacket->pData. pPacket->iDataSize is set to the length of pPacket->pData.
 *        Encoded  data is outputed to  pPacket->pData.
 * - decode: input of encoded multimedia data.
 *   --decode audio:user should supply pPacket->pData. pPacket->iDataSize is set to the length of pPacket->pData.
 *   --decode video:user should supply pPacket->pData. pPacket->iDataSize is set to the length of pPacket->pData.
 * @return :  < 0 if failed, >= 0 on sucesss.
 *      - encode: the length of encoded data in pPacket->pData
 *      - decode: the consumed data length from pPacket->pData. 
 *        (video: on success, user should check pFrame->ePictType.if pFrame->ePictType == SAV_PICTURE_TYPE_NONE, no frame is available)
, no frame is available)
 */
LIBSAV_API SAV_TYPE_INT32 SAVCodec_Process(SAVCodec_Context* pContext, SAV_Frame* pFrame, SAV_Packet* pPacket);

/**
 * send control command to codec.
 * 
 * @param[in] pContext: the codec context 
 * @param[in] cmd: the command
 * @param[in] pParam: the parameter for the command
 * @return: see SAV_Result.
 */
LIBSAV_API SAV_Result SAVCodec_Control(SAVCodec_Context* pContext, SAVCodec_ControlCmd cmd, SAVCodec_ControlParam* pParam);

/**
 * Close codec.
 * 
 * @param[in,out] pContext: the codec context 
 * @return: see SAV_Result.
 */
LIBSAV_API SAV_Result SAVCodec_Close(SAVCodec_Context* pContext);

#ifdef  __cplusplus
}
#endif
#endif
