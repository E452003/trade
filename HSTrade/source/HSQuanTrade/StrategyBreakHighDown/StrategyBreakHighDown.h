// StrategyBreakHighDown.h : StrategyBreakHighDown DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStrategyBreakHighDownApp
// �йش���ʵ�ֵ���Ϣ������� StrategyBreakHighDown.cpp
//

class CStrategyBreakHighDownApp : public CWinApp
{
public:
	CStrategyBreakHighDownApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
