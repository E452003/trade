// StrategyBreakHigh.h : StrategyBreakHigh DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStrategyBreakHighApp
// �йش���ʵ�ֵ���Ϣ������� StrategyBreakHigh.cpp
//

class CStrategyBreakHighApp : public CWinApp
{
public:
	CStrategyBreakHighApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
