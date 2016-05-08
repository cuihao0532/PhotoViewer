#include "StdAfx.h"
#include "UIEventHandler.h"
#include <math.h>


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

        // 求出图像所在的左上角坐标
        m_ptLeftTop.x += xDist;
        m_ptLeftTop.y += yDist; 
         
        // 图像绘制区域
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
        // 左上角 右下角 形成的举行，画矩形框
        if ( m_pUINotifier )
        {
            m_pUINotifier->SetRectangle(m_ptRButtonDownPos, point);
            m_pUINotifier->RedrawImg();
        }
    }

    // 更新当前鼠标位置
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


        // 缩放前的宽度 高度
        int nWidthBefor = m_rcDrawArea.Width();
        int nHeightBefore = m_rcDrawArea.Height();
        
        // 缩放后的宽度高度
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

            // 求出图像所在的左上角坐标
            m_ptLeftTop.x = ( m_rcCancasArea.Width() - m_fZoomRate * m_nImgWidth ) / 2;
            m_ptLeftTop.y = ( m_rcCancasArea.Height() - m_fZoomRate * m_nImgHeight ) / 2; 

            // 图像绘制区域
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
   
    // 设置旋转角度
    m_Image.SetRotation(nAngle);
    if ( m_pUINotifier )
    { 
        m_pUINotifier->RedrawImg();
    }  

    return TRUE;
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

    // 矩形选择区域
    CRect rcRectangle(ptLeftTop, ptRightBottom);

    // 图像区域
    CRect rcImage;
    rcImage.left = m_ptLeftTop.x;
    rcImage.top  = m_ptLeftTop.y;
    rcImage.right = rcImage.left + m_fZoomRate * m_nImgWidth;
    rcImage.bottom = rcImage.right + m_fZoomRate * m_nImgHeight; 

    // 选择的矩形区域和图像区域的交集
    CRect rcDest;
    rcDest.IntersectRect(&rcRectangle, &rcImage);

    // 交集为空则不予处理
    if ( rcDest.IsRectEmpty() ) return FALSE;
     
    // 把选择的矩形区域边框去除
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
    
    // 把交集部分的图像拷贝至CImage对
    CImage cImage;
    cImage.Create(nWidth, nHeight, nBitPerPixel);
    StretchBlt(cImage.GetDC(), 0, 0, nWidth, nHeight, hDCSrc, rcDest.left, rcDest.top, rcDest.Width(),
        rcDest.Height(), SRCCOPY);  
    cImage.ReleaseDC();

    // 释放DC
    m_pWnd->ReleaseDC(pDC);
    pDC = NULL;

    // 将二进制图像数据写入流
    COleStreamFile cImgStream;
    cImgStream.CreateMemoryStream(NULL);
    cImage.Save(cImgStream.GetStream(), Gdiplus::ImageFormatBMP);

    // 销毁当前图像
    m_Image.DestroyImage();
    // 复位相关参数
    m_Image.Reset(); 
    // 从stream加载图像
    m_Image.LoadImage(cImgStream.GetStream()); 
    // 开始渲染
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

BOOL CUIEventHandler::RecLine()
{
    if ( m_Image.IsNull() ) return FALSE;

    if ( m_pUINotifier )
    {
        m_pUINotifier->ShowWaitDlg();
    }

    HANDLE hThread = (HANDLE)_beginthreadex(
        NULL,
        0,
        RecLineThread,
        this,
        0,
        NULL);

    if ( hThread == INVALID_HANDLE_VALUE || !hThread )
    {
        if ( m_pUINotifier )
        {
            m_pUINotifier->ShowWaitDlg(FALSE);
        }
    }

#if 0
    int width = m_Image.GetWidth(); 
    int height = m_Image.GetHeight();

    Bitmap* img;
    img = new Bitmap(width, height,PixelFormat32bppRGB);
    Graphics gr(img);
    
    gr.SetInterpolationMode(InterpolationModeHighQuality);
    gr.DrawImage(m_Image.GetImgObj(), 0, 0, width, height); 

    std::vector<TheLine> vecLines;
    Hough(img, 1, 3.1415926 / 10, 100, vecLines, 10); 

    delete img;
    img = NULL;
#endif

    return TRUE;
}

