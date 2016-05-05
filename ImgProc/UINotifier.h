
#pragma once

__interface IUINotifier
{
    // 重绘UI(参数：是否立即重绘)
    void RedrawUI(BOOL bRedrawRightNow = FALSE);
    // 重绘图像区域(参数：是否立即重绘)
    void RedrawImg(BOOL bRedrawRightNow = FALSE);
    // 设置窗口标题
    void SetWindowTitle(LPCTSTR lpTitle);
    // 设置缩放比例
    void SetZoomRate(LPCTSTR lpZoomRate);
    // 设置缩放比例
    void SetZoomRate(float fRate);
    // 设置选择的矩形区域
    void SetRectangle(const CPoint& ptLeftTop, const CPoint& ptRightBottom);

};