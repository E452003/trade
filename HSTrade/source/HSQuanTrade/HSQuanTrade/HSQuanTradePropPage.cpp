// HSQuanTradePropPage.cpp : CHSQuanTradePropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "HSQuanTrade.h"
#include "HSQuanTradePropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CHSQuanTradePropPage, COlePropertyPage)

// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CHSQuanTradePropPage, COlePropertyPage)
END_MESSAGE_MAP()

// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CHSQuanTradePropPage, "HSQUANTRADE.HSQuanTradePropPage.1",
	0xb1b8ac58, 0xc93c, 0x4033, 0xb5, 0xb4, 0x8b, 0x5e, 0x97, 0x95, 0x7b, 0xb9)

// CHSQuanTradePropPage::CHSQuanTradePropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CHSQuanTradePropPage ��ϵͳע�����

BOOL CHSQuanTradePropPage::CHSQuanTradePropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_HSQUANTRADE_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}

// CHSQuanTradePropPage::CHSQuanTradePropPage - ���캯��

CHSQuanTradePropPage::CHSQuanTradePropPage() :
	COlePropertyPage(IDD, IDS_HSQUANTRADE_PPG_CAPTION)
{
}

// CHSQuanTradePropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CHSQuanTradePropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// CHSQuanTradePropPage ��Ϣ�������
