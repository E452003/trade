// StrategyLimitupSale.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "StrategyLimitupSale.h"
#include "..\\HSQuanTrade\\StrategyBase.h"
#include "StrategyLimitupSale_imp.h"

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

// CStrategyLimitupSaleApp

BEGIN_MESSAGE_MAP(CStrategyLimitupSaleApp, CWinApp)
END_MESSAGE_MAP()


// CStrategyLimitupSaleApp ����

CStrategyLimitupSaleApp::CStrategyLimitupSaleApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CStrategyLimitupSaleApp ����

CStrategyLimitupSaleApp theApp;


// CStrategyLimitupSaleApp ��ʼ��

BOOL CStrategyLimitupSaleApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

extern "C" _declspec(dllexport) long Interface(CStrategyBase ** pTradeBase)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pTradeBase = new CStrategyLimitupSale();

	return 0;
}