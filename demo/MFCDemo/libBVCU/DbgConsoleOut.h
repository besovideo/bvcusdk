/*====================================================
 * DbgConsoleOut.h
 *
 *  Created on: 2012-2-1
 *      Author: bv-corgan
  =====================================================*/

#ifndef __DBGCONSOLEOUT_H___
#define __DBGCONSOLEOUT_H___

#include <windows.h>

class CDbgConsoleOut
{
public:

#ifdef _DEBUG
static void RedirectIOToConsole();
#endif

//static void debugOut(const char *format, ...);
static void debugOut(const TCHAR *format, ...);
};


#endif /* End of __DBGCONSOLEOUT_H___ */
