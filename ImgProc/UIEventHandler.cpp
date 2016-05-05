#include "StdAfx.h"
#include "UIEventHandler.h"


CUIEventHandler::CUIEventHandler(void)
{
    m_pDC = NULL;
    m_pWnd = NULL;
    m_pUINotifier = NULL;
    m_fZoomRate = 1.0F;
    m_bLButtonDown = false;
    m_nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    m_nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    m_fMinZoomRate = 0.6F;
    m_fMaxZoomRate = 3.0F;
    m_fFitWindowRate = 1.0F;
    m_bFitWindow = true;
    m_nRotationAngle = 0;
    m_bRButtonDown = false;
    memset(&m_ptLeftTop, 0, sizeof(m_ptLeftTop));
}


CUIEventHandler::CUIEventHandler(CWnd* pWnd)
{
    m_pDC = NULL;
    m_pUINotifier = NULL;
    m_fZoomRate = 1.0F;
    m_bLButtonDown = false;
    m_nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    m_nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    m_fMinZoomRate = 0.6F;
    m_fMaxZoomRate = 3.0F;
    memset(&m_ptLeftTop, 0, sizeof(m_ptLeftTop));
    m_bFitWindow = true;
    m_fFitWindowRate = 1.0F;
    m_nRotationAngle = 0;
    m_bRButtonDown = false;


    m_pWnd = pWnd; 
}

CUIEventHandler::~CUIEventHandler(void)
{
    if ( m_pDC )
    {
        m_pWnd->ReleaseDC(m_pDC); 
        m_pDC = NULL;
    }
}

BOOL CUIEventHandler::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if ( m_pUINotifier )
    {
        m_pUINotifier->SetRectangle(0, 0);
        m_pUINotifier->RedrawImg();
    }

   
    CRect rcDest;
    CRect rcSrc1(100, 100, 200, 200);
    CRect rcSrc2(150, 150, 250, 250);

    IntersectRect(&rcDest, &rcSrc1, &rcSrc2);

    return TRUE;
}

BOOL CUIEventHandler::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bLButtonDown = true;
    m_ptCurMouse = point;
    return TRUE; 
}

BOOL CUIEventHandler::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bLButtonDown = false;
    m_ptCurMouse = point;
    return TRUE; 
}

BOOL CUIEventHandler::OnMouseMove(UINT nFlags, CPoint point)
{
    if ( m_bLButtonDown )
    {
        int xDist = point.x - m_ptCurMouse.x;
        int yDist = point.y - m_ptCurMouse.y;


        int nLeft = m_ptLeftTop.x + xDist;
        int nTop  = m_ptLeftTop.y + yDist;
        int nRight = nLeft + m_nImgWidth;
        int nBottom = nTop + m_nImgHeight;

        if ( nLeft < 0 || nTop < 0 || nRight > m_nScreenWidth || nBottom > m_nScreenHeight )
        {
            //return TRUE;
        }

        // ���ͼ�����ڵ����Ͻ�����
        m_ptLeftTop.x += xDist;
        m_ptLeftTop.y += yDist; 
         
        // ͼ���������
        m_rcDrawArea.left = m_ptLeftTop.x;
        m_rcDrawArea.top = m_ptLeftTop.y;
        m_rcDrawArea.right = m_rcDrawArea.left + m_fZoomRate * m_nImgWidth;
        m_rcDrawArea.bottom = m_rcDrawArea.top + m_fZoomRate * m_nImgHeight; 

        if ( m_pUINotifier )
        {
            m_pUINotifier->RedrawImg();
        } 
    }

    if ( m_bRButtonDown )
    {
        // ���Ͻ� ���½� �γɵľ��У������ο�
        if ( m_pUINotifier )
        {
            m_pUINotifier->SetRectangle(m_ptRButtonDownPos, point);
            m_pUINotifier->RedrawImg();
        }
    }

    // ���µ�ǰ���λ��
    m_ptCurMouse = point;

    return TRUE; 
}

