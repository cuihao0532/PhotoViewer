// PicDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImgProc.h"
#include "PicDlg.h"
#include "afxdialogex.h"


// CPicDlg �Ի���

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
 
// CPicDlg ��Ϣ�������


BOOL CPicDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    return TRUE;
    //return CDialog::OnEraseBkgnd(pDC);
}
