#pragma once
#include "eventhanlders.h"
#include "GdiPlusImage.h"
#include "UINotifier.h"
#include <vector>
#include <map>
#include <algorithm>
using namespace std;



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
    CString m_strFileName;           // �ļ���


    CRect m_rcCancasArea;           // ��������
    CRect m_rcDrawArea;             // ͼ�񻭵�����
    CRect m_rcZoomRate; 
    POINT m_ptLeftTop;

    float m_fZoomRate;              // ���ű���

    bool  m_bLButtonDown;           // �������Ƿ���
    bool  m_bRButtonDown;           // ����Ҽ��Ƿ���
    CPoint m_ptCurMouse;            // ��굱ǰλ��
    CPoint m_ptRButtonDownPos;      // ����Ҽ�����ʱ������λ��

    int m_nImgWidth;
    int m_nImgHeight;

    int m_nScreenWidth;
    int m_nScreenHeight;


    float m_fMinZoomRate;        // ��С���ű���
    float m_fMaxZoomRate;        // ������ű���
    float m_fFitWindowRate;      // ��Ӧ���ڴ�Сʱ�����ű��� 
    bool  m_bFitWindow;          // ͼƬ��Ӧ���� 
    int   m_nRotationAngle;      // ��ת�Ƕ�


 

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