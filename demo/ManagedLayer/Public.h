/*====================================================
 * Public.h
 *
 *  Created on: 2012-2-1
 *      Author: bv-corgan
  =====================================================*/

#include "DbgConsoleOut.h"

#define BVCU_WCHAR wchar_t
#define BVCU_IP_STRING_LEN 16
#define WINDOWS_MAX_FILE_PATH_LEN MAX_PATH
#define UNKNOW_DIALOG_EVENT_CODE -1
#define FALSE 0
#define TRUE 1

#define BV_DBG_CONSOLE_OUT(format, ...) {\
    CDbgConsoleOut::debugOut("[%s : %d]\t\n",__FILE__,__LINE__); \
    CDbgConsoleOut::debugOut((format), ##__VA_ARGS__); \
}


