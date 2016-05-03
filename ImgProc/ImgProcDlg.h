
// ImgProcDlg.h : ͷ�ļ�
//
#include "EventHanlders.h" 
#include "GdiPlusImage.h"
#include "PicDlg.h"
#include "UINotifier.h"
#pragma once



// CImgProcDlg �Ի���
class CImgProcDlg : public CDialogEx, public IUINotifier
{
// ����
public:
	CImgProcDlg(CWnd* pParent = NULL);	// ��׼���캯��
    CImgProcDlg(IUIEventHandler* pUIEventHandler, CWnd* pParent = NULL);	// ��׼���캯��
    virtual ~CImgProcDlg();

// �Ի�������
	enum { IDD = IDD_IMGPROC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��




protected:
    IUIEventHandler* m_pUIEventHandler;
    CRect m_rcImgArea;
    CRect m_rcToolArea;
    CRect m_rcOpenBtn;
    CRect m_rcZoomBtn;
    CRect m_rcZhuanBtn;
    CRect m_rcZhuanLeft;
    CRect m_rcZoomRate;
    CRect m_rcCutSaveBtn;

    int   m_nBottomHeight;   // �ײ�����߶�

    CString m_strFileName;
    CGdiPlusImage m_Image;
    CString m_strZoomRate;

    float m_fZoomRate;   



    
protected:
    void LayoutUI();
    void DrawBottomBk();
    bool OpenFile(CString& strFileName);
    void DrawImg();
    void DrawZoomRate();
    


public:
    // -----------IUINotifier------------------------
    void RedrawUI();
    void RedrawImg();

    void SetWindowTitle(LPCTSTR lpTitle);
    void SetZoomRate(LPCTSTR lpZoomRate);
    void SetZoomRate(float fRate); 






// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnBnClickedBtnOpen();
    afx_msg void OnBnClickedBtnZoom();
    afx_msg void OnBnClickedBtnZhuan();
    afx_msg void OnBnClickedBtnZhuanLeft();
    afx_msg void OnBnClickedBtnCutSave();
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};
