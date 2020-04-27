/*====================================================
 * CBVDisplay.h
 *  本类负责整个显示区域的绘制操作，如：颜色调整，绘制
 *  视频分割边框等。
 *  Created on: 2012-1-4
 *      Author: bv-corgan
 =====================================================*/

#ifndef __CBVDISPLAY_H___
#define __CBVDISPLAY_H___

#define LIB_BVDISPLAY_EXPORTS

extern "C"
{
#include "BVDisplay.h"
}
#include "Windows.h"

#define DISPLAY_RECT_FRAME_WIDTH 3
#define DISPLAY_RECT_FRAME_ALPHA 255
#define INVALID_INDEX -1

typedef void (WINAPI *DisplayFont)(BVCU_HDialog hDialog, SAV_TYPE_INT64 timeStamp);

class CBVDisplay
{
    static DisplayFont m_displayFont;

public:
    static void setDisplayFontFunc(DisplayFont displayFont);
    static BVCU_Result afterRender(BVCU_HDialog hDialog, SAVCodec_Context* pCodec, SAV_Frame* pFrame);
};

#endif /* End of __CBVDISPLAY_H___ */