unsigned int __stdcall RecLineThread(void* p)
{
    CUIEventHandler* pThis = (CUIEventHandler*)p;

    if ( !pThis ) return 0; 

    int width = pThis->m_Image.GetWidth(); 
    int height = pThis->m_Image.GetHeight();

    Bitmap* img;
    img = new Bitmap(width, height,PixelFormat32bppRGB);
    Graphics gr(img);

    gr.SetInterpolationMode(InterpolationModeHighQuality);
    gr.DrawImage(pThis->m_Image.GetImgObj(), 0, 0, width, height); 

    std::vector<TheLine> vecLines;
    pThis->Hough(img, 1, 3.1415926 / 10, 100, vecLines, 10); 

    delete img;
    img = NULL;

    if ( pThis->m_pUINotifier )
    {
        pThis->m_pUINotifier->ShowWaitDlg(FALSE);
    }
    
    return 0;
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

    // 打开图像时失适配窗口的缩放比例
    m_fFitWindowRate = m_fZoomRate;
    m_fMinZoomRate   = min(0.6F, m_fFitWindowRate);

    // 求出图像所在的左上角坐标
    m_ptLeftTop.x = ( m_rcCancasArea.Width() - m_fZoomRate * m_nImgWidth ) / 2;
    m_ptLeftTop.y = ( m_rcCancasArea.Height() - m_fZoomRate * m_nImgHeight ) / 2; 

    // 图像绘制区域
    m_rcDrawArea.left = m_ptLeftTop.x;
    m_rcDrawArea.top = m_ptLeftTop.y;
    m_rcDrawArea.right = m_rcDrawArea.left + m_fZoomRate * m_nImgWidth;
    m_rcDrawArea.bottom = m_rcDrawArea.top + m_fZoomRate * m_nImgHeight;

    // 重置旋转角度为0
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

 


double CUIEventHandler::Round(double val)
{
    return (val> 0.0) ? floor(val+ 0.5) : ceil(val- 0.5);
}



void CUIEventHandler::Gray(BitmapData *data)
{
    PARGBQuad p = (PARGBQuad)data->Scan0;
    INT offset = data->Stride - data->Width * sizeof(ARGBQuad);

    for (UINT y = 0; y < data->Height; y ++ )
    {
        for (UINT x = 0; x < data->Width; x ++, p ++)
        {
            p->Blue = p->Green = p->Red =
                (UINT)(p->Blue * 29 + p->Green * 150 + p->Red * 77 + 128) >> 8;

        }

        BYTE* pt = (BYTE*)p + offset;
        p = (PARGBQuad)pt;
    }
}


void CUIEventHandler::GrayAnd2Values(BitmapData *data, BYTE threshold)
{
    PARGBQuad p = (PARGBQuad)data->Scan0;
    INT offset = data->Stride - data->Width * sizeof(ARGBQuad); 

    for (UINT y = 0; y < data->Height; y ++ )
    {
        for (UINT x = 0; x < data->Width; x ++, p ++)
        {
            if (((p->Blue * 29 + p->Green * 150 + p->Red * 77 + 128) >> 8) < threshold)
                p->Color &= 0xff000000;
            else
                p->Color |= 0x00ffffff; 
        }

        BYTE* pt = (BYTE*)p + offset;
        p = (PARGBQuad)pt;
    }
}


void CUIEventHandler::LockBitmap(Gdiplus::Bitmap *bmp, BitmapData *data)
{
    Gdiplus::Rect r(0, 0, bmp->GetWidth(), bmp->GetHeight());
    bmp->LockBits(&r, ImageLockModeRead | ImageLockModeWrite,
        PixelFormat32bppARGB, data);
}

void CUIEventHandler::UnlockBitmap(Gdiplus::Bitmap *bmp, BitmapData *data)
{
    bmp->UnlockBits(data);
}



void CUIEventHandler::Hough(
    Bitmap* pImg, float rho, float theta, int threshold, 
    std::vector<TheLine>& vecLines, int linesMax)
{
    BitmapData data;
    LockBitmap(pImg, &data);  
    GrayAnd2Values(&data, 128); 

    PARGBQuad p = (PARGBQuad)(data.Scan0); 
    INT offset = data.Stride - data.Width * sizeof(ARGBQuad);

    int width = pImg->GetWidth();
    int height = pImg->GetHeight();
    int total = 0;
    float ang;
    int r, n;
    float irho = 1 / rho;
    double scale;
    int step = 1;
    double pi = 3.1415926; 

    int numangle = pi / theta;
    int numrho = ( ( width + height ) * 2 + 1 ) / rho;

    int* accum = new int[ (numangle + 2) * (numrho + 2)]();
    int* sort_buf = new int[ numangle * numrho ]();
    float* tabSin = new float[numangle]();
    float* tabCos = new float[numangle]();

    std::map<int, vector<CPoint> > mp_pts;

    for( ang = 0, n = 0; n < numangle; ang += theta, n++ ) // 计算正弦曲线的准备工作，查表
    { 
        tabSin[n] = (float)(sin(ang) * irho);
        tabCos[n] = (float)(cos(ang) * irho);
    }

    // stage 1. fill accumulator
    for( int i = 0; i < height; i++ )
    {
        for( int j = 0; j < width; j++, p++ )
        { 
            // 将每个非零点，转换为霍夫空间的离散正弦曲线，并统计。
            if ( p->Red != 0 && p->Green != 0 && p->Blue != 0 ) 
            {
                for( n = 0; n < numangle; n++ )
                {
                    r = Round(j * tabCos[n] + i * tabSin[n]);
                    r += (numrho - 1) / 2;

                    int nKey = (n + 1) * (numrho+2) + r + 1; 
                    accum[nKey]++;  

                    if ( mp_pts.end() == mp_pts.find(nKey) )
                    {
                        vector<CPoint> pts;
                        mp_pts[ nKey ] = pts;
                    }
                    else
                    {
                        mp_pts[nKey].push_back(CPoint(j, i));
                    }

                }
            } 

        }

        BYTE* pt = (BYTE*)p + offset;
        p = (PARGBQuad)pt;
    } 


    std::vector<Index2Num> vecIndex;

    for( int r = 0; r < numrho; r++ )   
    {
        for( int n = 0; n < numangle; n++ )
        {
            int base = (n+1) * (numrho+2) + r+1;
            if( accum[base] > threshold &&
                accum[base] > accum[base - 1] && accum[base] >= accum[base + 1] &&
                accum[base] > accum[base - numrho - 2] && accum[base] >= accum[base + numrho + 2] )
            {
                sort_buf[total++] = base;

                Index2Num obj;
                obj.index = base;
                obj.count = accum[ base ];

                vecIndex.push_back(obj);                
            }
        }
    } 


    std::sort(vecIndex.begin(), vecIndex.end(), greater<Index2Num>());

    // sort_buf中存放index
    // 所有index中对应的accum[ index ]值没有序
    // 需要按照accum[ index ]降序排列，并按照降序后的结果，也对sort_buf降序，不改变accum 
    // stage 4. store the first min(total,linesMax) lines to the output buffer
    linesMax = min(linesMax, total);
    linesMax = min(vecIndex.size(), linesMax);

    scale = 1./(numrho+2);
    for( int i = 0; i < linesMax; i++ )  // 依据霍夫空间分辨率，计算直线的实际r，theta参数
    {
        TheLine line;
        int idx = vecIndex[i].index;
        int n = floor(idx * scale) - 1;
        int r = idx - (n + 1) * (numrho + 2) - 1;
        line.rho = (r - (numrho - 1) * 0.5f) * rho;
        line.angle = n * theta; 
        vecLines.push_back(line); 
    }

    UnlockBitmap(pImg, &data);

    delete[] accum;
    delete[] sort_buf;
    delete[] tabSin;
    delete[] tabCos; 

    vector<Point> vecPoints;
    vector< vector<Point> > vvecPoints;

    for ( int i = 0; i < linesMax; ++ i )
    {
        CString str;
        auto at = mp_pts[ vecIndex[i].index ]; 

        for ( int i = 0; i < at.size(); ++ i )
        {
            TCHAR szBuf[100] = { 0 };
            _stprintf(szBuf, TEXT("(%d, %d)"), at[i].x, at[i].y); 
            str += CString(szBuf);
            str += CString(TEXT(", "));
             
            vecPoints.push_back(Point(at[ i ].x, at[ i ].y));
        }
         
        vvecPoints.push_back(vecPoints);  
    }

    m_Image.SetRecLines(vvecPoints);
    if ( m_pUINotifier )
    { 
        m_pUINotifier->RedrawImg();
    } 

}