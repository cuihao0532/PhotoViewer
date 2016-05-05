
#include "StdAfx.h"
#include "GdiPlusImage.h"

//构造函数
CGdiPlusImage::CGdiPlusImage(void)
{
    //设置变量
    m_pImage=NULL;
    m_szZoom.cx = m_szZoom.cy = 1.0f;
    m_nRotateAngle = 0;
    m_cbAlphaDepth = 255;
    m_nXPos = 0;
    m_nYPos = 0;
    m_fZoomRate = 1.0F;
    m_nShowWidth = 0;
    m_nShowHeight = 0;
    memset(&m_ptCurImgCenter, 0, sizeof(m_ptCurImgCenter));
    m_bOpenNewImg = true;
}

//析构函数
CGdiPlusImage::~CGdiPlusImage(void)
{
    //销毁图片
    DestroyImage(); 
}

//是否加载
bool CGdiPlusImage::IsNull()
{
    //加载判断
    if (m_pImage==NULL) return true;
    if (m_pImage->GetLastStatus()!=Ok) return true;

    return false;
}

//获取宽度
INT CGdiPlusImage::GetWidth()
{
    //加载判断
    ASSERT(IsNull()==false);
    if (IsNull()==true) return 0;

    //获取宽度
    return m_pImage->GetWidth();
}

//获取高度
INT CGdiPlusImage::GetHeight()
{
    //加载判断
    ASSERT(IsNull()==false);
    if (IsNull()==true) return 0;

    //获取高度
    return m_pImage->GetHeight();
}

//销毁图片
bool CGdiPlusImage::DestroyImage()
{
    //删除对象
    if (m_pImage!=NULL) 
    { 
        delete m_pImage;
        m_pImage = NULL;
    }

    return true;
}

//加载图片
bool CGdiPlusImage::LoadImage(LPCTSTR pszFileName)
{
    // 加载判断
    ASSERT(m_pImage == NULL);
    if (m_pImage != NULL) return false;

    // 加载文件
    CT2CW strFileName(pszFileName);
    m_pImage = Image::FromFile((LPCWSTR)strFileName); 

    // 错误判断
    if ((m_pImage==NULL)||(m_pImage->GetLastStatus()!=Ok)) 
    {
        DestroyImage();
        return false;
    }

    return true;
}

bool CGdiPlusImage::LoadImage(IStream* stream)
{
    // 加载判断
    ASSERT(m_pImage == NULL);
    if (m_pImage != NULL || !stream) return false;

    // 加载文件 
    m_pImage = Image::FromStream(stream); 

    // 错误判断
    if ((m_pImage==NULL)||(m_pImage->GetLastStatus()!=Ok)) 
    {
        DestroyImage();
        return false;
    }

    return true;
}

//绘画图像
bool CGdiPlusImage::DrawImage(CDC * pDC, INT nXPos, INT nYPos)
{
    //加载判断
    //ASSERT(m_pImage!=NULL);
    if (m_pImage==NULL) return false;

    //创建屏幕
    ASSERT(pDC!=NULL);
    Graphics graphics(pDC->GetSafeHdc());

    GraphicsContainer Containter = graphics.BeginContainer();
    graphics.SetSmoothingMode(SmoothingModeHighQuality);

    //获取属性
    INT nImageWidth=m_pImage->GetWidth();
    INT nImageHeight=m_pImage->GetHeight();

    //构造位置
    RectF rcDrawRect;
    rcDrawRect.X=0;//(REAL)nXPos;
    rcDrawRect.Y=0;//(REAL)nYPos;
    rcDrawRect.Width=(REAL)nImageWidth;
    rcDrawRect.Height=(REAL)nImageHeight;

    Matrix matrix;

    CPoint pt((nXPos+nImageWidth/2), (nYPos+nImageHeight/2));
    PointF ptBase((float)pt.x,(float)pt.y);

    //矩阵变换要注意顺序，先平移在缩放后旋转
    matrix.Translate((REAL)nXPos,(REAL)nYPos,MatrixOrderAppend);

    //缩放
    if ( m_szZoom.cx != 1.0f || m_szZoom.cy != 1.0f )
    {
        ptBase.X += m_szZoom.cx;
        ptBase.Y += m_szZoom.cy;

        matrix.Translate(-ptBase.X,-ptBase.Y,MatrixOrderAppend);
        matrix.Scale(m_szZoom.cx,m_szZoom.cy,MatrixOrderAppend);
        matrix.Translate(ptBase.X,ptBase.Y,MatrixOrderAppend);
    }

    //旋转
    if (m_nRotateAngle)
    {
        matrix.RotateAt((REAL)m_nRotateAngle,ptBase,MatrixOrderAppend);
    }

    graphics.SetTransform(&matrix);

    //透明矩阵
    ColorMatrix Matrix=
    {
        1.0f,0.0f,0.0f,0.0f,0.0f, 
        0.0f,1.0f,0.0f,0.0f,0.0f, 
        0.0f,0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,0.0f,m_cbAlphaDepth/255.0f,0.0f, 
        0.0f,0.0f,0.0f,0.0f,1.0f
    };

    //设置属性
    ImageAttributes Attributes; 
    Attributes.SetColorMatrix(&Matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap); 

    //绘画图像
    graphics.DrawImage(m_pImage,rcDrawRect,0,0,(REAL)nImageWidth,(REAL)nImageHeight,UnitPixel,&Attributes);

    graphics.ResetTransform();

    graphics.EndContainer(Containter);

    return true;
}
 

