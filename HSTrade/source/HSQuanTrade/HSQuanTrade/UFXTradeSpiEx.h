#pragma once
#include "HundsunTraderApi.h"


class CBaseTradeApi;

class CUFXTraderSpiEx : public CHundsunTraderSpi
{
public:
	CUFXTraderSpiEx(CHundsunTraderApi* lpObj)
	{
		lpUserApi = lpObj;
		m_RequestID = 0;
		m_orderref = 0;
	}

	bool IsErrorRspInfo(CHundsunRspInfoField *pRspInfo);
	int ReqClientLogin(char* szFundCount, char* szPassword, char cEntrustWay);
	int ReqFutuContractbailQry();
	int ReqOrderinsert(char* szExchType, char* szFutuCode, char cEntrustBS, char cEntrustDirection, char cHedgeType, int nEntrustAmount, double dEntrustPrice);
	int ReqFuncOrderQry();
	int ReqFuncWithdraw(int EntrustNo);
	int ReqFuncQueryCode();
	int ReqFuncQueryArbCode();
	int ReqArbOrderinsert(char* szExchType, char* szFutuCode, char cEntrustBS, char cEntrustDirection, char cHedgeType, int nEntrustAmount, double dEntrustPrice);

	bool IsMyOrder(int sessionid);


private:
	CHundsunTraderApi* lpUserApi;
	int m_RequestID;
	int m_SessionId;
public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	virtual void OnFrontDisconnected(int nReason);
	//331100 �ͻ���¼Ӧ��
	virtual void OnRspFuncClientLogin(CHundsunRspFuncClientLoginField *pfunc_client_login, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//330851 �ڻ���Լ��֤���ѯӦ��
	virtual void OnRspFuncFutuContractbailQry(CHundsunRspFuncFutuContractbailQryField *pRspfunc_futu_contractbail_qry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//338202 �ڻ�ί��ȷ��Ӧ��
	virtual void OnRspFuncOrderinsert(CHundsunRspFuncOrderinsertField *pRspFuncOrderinsert, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338301 �ڻ�����ί�в�ѯӦ��
    virtual void OnRspFuncOrderQry(CHundsunRspFuncOrderQryField *pRspFuncOrderQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//620003_33101����-�ڻ�ί�лر�Ӧ��
	virtual void OnRtnFuncQhEntrustPush(CHundsunRtnFuncQhEntrustPushField *pFuncQhEntrustPush);
	//620003_33101����-�ڻ��ɽ��ر�Ӧ��
	virtual void OnRtnFuncQhRealPush(CHundsunRtnFuncQhRealPushField *pFuncQhRealPush);
	///330853 ��ϴ����ѯӦ��
	virtual void OnRspFuncArginstrumentQry(CHundsunRspFuncArginstrumentQryField *pRspFuncArginstrumentQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338204 ���ί��ȷ��Ӧ��
    virtual void OnRspFuncArgorderinsert(CHundsunRspFuncArgorderinsertField *pRspFuncArgorderinsert, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	char m_szBroker[16];

	char m_szUserID[16];

	char m_szPwd[16];

	CBaseTradeApi * m_pTrade;

	int m_orderref;

};
