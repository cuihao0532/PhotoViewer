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
    return TRUE;

    LPCTSTR lpFileName = TEXT("E:\\Photos\\gf.JPG");

    if ( !m_Image.IsNull() )
    {
        m_Image.DestroyImage();
        return FALSE;
    }

    if ( !m_Image.LoadImage(lpFileName) ) 
        return FALSE;

    m_pWnd = AfxGetMainWnd();
    CDC* pDC = m_pWnd->GetDC();
    m_Image.DrawImage(pDC, 0, 0); 
    m_pWnd->ReleaseDC(pDC);

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

        m_ptCurMouse = point;
    }

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
            if ( m_fZoomRate >= m_fMaxZoomRate )
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

            float fTemp = min(m_fFitWindowRate, m_fMinZoomRate);

            if ( m_fZoomRate <= m_fMinZoomRate )
            { 
                m_fZoomRate = m_fMinZoomRate; 
            }
            else
            {
                m_fZoomRate -= 0.1F; 
            }
        }


        // ����ǰ�Ŀ��� �߶�
        int nWidthBefor = m_rcDrawArea.Width();
        int nHeightBefore = m_rcDrawArea.Height();
        
        // ���ź�Ŀ��ȸ߶�
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
            TCHAR szBuf[20] = { 0 };
            _stprintf(szBuf, TEXT("%.1fx"), m_fZoomRate); 
            m_pUINotifier->SetZoomRate(szBuf);
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

BOOL CUIEventHandler::OpenFile(LPCTSTR lpFileName)
{ 
    m_Image.DestroyImage();
    m_Image.LoadImage(lpFileName);
    m_Image.Reset();
    
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

    // ���ͼ�����ڵ����Ͻ�����
    m_ptLeftTop.x = ( m_rcCancasArea.Width() - m_fZoomRate * m_nImgWidth ) / 2;
    m_ptLeftTop.y = ( m_rcCancasArea.Height() - m_fZoomRate * m_nImgHeight ) / 2; 

    // ͼ���������
    m_rcDrawArea.left = m_ptLeftTop.x;
    m_rcDrawArea.top = m_ptLeftTop.y;
    m_rcDrawArea.right = m_rcDrawArea.left + m_fZoomRate * m_nImgWidth;
    m_rcDrawArea.bottom = m_rcDrawArea.top + m_fZoomRate * m_nImgHeight;
    

    if ( m_pUINotifier )
    {
        m_pUINotifier->SetZoomRate(m_fZoomRate);
        m_pUINotifier->RedrawUI();
    } 

    return TRUE;
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