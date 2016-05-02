// PicDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgProc.h"
#include "PicDlg.h"
#include "afxdialogex.h"


// CPicDlg 对话框

IMPLEMENT_DYNAMIC(CPicDlg, CDialog)

CPicDlg::CPicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicDlg::IDD, pParent)
{

}

CPicDlg::~CPicDlg()
{
}

void CPicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPicDlg, CDialog)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
 
// CPicDlg 消息处理程序


BOOL CPicDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    return TRUE;
    //return CDialog::OnEraseBkgnd(pDC);
}
