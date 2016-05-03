
// ImgProcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgProc.h"
#include "ImgProcDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImgProcDlg 对话框




CImgProcDlg::CImgProcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImgProcDlg::IDD, pParent)
{
    m_pUIEventHandler = NULL;
    m_nBottomHeight = 50; 
    m_fZoomRate = 1.0F;
    m_nAngle = 0;
    m_bLButtonDown = false; 
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CImgProcDlg::CImgProcDlg(IUIEventHandler* pUIEventHandler, CWnd* pParent /*= NULL*/) : m_pUIEventHandler(pUIEventHandler), CDialogEx(CImgProcDlg::IDD, pParent)
{ 
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_nBottomHeight = 50; 
    m_fZoomRate = 1.0F; 
    m_nAngle = 0;
    m_bLButtonDown = false;  
}



CImgProcDlg::~CImgProcDlg()
{
}

void CImgProcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImgProcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BTN_OPEN, &CImgProcDlg::OnBnClickedBtnOpen)
    ON_BN_CLICKED(IDC_BTN_ZOOM, &CImgProcDlg::OnBnClickedBtnZoom)
    ON_BN_CLICKED(IDC_BTN_ZHUAN_RIGHT, &CImgProcDlg::OnBnClickedBtnZhuan)
    ON_BN_CLICKED(IDC_BTN_ZHUAN_LEFT, &CImgProcDlg::OnBnClickedBtnZhuanLeft)
    ON_BN_CLICKED(IDC_BTN_CUT_SAVE, &CImgProcDlg::OnBnClickedBtnCutSave)
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CImgProcDlg 消息处理程序

bool CImgProcDlg::OpenFile(CString& strFileName)
{  
    CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
        NULL, 
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("JPG Files (*.jpg)|*.jpg|All Files (*.*)|*.*||"),
        NULL);
    if (dlg.DoModal() == IDOK)
    {
        strFileName = dlg.GetPathName();  
        return true; 
    }
    else
    {
        return false;
    } 
}

// 画图
void CImgProcDlg::DrawImg()
{ 
    if ( m_pUIEventHandler )
    {
        m_pUIEventHandler->DrawImage();
    }

    // 不画边界（PIC控件边缘）
    CRgn rgn;
    rgn.CreateRectRgn(m_rcImgArea.left + 1, m_rcImgArea.top + 1, m_rcImgArea.Width() - 1, m_rcImgArea.Height() - 1); 
 
    rgn.DeleteObject(); 

}

// 画 放大倍数
void CImgProcDlg::DrawZoomRate()
{ 
    GetDlgItem(IDC_STATIC_ZOOM_RATE)->SetWindowText(m_strZoomRate);  
}

void CImgProcDlg::RedrawUI()
{
    InvalidateRect(m_rcImgArea); 
    InvalidateRect(m_rcZoomRate);
   // InvalidateRect(m_rcToolArea); 
}

void CImgProcDlg::RedrawImg()
{
    InvalidateRect(m_rcImgArea);
}

void CImgProcDlg::SetWindowTitle(LPCTSTR lpTitle)
{

}

void CImgProcDlg::SetZoomRate(LPCTSTR lpZoomRate)
{ 
    m_strZoomRate = CString(lpZoomRate);    
}

void CImgProcDlg::SetZoomRate(float fRate)
{
    TCHAR szBuf[20] = { 0 };
    _stprintf(szBuf, TEXT("%.1fx"), fRate); 
    m_strZoomRate = CString(szBuf);
}

