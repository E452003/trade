// StragyGrid.h : StragyGrid DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStragyGridApp
// �йش���ʵ�ֵ���Ϣ������� StragyGrid.cpp
//

class CStragyGridApp : public CWinApp
{
public:
	CStragyGridApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
