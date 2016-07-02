#pragma once
#include "HundsunMdCenterApi.h"
#include "public.h"
#include <atomic> 
#include <string>
#include "typedefine.h"

class CStrategyManager;

class CMdSpi : public CHundsunMdCenterSpi
{
public:
	CMdSpi(CHundsunMdCenterApi* lpObj)
	{
		lpUserApi = lpObj;
		m_RequestID = 0;
	}

	bool IsErrorRspInfo(CHundsunFtdcRspInfoField *pRspInfo);

	CStrategyManager*m_MdStrategy;

private:
	CHundsunMdCenterApi* lpUserApi;

	int m_RequestID;

	void SubscribeMarketData();

	//��Լ��Ϣ
	map<pair<int, string>, CHundsunFtdcInstrumentField*> m_InstrumentMap; 

public:

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
    virtual void OnFrontConnected();

    ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
    virtual void OnFrontDisconnected(int nReason);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData);

	///K������֪ͨ
	virtual void OnRtnKLineData(CHundsunKLineDataField *pKLineData) {};

	///��ѯ��Լ֪ͨ
	virtual void OnRspQryInstrument(CHundsunFtdcInstrumentField *pInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


public:

	char ** m_ppInstrumentID;

	std::atomic_int  m_nInsNum;

	std::map<string,int> m_UfxCode;				   //ufx�ӿڶ��ĵĺ�Լ

	char m_szDate[16];
};