// 布局UI
void CImgProcDlg::LayoutUI()
{
    CRect rcClient;
    GetClientRect(rcClient);

    m_rcImgArea.left = 0;
    m_rcImgArea.top = 0;
    m_rcImgArea.right = rcClient.Width();
    m_rcImgArea.bottom = rcClient.Height() - m_nBottomHeight;

    m_rcToolArea.left = 0;
    m_rcToolArea.top = m_rcImgArea.bottom;
    m_rcToolArea.right = rcClient.Width();
    m_rcToolArea.bottom = rcClient.Height();

    m_rcOpenBtn.left = 100;
    m_rcOpenBtn.top = m_rcToolArea.top + 10;
    m_rcOpenBtn.right = m_rcOpenBtn.left + 60;
    m_rcOpenBtn.bottom = m_rcToolArea.bottom;

    m_rcZhuanLeft = m_rcOpenBtn;
    m_rcZhuanLeft.left = m_rcOpenBtn.right + 20;
    m_rcZhuanLeft.right = m_rcZhuanLeft.left + 60; 
     
    m_rcZhuanBtn = m_rcZhuanLeft;
    m_rcZhuanBtn.left = m_rcZhuanLeft.right + 20;
    m_rcZhuanBtn.right = m_rcZhuanBtn.left + 60;


    m_rcCutSaveBtn = m_rcZhuanBtn;
    m_rcCutSaveBtn.left = m_rcZhuanBtn.right + 20;
    m_rcCutSaveBtn.right = m_rcCutSaveBtn.left + 60;

    m_rcZoomRate = m_rcZhuanBtn;
    m_rcZoomRate.right = rcClient.Width() - 50;
    m_rcZoomRate.left  = m_rcZoomRate.right - 60;
     
    GetDlgItem(IDC_STATIC_PIC)->MoveWindow(m_rcImgArea); 
    GetDlgItem(IDC_STATIC_ZOOM_RATE)->MoveWindow(m_rcZoomRate);
    GetDlgItem(IDC_BTN_OPEN)->MoveWindow(m_rcOpenBtn); 
    GetDlgItem(IDC_BTN_ZHUAN_LEFT)->MoveWindow(m_rcZhuanLeft);
    GetDlgItem(IDC_BTN_ZHUAN_RIGHT)->MoveWindow(m_rcZhuanBtn); 
    GetDlgItem(IDC_BTN_CUT_SAVE)->MoveWindow(m_rcCutSaveBtn);
    
    // 重新为画图区域设置大小
    m_Image.SetDest(m_rcImgArea);
}

BOOL CImgProcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog(); 
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
     
	// TODO: 在此添加额外的初始化代码
    LayoutUI();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CImgProcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
 
void CImgProcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	} 
     
   
    DrawImg();
    DrawZoomRate(); 
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CImgProcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// 清除背景时，特别处理：对需要重绘的区域使用给定画刷擦除
BOOL CImgProcDlg::OnEraseBkgnd(CDC* pDC)
{    
    CRect rc;
    GetUpdateRect(&rc);  
    Bitmap bmp(rc.Width(), rc.Height());

    Graphics* pGraph = Graphics::FromImage(&bmp); 
    SolidBrush bgbrush(Color(238, 243, 250));  ; 
    pGraph->FillRectangle(&bgbrush, 0, 0, rc.Width(), rc.Height()); 

    Graphics graphics(pDC->GetSafeHdc()); 
    graphics.DrawImage(&bmp, rc.left, rc.top, rc.Width(), rc.Height());  

    delete pGraph;
    pGraph = NULL; 

    return TRUE; 
}


HBRUSH CImgProcDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{ 
    // 放大倍数 PIC控件，写文字时背景透明
    if ( IDC_STATIC_ZOOM_RATE == pWnd->GetDlgCtrlID() )
    {
        pDC->SetBkMode(TRANSPARENT); 
    }
    static HBRUSH hbr = CreateSolidBrush(BK_COLOR1);
    return hbr; 
}


void CImgProcDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{ 
    CDialogEx::OnLButtonDblClk(nFlags, point);
    if ( m_pUIEventHandler )
    {
        m_pUIEventHandler->OnLButtonDblClk(nFlags, point);
    }
}


void CImgProcDlg::OnLButtonDown(UINT nFlags, CPoint point)
{ 
    CDialogEx::OnLButtonDown(nFlags, point);
    if ( m_pUIEventHandler )
    {
        m_pUIEventHandler->OnLButtonDown(nFlags, point);
    }
    m_bLButtonDown = true;
    m_ptCur = point; 
}


void CImgProcDlg::OnLButtonUp(UINT nFlags, CPoint point)
{ 
    CDialogEx::OnLButtonUp(nFlags, point);
    if ( m_pUIEventHandler )
    {
        m_pUIEventHandler->OnLButtonUp(nFlags, point);
    }
    m_bLButtonDown = false;
    m_ptCur = point;

    InvalidateRect(m_rcToolArea);  
}


