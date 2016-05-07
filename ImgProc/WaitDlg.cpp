// WaitDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImgProc.h"
#include "WaitDlg.h"
#include "afxdialogex.h"


// CWaitDlg �Ի���

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


// CWaitDlg ��Ϣ�������


BOOL CWaitDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    CRect rcClient;
    GetClientRect(rcClient);

    int nWidth = GetSystemMetrics(SM_CXSCREEN);
    int nHeight = GetSystemMetrics(SM_CYSCREEN);

    MoveWindow((nWidth - rcClient.Width()) / 2, (nHeight - rcClient.Height()) / 2, rcClient.Width(), rcClient.Height());

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CWaitDlg::PreTranslateMessage(MSG* pMsg)
{ 
    //����ESC�رմ���/
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE ) return TRUE;

    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN && pMsg->wParam) return TRUE;
    else
        return CDialog::PreTranslateMessage(pMsg); 
}
