// StrategyArbitrage.h : StrategyArbitrage DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStrategyArbitrageApp
// �йش���ʵ�ֵ���Ϣ������� StrategyArbitrage.cpp
//

class CStrategyArbitrageApp : public CWinApp
{
public:
	CStrategyArbitrageApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
