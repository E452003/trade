// StrategyBreakHigh.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "StrategyBreakHigh.h"
#include "..\\HSQuanTrade\\StrategyBase.h"
#include "StrategyBreakHigh_imp.h"

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

// CStrategyBreakHighApp

BEGIN_MESSAGE_MAP(CStrategyBreakHighApp, CWinApp)
END_MESSAGE_MAP()


// CStrategyBreakHighApp ����

CStrategyBreakHighApp::CStrategyBreakHighApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CStrategyBreakHighApp ����

CStrategyBreakHighApp theApp;


// CStrategyBreakHighApp ��ʼ��

BOOL CStrategyBreakHighApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

extern "C" _declspec(dllexport) long Interface(CStrategyBase ** pTradeBase)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pTradeBase = new CStrategyBreakHigh();

	return 0;
}
