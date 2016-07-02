#include "stdafx.h"

#include "UFXOptTradeApi.h"

#include "TradeInterface.h"

#include "StrategyBase.h"

#include "UFXOptTradeSpi.h"


CUFXOptTradeApi::CUFXOptTradeApi(void)
{

}


CUFXOptTradeApi::~CUFXOptTradeApi(void)
{

}


void CUFXOptTradeApi::init(CString serverAddress)
{
	//WRITE_LOG(1,"init");

	m_pOptApiTrade = CHundsunTraderApi::CreateHundsunTraderApi();			

	m_pOptSpiTrade = new CUFXOptTraderSpi(m_pOptApiTrade);	

	strcpy(m_pOptSpiTrade->m_szBroker,m_szBroker);

	strcpy(m_pOptSpiTrade->m_szUserID ,m_szUserID);

	strcpy(m_pOptSpiTrade->m_szPwd,m_szPwd);

	m_pOptSpiTrade->m_pTrade = this;

	m_pOptApiTrade->RegisterSpi(m_pOptSpiTrade);

	//m_pOptApiTrade->RegisterFront("115.236.45.199:7221", 5);
	m_pOptApiTrade->RegisterFront(serverAddress.GetBuffer(0), 5);

	//m_pOptApiTrade->RegisterFront("192.168.73.165:9055", 5);

	m_pOptApiTrade->Init("license.dat");



}



//�µ�---��Ҫʵ�ֶ�������----���µ���ʽ���д���
int CUFXOptTradeApi::InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int orderref)
{
	//���������Ȩ��Լ���룬�������򣬿�ƽ�ַ���ί��������ί�м۸�"1", "11000527", '1', 'O', 1, 0.3

	//���ϲ�ͳһ�������ã�����Ҫʹ�����orderref
	m_pOptSpiTrade->m_orderref = orderref;

	return m_pOptSpiTrade->ReqFuncOptEntrust("1", instrument, direction, offsetflag, count, price);//"2", "20003928", '1', 'O', 1, 16

}

int CUFXOptTradeApi::WithDraw(int entrustno,char *sCode)
{
	return m_pOptSpiTrade->ReqFuncOptWithdraw(entrustno);
}


int CUFXOptTradeApi::QueryEntrust(int entrustno,int reqid)
{
	return 1;
}


int CUFXOptTradeApi::QueryPosition(char *account)
{
	//CUFXOptTraderSpi->ReqQryInvestorPosition();

	return 1;
}

void CUFXOptTradeApi::ReqFuncOptOptcodeQry()
{

}


void CUFXOptTradeApi::RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount)
{
	m_pTradeInterface->RtnOrder(orderref, ordersysid,status,bs,kp,ins);
}


void CUFXOptTradeApi::RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins)
{
	m_pTradeInterface->RtnTrade(userid,orderref,ordersysid,price,offsetflag,direction,tradetime,vol,ins);
}


void CUFXOptTradeApi::OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)
{
	m_pTradeInterface->OnRspOrderInsert(ref,entrustno,entrust_status,errorid);

}


//����״̬��Ϣ
bool CUFXOptTradeApi::IsWeiBao(char status)
{
	bool ret = false;
	if(status == '0')
		ret = true;

	return ret;
}

bool CUFXOptTradeApi::IsDaiBao(char status)
{
	bool ret = false;
	if(status == '1')
		ret = true;

	return ret;
}

bool CUFXOptTradeApi::IsYiBao(char status)
{
	bool ret = false;
	if(status == '2')
		ret = true;

	return ret;
}

bool CUFXOptTradeApi::IsBuChe(char status)
{
	bool ret = false;
	if(status == '5')
		ret = true;

	return ret;
}

bool CUFXOptTradeApi::IsYiChe(char status)
{
	bool ret = false;
	if(status == '6')
		ret = true;

	return ret;
}

bool CUFXOptTradeApi::IsBuCheng(char status)
{
	bool ret = false;
	if(status == '7')
		ret = true;

	return ret;
}

bool CUFXOptTradeApi::IsYiCheng(char status)
{
	bool ret = false;
	if(status == '8')
		ret = true;

	return ret;
}

bool CUFXOptTradeApi::IsFeiDan(char status)
{
	bool ret = false;
	if(status == '9')
		ret = true;

	return ret;

}


string CUFXOptTradeApi::TransBS(char bs)
{
	if(bs == UFX_ORDER_BUY)
		return "��";
	else if(bs== UFX_ORDER_SELL)
		return "��";
	else
		return "δ֪";
}

string CUFXOptTradeApi::TransKP(char kp)
{
	if(kp == UFX_SP_OPEN)
		return "����";
	else if(kp == UFX_SP_CLOSE)
		return "ƽ��";
	else
		return "δ֪";
}

string CUFXOptTradeApi::TransStatus(char st)
{
	if(st == '0')
		return "δ��";
	else if(st =='1')
		return "����";
	else if(st =='2')
		return "�ѱ�";
	else if(st == '5')
		return "����";
	else  if(st =='6')
		return "�ѳ�";
	else  if(st =='7')
		return "����";
	else  if(st =='8')
		return "�ѳ�";
	else  if(st =='9')
		return "�ϵ�";
	else 
		return "δ֪";
}


int CUFXOptTradeApi::GetInterfaceType()
{
	return TRADE_UFX_OPT;
}


