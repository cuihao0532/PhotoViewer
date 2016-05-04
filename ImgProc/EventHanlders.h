
#include "UINotifier.h"
#pragma once

__interface IUIEventHandler
{
    BOOL OnLButtonDblClk(UINT nFlags, CPoint point);
    BOOL OnLButtonDown(UINT nFlags, CPoint point);
    BOOL OnLButtonUp(UINT nFlags, CPoint point);
    BOOL OnMouseMove(UINT nFlags, CPoint point);
    BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    BOOL OnRButtonDown(UINT nFlags, CPoint point);
    BOOL OnRButtonUp(UINT nFlags, CPoint point);
    BOOL OnSize(UINT nType, int cx, int cy);
    BOOL OpenFile(LPCTSTR lpFileName);
    BOOL SetUINotifier(IUINotifier* pNotifier);
    BOOL SetImgDest(CRect& rect);
    BOOL SetZoomRateDest(CRect& rect); 
    BOOL DrawImage();
    BOOL SetRotation(int nAngle);
    BOOL RotationLeft();
    BOOL RotationRight();
    BOOL Capture(const CPoint& ptLeftTop, const CPoint& ptRightBottom);

};
 
