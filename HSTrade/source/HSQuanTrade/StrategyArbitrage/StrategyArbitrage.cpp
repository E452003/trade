// StrategyArbitrage.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "StrategyArbitrage.h"
#include "StrategyArbitrageImpl.h"

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

// CStrategyArbitrageApp

BEGIN_MESSAGE_MAP(CStrategyArbitrageApp, CWinApp)
END_MESSAGE_MAP()


// CStrategyArbitrageApp ����

CStrategyArbitrageApp::CStrategyArbitrageApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CStrategyArbitrageApp ����

CStrategyArbitrageApp theApp;


// CStrategyArbitrageApp ��ʼ��

BOOL CStrategyArbitrageApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

extern "C" _declspec(dllexport) long Interface(CStrategyArbitrageImpl ** pTradeBase)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pTradeBase = new CStrategyArbitrageImpl();

	return 0;
}