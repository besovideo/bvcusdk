#ifndef __LIBSAV_AUDIO_MIXER_H__
#define __LIBSAV_AUDIO_MIXER_H__


#ifdef  __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif


EXTERN_C_BEGIN

#include "SAVCommon.h"
#include "SAVUtil.h"

typedef struct _SAVAudioMix_Context {
    /*struct size.must be initialized to sizeof(SAVCodec_Context)*/
    SAV_TYPE_INT32 iSize;

    /**
    * 混音数
    */
    SAV_TYPE_INT32 iAudioInputCount;
    /**
    * 混音输入参数
    */
    SAV_AudioParam* stAudioInputs;

    /**
    * 混音输出参数
    */
    SAV_AudioParam stAudioOutput;

    /**
    * internal data used by library. User must NOT change it.
    */
    void* privateData;

} SAVAudioMix_Context;

/**
 * SAVAudioMix_Open 混音
 *
 * @param[in,out] pContext: 
 * @return: see SAV_Result.
 */
LIBSAV_API SAV_Result SAVAudioMix_Open(SAVAudioMix_Context* pContext);

/**
 * SAVAudioMix_AddFrame
 *
 * @param[in] pContext: 
 * @param[in] inputIndex: SAVAudioMix_Context->stAudioInputs[inputIndex] 音频输入索引
 * @param[in] frame: 音频输入数据
 * @return: see SAV_Result.
 * 
 * SAV_Frame frame = { 0 };
 *   frame.ppData[0] = pcm_ram_data;
 *   frame.iDataSize[0] = pcm_ram_data_len;
 */
LIBSAV_API SAV_Result SAVAudioMix_AddFrame(SAVAudioMix_Context* pContext, SAV_TYPE_INT32 inputIndex, SAV_Frame* frame);

/**
 * SAVAudioMix_GetFrame
 *
 * @param[in] pContext: 
 * @param[in, out] frame: 获取混音数据
 * @return: > 0 混音数据大小; = 0 缓冲区小
 * 
 * SAV_Frame frame = { 0 };
 *   frame.ppData[0] = out_buffer;
 *   frame.iDataSize[0] = 10240;
 */
LIBSAV_API SAV_TYPE_INT32 SAVAudioMix_GetFrame(SAVAudioMix_Context* pContext, SAV_Frame* frame);

/**
 * SAVAudioMix_Close
 *
 * @param[in] pContext:
 */
LIBSAV_API SAV_Result SAVAudioMix_Close(SAVAudioMix_Context* pContext);

EXTERN_C_END

#endif