BOOL CUIEventHandler::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    AfxGetMainWnd()->ScreenToClient(&pt);

    if ( m_rcCancasArea.PtInRect(pt) )
    {
        m_bFitWindow = false;

        bool bBig = true;

        if ( zDelta > 0 )
        {
            if ( m_fZoomRate + 0.1F >= m_fMaxZoomRate )
            {  
                m_fZoomRate = m_fMaxZoomRate; 
            }
            else
            {
                m_fZoomRate += 0.1F; 
            } 
        }
        else
        {
            bBig = false; 
 
            if ( m_fZoomRate - 0.1F <= m_fMinZoomRate )
            { 
                m_fZoomRate = m_fMinZoomRate; 
            }
            else
            {
                m_fZoomRate -= 0.1F; 
            } 
        }


        // ����ǰ�Ŀ�� �߶�
        int nWidthBefor = m_rcDrawArea.Width();
        int nHeightBefore = m_rcDrawArea.Height();
        
        // ���ź�Ŀ�ȸ߶�
        int nNewWidth = m_nImgWidth * m_fZoomRate;
        int nNewHeight = m_nImgHeight * m_fZoomRate; 

        int nNewLeft = 0;
        int nNewTop  = 0;
        int nNewRight = 0;
        int nNewBottom = 0;

        if ( bBig )
        {
            nNewLeft = m_ptLeftTop.x - ( nNewWidth - nWidthBefor ) / 2;
            nNewTop = m_ptLeftTop.y - ( nNewHeight - nHeightBefore ) / 2;

            nNewRight = nNewWidth - nNewLeft;
            nNewBottom = nNewHeight - nNewTop;
        } 
        else
        {
            nNewLeft = m_ptLeftTop.x + ( nWidthBefor - nNewWidth ) / 2;
            nNewTop = m_ptLeftTop.y + ( nHeightBefore - nNewHeight ) / 2;

            nNewRight = nNewWidth - nNewLeft;
            nNewBottom = nNewHeight - nNewTop;
        }


        m_ptLeftTop.x = nNewLeft;
        m_ptLeftTop.y = nNewTop;

        m_rcDrawArea.left = m_ptLeftTop.x;
        m_rcDrawArea.top = m_ptLeftTop.y;
        m_rcDrawArea.right = nNewWidth + m_rcDrawArea.left;
        m_rcDrawArea.bottom = nNewHeight + m_rcDrawArea.top;

        if ( m_pUINotifier )
        { 
            m_pUINotifier->SetZoomRate(m_fZoomRate);
            m_pUINotifier->RedrawUI();
        }
    }

    return TRUE; 
}

BOOL CUIEventHandler::OnSize(UINT nType, int cx, int cy)
{    
    if ( !m_Image.IsNull() )
    { 
        if ( m_bFitWindow )
        {
            if ( m_nImgWidth >= m_rcCancasArea.Width() || m_nImgHeight >= m_rcCancasArea.Height() )
            {
                float fXRate = 1.0F * m_rcCancasArea.Width() / m_nImgWidth;
                float fYRate = 1.0F * m_rcCancasArea.Height() / m_nImgHeight;

                m_fZoomRate = min(fXRate, fYRate);
            }
            else
            {
                m_fZoomRate = 1.0F;
            }

            // ���ͼ�����ڵ����Ͻ�����
            m_ptLeftTop.x = ( m_rcCancasArea.Width() - m_fZoomRate * m_nImgWidth ) / 2;
            m_ptLeftTop.y = ( m_rcCancasArea.Height() - m_fZoomRate * m_nImgHeight ) / 2; 

            // ͼ���������
            m_rcDrawArea.left = m_ptLeftTop.x;
            m_rcDrawArea.top = m_ptLeftTop.y;
            m_rcDrawArea.right = m_rcDrawArea.left + m_fZoomRate * m_nImgWidth;
            m_rcDrawArea.bottom = m_rcDrawArea.top + m_fZoomRate * m_nImgHeight;
        }  // if ( m_bFitWindow )
    }
    else
    {

    } 

    if ( m_pUINotifier )
    {
        m_pUINotifier->RedrawUI();
    }
    
    return TRUE;
}

