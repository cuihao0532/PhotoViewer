#pragma once


// CPicDlg 对话框

class CPicDlg : public CDialog
{
	DECLARE_DYNAMIC(CPicDlg)

public:
	CPicDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPicDlg();

// 对话框数据
	enum { IDD = IDD_PICDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
