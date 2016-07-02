#include "stdafx.h"
#include "FeimaTraderSpi.h"
#include <string.h>
#include <stdio.h>
using namespace std;
#include <iostream>
#include <vector>




void CFeimaTraderSpi::OnFrontConnected()
{
	strcpy(ORDER_REF,"\0");

	strcpy(m_szMyInvestorID,"\0");

	m_iRequestID = 0;

	m_OrderActionID = 0;

	CUstpFtdcReqUserLoginField req;

	memset(&req,0,sizeof(CUstpFtdcReqUserLoginField));

	strcpy(req.TradingDay,m_pUserApiTrade->GetTradingDay());

	strcpy(req.BrokerID,m_szBroker);

	strcpy(req.UserID,m_szUserID);

	strcpy(req.Password,m_szPwd);

	strcpy(req.UserProductInfo, "fuhaoming hft1.0");

	m_pUserApiTrade->ReqUserLogin(&req,m_iRequestID++);

	m_nOrderRefQry = -1;
	
}
	
///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void CFeimaTraderSpi::OnFrontDisconnected(int nReason)
{
}
		
///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
void CFeimaTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
}
///���Ļص���ʼ֪ͨ����API�յ�һ�����ĺ����ȵ��ñ�������Ȼ���Ǹ�������Ļص�������Ǳ��Ļص�����֪ͨ��
///@param nTopicID ������루��˽���������������������ȣ�
///@param nSequenceNo �������
void CFeimaTraderSpi::OnPackageStart(int nTopicID, int nSequenceNo)
{
}	
///���Ļص�����֪ͨ����API�յ�һ�����ĺ����ȵ��ñ��Ļص���ʼ֪ͨ��Ȼ���Ǹ�������Ļص��������ñ�������
///@param nTopicID ������루��˽���������������������ȣ�
///@param nSequenceNo �������
void CFeimaTraderSpi::OnPackageEnd(int nTopicID, int nSequenceNo)
{
}


///����Ӧ��
void CFeimaTraderSpi::OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	IsErrorRspInfo(pRspInfo);
}

///���ǰ��ϵͳ�û���¼Ӧ��
void CFeimaTraderSpi::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if( bIsLast && !IsErrorRspInfo(pRspInfo) && pRspUserLogin  )
	{
		m_iNextOrderRef = atoi( pRspUserLogin->MaxOrderLocalID)+1+m_nOrderRefOffset;	
	
		InitOrderReq();
	}
}
///�û��˳�Ӧ��
void CFeimaTraderSpi::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}

///����¼��Ӧ��
void CFeimaTraderSpi::OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	IsErrorRspInfo(pRspInfo);
}
///��������Ӧ��
void CFeimaTraderSpi::OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	IsErrorRspInfo(pRspInfo);
}
///����������֪ͨ
void CFeimaTraderSpi::OnRtnFlowMessageCancel(CUstpFtdcFlowMessageCancelField *pFlowMessageCancel) 
{
}


///�ɽ��ر�
void CFeimaTraderSpi::OnRtnTrade(CUstpFtdcTradeField *pTrade) 
{
	if( pTrade && strcmp(pTrade->UserID,m_szUserID) == 0 )//���userid���ж������˷Ǳ��ʺŵĳɽ�������Ҫ������֤�Ƿ���Ч
	{
	}
}

bool CFeimaTraderSpi::IsTradingOrder(CUstpFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != USTP_FTDC_OS_PartTradedNotQueueing) &&
			(pOrder->OrderStatus != USTP_FTDC_OS_Canceled));
}

///�����ر�
void CFeimaTraderSpi::OnRtnOrder(CUstpFtdcOrderField *pOrder) 
{
	if( !pOrder )
		return; 


}

///����¼�����ر�
void CFeimaTraderSpi::OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo) 
{
	IsErrorRspInfo(pRspInfo);
}

///������������ر�
void CFeimaTraderSpi::OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo) 
{
	if( !pOrderAction )
		return;

	char temp[128];
	sprintf(temp,"OnErrRtnOrderAction  %s %s %s",pOrderAction->OrderSysID,pOrderAction->UserOrderActionLocalID,pOrderAction->UserOrderLocalID);


	IsErrorRspInfo(pRspInfo);
}

