#pragma once

// HSQuanTradePropPage.h : CHSQuanTradePropPage ����ҳ���������


// CHSQuanTradePropPage : �й�ʵ�ֵ���Ϣ������� HSQuanTradePropPage.cpp��

class CHSQuanTradePropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CHSQuanTradePropPage)
	DECLARE_OLECREATE_EX(CHSQuanTradePropPage)

// ���캯��
public:
	CHSQuanTradePropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_HSQUANTRADE };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

