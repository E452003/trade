// PublicData.h : PublicData DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPublicDataApp
// �йش���ʵ�ֵ���Ϣ������� PublicData.cpp
//

class CPublicDataApp : public CWinApp
{
public:
	CPublicDataApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
