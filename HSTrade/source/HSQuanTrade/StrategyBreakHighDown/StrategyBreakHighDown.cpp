// StrategyBreakHighDown.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "StrategyBreakHighDown.h"

#include "StrategyBkHiDownImpl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CStrategyBreakHighDownApp

BEGIN_MESSAGE_MAP(CStrategyBreakHighDownApp, CWinApp)
END_MESSAGE_MAP()


// CStrategyBreakHighDownApp ����

CStrategyBreakHighDownApp::CStrategyBreakHighDownApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CStrategyBreakHighDownApp ����

CStrategyBreakHighDownApp theApp;


// CStrategyBreakHighDownApp ��ʼ��

BOOL CStrategyBreakHighDownApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

extern "C" _declspec(dllexport) long Interface(CStrategyBkHiDownImpl ** pTradeBase)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pTradeBase = new CStrategyBkHiDownImpl();

	return 0;
}