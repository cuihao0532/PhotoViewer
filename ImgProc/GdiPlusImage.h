#pragma once
  
#include <GdiPlus.h>
using namespace Gdiplus; 

typedef struct tagSize
{
    float        cx;
    float        cy;
} _SIZE, *_PSIZE, *_LPSIZE;
  
  
class CGdiPlusImage
{
    //��������
protected:
    Image *                         m_pImage;                           //ͼƬ����
  
    tagSize                         m_szZoom;                           //���ű���
    int                             m_nRotateAngle;                     //��ת�Ƕ�
    BYTE                            m_cbAlphaDepth;                     //Alpha��ֵ
    POINT                           m_ptLeftTop;                        // ͼ������Ͻǵ�����
    POINT                           m_ptCurImgCenter;                   // ͼ��ĵ�ǰ���ĵ�
    float                           m_fZoomRate;                        // ���ű���
    int                             m_nShowWidth;                       // ��ʾ���
    int                             m_nShowHeight;                      // ��ʾ�߶�
    bool                            m_bOpenNewImg;                      // �Ƿ����ͼ��

  
    //��������
public:
    //���캯��
    CGdiPlusImage();
    //��������
    virtual ~CGdiPlusImage();
      
    //״̬����
public:
    //�Ƿ����
    bool IsNull();
    //��ȡ���
    INT GetWidth();
    //��ȡ�߶�
    INT GetHeight();
  
    //������
public:
    //����ͼƬ
    bool DestroyImage();
    //����ͼƬ
    bool LoadImage(LPCTSTR pszFileName);
  
    //���ܺ���
public:
    //������ת
    void SetRotation(int nAngle);
    //��������
    void SetScale(float cx,float cy);
    //����Alpha
    void SetAlpha(BYTE cbAlphaDepth);
    //����ͼ��
    bool Save(LPCTSTR pszFileName,LPCTSTR format=TEXT("image/png"));
    //��ȡ������
    int GetEncoderClsid(LPCTSTR format, CLSID* pClsid);
  
    //�滭����
public:
    //�滭ͼ��
    bool DrawImage(CDC * pDC, INT nXPos, INT nYPos);
    bool DrawImage(CDC*  pDC); 
    bool DrawImage(CDC*  pDC, CRect& rcDraw, CRect& rcCanvas); 

    bool SetPos(int nXPos, int nYPos);
    void SetDest(CRect& rect);
    void Reset();
    void MoveTo(POINT& point);

    int m_nXPos;
    int m_nYPos;
    
    CRect m_rcDest;
};