bool CGdiPlusImage::DrawImage(CDC* pDC)
{
    //创建屏幕
    ASSERT(pDC!=NULL); 
    if ( !pDC ) return false; 

    Bitmap bmp(m_rcDest.Width(), m_rcDest.Height()); 
    Graphics* pGraphics = Graphics::FromImage(&bmp); 
    if ( !pGraphics ) return false;

    GraphicsContainer Containter = pGraphics->BeginContainer();
    pGraphics->SetSmoothingMode(SmoothingModeHighQuality); 

    SolidBrush bgbrush(Color(238, 243, 250));
    pGraphics->FillRectangle(&bgbrush, 0, 0, m_rcDest.Width(), m_rcDest.Height());
    
    if ( m_pImage )
    {
        //获取属性 
        INT nImageWidth  = m_pImage->GetWidth();
        INT nImageHeight = m_pImage->GetHeight(); 


        float fMin = 1.0F; 
        float fZoomRate = 1.0F; 
        bool bZoomOut = false; 
        int nXSrc = 0;
        int nYSrc = 0; 

        if ( nImageWidth >= m_rcDest.Width() || nImageHeight >= m_rcDest.Height() )
        { 
            float fXRate = 1.0F * m_rcDest.Width() / nImageWidth;
            float fYRate = 1.0F * m_rcDest.Height() / nImageHeight;
            fMin = min(fXRate, fYRate);   
            fZoomRate = max(nImageWidth * 1.0F / m_rcDest.Width(), nImageHeight * 1.0F / m_rcDest.Height()); 
            bZoomOut = true;
        } 


        int nDestWidth  = fMin * nImageWidth;
        int nDestHeight = fMin * nImageHeight;


        //构造位置
        RectF rcDrawRect;  
        rcDrawRect.X = m_rcDest.left + (m_rcDest.Width() - nDestWidth ) / 2;   
        rcDrawRect.Y = m_rcDest.top + (m_rcDest.Height() - nDestHeight ) / 2; 
        rcDrawRect.Width = nDestWidth;
        rcDrawRect.Height = nDestHeight; 

        Matrix matrix;

        CPoint pt(m_rcDest.Width() / 2, m_rcDest.Height() / 2);
        PointF ptBase((float)pt.x, (float)pt.y);  

        //矩阵变换要注意顺序，先平移在缩放后旋转
        matrix.Translate((REAL)m_rcDest.left,(REAL)m_rcDest.top,MatrixOrderAppend);

        //缩放
        if ( m_szZoom.cx != 1.0f || m_szZoom.cy != 1.0f )
        {
            ptBase.X += m_szZoom.cx;
            ptBase.Y += m_szZoom.cy;

            matrix.Translate(-ptBase.X,-ptBase.Y,MatrixOrderAppend);
            matrix.Scale(m_szZoom.cx,m_szZoom.cy,MatrixOrderAppend);
            matrix.Translate(ptBase.X,ptBase.Y,MatrixOrderAppend);
        }

        //旋转
        if (m_nRotateAngle)
        {
            matrix.RotateAt((REAL)m_nRotateAngle,ptBase,MatrixOrderAppend);
        }

        pGraphics->SetTransform(&matrix);

        //透明矩阵
        ColorMatrix Matrix=
        {
            1.0f,0.0f,0.0f,0.0f,0.0f, 
            0.0f,1.0f,0.0f,0.0f,0.0f, 
            0.0f,0.0f,1.0f,0.0f,0.0f,
            0.0f,0.0f,0.0f,m_cbAlphaDepth/255.0f,0.0f, 
            0.0f,0.0f,0.0f,0.0f,1.0f
        };

        //设置属性
        ImageAttributes Attributes; 
        Attributes.SetColorMatrix(&Matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap); 

        //绘画图像
        pGraphics->DrawImage(m_pImage, rcDrawRect, nXSrc, nYSrc, (REAL)nImageWidth - nXSrc * 2, (REAL)nImageHeight - nYSrc * 2, UnitPixel,&Attributes);  
        pGraphics->ResetTransform();   
        pGraphics->EndContainer(Containter);

    } // if ( m_pImage )
   
    // 从内存拷贝至设备
    Graphics graphicsreal(pDC->GetSafeHdc()); 
    graphicsreal.DrawImage(&bmp, 0, 0, m_rcDest.Width(), m_rcDest.Height()); 
    delete pGraphics;
    pGraphics = NULL;

    return true;
}

 
bool CGdiPlusImage::DrawImage(CDC* pDC, CRect& rcDraw, CRect& rcCanvas)
{
    //创建屏幕
    ASSERT(pDC!=NULL); 
    if ( !pDC ) return false; 

    Bitmap bmp(rcCanvas.Width(), rcCanvas.Height()); 
    Graphics* pGraphics = Graphics::FromImage(&bmp); 
    if ( !pGraphics ) return false;

    //GraphicsContainer Containter = pGraphics->BeginContainer();
    pGraphics->SetSmoothingMode(SmoothingModeHighQuality); 

    SolidBrush bgbrush(Color(238, 243, 250));
    pGraphics->FillRectangle(&bgbrush, 0, 0, rcCanvas.Width(), rcCanvas.Height());

    if ( m_pImage )
    {
        //获取属性 
        INT nImageWidth  = m_pImage->GetWidth();
        INT nImageHeight = m_pImage->GetHeight(); 


        float fMin = 1.0F;  
        bool bZoomOut = false; 
        int nXSrc = 0;
        int nYSrc = 0;  
         
        //构造位置
        RectF rcDrawRect;  
        rcDrawRect.X = rcDraw.left;
        rcDrawRect.Y = rcDraw.top;
        rcDrawRect.Width = rcDraw.Width();
        rcDrawRect.Height = rcDraw.Height(); 

        Matrix mtr;
        mtr.Translate(rcDrawRect.X + rcDrawRect.Width / 2, rcDrawRect.Y + rcDrawRect.Height / 2);
        mtr.Rotate(m_nRotateAngle);
        mtr.Translate(-(rcDrawRect.X + rcDrawRect.Width / 2), -(rcDrawRect.Y + rcDrawRect.Height / 2));

        pGraphics->SetTransform(&mtr);

        //绘画图像 
        //pGraphics->DrawImage(m_pImage, rcDrawRect, nXSrc, nYSrc, (REAL)nImageWidth - nXSrc * 2, (REAL)nImageHeight - nYSrc * 2, UnitPixel,&Attributes);  
        pGraphics->DrawImage(
            m_pImage, rcDrawRect,
            nXSrc, nYSrc,
            (REAL)nImageWidth - nXSrc * 2, (REAL)nImageHeight - nYSrc * 2, UnitPixel);  
        pGraphics->ResetTransform();   
        //pGraphics->EndContainer(Containter);

    } // if ( m_pImage )

    // 从内存拷贝至设备
    Graphics graphicsreal(pDC->GetSafeHdc()); 
    graphicsreal.DrawImage(&bmp, 0, 0, rcCanvas.Width(), rcCanvas.Height()); 
    delete pGraphics;
    pGraphics = NULL;

    return true;
}


