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
     * 句柄指针, 32位系统为Int32, 64位系统为Int64
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
    BVDISPLAY_COLOR_FMT_INVALID = 0xffffffff // 强制枚举为4字节
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
    BVDISPLAY_TEXT_EFFECT_NOEFFECT = 0,         // 无效果, 直接绘制字体
    BVDISPLAY_TEXT_EFFECT_OUTLINE = 1,          // 字体轮廓反色绘制
    BVDISPLAY_TEXT_EFFECT_SHADOW = 2,           // 字体阴影
    BVDISPLAY_TEXT_EFFECT_BACKGROUND = 3,       // 字体背景色
    BVDISPLAY_TEXT_EFFECT_INVALID = 0xffffffff // 强制枚举为4字节
}BVDisplay_TextEffect;

/**
 * 用于视频显示的流, 每个结构存储一个YV12帧
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
    BVDisplay_PixelFormat eFormat;                  // 视频帧的格式
    BVDisplay_INT32 iFrameWidth;                    // 视频帧的数据宽度
    BVDisplay_INT32 iFrameHeight;                   // 视频帧的数据高度
    BVDisplay_INT32 aiStrike[BV_DISPLAY_FRAME_COMP]; // 数据的实际宽度
    BVDisplay_BYTE* abData[BV_DISPLAY_FRAME_COMP];  // 视频帧的数据
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
 * 字体显示的效果设置
 */
