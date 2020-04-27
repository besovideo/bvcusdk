#ifndef __LIBSAVUTIL_H__
#define __LIBSAVUTIL_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include "SAVCommon.h"
    typedef struct {
        SAV_TYPE_INT8 *pKey;   /*UTF-8 coded*/
        SAV_TYPE_INT8 *pValue; /*UTF-8 coded*/
    }SAVDict_Entry;

    typedef enum {
        SAVDICT_FLAG_MATCH_CASE = (1 << 0), /*match case*/
        SAVDICT_FLAG_IGNORE_SUFFIX = (1 << 1)  /*compare only the first strlen(pKey) characters*/
    }SAVDict_Flag;
    typedef struct SAVDictionary SAVDictionary;

    /**
     * Get a dictionary entry with matching key.
     *
     * @param[in] pKey: the required key name.
     * @param[in] eFlags: Options to compare key.
     * @return: Found entry or NULL, changing key or value leads to undefined behavior.
     */
    LIBSAV_API SAV_TYPE_INT8* SAVDict_Get(SAVDictionary* pDict, const SAV_TYPE_INT8* pKey, SAVDict_Flag eFlags);
    /**
     * Enumerate a dictionary entry.
     *
     * @param[in] pEntryPrev: Set to the previous matching element to find the next.
     *             If set to NULL the first matching element is returned.
     * @return: Found entry or NULL(which means end), changing key or value leads to undefined behavior.
     */
    LIBSAV_API SAVDict_Entry* SAVDict_Enum(SAVDictionary* pDict, SAVDict_Entry* pEntryPrev);

    /**
     * Set the given entry in *ppDict, overwriting an existing entry.
     *
     * @param[in,out] ppDict: pointer to a pointer to a dictionary struct. If *ppDict is NULL,
     * a dictionary struct is allocated and put in *ppDict.
     * @param[in] pEntry: pEntry to add to *ppDict (pKey/pValue will be duplicated by library)
     *        Passing a NULL pValue will cause an existing tag to be deleted.
     *        If pEntry->pKey exists in ppDict, it will be overwritten.
     * @param[in] eFlags: Options to compare key.
     * @return: >= 0 on success otherwise an error code <0
     */
    LIBSAV_API SAV_Result SAVDict_Set(SAVDictionary** ppDict, SAVDict_Entry* pEntry, SAVDict_Flag eFlags);

    /**
     * Free all the memory allocated for an AVDictionary struct
     * and all keys and values.
     * @param[in] ppDict: pointer to a pointer to a dictionary struct.
     */
    LIBSAV_API void SAVDict_Free(SAVDictionary** ppDict);

    /**
     * return the size of an image.
     * @param[in] pParam: video param. ePixelFmt/iWidth/iHeight must be set
     * @param[in] pFrame: video frame. If NULL, calculate line stride using pParam.iWidth, else use pFrame->iDataSize as line stride.
     * return: image size in bytes, < 0 on error
     */
    LIBSAV_API SAV_TYPE_INT32 SAVImg_GetSize(SAV_VideoParam* pParam, SAV_Frame* pFrame);

    /**
     * set the ppData/iDataSize for SAV_Frame by SAV_VideoParam. Other field of SAV_Frame is not touched.
     * @param[in] pParam: video param. ePixelFmt/iWidth/iHeight must be set
     * @param[in] pFrame: video frame.
     * @param[in] pBuf: buffer to be used for pFrame data. pBuf should be allocated by user and size should be at least SAVImg_GetSize().
     * return: >=0 on success. < 0 on fail.
     * Example: to allocate a frame.
     * 	SAV_VideoParam vp;
     * 	SAV_Frame frame;
     * 	frame.iSize = sizeof(frame);
     * 	vp.iSize = sizeof(vp);
     * 	vp.iWidth = 352; vp.iHeight = 288; vp.ePixelFmt = SAV_PIX_FMT_YUV420P;
     * 	int iSize = SAVImg_GetSize(&vp,NULL);
     * 	void* pBuf = malloc(iSize);
     * 	SAVImg_SetFramePointer(&vp,&frame,pBuf);
     */
    LIBSAV_API SAV_TYPE_INT32 SAVImg_SetFramePointer(SAV_VideoParam* pParam, SAV_Frame* pFrame, void* pBuf);

    /**
     * copy an image from pFrameSrc to pFrameDst
     * @param[in] pParam: video param. ePixelFmt/iWidth/iHeight must be set
     * @param[in] pFrameSrc: source frame.pFrame->ppData/iDataSize must be set.
     * @param[out] pFrameDst: destination frame.ppData/iDataSize must be set if pBufDst == NULL.
     * @param[in] pBufDst: if pFrameDst->ppData/iDataSize is NULL/0 and pBufDst != NULL, library will
     * set pFrameDst->ppData/iDataSize based on pBufDst/pFrameSrc.
     * If pFrameDst->ppData[*] != NULL, pBufDst is ignored.
     * If pBufDst == NULL && (pFrameDst->ppData[*] == NULL || pFrameDst->iDataSize[*] != pFrameSrc->iDataSize[*]), return with failure.
     * return: >=0 on success. < 0 on fail.
     * Example:
     *  SAV_VideoParam vp;
     * 	SAV_Frame frameDst;
     * 	frame.iSize = sizeof(frame);
     * 	vp.iSize = sizeof(vp);
     * 	vp.iWidth = 352; vp.iHeight = 288; vp.ePixelFmt = SAV_PIX_FMT_YUV420P;
     * //init pFrameSrc
     * 	int iSize = SAVImg_GetSize(&vp,pFrameSrc);
     * 	void* pBuf = malloc(iSize);
     *  SAVImg_Copy(&vp,pFrameSrc,&frameDst,pBuf);
     */
    LIBSAV_API SAV_Result SAVImg_Copy(SAV_VideoParam* pParam, SAV_Frame* pFrameSrc, SAV_Frame* pFrameDst, void* pBufDst);

    /**
     * callback function called by SAVImg_Save
     * @param[in] pUserData: any user defined data. for example, FILE* for stream I/O.
     * @param[in] pData: pointer to one image line
     * @param[in] iLength: length of the imge line in byte
     * return:  >=0 on success. < 0 on fail.
     */
    typedef SAV_Result(*fnSAVImg_Save)(void* pUserData, void* pData, SAV_TYPE_INT32 iLength);

    /**
     *save an imag in pFrame by calling fCallback
     * @param[in] pParam: video param. ePixelFmt/iWidth/iHeight must be set
     * @param[in] pFrame: image frame. pFrame->ppData/iDataSize must be set.
     * @param[in] fCallback: callback function. if fCallback return with failure,SAVImg_Save will abort and return with failure.
     * @param[in] pUserData: the first parameter of fCallback
     * return: >=0 on success. < 0 on fail.
     */
    LIBSAV_API SAV_Result SAVImg_Save(SAV_VideoParam* pParam, SAV_Frame* pFrame, fnSAVImg_Save fCallback, void* pUserData);
    /**
     * SAVImg_Convert_* : Colorspace converter.
     */
    typedef struct _SAVImg_Convert_Context {
        /*struct size.must be initialized to sizeof(SAVImg_Convert_Context) by user*/
        int iSize;

        /**
        * internal data used by library. User must NOT change it after opened.
        */
        void* pObj;

        void* pObjTemp;
        void* pObjTempBuffer;

        /* source Pixel format. set by user*/
        SAV_PixelFormat ePixelFmtSrc;

        /* destination Pixel format. set by user*/
        SAV_PixelFormat ePixelFmtDst;

        /* image width. set by user*/
        SAV_TYPE_INT32 iWidth;

        /* image height. set by user*/
        SAV_TYPE_INT32 iHeight;
    }SAVImg_Convert_Context;

    /**
     * Open converter.
     *
     * @param[in,out] pContext: the context
     * @return: see SAV_Result.
     */
    LIBSAV_API SAV_Result SAVImg_Convert_Open(SAVImg_Convert_Context* pContext);

    /**
     * do color convert.
     * To get better performance on x86, src/dst Frame ppData[*] should be 16 bytes aligned and iDataSize[*] should be divided by 16
     * @param[in]: source frame.
     * @param[out]: destionation frame. user should set ppData[*]/iDataSize[*] of pFrameDst before call.
     * @return: see SAV_Result.
     */
    LIBSAV_API SAV_Result SAVImg_Convert_Process(SAVImg_Convert_Context* pContext, SAV_Frame* pFrameSrc, SAV_Frame* pFrameDst);

    /**
     * Close converter.
     *
     * @param[in,out] pContext: the context
     * @return: see SAV_Result.
     */
    LIBSAV_API SAV_Result SAVImg_Convert_Close(SAVImg_Convert_Context* pContext);


    /**
     * extradata splite and parser
     */
    typedef struct _SAV_ExtraData {
        int iSize;
        SAV_Bit_Context vps;
        SAV_Bit_Context sps;
        SAV_Bit_Context pps;
        union {
            SAV_VideoParam stVideoParam;
            SAV_AudioParam stAudioParam;
        };
    } SAV_ExtraData;

    /**
     * @param[in]    codecId: see SAVCodec_ID
     * @param[in] pExtraData: h264¡¢h265¡¢aac extradata
     * @param[out]  pContext: the context
     * @return: see SAV_Result
     */
    LIBSAV_API SAV_Result SAV_ExtraData_Parser(SAV_TYPE_INT32 codecId, SAV_Bit_Context* pExtraData, SAV_ExtraData* pContext);

    /** Set the logging callback
     * @param[in] callback: log callback
     */
    LIBSAV_API SAV_Result SAV_Log_CallBack(void(*callback)(int level, const char* funname, int line, const char* fmt, va_list vl));

    LIBSAV_API void SAV_jpg_cryption(char *frame, int frameLength, char isEncryption);
#ifdef  __cplusplus
}
#endif
#endif
