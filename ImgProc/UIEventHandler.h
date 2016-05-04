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


};