
// ImgProc.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include "UIEventHandler.h" 
#include <GdiPlus.h>
using namespace Gdiplus; 
#pragma comment(lib, "gdiplus.lib")


// CImgProcApp:
// �йش����ʵ�֣������ ImgProc.cpp
//

class CImgProcApp : public CWinApp
{
public:
	CImgProcApp();

// ��д
public:
	virtual BOOL InitInstance();
    bool    Init();
    bool    Uninit();
    IUIEventHandler* m_pUIEventHandler;
    ULONG_PTR m_gdiplusToken;

// ʵ��

	DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
};

extern CImgProcApp theApp;