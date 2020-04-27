/**=====================================================
    Display library for preview and picture show.
    Copyright(c) BesoVideo Ltd. 2011
    jijun.he@besovideo.com
========================================================*/

#ifndef __LIBBVDISPLAY_H__
#define __LIBBVDISPLAY_H__


#ifdef _MSC_VER
    #ifdef LIB_BVDISPLAY_EXPORTS
        #define LIB_BVDISPLAY_API __declspec(dllexport)
    #else
        #define LIB_BVDISPLAY_API __declspec(dllimport)
    #endif
#else
    #define LIB_BVDISPLAY_API 
#endif//_MSC_VER

#define BV_LOG_DBG_MSG

#ifndef NULL
    #define NULL 0
#endif

#ifdef __ANDROID__
    #include <jni.h>
    #include <GLES/gl.h>
    #include <EGL/egl.h>
    #include <android/native_window_jni.h>
    typedef struct ANativeWindow*           BVDisplay_HANDLE;
#else
    /**
     * ���ָ��, 32λϵͳΪInt32, 64λϵͳΪInt64
     */
    typedef unsigned int BVDisplay_HANDLE;
#endif

typedef int BVDisplay_INT32;

typedef int BVDisplay_ERRCode;      // Error code, the same definition with HRESULT in MS windows

typedef unsigned char BVDisplay_BYTE;

typedef BVDisplay_HANDLE BVDisplay_HWND;

typedef wchar_t BVDisplay_WCHAR;     // 16-bit UNICODE character

typedef struct _BVDisplay_RECT
{
    BVDisplay_INT32 iTop;
    BVDisplay_INT32 iLeft;
    BVDisplay_INT32 iBottom;
    BVDisplay_INT32 iRight;
}BVDisplay_RECT;

typedef enum
{
    BVDISPLAY_COLOR_FMT_RGB = 0,
    BVDISPLAY_COLOR_FMT_YUV = 1,
    BVDISPLAY_COLOR_FMT_INVALID = 0xffffffff // ǿ��ö��Ϊ4�ֽ�
}BVDisplay_ColorFmt;

typedef struct _BVDisplay_Color
{
    BVDisplay_ColorFmt eColorFormat;
    union
    {
        struct
        {
            BVDisplay_BYTE bRed;
            BVDisplay_BYTE bGreen;
            BVDisplay_BYTE bBlue;
        };
        struct
        {
            BVDisplay_BYTE bY;
            BVDisplay_BYTE bU;
            BVDisplay_BYTE bV;
        };
    };
    BVDisplay_BYTE bAlpha;
}BVDisplay_Color;

typedef enum {
    BVDISPLAY_TEXT_EFFECT_NOEFFECT = 0,         // ��Ч��, ֱ�ӻ�������
    BVDISPLAY_TEXT_EFFECT_OUTLINE = 1,          // ����������ɫ����
    BVDISPLAY_TEXT_EFFECT_SHADOW = 2,           // ������Ӱ
    BVDISPLAY_TEXT_EFFECT_BACKGROUND = 3,       // ���屳��ɫ
    BVDISPLAY_TEXT_EFFECT_INVALID = 0xffffffff // ǿ��ö��Ϊ4�ֽ�
}BVDisplay_TextEffect;

/**
 * ������Ƶ��ʾ����, ÿ���ṹ�洢һ��YV12֡
 */
#define BV_DISPLAY_FRAME_COMP 4
typedef enum {
    BV_DISPLAY_PIX_FMT_NONE = -1,
    BV_DISPLAY_PIX_FMT_YUV420P,   ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
    BV_DISPLAY_PIX_FMT_YUYV422,   ///< packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
    BV_DISPLAY_PIX_FMT_RGB24,     ///< packed RGB 8:8:8, 24bpp, RGBRGB...
    BV_DISPLAY_PIX_FMT_BGR24,     ///< packed RGB 8:8:8, 24bpp, BGRBGR...
    BV_DISPLAY_PIX_FMT_YUV422P,   ///< planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)     
}BVDisplay_PixelFormat;

typedef struct _BVDisplay_Frame
{
    BVDisplay_INT32 iSize;
    BVDisplay_PixelFormat eFormat;                  // ��Ƶ֡�ĸ�ʽ
    BVDisplay_INT32 iFrameWidth;                    // ��Ƶ֡�����ݿ��
    BVDisplay_INT32 iFrameHeight;                   // ��Ƶ֡�����ݸ߶�
    BVDisplay_INT32 aiStrike[BV_DISPLAY_FRAME_COMP]; // ���ݵ�ʵ�ʿ��
    BVDisplay_BYTE* abData[BV_DISPLAY_FRAME_COMP];  // ��Ƶ֡������
}BVDisplay_Frame;

