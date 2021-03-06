
// ImgProcDlg.h : 头文件
//
#include "EventHanlders.h" 
#include "GdiPlusImage.h"
#include "PicDlg.h"
#include "UINotifier.h"
#include "WaitDlg.h"
#pragma once



// CImgProcDlg 对话框
class CImgProcDlg : public CDialogEx, public IUINotifier
{
// 构造
public:
	CImgProcDlg(CWnd* pParent = NULL);	// 标准构造函数
    CImgProcDlg(IUIEventHandler* pUIEventHandler, CWnd* pParent = NULL);	// 标准构造函数
    virtual ~CImgProcDlg();

// 对话框数据
	enum { IDD = IDD_IMGPROC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持




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
    CRect m_rcSaveAsBtn;
    CRect m_rcRecLineBtn;

    int   m_nBottomHeight;             // 底部区域高度

    CString m_strFileName;
    CGdiPlusImage m_Image;
    CString m_strZoomRate;

    float m_fZoomRate;   

    CPoint m_ptSelectAreaLeftTop;       // 矩形区域的左上角 
    CPoint m_ptSelectAreaRightBottom;   // 矩形区域的右下角

    vector<CPoint> m_vecPoints;
    vector< vector<CPoint> > m_vvecPoints;

    CWaitDlg* m_pWaitDlg;


    
protected:
    void LayoutUI();
    void DrawBottomBk();
    bool OpenFile(CString& strFileName);
    void DrawImg();
    void DrawZoomRate();
    void DrawRectangle();
    void DrawRecLine();
    

    void ShowWaitUI(BOOL bShow = TRUE);


public:
    // -----------IUINotifier------------------------
    void RedrawUI(BOOL bRedrawRightNow = FALSE);
    void RedrawImg(BOOL bRedrawRightNow = FALSE);

    void SetWindowTitle(LPCTSTR lpTitle);
    void SetZoomRate(LPCTSTR lpZoomRate);
    void SetZoomRate(float fRate);  
    void SetRectangle(const CPoint& ptLeftTop, const CPoint& ptRightBottom);
    void SetRecLines( std::vector< std::vector<CPoint> >& vvecPoints);
    void ShowWaitDlg(BOOL bShow = TRUE ); 





// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
    afx_msg void OnBnClickedBtnSaveAs();
    afx_msg void OnBnClickedBtnRecLine();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