BOOL CUIEventHandler::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_bRButtonDown = true; 
    m_ptCurMouse = point; 
    m_ptRButtonDownPos = point;
    return TRUE;
}

BOOL CUIEventHandler::OnRButtonUp(UINT nFlags, CPoint point)
{
    m_bRButtonDown = false; 
    m_ptCurMouse = point;
    return TRUE;
}

BOOL CUIEventHandler::OpenFile(LPCTSTR lpFileName)
{ 
    m_Image.DestroyImage();
    m_Image.Reset();
    BOOL bRet = m_Image.LoadImage(lpFileName); 
    
    if ( bRet )
    {
        bRet = BeginRender(); 
    }

    return bRet;
}

BOOL CUIEventHandler::SetUINotifier(IUINotifier* pNotifier)
{
    m_pUINotifier = pNotifier;
    return TRUE;
}

BOOL CUIEventHandler::SetImgDest(CRect& rect)
{
    m_rcCancasArea = rect;
    return TRUE;
}

BOOL CUIEventHandler::SetZoomRateDest(CRect& rect)
{
    m_rcZoomRate = rect;
    return TRUE;
}

BOOL CUIEventHandler::DrawImage()
{ 
    m_pWnd = AfxGetMainWnd();
    CDC* pDC = m_pWnd->GetDC();
    if ( !pDC ) return FALSE;

    CRgn rgn;
    rgn.CreateRectRgn(m_rcCancasArea.left + 1, m_rcCancasArea.top + 1, m_rcCancasArea.Width() - 1, m_rcCancasArea.Height() - 1);  
    pDC->SelectClipRgn(&rgn);
    m_Image.DrawImage(pDC, m_rcDrawArea, m_rcCancasArea);
    m_pWnd->ReleaseDC(pDC);  
    rgn.DeleteObject(); 

    return TRUE;
}

BOOL CUIEventHandler::SetRotation(int nAngle)
{
    if ( m_Image.IsNull() ) return FALSE;
   
    // ������ת�Ƕ�
    m_Image.SetRotation(nAngle);
    if ( m_pUINotifier )
    { 
        m_pUINotifier->RedrawImg();
    }  
}

BOOL CUIEventHandler::RotationLeft()
{
    if ( m_Image.IsNull() ) return FALSE;

    m_nRotationAngle -= 90;
    m_nRotationAngle = m_nRotationAngle % 360;

    m_Image.SetRotation(m_nRotationAngle);
    if ( m_pUINotifier )
    { 
        m_pUINotifier->RedrawImg();
    }  
    return TRUE;
}

BOOL CUIEventHandler::RotationRight()
{
    if ( m_Image.IsNull() ) return FALSE;

    m_nRotationAngle += 90;
    m_nRotationAngle = m_nRotationAngle % 360;

    m_Image.SetRotation(m_nRotationAngle);
    if ( m_pUINotifier )
    { 
        m_pUINotifier->RedrawImg();
    }  
    return TRUE;
}

