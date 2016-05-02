
// ImgProc.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "UIEventHandler.h" 
#include <GdiPlus.h>
using namespace Gdiplus; 
#pragma comment(lib, "gdiplus.lib")


// CImgProcApp:
// 有关此类的实现，请参阅 ImgProc.cpp
//

class CImgProcApp : public CWinApp
{
public:
	CImgProcApp();

// 重写
public:
	virtual BOOL InitInstance();
    bool    Init();
    bool    Uninit();
    IUIEventHandler* m_pUIEventHandler;
    ULONG_PTR m_gdiplusToken;

// 实现

	DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
};

extern CImgProcApp theApp;