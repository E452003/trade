// StrategyLimitupSale.h : StrategyLimitupSale DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStrategyLimitupSaleApp
// �йش���ʵ�ֵ���Ϣ������� StrategyLimitupSale.cpp
//

class CStrategyLimitupSaleApp : public CWinApp
{
public:
	CStrategyLimitupSaleApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
