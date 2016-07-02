#include "HundsunMdCenterApi.h"
#include "MarkDataTraderApi.h"
#include "public.h"

class CDBSpi : public CMarkDataTraderSpi
{
public:
	CDBSpi(CMarkDataTraderApi* lpObj)
	{
		lpUserApi = lpObj;
		m_RequestID = 0;
		bConnected = false;
	}

	bool IsErrorRspInfo(CMarkDataRspInfoField *pRspInfo);

	int MarkdataInsert(CHundsunDepthMarketDataField *pDepthMarketData);
private:
	CMarkDataTraderApi* lpUserApi;
	int m_RequestID;
	bool bConnected;
public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
    virtual void OnFrontConnected();

    ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
    virtual void OnFrontDisconnected(int nReason);

	///338499 ����ؿ�Ӧ��
    virtual void OnRspFuncMarkdatainsert(CMarkDataRspFuncMarkdatainsertField *pRspFuncMarkdatainsert, CMarkDataRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
};