///��Լ����״̬֪ͨ
void CFeimaTraderSpi::OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus) 
{
}

///�˻������ر�
void CFeimaTraderSpi::OnRtnInvestorAccountDeposit(CUstpFtdcInvestorAccountDepositResField *pInvestorAccountDepositRes) 
{
}

///������ѯӦ��
//������в�ѯ�Ĺ��ˣ�ֻ�Բ�ѯ��orderref���з��ء�
void CFeimaTraderSpi::OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if( !IsErrorRspInfo(pRspInfo) && pOrder )
	{
		if( m_nOrderRefQry == -1 || atoi(pOrder->UserOrderLocalID )== m_nOrderRefQry )
		{
			
			
		}		
	}
}

///�ɽ�����ѯӦ��
void CFeimaTraderSpi::OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{	
	if( pTrade && !IsErrorRspInfo(pRspInfo)  )
	{
		if( m_nTradeQry == -1 || m_nTradeQry == atoi( pTrade->UserOrderLocalID )  )
		{
			

		}
	}
}

///����Ͷ�����˻���ѯӦ��
void CFeimaTraderSpi::OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}
///���ױ����ѯӦ��
void CFeimaTraderSpi::OnRspQryTradingCode(CUstpFtdcRspTradingCodeField *pRspTradingCode, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}

///Ͷ�����ʽ��˻���ѯӦ��
void CFeimaTraderSpi::OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if( !pRspInvestorAccount )
		return; 

	strcpy(m_szMyInvestorID, pRspInvestorAccount->InvestorID);
	

}

///��Լ��ѯӦ��
void CFeimaTraderSpi::OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}
///��������ѯӦ��
void CFeimaTraderSpi::OnRspQryExchange(CUstpFtdcRspExchangeField *pRspExchange, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	
}

//��ѯ�ֲ�
void CFeimaTraderSpi::RspQryInvestorPosition()
{
	m_vectorPos.clear();

	CUstpFtdcQryInvestorPositionField req;
	memset(&req,0,sizeof(CUstpFtdcQryInvestorPositionField));
	strcpy(req.BrokerID,m_szBroker);
	strcpy(req.ExchangeID,"CFFEX");
	strcpy(req.InstrumentID,m_szInstrumentID);
	strcpy(req.UserID,m_szUserID);
	strcpy(req.InvestorID,m_szMyInvestorID);
	m_pUserApiTrade->ReqQryInvestorPosition(&req,++m_iRequestID);
}

///Ͷ���ֲֲ߳�ѯӦ��
void CFeimaTraderSpi::OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if( !IsErrorRspInfo(pRspInfo) && pRspInvestorPosition )
	{
	
	}
}


///��������Ӧ��
void CFeimaTraderSpi::OnRspSubscribeTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}


///�Ϲ������ѯӦ��
void CFeimaTraderSpi::OnRspQryComplianceParam(CUstpFtdcRspComplianceParamField *pRspComplianceParam, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}
///�����ѯӦ��
void CFeimaTraderSpi::OnRspQryTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}
///Ͷ�����������ʲ�ѯӦ��
void CFeimaTraderSpi::OnRspQryInvestorFee(CUstpFtdcInvestorFeeField *pInvestorFee, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if(!IsErrorRspInfo(pRspInfo) && pInvestorFee )
	{
	}
}
///Ͷ���߱�֤���ʲ�ѯӦ��
void CFeimaTraderSpi::OnRspQryInvestorMargin(CUstpFtdcInvestorMarginField *pInvestorMargin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if(!IsErrorRspInfo(pRspInfo) && pInvestorMargin )
	{
	}
}


int CFeimaTraderSpi::ReqOrderAction(CUstpFtdcOrderField *pOrder)
{
	return ReqOrderAction(atoi(pOrder->UserOrderLocalID));	
}