void CImgProcDlg::OnMouseMove(UINT nFlags, CPoint point)
{ 
    CDialogEx::OnMouseMove(nFlags, point);
    if ( m_pUIEventHandler )
    {
        m_pUIEventHandler->OnMouseMove(nFlags, point);
    }

    if ( m_bLButtonDown )
    {
        POINT pt;
        pt.x = point.x - m_ptCur.x;
        pt.y = point.y - m_ptCur.y;
        m_Image.MoveTo(pt);  
         
        InvalidateRect(m_rcImgArea);
        InvalidateRect(m_rcZoomRate);
        InvalidateRect(m_rcToolArea); 
        m_ptCur = point;
    }
}


BOOL CImgProcDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{ 
    BOOL bRet = FALSE;
    bRet = CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
    if ( m_pUIEventHandler )
    {
        bRet = m_pUIEventHandler->OnMouseWheel(nFlags, zDelta, pt);
    }

    ScreenToClient(&pt);
    if ( m_rcImgArea.PtInRect(pt ))
    {
        if ( m_Image.IsNull() )
            return TRUE;

        if ( zDelta > 0 )
        {
            if ( m_fZoomRate >= 4.99F )
            {
                m_fZoomRate = 5.0F;  
                return TRUE;
            }
            else
            {
                m_fZoomRate += 0.2F; 
            } 
        }
        else
        {
            if ( m_fZoomRate <= 0.21F )
            {
                m_fZoomRate = 0.2F;
                return TRUE;
            }
            else
            {
                m_fZoomRate -= 0.2F; 
            }
        }

        m_Image.SetScale(m_fZoomRate, m_fZoomRate);  
        InvalidateRect(m_rcImgArea); 
        InvalidateRect(m_rcZoomRate);
        InvalidateRect(m_rcToolArea); 
    }
    
    return bRet;
}


void CImgProcDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    LayoutUI();  

    if ( m_pUIEventHandler )
    { 
        m_pUIEventHandler->SetImgDest(m_rcImgArea);
        m_pUIEventHandler->SetZoomRateDest(m_rcZoomRate);
        m_pUIEventHandler->OnSize(nType, cx, cy);
    } 
}


void CImgProcDlg::OnBnClickedBtnOpen()
{
    CString strFileName;
    if ( OpenFile(strFileName) )
    {
        if (m_pUIEventHandler)
        {
            m_pUIEventHandler->SetImgDest(m_rcImgArea);
            m_pUIEventHandler->SetZoomRateDest(m_rcZoomRate);
            m_pUIEventHandler->OpenFile(strFileName);
            this->SetWindowText(strFileName);
        } 
    }
}


void CImgProcDlg::OnBnClickedBtnZoom()
{  
    m_Image.SetScale(m_fZoomRate, m_fZoomRate);
    InvalidateRect(m_rcImgArea); 

    m_fZoomRate += 0.2F;
    if ( m_fZoomRate >= 10.0F )
    {
        m_fZoomRate = 1.0F;
    }
}


void CImgProcDlg::OnBnClickedBtnZhuan()
{  
    if ( m_pUIEventHandler )
    {
        m_pUIEventHandler->RotationRight();
    }
}


void CImgProcDlg::OnBnClickedBtnZhuanLeft()
{  
    if ( m_pUIEventHandler )
    {
        m_pUIEventHandler->RotationLeft();
    }
}


void CImgProcDlg::OnBnClickedBtnCutSave()
{
    if ( m_Image.IsNull() ) return;
    
    //图像保存
    CLSID Clsid;
    int nRet = m_Image.GetEncoderClsid(TEXT("image/jpeg"), &Clsid);
    if ( nRet < 0 )
        return;

    Bitmap bmp(200, 200); 
    Graphics* pCanvas = Graphics::FromImage(&bmp);
    pCanvas->DrawImage(Image::FromFile(m_strFileName), 0, 0, bmp.GetWidth(), bmp.GetHeight());
    pCanvas->Save();
    bmp.Save(TEXT("F:\\test.jpg"), &Clsid); 
     
    delete pCanvas;
}


void CImgProcDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialogEx::OnRButtonDown(nFlags, point);
}


void CImgProcDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialogEx::OnRButtonUp(nFlags, point);
}