typedef struct _BVDisplay_FontDisplayMode
{
    BVDisplay_INT32 iSize;
    BVDisplay_TextAlignType eAlignType;
    BVDisplay_INT32 iRate;      // 字幕滚动速度, 单位:(pixel/per display interval)
                                // 如果为0, 则代表无字幕滚动效果
                                
    BVDisplay_TextEffect eTextEffect;
    
    BVDisplay_Color sFontColor;         // 字体颜色, 包含alpha值
    
    BVDisplay_Color sBackColor;         // background效果时为背景色, 
                                        // shadow效果时为阴影色, 
                                        // outline效果时为轮廓颜色
                                        // 包含alpha值

    BVDisplay_INT32 iEffectSize;        // 字体效果尺寸
                                        // background效果时为无效
                                        // shadow效果时为阴影像素数
                                        // outline效果时为轮廓像素数

}BVDisplay_FontDisplayMode;
extern "C" {
/**=====================================================
 初始化库
  ====================================================*/
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_Open();

/**=====================================================
 设备初始化接口
  ====================================================*/
/**
 * 初始化显示设备, 为用户初始化到指定窗口区域的显示句柄.
 * @param[in] hWnd: 用户层提供的绘图区域句柄;
 * @param[in] width: 绘图区域最大宽度
 * @param[in] height: 绘图区域最大高度
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DeviceInit(const BVDisplay_HWND hWnd, const int width, const int height);


/**=====================================================
 各成员实例创建接口
  ====================================================*/
/**
 * 在device的客户区内创建视频播放区
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @param[out] displayRectIndex: 视频播放区域索引;
 * @param[in] dispRect: 在device内的显示区域;
 * @param[in] videoRect: video帧数据大小, 其top, left值应为0; bottom, right值分别为输入视频流的高和宽;
 * @param[in] pixelFormat: 视频格式, 当前仅支持BV_DISPLAY_PIX_FMT_YUV420P
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_CreateDisplayRect(const BVDisplay_HWND hWnd, int *displayRectIndex, const BVDisplay_RECT *dispRect, const BVDisplay_RECT *videoRect, const BVDisplay_PixelFormat pixelFormat);

/**
 * 创建字体用于用户定义信息的显示.
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @param[out] fontIndex: 创建的字体的索引;
 * @param[in] fontSize: 字体大小, 单位为相对值;
 * @param[in] fontName: 字体名称, 如: "Arial", "Times New Roman", "宋体"等;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_CreateFont(const BVDisplay_HWND hWnd, int *fontIndex, const int fontSize, const BVDisplay_WCHAR *fontName);

/**
 * 载入用户自定义的图片文件
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @param[out] customFigureIdx: 创建的图片的索引;
 * @param[in] filePath: 用户要显示的图片, 支持格式参见: http://msdn.microsoft.com/en-us/library/bb172878%28v=VS.85%29.aspx；
 * @param[in] keyColor: 图片中的透明色;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_CreateCustomFile(const BVDisplay_HWND hWnd, int *customFigureIdx, const BVDisplay_WCHAR* filePath, const BVDisplay_Color *keyColor);

/**
 * 载入用户自定义的图片资源
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @param[out] customFigureIdx: 创建的图片的索引;
 * @param[in] resourceName: 用户要显示的图片, 支持格式参见: http://msdn.microsoft.com/en-us/library/bb172878%28v=VS.85%29.aspx；
 * @param[in] keyColor: 图片中的透明色;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_CreateCustomResource(const BVDisplay_HWND hWnd, int *customFigureIdx, BVDisplay_HANDLE hSrcModule, const BVDisplay_WCHAR* resourceName, const BVDisplay_Color *keyColor);


/**=====================================================
 基本显示接口
  ====================================================*/
/**
 * 将窗口清为指定颜色, 避免屏幕中没有视频播放的区域出现乱码色
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @param[in] color: 需要的背景颜色;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_ClearBackground(const BVDisplay_HWND hWnd, const BVDisplay_Color* color);

/**
 * 根据用户设定的相对位置在device的clientRect区域内显示一帧.
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @param[in] displayRectIndex: 视频播放区索引列表;
 * @param[in] frameRect: 局部区域列表, 如果为NULL, 则显示displayRectIndex代表的整个区域, 否则显示帧画面中指定的一部分矩形区域
 * @param[in] frameData: 要显示的帧列表, 如果这里传入的视频大小与CreateDisplayRect创建的不一致, 则返回错误;
 * @param[in] frameCnt: 列表中的元素个数, 前三个数组的元素个数必须等于frameCnt
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DrawFrame(const BVDisplay_HWND hWnd, const int* displayRectIndex, const BVDisplay_RECT** frameRect, const BVDisplay_Frame **frameData, const int frameCnt);

/**
* 根据用户设定的相对位置在device的clientRect区域内显示一帧.
* @param[in] hWnd: 初始化过的绘图窗口句柄;
* @param[in] displayRectIndex: 视频播放区索引列表;
* @param[in] frameRect: 局部区域列表, 如果为NULL, 则显示displayRectIndex代表的整个区域, 否则显示帧画面中指定的一部分矩形区域
* @param[in] frameData: 要显示的帧列表, 如果这里传入的视频大小与CreateDisplayRect创建的不一致, 则返回错误;
* @param[in] frameCnt: 列表中的元素个数, 前三个数组的元素个数必须等于frameCnt
* @return: < 0 if failed, >= 0 on success.
*/
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DrawOneFrame(const BVDisplay_HWND hWnd, const int displayRectIndex, const BVDisplay_RECT* frameRect, const BVDisplay_Frame *frameData);

/**
 * 将绘制在device的客户区缓存中的画面显示出来
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @return: < 0 if failed, >= 0 on success.
 * 本函数必须与InitDevice函数在同一线程调用，否则可能引发设备丢失异常
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_PresentFrame(const BVDisplay_HWND hWnd);

/**
 * 在指定区域内显示字体, 字体在视频上的位置由dispRect的top, left, bottom, right参数确定,  颜色由color确定.
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @param[in] fontIndex: 字体索引;
 * @param[in] dispRect: 字体显示的范围, 超出范围的字体将被截断, 其最大范围为DeviceInit时指定的高和宽;
 * @param[in] color: 字体的ARGB颜色;
 * @param[in] text: 要显示的文字内容;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DisplayFont(const BVDisplay_HWND hWnd, const int fontIndex, const BVDisplay_RECT *dispRect, const BVDisplay_Color *color, const BVDisplay_WCHAR* text);

/**
 * 显示用户输入的图片文件或资源.
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @param[in] customFigureIdx: 图片文件或资源的索引;
 * @param[in] dispRect: 绘图位置;
 * @param[in] alpha: 显示位图的透明度, 255为不透明, 0为完全透明不可见;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DisplayCustomFigure(const BVDisplay_HWND hWnd, const int customFigureIdx, const BVDisplay_RECT *dispRect, const int alpha);


/**=====================================================
 显示参数设置接口
  ====================================================*/
/**
 * 重新设置视频播放区大小和位置, 或重新设置输入帧数据大小
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @param[in] displayRectIndex: 视频播放区索引;
 * @param[in] dispRect: 新的视频播放区, 如果为NULL, 则保持原显示区域不变;
 * @param[in] videoRect: 新的视频帧数据大小, 如果为NULL, 则保持原视频帧数据大小不变;
 * @param[in] rotateAngle: 视频的旋转角度(仅支持90度的整倍数, 如-90/0/270/720等), 默认不旋转
 * @param[in] displayMode: 视频显示模式(0: 填充, 1:等比例缩放)
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_ModifyDisplayRect(const BVDisplay_HWND hWnd, const int displayRectIndex, const BVDisplay_RECT *dispRect, const BVDisplay_RECT *videoRect, const int rotateAngle = 0, const int displayMode = 0);

/**
 * 设置视频画面的亮度, 对比度, 饱和度和色调
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @param[in] displayRectIndex: 视频播放区索引; (暂不支持)
 * @param[in] brightness: 亮度, 范围从-1到1, 默认值为0;
 * @param[in] contrast: 对比度, 范围从-1到1, 默认值为0;
 * @param[in] saturation: 饱和度, 范围从-1到1, 默认值为0;
 * @param[in] hue: 色调, 范围从0到1, 默认值为0; (暂不支持)
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_SetColorControl(const BVDisplay_HWND hWnd, const int displayRectIndex, const float brightness, const float contrast, const float saturation, const float hue);

/**
 * 设置视频播放区是否需要边框, 并设置边框的宽度和颜色
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @param[in] displayRectIndex: 视频播放区索引;
 * @param[in] color: 边框颜色
 * @param[in] width: 边框宽度, 单位为像素, 如果为0, 则取消边框
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_SetDisplayRectOutline(const BVDisplay_HWND hWnd, const int displayRectIndex, const BVDisplay_Color* color, const int width);

/**
 * 设置字体显示效果, 设置当显示文字内容过多, 给定区域无法完全显示时的, 字体的显示方式, 包括多行显示和滚动显示等.
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @param[in] fontIndex: 字体索引;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_SetFontDisplayMode(const BVDisplay_HWND hWnd, const int fontIndex, const BVDisplay_FontDisplayMode *dispMode);


/**=====================================================
 各成员实例删除接口
  ====================================================*/
/**
 * 删除指定的用户自定义图片实例.
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @param[in] customFigureIdx: 用户自定义图片索引;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DeleteCustomFigure(const BVDisplay_HWND hWnd, const int customFigureIdx);

/**
 * 删除device下所有的用户自定义图片实例.
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DeleteAllCustomFigure(const BVDisplay_HWND hWnd);

/**
 * 删除指定的视频播放区实例.
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @param[in] displayRectIndex: 显示区域索引;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DeleteDisplayRect(const BVDisplay_HWND hWnd, const int displayRectIndex);

/**
 * 删除device下所有的视频播放区实例.
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DeleteAllDisplayRect(const BVDisplay_HWND hWnd);

/**
 * 删除指定的字体.
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @param[in] fontIndex: 字体索引;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DeleteFont(const BVDisplay_HWND hWnd, const int fontIndex);

/**
 * 删除device下所有的字体实例.
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_DeleteAllFont(const BVDisplay_HWND hWnd);


/**=====================================================
 设备删除接口
  ====================================================*/
/**
 * 删除device实例.
 * @param[in] hWnd: 初始化过的绘图窗口句柄;
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_ReleaseDevice(const BVDisplay_HWND hWnd);


/**
 * 删除所有device实例.
 * @return: < 0 if failed, >= 0 on success.
 */
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_ReleaseAllDevice();

/**=====================================================
 关闭库
  ====================================================*/
LIB_BVDISPLAY_API BVDisplay_ERRCode BVDisplay_Close();
}
#endif  // end __LIBBVDISPLAY_H__

