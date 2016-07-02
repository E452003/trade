#pragma once
#include "HundsunTraderApi.h"
#include "public.h"

class CBaseTradeApi;

class CUFXOptTraderSpi : public CHundsunTraderSpi
{
public:
	CUFXOptTraderSpi(CHundsunTraderApi* lpObj)
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

	int ReqFuncOptOptcodeQry();
	int ReqFuncOptEnTrade(char* szExchType, char* szOptionCode, char cEntrustBS, char cEntrustOC, double dEntrustPrice);
	int ReqFuncOptEntrust(char* szExchType, char* szOptionCode, char cEntrustBS, char cEntrustOC, int nEntrustAmount, double dEntrustPrice);
	int ReqFuncOptWithdraw(int EntrustNo);
	int ReqFuncOptEntrustQry();
	int ReqFuncOptRealtimeQry();
	int ReqFuncOptEntrustSubscribe();
	int ReqFuncOptDealSubscribe();
private:
	CHundsunTraderApi* lpUserApi;
	int m_RequestID;
public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	void OnFrontConnected();
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	void OnFrontDisconnected(int nReason);
	//331100 �ͻ���¼Ӧ��
	void OnRspFuncClientLogin(CHundsunRspFuncClientLoginField *pfunc_client_login, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//330851 �ڻ���Լ��֤���ѯӦ��
	void OnRspFuncFutuContractbailQry(CHundsunRspFuncFutuContractbailQryField *pRspfunc_futu_contractbail_qry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//338202 �ڻ�ί��ȷ��Ӧ��
	void OnRspFuncOrderinsert(CHundsunRspFuncOrderinsertField *pRspFuncOrderinsert, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338301 �ڻ�����ί�в�ѯӦ��
    void OnRspFuncOrderQry(CHundsunRspFuncOrderQryField *pRspFuncOrderQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//620003_33101����-�ڻ�ί�лر�Ӧ��
	void OnRtnFuncQhEntrustPush(CHundsunRtnFuncQhEntrustPushField *pFuncQhEntrustPush);
	//620003_33101����-�ڻ��ɽ��ر�Ӧ��
	void OnRtnFuncQhRealPush(CHundsunRtnFuncQhRealPushField *pFuncQhRealPush);
	///338000 ��Ȩ������Ϣ��ѯӦ��
	void OnRspFuncOptOptcodeQry(CHundsunRspFuncOptOptcodeQryField *pRspFuncOptOptcodeQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338010 ��Ȩ�ɽ���������ȡӦ��
	void OnRspFuncOptEnTrade(CHundsunRspFuncOptEnTradeField *pRspFuncOptEnTrade, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338011 ��Ȩί��Ӧ��
	void OnRspFuncOptEntrust(CHundsunRspFuncOptEntrustField *pRspFuncOptEntrust, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338012 ��Ȩ����Ӧ��
	void OnRspFuncOptWithdraw(CHundsunRspFuncOptWithdrawField *pRspFuncOptWithdraw, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338020 ��Ȩί�в�ѯӦ��
	void OnRspFuncOptEntrustQry(CHundsunRspFuncOptEntrustQryField *pRspFuncOptEntrustQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338021 ��Ȩ�ɽ���ѯӦ��
	void OnRspFuncOptRealtimeQry(CHundsunRspFuncOptRealtimeQryField *pRspFuncOptRealtimeQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    //620003_33012 ����-������Ȩί�л�дӦ��
	void OnRtnFuncOptEntrustPush(CHundsunRtnFuncOptEntrustPushField *pFuncOptEntrustPush);
	//620003_33011 ����-������Ȩ�ɽ��ر�Ӧ��
	void OnRtnFuncOptDealPush(CHundsunRtnFuncOptDealPushField *pFuncOptDealPush);
	//620001_33011 ����-������Ȩ�ɽ���дӦ��
    void OnRspFuncOptDealSubscribe(CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//620001_33012 ����-������Ȩί�л�дӦ��
    void OnRspFuncOptEntrustSubscribe(CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	char m_szBroker[16];

	char m_szUserID[16];

	char m_szPwd[16];

	CBaseTradeApi * m_pTrade;

	int m_orderref;

};