int CFeimaTraderSpi::ReqOrderAction(int localID)
{
	CUstpFtdcOrderActionField req;

	memset(&req, 0, sizeof(req));
	///���͹�˾����
	strcpy(req.BrokerID, m_szBroker);
	///Ͷ���ߴ���
	strcpy(req.InvestorID, m_szMyInvestorID);

	strcpy(req.UserID,m_szUserID);

	///�������ã����������ı��ر������
	sprintf(req.UserOrderLocalID,"%012d",localID);
	///������־
	req.ActionFlag = USTP_FTDC_AF_Delete;

	strcpy(req.ExchangeID,"CFFEX");
		
	strcpy(req.OrderSysID,"\0");
	
	//��CTP��һ���ĵط�������Ҳ��Ҫ���е������ᵼ��OrderRtn��ORDER_REF��UserOrderLocalID��һ�µ������
	m_iNextOrderRef++;

	sprintf(req.UserOrderActionLocalID, "%012d", m_iNextOrderRef);

	int iResult = m_pUserApiTrade->ReqOrderAction(&req, ++m_iRequestID);

	InitOrderAction();

	return m_iNextOrderRef;
}


int CFeimaTraderSpi::ReqOrderInsert(char * szHeyue,TUstpFtdcOffsetFlagType flagtype,TUstpFtdcDirectionType dir,double price,int count,TUstpFtdcTimeConditionType type )
{
	if( szHeyue[0] == 'I' )
		price = int((price+0.001)/0.2)*0.2;

	CUstpFtdcInputOrderField req;

	memset(&req, 0, sizeof(req));

	strcpy(req.BrokerID, m_szBroker);

	strcpy(req.InvestorID,m_szMyInvestorID);

	strcpy(req.UserID,m_szUserID);

	strcpy(req.InstrumentID, szHeyue);

	strcpy(req.ExchangeID,"CFFEX");

	m_iNextOrderRef++;

	sprintf(req.UserOrderLocalID, "%012d", m_iNextOrderRef);
	
	req.OrderPriceType = USTP_FTDC_OPT_LimitPrice;

	req.Direction = dir;

	req.OffsetFlag = flagtype;

	req.HedgeFlag = USTP_FTDC_CHF_Speculation;

	req.LimitPrice = price;

	req.Volume = count;

	req.TimeCondition = type;
	
	req.VolumeCondition = USTP_FTDC_VC_AV;

	req.MinVolume = 1;

	req.ForceCloseReason = USTP_FTDC_FCR_NotForceClose;

	req.IsAutoSuspend = 0;

	int iResult = m_pUserApiTrade->ReqOrderInsert(&req, ++m_iRequestID);

	return m_iNextOrderRef;
}

bool CFeimaTraderSpi::IsErrorRspInfo(CUstpFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
	{
	}
	return bResult;
}

void CFeimaTraderSpi::ReqQryInvestorAccount(void)
{
	CUstpFtdcQryInvestorAccountField req;

	memset(&req,0,sizeof(CUstpFtdcQryInvestorAccountField));

	strcpy(req.BrokerID,m_szBroker);

	strcpy(req.UserID,m_szUserID);

	m_pUserApiTrade->ReqQryInvestorAccount(&req,m_iRequestID++);
}


void CFeimaTraderSpi::ReqQryOrder(int orderref)
{
	m_nOrderRefQry = orderref;

	CUstpFtdcQryOrderField req;

	memset(&req,0,sizeof(CUstpFtdcQryOrderField));

	strcpy(req.BrokerID,m_szBroker);

	strcpy(req.ExchangeID,"CFFEX");

	strcpy(req.InvestorID,m_szMyInvestorID);

	strcpy(req.UserID,m_szUserID);

	//strcpy(req.InstrumentID,m_szInstrumentID);//���Բ���д���ǲ��Ƿ������к�Լ�ģ�

	int nt = m_pUserApiTrade->ReqQryOrder(&req,m_iRequestID++);

	char msg[128];

	sprintf(msg,"ReqQryOrder %d %d",nt,orderref);


	while( nt != 0 )
	{
		Sleep(1000);

		nt = m_pUserApiTrade->ReqQryOrder(&req,m_iRequestID++);

		sprintf(msg,"ReqQryOrder %d %d",nt,orderref);

	}
}

