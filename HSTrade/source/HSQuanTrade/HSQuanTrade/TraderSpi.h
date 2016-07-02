#pragma once

#include "..\hsquantrade\include\ThostFtdcTraderApi.h"

#include <string.h>

#include <vector>

#include <atomic> 
#include<mutex>

using namespace std;

class CThostFtdcTraderApi;

struct CInsPosition;

class CBaseTradeApi;

class CTraderSpi : public CThostFtdcTraderSpi
{
public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ǳ�������Ӧ
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///Ͷ���߽�����ȷ����Ӧ
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ�ʽ��˻���Ӧ
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯͶ���ֲ߳���Ӧ
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����¼��������Ӧ
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������������Ӧ
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	virtual void OnFrontDisconnected(int nReason);
		
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	virtual void OnHeartBeatWarning(int nTimeLapse);
	
	///����֪ͨ
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///�ɽ�֪ͨ
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��Լ����״̬֪ͨ
	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);

	///�����ѯ��Լ��֤������Ӧ
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
	///�û���¼����
	void ReqUserLogin();
	///�û��ǳ�����
	int ReqUserLogout();
	///Ͷ���߽�����ȷ��
	void ReqSettlementInfoConfirm();
	///�����ѯ��Լ
	void ReqQryInstrument();	
	

	///����¼������	
	// �Ƿ��յ��ɹ�����Ӧ
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	// �Ƿ��ҵı����ر�
	bool IsMyOrder(CThostFtdcOrderField *pOrder);
	// �Ƿ����ڽ��׵ı���
	bool IsTradingOrder(CThostFtdcOrderField *pOrder);

public:
	void ReqOrderAction(CThostFtdcOrderField *pOrder);

	int ReqOrderAction(int orderid);

	int ReqOrderAction(int orderid,char *sCode);

	int ReqOrderInsert(char * szHeyue,TThostFtdcOffsetFlagType flagtype,TThostFtdcDirectionType dir,double price,int count,TThostFtdcTimeConditionType timecondition,char fakfok);

	//std::atomic_int m_iNextOrderRef;

	//��Ϊȫ�־�̬��������Ϊ����˻�ͬʱ��¼�����������ڱ��������ظ������
	//���ڸ�Ϊͳһ��tradeinterface�ļ���,ȷ��ÿ���ӿڲ��ظ�
	static std::atomic_int m_iNextOrderRef;

	static bool bHaveQryInstrument;

	vector<CInsPosition *> m_listPos;

	///�����ѯͶ���ֲ߳�
	void ReqQryInvestorPosition();

	char m_szBroker[16];

	char m_szUserID[16];

	char m_szPwd[16];

	int m_iRequestID;

	CThostFtdcTraderApi* m_pUserApiTrade;

	char m_szInstrumentID[32];

	///�����ѯ�ʽ��˻�
	void ReqQryTradingAccount();

	void ReqQryOrder(int orderref);

	void ReqQryInvestFee(void);

	void ReqQryInvestMargin(void);

	int m_nOrderRefOffset;

	bool m_bLogin;

	CThostFtdcInputOrderField m_reqOrderIndset;

	CThostFtdcInputOrderActionField m_reqOrderAction;

	void Init(void);

	void ReqQryTrade(int orderref);

	int m_nOrderRef;

	int m_nTradeQry;

	CBaseTradeApi * m_pTrade;

	///�û����·����¼
	void ReLogin();

	int LogOut();
	///��¼���

	HANDLE m_hThread;

	static unsigned int _stdcall ThreadRtnProcess(void * pt);

	char m_offsetflag;

	char m_direction;

	int m_ordersysid;

	int m_orderref;

	TThostFtdcFrontIDType	FRONT_ID;	//ǰ�ñ��

	TThostFtdcSessionIDType	SESSION_ID;	//�Ự���
	
};