bool CGdiPlusImage::SetPos(int nXPos, int nYPos)
{
    m_nXPos = nXPos;
    m_nYPos = nYPos;

    return true;
}

void CGdiPlusImage::SetDest(CRect& rect)
{
    m_rcDest = rect;
}

void CGdiPlusImage::Reset()
{
    m_szZoom.cx = m_szZoom.cy = 1.0f;
    m_nRotateAngle = 0; 
    m_fZoomRate = 1.0F; 
}

void CGdiPlusImage::MoveTo(POINT& point)
{ 
    m_ptLeftTop.x += point.x;
    m_ptLeftTop.y += point.y; 
}

void CGdiPlusImage::SetRotation( int nAngle)
{
    m_nRotateAngle = nAngle;
}

void CGdiPlusImage::SetScale(float cx,float cy)
{
    m_szZoom.cx = cx;
    m_szZoom.cy = cy;
}

void CGdiPlusImage::SetAlpha( BYTE cbAlphaDepth )
{
    if ( cbAlphaDepth < 0 ) m_cbAlphaDepth = 0;
    if ( cbAlphaDepth > 255 ) m_cbAlphaDepth = 255;

    m_cbAlphaDepth = cbAlphaDepth;
}

int CGdiPlusImage::GetEncoderClsid(LPCTSTR format, CLSID* pClsid)
{
    UINT  num = 0;          
    UINT  size = 0;        

    ImageCodecInfo* pImageCodecInfo = NULL;   

    GetImageEncodersSize(&num, &size);   
    if(size == 0)   
        return -1;  

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));   
    if(pImageCodecInfo == NULL)   
        return -1;  

    GetImageEncoders(num, size, pImageCodecInfo);   

    for(UINT j = 0; j < num; ++j)   
    {   
        if( StrCmp(pImageCodecInfo[j].MimeType, format) == 0 )   
        {   
            *pClsid = pImageCodecInfo[j].Clsid;   
            free(pImageCodecInfo);   
            return j; 
        }       
    }   

    free(pImageCodecInfo);   
    return -1;  
}

bool CGdiPlusImage::Save(LPCTSTR pszFileName,LPCTSTR format)
{
    //加载文件
    CT2CW strFileName(pszFileName);

    //图像保存
    CLSID Clsid;
    GetEncoderClsid(format, &Clsid);
    if( m_pImage->Save((LPCWSTR)strFileName, &Clsid) == Ok )
        return true;

    return false;
}