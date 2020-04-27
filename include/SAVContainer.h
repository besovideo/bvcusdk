#ifndef __LIBSAVCONTAINER_H__
#define __LIBSAVCONTAINER_H__
#ifdef  __cplusplus
extern "C" {
#endif
#include "SAVCommon.h"
#include "SAVUtil.h"
#include "SAVCodec.h"

#define SAV_STREAM_INDEX_DEFAULT -1
typedef struct _SAVContainer* SAVContainerHandle;

typedef struct _SAVContainer_Context{
	/**
	*set by user.struct size.must be initialized to sizeof(SAVContainer_Context)
	*/
	SAV_TYPE_INT32 iSize;

	/**
	* internal data used by library. User must NOT change it.
	* - mux/demux: Set by library
	*/	
	SAVContainerHandle pContainer;

	/**
	* Mux or demux.SAVCodec_BOOL_TRUE: mux; SAVCodec_BOOL_FALSE: demux 
	* -mux/demux; set by user.
	*/
	SAV_TYPE_BOOL  bMux;

	/**
	* the container format name.
	* If user does not set container name, library will autodetect the name by filename extension(for encode) and file head(for decode).
	*  On SAVContainer_Open return, sContainerName will be set to the detected container name
	* If user set container name , force the library to use the set container.
	* Known container name: 
	*   "matroska" -- .mkv file
	*   "yxdat"    -- besovideo private .dat file
	*	"zlav"	   -- besovideo private .dav file
	*/
	SAV_TYPE_INT8  sContainerName[128];

	/**
	* File name for mux/demux. in UTF-8
	* -mux/demux:set by user.
	*/
	SAV_TYPE_INT8* sFileName;

	/**
	* Available streams count.
	* -mux: set by user;
	* -demux: set by library.
	*/
	SAV_TYPE_INT32 iStreamCount;

	/**
	* Available streams information.
	* -mux:  allocated/set/freed by user.
	* -demux: allocated/set/freed by library.
	*/
	SAVCodec_Context** ppStreams;

	/**
	* The seek index count for streams.
	* -mux: not used.should be NULL
	* -demux: allocated/set/freed by library. It maybe 0 after SAVContainer_Open since the delay retrieval of seek index for some files.
	* In this case, a seek operation by SAVContainer_Control will read the index. The seek index count may vary after seek/read operation.
	* Some files/streams may have 0 seek index.
	*/
	SAV_TYPE_INT32* piStreamSeekIndexCount;

	/**
	* File creation time(the first frame time in the file).
	* In 1/SAV_TIME_BASE second since the Epoch, 1970-01-01 00:00:00 +0000 (UTC)
	* May not available for some container format.
	* -mux: set by user.
	* -demux: set by library. SAV_NOPTS_VALUE if not available.
	*/
	SAV_TYPE_INT64 iCreationTime;
	
	/**
	* File duration in 1/SAV_TIME_BASE second
	* May not available for some container format.
	* -mux/demux: set by library. SAV_NOPTS_VALUE if not available.
	*/
	SAV_TYPE_INT64 iDuration;
	
	/**
	* File length in bytes
	* May not available for some container format.
	* -mux: not used.
	* -demux: set by library. -1 if not available.
	*/
	SAV_TYPE_INT64 iFileSize;
	
	/**
	* Not used now. Must be set to 0
	*/
	SAV_TYPE_INT32 iFlags;
	
	/**
	* A dictionary filled with additional information such as title,author etc.
	* pMetadata should be initialized to NULL before call SAVDict_Set.
	* - mux: set by user before SAVContainer_Open.User may call SAVDict_Free after SAVContainer_Open to free memory. 
	*   If user doesn't free the dictionary, SAVContainer_Close() will call SAVDict_Free to free it.
	* - demux: set by library. User should not call SAVDict_Free/SAVDict_Set.
	*/
	SAVDictionary* pMetaData;

}SAVContainer_Context;

typedef enum {
	SAVCONTAINER_CONTROLCMD_SEEK = 1 /*Seek in file for demux.*/
}SAVContainer_ControlCmd;

enum {
	SAVCONTAINER_SEEK_FLAG_BACKWARD = (1<<0), /*seek backward*/
	SAVCONTAINER_SEEK_FLAG_BYTE     = (1<<1), /*seeking based on position in bytes.  Not used now.*/
	SAVCONTAINER_SEEK_FLAG_ANY      = (1<<2), /*seek to any frame, even non-keyframes. Not used now.*/
	SAVCONTAINER_SEEK_FLAG_FRAME    = (1<<3), /*seeking based on frame number*/
};

/*common parameters for SAVContainer_Control*/
typedef struct _SAVContainer_ControlParam{
	/*struct size.*/
	SAV_TYPE_INT32 iSize;	
}SAVContainer_ControlParam;

/*options of seek for demuxing*/
typedef struct _SAVContainer_SeekOpt{
	SAVContainer_ControlParam cp;/*struct size of cp must be initialized to sizeof(SAVContainer_SeekOpt)*/

	/**
	*Stream index to seek. If set to SAV_STREAM_INDEX_DEFAULT, a default stream is selected.
	*/
	SAV_TYPE_INT32 iStreamIndex;

	/**
	* Seek to timestamp iTargetTs.
	* Seeking will be done so that the point from which the stream
	* can be presented successfully will be closest to iTargetTs and within iMinTs/iMaxTs.*
	* If iFlag contain SAVCONTAINER_SEEK_FLAG_FRAME, then all timestamps are in frames
	* in the stream with iStreamIndex (this may not be supported by all demuxers).
	* Otherwise all timestamps are in 1/SAV_TIME_BASE units.
	*/
	SAV_TYPE_INT64 iMinTs;
	SAV_TYPE_INT64 iMaxTs;
	SAV_TYPE_INT64 iTargetTs;
	SAV_TYPE_INT32 iFlags;//see SAVCONTAINER_SEEK_FLAG_*
}SAVContainer_SeekOpt;

/**
 * Open container.
 * 
 * @param[in,out] pContext: the container context.
 * @return: see SAV_Result.
 */
LIBSAV_API SAV_Result SAVContainer_Open(SAVContainer_Context* pContext);

/**
 * write pPacket into file, or read from file into pPacket
 *
 * @param[in] pContext: the container context.
 * @param[in,out] pPacket: 
 * - mux[in]: data to write.iPTS/iDuration/pData/iDataSize/iStreamIndex/iFlags of SAV_Packet must be set.
 *   pPacket->pData is allocated/set/freed by user. pPacket->iDataSize is set to the length of pPacket->pData.
 * - demux[out]: data read in.
 *   iPTS/iDuration/pData/iDataSize/iStreamIndex/iFlags of SAV_Packet is set by library. pPacket->pData is allocated/set/freed 
 *   by library and is valid  until the next SAVContainer_Process() or until SAVContainer_Close().
 * @return :  See SAV_Result
 */
LIBSAV_API SAV_Result SAVContainer_Process(SAVContainer_Context* pContext, SAV_Packet* pPacket);

/**
 * send control command to container.
 * 
 * @param[in] pContext: the container context 
 * @param[in] cmd: the command
 * @param[in] pParam: the parameter for the command
 * @return: see SAV_Result.
 */
LIBSAV_API SAV_Result SAVContainer_Control(SAVContainer_Context* pContext, SAVContainer_ControlCmd cmd, SAVContainer_ControlParam* pParam);

/**
 * Close container.
 * 
 * @param[in,out] pContext: the container context 
 * @return: see SAV_Result.
 */
LIBSAV_API SAV_Result SAVContainer_Close(SAVContainer_Context* pContext);

#ifdef  __cplusplus
}
#endif
#endif
