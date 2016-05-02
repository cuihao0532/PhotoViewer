
// ImgProc.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ImgProc.h"
#include "ImgProcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImgProcApp

BEGIN_MESSAGE_MAP(CImgProcApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CImgProcApp ����

CImgProcApp::CImgProcApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
    m_pUIEventHandler = NULL;
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CImgProcApp ����

CImgProcApp theApp;


// CImgProcApp ��ʼ��

BOOL CImgProcApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

    bool bRet = Init(); 
	if ( bRet )
	{ 
	}
	else  
	{ 
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

bool CImgProcApp::Init()
{
    m_pUIEventHandler = new CUIEventHandler();
    CImgProcDlg dlg(m_pUIEventHandler);
    m_pUIEventHandler->SetUINotifier(&dlg);
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();

    bool bRet = false;

    if (nResponse == IDOK)
    {
        bRet = true;
    }
    else if (nResponse == IDCANCEL)
    {
        bRet = false;
    }

    return bRet;
}

bool CImgProcApp::Uninit()
{
    if ( m_pUIEventHandler )
    {
        delete m_pUIEventHandler;
        m_pUIEventHandler = NULL;
    }

    return true;
}



int CImgProcApp::ExitInstance()
{
    Uninit();
    GdiplusShutdown(m_gdiplusToken); 
    return CWinApp::ExitInstance();
}
