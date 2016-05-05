
#include "StdAfx.h"
#include "GdiPlusImage.h"

//���캯��
CGdiPlusImage::CGdiPlusImage(void)
{
    //���ñ���
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

//��������
CGdiPlusImage::~CGdiPlusImage(void)
{
    //����ͼƬ
    DestroyImage(); 
}

//�Ƿ����
bool CGdiPlusImage::IsNull()
{
    //�����ж�
    if (m_pImage==NULL) return true;
    if (m_pImage->GetLastStatus()!=Ok) return true;

    return false;
}

//��ȡ���
INT CGdiPlusImage::GetWidth()
{
    //�����ж�
    ASSERT(IsNull()==false);
    if (IsNull()==true) return 0;

    //��ȡ���
    return m_pImage->GetWidth();
}

//��ȡ�߶�
INT CGdiPlusImage::GetHeight()
{
    //�����ж�
    ASSERT(IsNull()==false);
    if (IsNull()==true) return 0;

    //��ȡ�߶�
    return m_pImage->GetHeight();
}

//����ͼƬ
bool CGdiPlusImage::DestroyImage()
{
    //ɾ������
    if (m_pImage!=NULL) 
    { 
        delete m_pImage;
        m_pImage = NULL;
    }

    return true;
}

//����ͼƬ
bool CGdiPlusImage::LoadImage(LPCTSTR pszFileName)
{
    // �����ж�
    ASSERT(m_pImage == NULL);
    if (m_pImage != NULL) return false;

    // �����ļ�
    CT2CW strFileName(pszFileName);
    m_pImage = Image::FromFile((LPCWSTR)strFileName); 

    // �����ж�
    if ((m_pImage==NULL)||(m_pImage->GetLastStatus()!=Ok)) 
    {
        DestroyImage();
        return false;
    }

    return true;
}

bool CGdiPlusImage::LoadImage(IStream* stream)
{
    // �����ж�
    ASSERT(m_pImage == NULL);
    if (m_pImage != NULL || !stream) return false;

    // �����ļ� 
    m_pImage = Image::FromStream(stream); 

    // �����ж�
    if ((m_pImage==NULL)||(m_pImage->GetLastStatus()!=Ok)) 
    {
        DestroyImage();
        return false;
    }

    return true;
}

//�滭ͼ��
bool CGdiPlusImage::DrawImage(CDC * pDC, INT nXPos, INT nYPos)
{
    //�����ж�
    //ASSERT(m_pImage!=NULL);
    if (m_pImage==NULL) return false;

    //������Ļ
    ASSERT(pDC!=NULL);
    Graphics graphics(pDC->GetSafeHdc());

    GraphicsContainer Containter = graphics.BeginContainer();
    graphics.SetSmoothingMode(SmoothingModeHighQuality);

    //��ȡ����
    INT nImageWidth=m_pImage->GetWidth();
    INT nImageHeight=m_pImage->GetHeight();

    //����λ��
    RectF rcDrawRect;
    rcDrawRect.X=0;//(REAL)nXPos;
    rcDrawRect.Y=0;//(REAL)nYPos;
    rcDrawRect.Width=(REAL)nImageWidth;
    rcDrawRect.Height=(REAL)nImageHeight;

    Matrix matrix;

    CPoint pt((nXPos+nImageWidth/2), (nYPos+nImageHeight/2));
    PointF ptBase((float)pt.x,(float)pt.y);

    //����任Ҫע��˳����ƽ�������ź���ת
    matrix.Translate((REAL)nXPos,(REAL)nYPos,MatrixOrderAppend);

    //����
    if ( m_szZoom.cx != 1.0f || m_szZoom.cy != 1.0f )
    {
        ptBase.X += m_szZoom.cx;
        ptBase.Y += m_szZoom.cy;

        matrix.Translate(-ptBase.X,-ptBase.Y,MatrixOrderAppend);
        matrix.Scale(m_szZoom.cx,m_szZoom.cy,MatrixOrderAppend);
        matrix.Translate(ptBase.X,ptBase.Y,MatrixOrderAppend);
    }

    //��ת
    if (m_nRotateAngle)
    {
        matrix.RotateAt((REAL)m_nRotateAngle,ptBase,MatrixOrderAppend);
    }

    graphics.SetTransform(&matrix);

    //͸������
    ColorMatrix Matrix=
    {
        1.0f,0.0f,0.0f,0.0f,0.0f, 
        0.0f,1.0f,0.0f,0.0f,0.0f, 
        0.0f,0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,0.0f,m_cbAlphaDepth/255.0f,0.0f, 
        0.0f,0.0f,0.0f,0.0f,1.0f
    };

    //��������
    ImageAttributes Attributes; 
    Attributes.SetColorMatrix(&Matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap); 

    //�滭ͼ��
    graphics.DrawImage(m_pImage,rcDrawRect,0,0,(REAL)nImageWidth,(REAL)nImageHeight,UnitPixel,&Attributes);

    graphics.ResetTransform();

    graphics.EndContainer(Containter);

    return true;
}
 

bool CGdiPlusImage::DrawImage(CDC* pDC)
{
    //������Ļ
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
        //��ȡ���� 
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


        //����λ��
        RectF rcDrawRect;  
        rcDrawRect.X = m_rcDest.left + (m_rcDest.Width() - nDestWidth ) / 2;   
        rcDrawRect.Y = m_rcDest.top + (m_rcDest.Height() - nDestHeight ) / 2; 
        rcDrawRect.Width = nDestWidth;
        rcDrawRect.Height = nDestHeight; 

        Matrix matrix;

        CPoint pt(m_rcDest.Width() / 2, m_rcDest.Height() / 2);
        PointF ptBase((float)pt.x, (float)pt.y);  

        //����任Ҫע��˳����ƽ�������ź���ת
        matrix.Translate((REAL)m_rcDest.left,(REAL)m_rcDest.top,MatrixOrderAppend);

        //����
        if ( m_szZoom.cx != 1.0f || m_szZoom.cy != 1.0f )
        {
            ptBase.X += m_szZoom.cx;
            ptBase.Y += m_szZoom.cy;

            matrix.Translate(-ptBase.X,-ptBase.Y,MatrixOrderAppend);
            matrix.Scale(m_szZoom.cx,m_szZoom.cy,MatrixOrderAppend);
            matrix.Translate(ptBase.X,ptBase.Y,MatrixOrderAppend);
        }

        //��ת
        if (m_nRotateAngle)
        {
            matrix.RotateAt((REAL)m_nRotateAngle,ptBase,MatrixOrderAppend);
        }

        pGraphics->SetTransform(&matrix);

        //͸������
        ColorMatrix Matrix=
        {
            1.0f,0.0f,0.0f,0.0f,0.0f, 
            0.0f,1.0f,0.0f,0.0f,0.0f, 
            0.0f,0.0f,1.0f,0.0f,0.0f,
            0.0f,0.0f,0.0f,m_cbAlphaDepth/255.0f,0.0f, 
            0.0f,0.0f,0.0f,0.0f,1.0f
        };

        //��������
        ImageAttributes Attributes; 
        Attributes.SetColorMatrix(&Matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap); 

        //�滭ͼ��
        pGraphics->DrawImage(m_pImage, rcDrawRect, nXSrc, nYSrc, (REAL)nImageWidth - nXSrc * 2, (REAL)nImageHeight - nYSrc * 2, UnitPixel,&Attributes);  
        pGraphics->ResetTransform();   
        pGraphics->EndContainer(Containter);

    } // if ( m_pImage )
   
    // ���ڴ濽�����豸
    Graphics graphicsreal(pDC->GetSafeHdc()); 
    graphicsreal.DrawImage(&bmp, 0, 0, m_rcDest.Width(), m_rcDest.Height()); 
    delete pGraphics;
    pGraphics = NULL;

    return true;
}

 
bool CGdiPlusImage::DrawImage(CDC* pDC, CRect& rcDraw, CRect& rcCanvas)
{
    //������Ļ
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
        //��ȡ���� 
        INT nImageWidth  = m_pImage->GetWidth();
        INT nImageHeight = m_pImage->GetHeight(); 


        float fMin = 1.0F;  
        bool bZoomOut = false; 
        int nXSrc = 0;
        int nYSrc = 0;  
         
        //����λ��
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

        //�滭ͼ�� 
        //pGraphics->DrawImage(m_pImage, rcDrawRect, nXSrc, nYSrc, (REAL)nImageWidth - nXSrc * 2, (REAL)nImageHeight - nYSrc * 2, UnitPixel,&Attributes);  
        pGraphics->DrawImage(
            m_pImage, rcDrawRect,
            nXSrc, nYSrc,
            (REAL)nImageWidth - nXSrc * 2, (REAL)nImageHeight - nYSrc * 2, UnitPixel);  
        pGraphics->ResetTransform();   
        //pGraphics->EndContainer(Containter);

    } // if ( m_pImage )

    // ���ڴ濽�����豸
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
    //�����ļ�
    CT2CW strFileName(pszFileName);

    //ͼ�񱣴�
    CLSID Clsid;
    GetEncoderClsid(format, &Clsid);
    if( m_pImage->Save((LPCWSTR)strFileName, &Clsid) == Ok )
        return true;

    return false;
}