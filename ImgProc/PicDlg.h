#pragma once


// CPicDlg �Ի���

class CPicDlg : public CDialog
{
	DECLARE_DYNAMIC(CPicDlg)

public:
	CPicDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPicDlg();

// �Ի�������
	enum { IDD = IDD_PICDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
