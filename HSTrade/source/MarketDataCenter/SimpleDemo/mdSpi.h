#include "HundsunMdCenterApi.h"
#include <iostream>
#include <string>

using namespace std;

class CMdSpi : public CHundsunMdCenterSpi
{
public:
	CMdSpi(CHundsunMdCenterApi* lpObj)
	{
		lpUserApi = lpObj;
		m_RequestID = 0;
	}

	bool IsErrorRspInfo(CHundsunFtdcRspInfoField *pRspInfo);
private:
	CHundsunMdCenterApi* lpUserApi;
	int m_RequestID;
public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
    virtual void OnFrontConnected();

    ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
    virtual void OnFrontDisconnected(int nReason);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData);

	///K������֪ͨ
	virtual void OnRtnKLineData(CHundsunKLineDataField *pKLineData) {};
};