BOOL CUIEventHandler::Capture(const CPoint& ptLeftTop, const CPoint& ptRightBottom)
{
    if ( m_Image.IsNull() ) return FALSE; 
    if ( ptLeftTop == ptRightBottom ) return FALSE;

    // ����ѡ������
    CRect rcRectangle(ptLeftTop, ptRightBottom);

    // ͼ������
    CRect rcImage;
    rcImage.left = m_ptLeftTop.x;
    rcImage.top  = m_ptLeftTop.y;
    rcImage.right = rcImage.left + m_fZoomRate * m_nImgWidth;
    rcImage.bottom = rcImage.right + m_fZoomRate * m_nImgHeight; 

    // ѡ��ľ��������ͼ������Ľ���
    CRect rcDest;
    rcDest.IntersectRect(&rcRectangle, &rcImage);

    // ����Ϊ�����账��
    if ( rcDest.IsRectEmpty() ) return FALSE;
     
    // ��ѡ��ľ�������߿�ȥ��
    if ( m_pUINotifier )
    {
        m_pUINotifier->SetRectangle(0, 0);
        m_pUINotifier->RedrawImg(TRUE);
    }

    CDC* pDC = m_pWnd->GetDC();

    HDC hDCSrc = pDC->GetSafeHdc();
    int nBitPerPixel = GetDeviceCaps(hDCSrc, BITSPIXEL);
    int nWidth = rcDest.Width();
    int nHeight = rcDest.Height();
    
    // �ѽ������ֵ�ͼ�񿽱���CImage��
    CImage cImage;
    cImage.Create(nWidth, nHeight, nBitPerPixel);
    StretchBlt(cImage.GetDC(), 0, 0, nWidth, nHeight, hDCSrc, rcDest.left, rcDest.top, rcDest.Width(),
        rcDest.Height(), SRCCOPY);  
    cImage.ReleaseDC();

    // �ͷ�DC
    m_pWnd->ReleaseDC(pDC);
    pDC = NULL;

    // ��������ͼ������д����
    COleStreamFile cImgStream;
    cImgStream.CreateMemoryStream(NULL);
    cImage.Save(cImgStream.GetStream(), Gdiplus::ImageFormatBMP);

    // ���ٵ�ǰͼ��
    m_Image.DestroyImage();
    // ��λ��ز���
    m_Image.Reset(); 
    // ��stream����ͼ��
    m_Image.LoadImage(cImgStream.GetStream()); 
    // ��ʼ��Ⱦ
    BeginRender();    

    return TRUE;
}

BOOL CUIEventHandler::SaveAs(LPCTSTR lpFileName)
{
    if ( !lpFileName ) return FALSE; 
    if ( m_Image.IsNull() ) return FALSE;

    m_Image.Save(lpFileName, TEXT("image/jpeg"));  

    return TRUE;
}

BOOL CUIEventHandler::BeginRender()
{
    m_bFitWindow = true;
    m_nImgWidth  = m_Image.GetWidth();
    m_nImgHeight = m_Image.GetHeight(); 

    if ( m_nImgWidth >= m_rcCancasArea.Width() || m_nImgHeight >= m_rcCancasArea.Height() )
    {
        float fXRate = 1.0F * m_rcCancasArea.Width() / m_nImgWidth;
        float fYRate = 1.0F * m_rcCancasArea.Height() / m_nImgHeight;

        m_fZoomRate = min(fXRate, fYRate);
    }
    else
    {
        m_fZoomRate = 1.0F;
    }

    // ��ͼ��ʱʧ���䴰�ڵ����ű���
    m_fFitWindowRate = m_fZoomRate;
    m_fMinZoomRate   = min(0.6F, m_fFitWindowRate);

    // ���ͼ�����ڵ����Ͻ�����
    m_ptLeftTop.x = ( m_rcCancasArea.Width() - m_fZoomRate * m_nImgWidth ) / 2;
    m_ptLeftTop.y = ( m_rcCancasArea.Height() - m_fZoomRate * m_nImgHeight ) / 2; 

    // ͼ���������
    m_rcDrawArea.left = m_ptLeftTop.x;
    m_rcDrawArea.top = m_ptLeftTop.y;
    m_rcDrawArea.right = m_rcDrawArea.left + m_fZoomRate * m_nImgWidth;
    m_rcDrawArea.bottom = m_rcDrawArea.top + m_fZoomRate * m_nImgHeight;

    // ������ת�Ƕ�Ϊ0
    m_nRotationAngle = 0;
    m_Image.SetRotation(m_nRotationAngle);

    if ( m_pUINotifier )
    {
        m_pUINotifier->SetRectangle(0, 0); 
        m_pUINotifier->SetZoomRate(m_fZoomRate);
        m_pUINotifier->RedrawUI();
    } 

    return TRUE;
}

 