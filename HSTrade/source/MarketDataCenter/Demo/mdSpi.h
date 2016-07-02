#include "HundsunMdCenterApi.h"
#include "public.h"

class CDBSpi;

class CMdSpi : public CHundsunMdCenterSpi
{
public:
	CMdSpi(CHundsunMdCenterApi* lpObj, CDBSpi* lpDb)
	{
		lpUserApi = lpObj;
		lpUserDBSpi = lpDb;
		m_RequestID = 0;
	}

	bool IsErrorRspInfo(CHundsunFtdcRspInfoField *pRspInfo);

	void SubAll();
private:
	CHundsunMdCenterApi* lpUserApi;
	CDBSpi* lpUserDBSpi;
	int m_RequestID;

	//��Լ��Ϣ
	map<pair<int, string>, CHundsunFtdcInstrumentField*> m_InstrumentMap; 

	//��ú�Լ��Ϣ
	CHundsunFtdcInstrumentField* GetInstrument(THundsunExchangeType ExchangeID, THundsunFtdcInstrument pszInstrument);
	//���º�Լ��Ϣ
	void UpdateInstrument(CHundsunFtdcInstrumentField *pInstrument);
public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
    virtual void OnFrontConnected();

    ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
    virtual void OnFrontDisconnected(int nReason);

	///��¼������Ӧ
	virtual void OnRspUserLogin(CHundsunFtdcRspUserLoginField *pRspUserLogin, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CHundsunFtdcInstrumentField *pInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData);

	///K������֪ͨ
	virtual void OnRtnKLineData(CHundsunKLineDataField *pKLineData) {};
};
