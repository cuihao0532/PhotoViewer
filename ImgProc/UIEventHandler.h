#pragma once
#include "eventhanlders.h"
#include "GdiPlusImage.h"
#include "UINotifier.h"
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

#ifndef PI
#define PI 3.141592654
#endif

struct TheLine
{
    float rho;
    float angle;
};


struct Index2Num
{
    Index2Num(int n, int c) : index(n), count(c)
    { 
    }

    Index2Num()
    {
        memset(this, 0, sizeof(*this));
    }

    int index;
    int count; 

    bool operator<(const Index2Num& obj) const
    {
        if ( this->count < obj.count )
            return true;

        return false;
    }

    bool operator>(const Index2Num& obj) const
    {
        if ( this->count > obj.count )
            return true;

        return false;
    }

};

typedef union
{
    ARGB Color;
    struct
    {
        BYTE Blue;
        BYTE Green;
        BYTE Red;
        BYTE Alpha;
    };
}ARGBQuad, *PARGBQuad;



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
    BOOL OnRButtonDown(UINT nFlags, CPoint point);
    BOOL OnRButtonUp(UINT nFlags, CPoint point);
    BOOL OpenFile(LPCTSTR lpFileName);
    BOOL SetUINotifier(IUINotifier* pNotifier);
    BOOL SetImgDest(CRect& rect);
    BOOL SetZoomRateDest(CRect& rect);
    BOOL DrawImage();
    BOOL SetRotation(int nAngle);
    BOOL RotationLeft();
    BOOL RotationRight();
    BOOL Capture(const CPoint& ptLeftTop, const CPoint& ptRightBottom);
    BOOL SaveAs(LPCTSTR lpFileName);
    BOOL RecLine();


    friend unsigned int __stdcall RecLineThread(void* p);


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
    bool  m_bRButtonDown;           // 鼠标右键是否按下
    CPoint m_ptCurMouse;            // 鼠标当前位置
    CPoint m_ptRButtonDownPos;      // 鼠标右键按下时，鼠标的位置

    int m_nImgWidth;
    int m_nImgHeight;

    int m_nScreenWidth;
    int m_nScreenHeight;


    float m_fMinZoomRate;        // 最小缩放比例
    float m_fMaxZoomRate;        // 最大缩放比例
    float m_fFitWindowRate;      // 适应窗口大小时的缩放比例 
    bool  m_bFitWindow;          // 图片适应窗口 
    int   m_nRotationAngle;      // 旋转角度


 

    BOOL   BeginRender(); 
    double Round(double val);
    void   Gray(BitmapData *data);
    void   GrayAnd2Values(BitmapData *data, BYTE threshold);
    void   LockBitmap(Gdiplus::Bitmap *bmp, BitmapData *data);
    void   UnlockBitmap(Gdiplus::Bitmap *bmp, BitmapData *data);
    void CUIEventHandler::Hough(
        Bitmap* pImg, float rho, float theta, int threshold, 
        std::vector<TheLine>& vecLines, int linesMax);

    


};