void CFeimaTraderSpi::ReqQryTrader(int orderref)
{
	m_nTradeQry = orderref;

	CUstpFtdcQryTradeField req;

	memset(&req,0,sizeof(CUstpFtdcQryTradeField));

	strcpy(req.BrokerID,m_szBroker);

	strcpy(req.ExchangeID,"CFFEX");

	strcpy(req.InvestorID,m_szMyInvestorID);

	strcpy(req.UserID,m_szUserID);

	strcpy(req.InstrumentID,m_szInstrumentID);

	int nt = m_pUserApiTrade->ReqQryTrade(&req,m_iRequestID++);	

	if( nt != 0 )
	{
		char temp[128];

		sprintf(temp,"ReqQryTrader Error %d",nt);

	}
}


void CFeimaTraderSpi::ReqQryInvestFee(void)
{
	CUstpFtdcQryInvestorFeeField req;
	memset(&req,0,sizeof(CUstpFtdcQryInvestorFeeField));
	strcpy(req.BrokerID,m_szBroker);
	strcpy(req.ExchangeID,"CFFEX");
	strcpy(req.InvestorID,m_szMyInvestorID);
	strcpy(req.UserID,m_szUserID);
	m_pUserApiTrade->ReqQryInvestorFee(&req,m_iRequestID++);	
}

void CFeimaTraderSpi::ReqQryInvestMargin(void)
{

	CUstpFtdcQryInvestorMarginField req;
	memset(&req,0,sizeof(CUstpFtdcQryInvestorMarginField));
	strcpy(req.BrokerID,m_szBroker);
	strcpy(req.ExchangeID,"CFFEX");
	strcpy(req.InvestorID,m_szMyInvestorID);
	strcpy(req.UserID,m_szUserID);
	m_pUserApiTrade->ReqQryInvestorMargin(&req,m_iRequestID++);
	
}

void CFeimaTraderSpi::ModifyPwd(char * pwd1, char * pwd2)
{
	CUstpFtdcUserPasswordUpdateField req;
	memset(&req,0,sizeof(CUstpFtdcUserPasswordUpdateField));
	strcpy(req.BrokerID,m_szBroker);
	strcpy(req.UserID,m_szUserID);
	strcpy(req.OldPassword,pwd1);
	strcpy(req.NewPassword,pwd2);

	m_pUserApiTrade->ReqUserPasswordUpdate(&req,m_iRequestID++);
}


void CFeimaTraderSpi::OnRspUserPasswordUpdate(CUstpFtdcUserPasswordUpdateField *pUserPasswordUpdate, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if( !IsErrorRspInfo(pRspInfo) &&bIsLast )
	{

	}
}


void CFeimaTraderSpi::InitOrderReq(void)
{
	memset(&m_reqOrderInsert, 0, sizeof(CUstpFtdcInputOrderField));

	strcpy(m_reqOrderInsert.BrokerID, m_szBroker);

	strcpy(m_reqOrderInsert.InvestorID,m_szMyInvestorID);

	strcpy(m_reqOrderInsert.UserID,m_szUserID);

	strcpy(m_reqOrderInsert.InstrumentID, m_szInstrumentID);

	strcpy(m_reqOrderInsert.ExchangeID,"CFFEX");

	m_iNextOrderRef++;

	sprintf(m_reqOrderInsert.UserOrderLocalID, "%012d", m_iNextOrderRef);
	
	m_reqOrderInsert.OrderPriceType = USTP_FTDC_OPT_LimitPrice;

	m_reqOrderInsert.HedgeFlag = USTP_FTDC_CHF_Speculation;

	m_reqOrderInsert.VolumeCondition = USTP_FTDC_VC_AV;

	m_reqOrderInsert.MinVolume = 1;

	m_reqOrderInsert.ForceCloseReason = USTP_FTDC_FCR_NotForceClose;

	m_reqOrderInsert.IsAutoSuspend = 0;
}


void CFeimaTraderSpi::InitOrderAction(void)
{
	memset(&m_reqOrderAction, 0, sizeof(CUstpFtdcOrderActionField));

	strcpy(m_reqOrderAction.BrokerID, m_szBroker);

	strcpy(m_reqOrderAction.InvestorID, m_szMyInvestorID);

	strcpy(m_reqOrderAction.UserID,m_szUserID);
	
	m_reqOrderAction.ActionFlag = USTP_FTDC_AF_Delete;

	strcpy(m_reqOrderAction.ExchangeID,"CFFEX");
		
	strcpy(m_reqOrderAction.OrderSysID,"\0");	
}
