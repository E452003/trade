
// TestOCX.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestOCXApp:
// �йش����ʵ�֣������ TestOCX.cpp
//

class CTestOCXApp : public CWinApp
{
public:
	CTestOCXApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestOCXApp theApp;