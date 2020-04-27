/*====================================================
 * CBVDisplay.cpp
 *
 *  Created on: 2012-1-4
 *      Author: bv-corgan
 =====================================================*/

#include "Windows.h"
#include "BVCU.h"
#include "../Public.h"
#include "CBVDisplay.h"

#pragma comment(lib, "bvdisplay.lib")

DisplayFont CBVDisplay::m_displayFont = NULL;

void CBVDisplay::setDisplayFontFunc(DisplayFont displayFont)
{
    m_displayFont = displayFont;
}

BVCU_Result CBVDisplay::afterRender(BVCU_HDialog hDialog, SAVCodec_Context* pCodec,SAV_Frame* pFrame)
{
    if (NULL != pFrame)
    {
        m_displayFont(hDialog, pFrame->iPTS);
    }
    return BVCU_RESULT_S_OK;
}