typedef enum {
    BV_DISPLAY_TEXT_ALIGN_LEFT_TOP = 0,
    BV_DISPLAY_TEXT_ALIGN_LEFT_CENTER = 1,
    BV_DISPLAY_TEXT_ALIGN_LEFT_BOTTOM = 2,
    BV_DISPLAY_TEXT_ALIGN_MIDDLE_TOP = 3,
    BV_DISPLAY_TEXT_ALIGN_MIDDLE_CENTER = 4,
    BV_DISPLAY_TEXT_ALIGN_MIDDLE_BOTTOM = 5,
    BV_DISPLAY_TEXT_ALIGN_RIGHT_TOP = 6,
    BV_DISPLAY_TEXT_ALIGN_RIGHT_CENTER = 7,
    BV_DISPLAY_TEXT_ALIGN_RIGHT_BOTTOM = 8,
}BVDisplay_TextAlignType;

/**
 * ������ʾ��Ч������
 */
typedef struct _BVDisplay_FontDisplayMode
{
    BVDisplay_INT32 iSize;
    BVDisplay_TextAlignType eAlignType;
    BVDisplay_INT32 iRate;      // ��Ļ�����ٶ�, ��λ:(pixel/per display interval)
                                // ���Ϊ0, ���������Ļ����Ч��
                                
    BVDisplay_TextEffect eTextEffect;
    
    BVDisplay_Color sFontColor;         // ������ɫ, ����alphaֵ
    
    BVDisplay_Color sBackColor;         // backgroundЧ��ʱΪ����ɫ, 
                                        // shadowЧ��ʱΪ��Ӱɫ, 
                                        // outlineЧ��ʱΪ������ɫ
                                        // ����alphaֵ

    BVDisplay_INT32 iEffectSize;        // ����Ч���ߴ�
                                        // backgroundЧ��ʱΪ��Ч
                                        // shadowЧ��ʱΪ��Ӱ������
                                        // outlineЧ��ʱΪ����������

}BVDisplay_FontDisplayMode;
extern "C" {
/**=====================================================
 ��ʼ����
  ====================================================*/
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_Open();

/**=====================================================
 �豸��ʼ���ӿ�
  ====================================================*/
/**
 * ��ʼ����ʾ�豸, Ϊ�û���ʼ����ָ�������������ʾ���.
 * @param[in] hWnd: �û����ṩ�Ļ�ͼ������;
 * @param[in] width: ��ͼ���������
 * @param[in] height: ��ͼ�������߶�
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DeviceInit(const BVDisplay_HWND hWnd, const int width, const int height);


/**=====================================================
 ����Աʵ�������ӿ�
  ====================================================*/
/**
 * ��device�Ŀͻ����ڴ�����Ƶ������
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @param[out] displayRectIndex: ��Ƶ������������;
 * @param[in] dispRect: ��device�ڵ���ʾ����;
 * @param[in] videoRect: video֡���ݴ�С, ��top, leftֵӦΪ0; bottom, rightֵ�ֱ�Ϊ������Ƶ���ĸߺͿ�;
 * @param[in] pixelFormat: ��Ƶ��ʽ, ��ǰ��֧��BV_DISPLAY_PIX_FMT_YUV420P
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_CreateDisplayRect(const BVDisplay_HWND hWnd, int *displayRectIndex, const BVDisplay_RECT *dispRect, const BVDisplay_RECT *videoRect, const BVDisplay_PixelFormat pixelFormat);

/**
 * �������������û�������Ϣ����ʾ.
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @param[out] fontIndex: ���������������;
 * @param[in] fontSize: �����С, ��λΪ���ֵ;
 * @param[in] fontName: ��������, ��: "Arial", "Times New Roman", "����"��;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_CreateFont(const BVDisplay_HWND hWnd, int *fontIndex, const int fontSize, const BVDisplay_WCHAR *fontName);

/**
 * �����û��Զ����ͼƬ�ļ�
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @param[out] customFigureIdx: ������ͼƬ������;
 * @param[in] filePath: �û�Ҫ��ʾ��ͼƬ, ֧�ָ�ʽ�μ�: http://msdn.microsoft.com/en-us/library/bb172878%28v=VS.85%29.aspx��
 * @param[in] keyColor: ͼƬ�е�͸��ɫ;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_CreateCustomFile(const BVDisplay_HWND hWnd, int *customFigureIdx, const BVDisplay_WCHAR* filePath, const BVDisplay_Color *keyColor);

/**
 * �����û��Զ����ͼƬ��Դ
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @param[out] customFigureIdx: ������ͼƬ������;
 * @param[in] resourceName: �û�Ҫ��ʾ��ͼƬ, ֧�ָ�ʽ�μ�: http://msdn.microsoft.com/en-us/library/bb172878%28v=VS.85%29.aspx��
 * @param[in] keyColor: ͼƬ�е�͸��ɫ;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_CreateCustomResource(const BVDisplay_HWND hWnd, int *customFigureIdx, BVDisplay_HANDLE hSrcModule, const BVDisplay_WCHAR* resourceName, const BVDisplay_Color *keyColor);


/**=====================================================
 ������ʾ�ӿ�
  ====================================================*/
/**
 * ��������Ϊָ����ɫ, ������Ļ��û����Ƶ���ŵ������������ɫ
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @param[in] color: ��Ҫ�ı�����ɫ;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_ClearBackground(const BVDisplay_HWND hWnd, const BVDisplay_Color* color);

/**
 * �����û��趨�����λ����device��clientRect��������ʾһ֡.
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @param[in] displayRectIndex: ��Ƶ�����������б�;
 * @param[in] frameRect: �ֲ������б�, ���ΪNULL, ����ʾdisplayRectIndex�������������, ������ʾ֡������ָ����һ���־�������
 * @param[in] frameData: Ҫ��ʾ��֡�б�, ������ﴫ�����Ƶ��С��CreateDisplayRect�����Ĳ�һ��, �򷵻ش���;
 * @param[in] frameCnt: �б��е�Ԫ�ظ���, ǰ���������Ԫ�ظ����������frameCnt
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DrawFrame(const BVDisplay_HWND hWnd, const int* displayRectIndex, const BVDisplay_RECT** frameRect, const BVDisplay_Frame **frameData, const int frameCnt);

/**
* �����û��趨�����λ����device��clientRect��������ʾһ֡.
* @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
* @param[in] displayRectIndex: ��Ƶ�����������б�;
* @param[in] frameRect: �ֲ������б�, ���ΪNULL, ����ʾdisplayRectIndex�������������, ������ʾ֡������ָ����һ���־�������
* @param[in] frameData: Ҫ��ʾ��֡�б�, ������ﴫ�����Ƶ��С��CreateDisplayRect�����Ĳ�һ��, �򷵻ش���;
* @param[in] frameCnt: �б��е�Ԫ�ظ���, ǰ���������Ԫ�ظ����������frameCnt
* @return: < 0 if failed, >= 0 on success.
*/
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DrawOneFrame(const BVDisplay_HWND hWnd, const int displayRectIndex, const BVDisplay_RECT* frameRect, const BVDisplay_Frame *frameData);

/**
 * ��������device�Ŀͻ��������еĻ�����ʾ����
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @return: < 0 if failed, >= 0 on success.
 * ������������InitDevice������ͬһ�̵߳��ã�������������豸��ʧ�쳣
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_PresentFrame(const BVDisplay_HWND hWnd);

/**
 * ��ָ����������ʾ����, ��������Ƶ�ϵ�λ����dispRect��top, left, bottom, right����ȷ��,  ��ɫ��colorȷ��.
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @param[in] fontIndex: ��������;
 * @param[in] dispRect: ������ʾ�ķ�Χ, ������Χ�����彫���ض�, �����ΧΪDeviceInitʱָ���ĸߺͿ�;
 * @param[in] color: �����ARGB��ɫ;
 * @param[in] text: Ҫ��ʾ����������;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DisplayFont(const BVDisplay_HWND hWnd, const int fontIndex, const BVDisplay_RECT *dispRect, const BVDisplay_Color *color, const BVDisplay_WCHAR* text);

/**
 * ��ʾ�û������ͼƬ�ļ�����Դ.
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @param[in] customFigureIdx: ͼƬ�ļ�����Դ������;
 * @param[in] dispRect: ��ͼλ��;
 * @param[in] alpha: ��ʾλͼ��͸����, 255Ϊ��͸��, 0Ϊ��ȫ͸�����ɼ�;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DisplayCustomFigure(const BVDisplay_HWND hWnd, const int customFigureIdx, const BVDisplay_RECT *dispRect, const int alpha);


/**=====================================================
 ��ʾ�������ýӿ�
  ====================================================*/
/**
 * ����������Ƶ��������С��λ��, ��������������֡���ݴ�С
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @param[in] displayRectIndex: ��Ƶ����������;
 * @param[in] dispRect: �µ���Ƶ������, ���ΪNULL, �򱣳�ԭ��ʾ���򲻱�;
 * @param[in] videoRect: �µ���Ƶ֡���ݴ�С, ���ΪNULL, �򱣳�ԭ��Ƶ֡���ݴ�С����;
 * @param[in] rotateAngle: ��Ƶ����ת�Ƕ�(��֧��90�ȵ�������, ��-90/0/270/720��), Ĭ�ϲ���ת
 * @param[in] displayMode: ��Ƶ��ʾģʽ(0: ���, 1:�ȱ�������)
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_ModifyDisplayRect(const BVDisplay_HWND hWnd, const int displayRectIndex, const BVDisplay_RECT *dispRect, const BVDisplay_RECT *videoRect, const int rotateAngle = 0, const int displayMode = 0);

/**
 * ������Ƶ���������, �Աȶ�, ���ͶȺ�ɫ��
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @param[in] displayRectIndex: ��Ƶ����������; (�ݲ�֧��)
 * @param[in] brightness: ����, ��Χ��-1��1, Ĭ��ֵΪ0;
 * @param[in] contrast: �Աȶ�, ��Χ��-1��1, Ĭ��ֵΪ0;
 * @param[in] saturation: ���Ͷ�, ��Χ��-1��1, Ĭ��ֵΪ0;
 * @param[in] hue: ɫ��, ��Χ��0��1, Ĭ��ֵΪ0; (�ݲ�֧��)
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_SetColorControl(const BVDisplay_HWND hWnd, const int displayRectIndex, const float brightness, const float contrast, const float saturation, const float hue);

/**
 * ������Ƶ�������Ƿ���Ҫ�߿�, �����ñ߿�Ŀ�Ⱥ���ɫ
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @param[in] displayRectIndex: ��Ƶ����������;
 * @param[in] color: �߿���ɫ
 * @param[in] width: �߿���, ��λΪ����, ���Ϊ0, ��ȡ���߿�
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_SetDisplayRectOutline(const BVDisplay_HWND hWnd, const int displayRectIndex, const BVDisplay_Color* color, const int width);

/**
 * ����������ʾЧ��, ���õ���ʾ�������ݹ���, ���������޷���ȫ��ʾʱ��, �������ʾ��ʽ, ����������ʾ�͹�����ʾ��.
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @param[in] fontIndex: ��������;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_SetFontDisplayMode(const BVDisplay_HWND hWnd, const int fontIndex, const BVDisplay_FontDisplayMode *dispMode);


/**=====================================================
 ����Աʵ��ɾ���ӿ�
  ====================================================*/
/**
 * ɾ��ָ�����û��Զ���ͼƬʵ��.
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @param[in] customFigureIdx: �û��Զ���ͼƬ����;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DeleteCustomFigure(const BVDisplay_HWND hWnd, const int customFigureIdx);

/**
 * ɾ��device�����е��û��Զ���ͼƬʵ��.
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DeleteAllCustomFigure(const BVDisplay_HWND hWnd);

/**
 * ɾ��ָ������Ƶ������ʵ��.
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @param[in] displayRectIndex: ��ʾ��������;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DeleteDisplayRect(const BVDisplay_HWND hWnd, const int displayRectIndex);

/**
 * ɾ��device�����е���Ƶ������ʵ��.
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DeleteAllDisplayRect(const BVDisplay_HWND hWnd);

/**
 * ɾ��ָ��������.
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @param[in] fontIndex: ��������;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DeleteFont(const BVDisplay_HWND hWnd, const int fontIndex);

/**
 * ɾ��device�����е�����ʵ��.
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DeleteAllFont(const BVDisplay_HWND hWnd);


/**=====================================================
 �豸ɾ���ӿ�
  ====================================================*/
/**
 * ɾ��deviceʵ��.
 * @param[in] hWnd: ��ʼ�����Ļ�ͼ���ھ��;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_ReleaseDevice(const BVDisplay_HWND hWnd);


/**
 * ɾ������deviceʵ��.
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_ReleaseAllDevice();

/**=====================================================
 �رտ�
  ====================================================*/
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_Close();
}
#endif  // end __LIBBVDISPLAY_H__

