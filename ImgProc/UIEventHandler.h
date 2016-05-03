#pragma once
#include "eventhanlders.h"
#include "GdiPlusImage.h"
#include "UINotifier.h"

class CUIEventHandler :
    public IUIEventHandler
{
public:
    CUIEventHandler(void);
    CUIEventHandler(CWnd* pWnd);
    ~CUIEventHandler(void);

public:
    // ---------- IUIEventHandler ------------------
    BOOL OnLButtonDblClk(UINT nFlags, CPoint point);
    BOOL OnLButtonDown(UINT nFlags, CPoint point);
    BOOL OnLButtonUp(UINT nFlags, CPoint point);
    BOOL OnMouseMove(UINT nFlags, CPoint point);
    BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    BOOL OnSize(UINT nType, int cx, int cy); 
    BOOL OpenFile(LPCTSTR lpFileName);
    BOOL SetUINotifier(IUINotifier* pNotifier);
    BOOL SetImgDest(CRect& rect);
    BOOL SetZoomRateDest(CRect& rect);
    BOOL DrawImage();
    BOOL SetRotation(int nAngle);
    BOOL RotationLeft();
    BOOL RotationRight();

protected:
    CGdiPlusImage m_Image;
    CDC* m_pDC;
    CWnd *m_pWnd;
    IUINotifier* m_pUINotifier;
    CString m_strFileName;           // 文件名


    CRect m_rcCancasArea;           // 画布区域
    CRect m_rcDrawArea;             // 图像画的区域
    CRect m_rcZoomRate; 
    POINT m_ptLeftTop;

    float m_fZoomRate;              // 缩放比例

    bool  m_bLButtonDown;           // 鼠标左键是否按下
    CPoint m_ptCurMouse;  

    int m_nImgWidth;
    int m_nImgHeight;

    int m_nScreenWidth;
    int m_nScreenHeight;


    float m_fMinZoomRate;        // 最小缩放比例
    float m_fMaxZoomRate;        // 最大缩放比例
    float m_fFitWindowRate;      // 适应窗口大小时的缩放比例

    bool  m_bFitWindow;          // 图片适应窗口

    int   m_nRotationAngle;      // 旋转角度


};