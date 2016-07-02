#include "HundsunH5sdkApi.h"
#include "public.h"
#include "boost\date_time\gregorian\gregorian.hpp"

using namespace boost::gregorian;

const char* GetHSExchagneType(const char* pszHqTypeCode);
const char* GetCandlePeriodName(int iCandlePeriod);

class CH5sdkSpi : public CHundsunH5sdkSpi
{
public:
	CH5sdkSpi(CHundsunH5sdkApi* lpObj)
	{
		lpUserApi = lpObj;
		lpLog = NULL;

		m_RequestID = 0;
		b_Query = false;
		memset(szHqTypeCode, 0, sizeof(szHqTypeCode));
		memset(szProdCode, 0, sizeof(szProdCode));
		memset(szQueryDate, 0, sizeof(szQueryDate));
		iCandlePeriod = 0;
	}

	bool IsErrorRspInfo(CHundsunH5sdkRspInfoField *pRspInfo);

	int ReqQryMarket();
	int ReqQryInstrument();

	int ReqH5sdkSnapshot();

	int ReqH5sdkSubscribe();

	int ReqH5sdkTrend();
	int ReqH5sdkCandleByRange();
	int ReqH5sdkTickMin();
	
	bool GetQueryStatus();

	void SetQueryStatus(bool val);
	void SetHqTypeCode(const char* pszHqTypeCode);
	void SetProdCode(const char* pszProdCode);
	void SetCandlePeriod(int val);
	void SetQueryDate(const char* pszDate);
private:
	CHundsunH5sdkApi* lpUserApi;
	CLogInfo* lpLog;

	int m_RequestID;
	bool b_Query;
	char szHqTypeCode[16];
	char szProdCode[32];
	char szQueryDate[9];
	int  iCandlePeriod;
public:
    ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
    virtual void OnH5sdkFrontConnected();

    ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
    virtual void OnH5sdkFrontDisconnected(int nReason);

	///1001 ��������Ӧ��
    virtual void OnRspH5sdkLogin(CHundsunH5sdkRspLoginField *pRspLogin, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///2002 �г�������ϢӦ��
    virtual void OnRspH5sdkMarketTypes(CHundsunH5sdkRspMarketTypesField *pRspMarketTypes, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
    ///2003 �г������Ӧ��
    virtual void OnRspH5sdkMarketReference(CHundsunH5sdkRspMarketReferenceField *pRspMarketReference, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///3001 �������Ӧ��
    virtual void OnRspH5sdkSnapshot(CHundsunH5sdkRspSnapshotField *pRspSnapshot, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///4001 ������ն���Ӧ��
    virtual void OnRspH5sdkSubscribe(CHundsunH5sdkRspSubscribeField *pRspSubscribe, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///6001 ��ָ�������ڻ�ƫ��ȡ��ʱ����Ӧ��
    virtual void OnRspH5sdkTrend(CHundsunH5sdkRspTrendField *pRspTrend, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///6003 ������ʱ�䷶ΧȡK��Ӧ��
    virtual void OnRspH5sdkCandleByRange(CHundsunH5sdkRspCandleByRangeField *pRspCandleByRange, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///6004 ָ�����ӵķֱ�Ӧ��
    virtual void OnRspH5sdkTickMin(CHundsunH5sdkRspTickMinField *pRspTickMin, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

};


