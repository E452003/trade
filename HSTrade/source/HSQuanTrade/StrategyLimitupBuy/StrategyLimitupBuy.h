// StrategyLimitupBuy.h : StrategyLimitupBuy DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStrategyLimitupBuyApp
// �йش���ʵ�ֵ���Ϣ������� StrategyLimitupBuy.cpp
//

class CStrategyLimitupBuyApp : public CWinApp
{
public:
	CStrategyLimitupBuyApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
