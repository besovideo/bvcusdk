/*
 * 本类用于在对话窗口的每一帧显示画面上叠加字幕
 */

using System;
using System.Collections;

namespace WindowsFormsTest
{
    class AfterRender
    {
        enum INDEX_TYPE
        {
            FONT
        }

        class AfterRenderIndex
        {
            public IntPtr dialogHandle; // BVCU SDK 对话句柄
            public IntPtr dispHandle;   // 显示对话的窗口句柄
            public BVRect dispRect;     // 字体显示矩形范围
            public INDEX_TYPE type;     // 显示类型，这里仅有文字显示
            public int index;           // 文字对象句柄
            public AfterRenderIndex()
            {
                index = BVCU.BVDISPLAY_INVALID_INDEX;
            }
        }

        BVCU.DisplayFont afterRenderDisplayFont;    // 用于显示叠加字体的函数代理(函数指针)
        ArrayList m_afterRenderList;                // 叠加字体对象列表
        Dialog m_dialog;                            // 全局Dialog类引用


        public AfterRender(Dialog dlg)
        {
            m_dialog = dlg;
            afterRenderDisplayFont = new BVCU.DisplayFont(displayFont);
            BVCU.ManagedLayer_DispSetDisplayFontFunc(afterRenderDisplayFont);
            m_afterRenderList = new ArrayList();
            refreshAfterRenderIdx = new DelegateCuDialogAndTimeStampIn(procRefreshAfterRenderIdx);
            createFontInDialog = new DelegateCuDialogIn(procCreateFontInDialog);
        }


        /// <summary>
        /// 创建用于叠加显示的字体
        /// </summary>
        /// <param name="dialog"></param>
        public void createTimeStampFont(OneDialog dialog)
        {
            if (null == dialog)
            {
                return;
            }
            dialog.panel.BeginInvoke(createFontInDialog, new object[] { dialog });
        }

        delegate void DelegateCuDialogIn(OneDialog dialog);
        DelegateCuDialogIn createFontInDialog;
        void procCreateFontInDialog(OneDialog dialog)
        {
            AfterRenderIndex idx = new AfterRenderIndex();
            idx.type = INDEX_TYPE.FONT;
            idx.dispHandle = dialog.panel.Handle;
            idx.dialogHandle = dialog.dialogHandle;
            if (!BVCU.SUCCEEDED(BVCU.BVDisplay_CreateFont(idx.dispHandle,
                ref idx.index, 15, "Arial")))
            {
                return;
            }
            BVCU.FontDisplayMode effect = new BVCU.FontDisplayMode();
            effect.size = BVCU.FONT_DISPLAY_MODE_SIZE;
            effect.fontColor = new BVCU.Color(0xFFFFFFFF);
            effect.backColor = new BVCU.Color(0xFF000000);
            effect.effect = BVCU.BVDISPLAY_TEXT_EFFECT_OUTLINE;
            effect.effectSize = 1;
            BVCU.BVDisplay_SetFontDisplayMode(idx.dispHandle, idx.index, ref effect);
            m_afterRenderList.Add(idx);
        }


        /// <summary>
        /// 将BVCU SDK传入的长整型时间转为DateTime显示
        /// </summary>
        delegate void DelegateCuDialogAndTimeStampIn(OneDialog dialog, Int64 timeStamp);
        DelegateCuDialogAndTimeStampIn refreshAfterRenderIdx;
        void procRefreshAfterRenderIdx(OneDialog dialog, Int64 timeStamp)
        {
            AfterRenderIndex idx = getIdx(dialog.dialogHandle);
            if (null == idx)
            {
                return;
            }
            idx.dispHandle = dialog.panel.Handle;
            idx.dispRect = new BVRect(dialog.panel.ClientRectangle.Top,
                dialog.panel.ClientRectangle.Left,
                dialog.panel.ClientRectangle.Bottom,
                dialog.panel.ClientRectangle.Right);
            double time = timeStamp / BVCU.SAV_TIME_BASE + 28800;
            DateTime dateTime = new DateTime(1970, 1, 1, 0, 0, 0).AddSeconds(time);
            dialog.timeStamp = dateTime;
        }

        void displayFont(IntPtr dialog, Int64 timeStamp)
        {
            OneDialog cuDlg = m_dialog.getDialog(dialog);
            if (null == cuDlg)
            {
                return;
            }
            if (cuDlg.panel != null)
            {
                AfterRenderIndex idx = getIdx(dialog);
                if (idx == null)
                {
                    return;
                }
                cuDlg.panel.BeginInvoke(refreshAfterRenderIdx, new object[] { cuDlg, timeStamp });
                BVCU.Color color = new BVCU.Color(0xFFFFFFFF);
                try
                {
                    BVCU.BVDisplay_DisplayFont(idx.dispHandle, idx.index, ref idx.dispRect,
                        ref color, cuDlg.timeStamp.ToString("yyyy'/'MM'/'dd HH:mm:ss"));
                }
                catch
                { }
            }
        }

        /// <summary>
        /// 查询函数
        /// </summary>
        /// <param name="handle"></param>
        /// <returns></returns>
        AfterRenderIndex getIdx(IntPtr handle)
        {
            foreach (AfterRenderIndex index in m_afterRenderList)
            {
                if (handle == index.dialogHandle)
                {
                    return index;
                }
            }
            return null;
        }
    }
}
