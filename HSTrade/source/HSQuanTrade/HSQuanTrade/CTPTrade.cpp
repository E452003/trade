#include "stdafx.h"
#include "CTPTrade.h"
#include "TradeInterface.h"



CCTPTrade::CCTPTrade(void)
{

}


CCTPTrade::~CCTPTrade(void)
{

}


void CCTPTrade::init(CString serverAddress)
{
	//WRITE_LOG(1,"init");

	m_pUserApiTrade_CTP = CThostFtdcTraderApi::CreateFtdcTraderApi();			

	m_pUserSpiTrade_CTP = new CTraderSpi();	

	//m_pUserSpiTrade_CTP->m_nOrderRefOffset = m_nOrderRefOffset;

	m_pUserSpiTrade_CTP->m_nOrderRefOffset = 10000;

	//m_pUserSpiTrade_CTP->m_iNextOrderRef = 0;
	CTraderSpi::m_iNextOrderRef = 0;

	m_pUserSpiTrade_CTP->m_pTrade = this;

	//strcpy(m_pUserSpiTrade_CTP->m_szInstrumentID,m_szInstrument);

	m_pUserSpiTrade_CTP->m_pUserApiTrade = m_pUserApiTrade_CTP;

	strcpy_s(m_pUserSpiTrade_CTP->m_szBroker,m_szBroker);

	strcpy_s(m_pUserSpiTrade_CTP->m_szUserID ,m_szUserID);

	strcpy_s(m_pUserSpiTrade_CTP->m_szPwd,m_szPwd);

	m_pUserApiTrade_CTP->RegisterSpi((CThostFtdcTraderSpi*)m_pUserSpiTrade_CTP);		

	m_pUserApiTrade_CTP->SubscribePublicTopic(THOST_TERT_RESTART);	

	m_pUserApiTrade_CTP->SubscribePrivateTopic(THOST_TERT_QUICK);	

	if( strlen(m_szTradeServer) > 1 )
	{
		m_pUserApiTrade_CTP->RegisterFront(m_szTradeServer);	

		m_pUserApiTrade_CTP->Init();
	}
}



//�µ�---��Ҫʵ�ֶ�������----���µ���ʽ���д���
int CCTPTrade::InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int orderref)
{
	CTraderSpi::m_iNextOrderRef = orderref;
	return m_pUserSpiTrade_CTP->ReqOrderInsert(instrument,offsetflag,direction,price,count,THOST_FTDC_TC_GFD,'0');
}

int CCTPTrade::WithDraw(int entrustno,char *sCode)
{
	return m_pUserSpiTrade_CTP->ReqOrderAction(entrustno,sCode);
}

int CCTPTrade::QueryEntrust(int entrustno,int reqid)
{
	m_pUserSpiTrade_CTP->ReqQryOrder(entrustno);
	return 1;
}

int CCTPTrade::QueryPosition(char *account)
{
	m_pUserSpiTrade_CTP->ReqQryInvestorPosition();

	return 1;
}


void CCTPTrade::RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount)
{
	m_pTradeInterface->RtnOrder(orderref, ordersysid,status,bs,kp,ins);
}


void CCTPTrade::RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins)
{
	m_pTradeInterface->RtnTrade(userid,orderref,ordersysid,price,offsetflag,direction,tradetime,vol,ins);
}

void CCTPTrade::OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)
{
	m_pTradeInterface->OnRspOrderInsert(ref,entrustno,entrust_status,errorid);

}


//����״̬��Ϣ
bool CCTPTrade::IsWeiBao(char status)
{
	bool ret = false;
	if(status == HS_ORDER_Unknown)
		ret = true;

	return ret;
}

bool CCTPTrade::IsDaiBao(char status)
{
	bool ret = false;
	if(status == HS_ORDER_Unknown)
		ret = true;

	return ret;
}

bool CCTPTrade::IsYiBao(char status)
{
	bool ret = false;
	if(status == HS_ORDER_HASREPORT)
		ret = true;

	return ret;
}

//ctp�ӿ�û�в���״̬����ֵ��ֻ�ܸ��ݳɽ�������жϣ�����Լ��һ��ֵ 'X'
bool CCTPTrade::IsBuChe(char status)
{
	bool ret = false;
	if(status == 'X')
		ret = true;

	return ret;
}

bool CCTPTrade::IsYiChe(char status)
{
	bool ret = false;
	if(status == HS_ORDER_CANCELED)
		ret = true;

	return ret;
}

bool CCTPTrade::IsBuCheng(char status)
{
	bool ret = false;
	if(status == HS_ORDER_PartTraded)
		ret = true;

	return ret;
}

bool CCTPTrade::IsYiCheng(char status)
{
	bool ret = false;
	if(status == HS_ORDER_AllTraded)
		ret = true;

	return ret;
}

bool CCTPTrade::IsFeiDan(char status)
{

	bool ret = false;
	if(status == '4')
		ret = true;

	return ret;
}


string CCTPTrade::TransBS(char bs)
{
	if(bs == POSTION_DIRECTION_BUY)
		return "��";
	else if(bs== POSTION_DIRECTION_SELL)
		return "��";
	else
		return "δ֪";

}


string CCTPTrade::TransKP(char kp)
{
	if(kp == HS_THOST_FTDC_OF_Open)
		return "����";
	else if(kp == HS_THOST_FTDC_OF_Close)
		return "ƽ��";
	else if(kp == HS_THOST_FTDC_OF_CloseToday)
		return "ƽ��";
	else if(kp == HS_THOST_FTDC_OF_CloseYesterday)
		return "ƽ��";
	else
		return "δ֪";

}


string CCTPTrade::TransStatus(char st)
{
	if(st == HS_ORDER_AllTraded)
		return "�ѳ�";
	else if(st ==HS_ORDER_Unknown)
		return "����";
	else if(st ==HS_ORDER_PartTraded)
		return "����";
	else if(st == HS_ORDER_HASREPORT)
		return "�ѱ�";
	else  if(st ==HS_ORDER_CANCELED)
		return "�ѳ�";
	else 
		return "δ֪";
}


void CCTPTrade::Login()
{
	m_pUserSpiTrade_CTP->ReLogin();

}


int CCTPTrade::LogOut()
{
	return m_pUserSpiTrade_CTP->LogOut();

}

int CCTPTrade::GetInterfaceType()
{
	return TRADE_CTP;
}


