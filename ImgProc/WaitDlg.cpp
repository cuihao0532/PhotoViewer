// WaitDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgProc.h"
#include "WaitDlg.h"
#include "afxdialogex.h"


// CWaitDlg 对话框

IMPLEMENT_DYNAMIC(CWaitDlg, CDialog)

CWaitDlg::CWaitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitDlg::IDD, pParent)
{

}

CWaitDlg::~CWaitDlg()
{
}

void CWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaitDlg, CDialog)
END_MESSAGE_MAP()


// CWaitDlg 消息处理程序


BOOL CWaitDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    CRect rcClient;
    GetClientRect(rcClient);

    int nWidth = GetSystemMetrics(SM_CXSCREEN);
    int nHeight = GetSystemMetrics(SM_CYSCREEN);

    MoveWindow((nWidth - rcClient.Width()) / 2, (nHeight - rcClient.Height()) / 2, rcClient.Width(), rcClient.Height());

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


BOOL CWaitDlg::PreTranslateMessage(MSG* pMsg)
{ 
    //屏蔽ESC关闭窗体/
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE ) return TRUE;

    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN && pMsg->wParam) return TRUE;
    else
        return CDialog::PreTranslateMessage(pMsg); 
}
