#include "stdafx.h"
#include "UFXTradeEx.h"
#include "TradeInterface.h"


CUFXTradeEx::CUFXTradeEx()
{


}

CUFXTradeEx::~CUFXTradeEx()
{


}





void CUFXTradeEx::init(CString serverAddress)
{
		
	m_pUfxApiTrade = CHundsunTraderApi::CreateHundsunTraderApi();			

	m_pUfxSpiTrade = new CUFXTraderSpiEx(m_pUfxApiTrade);	

	strcpy(m_pUfxSpiTrade->m_szBroker,m_szBroker);

	strcpy(m_pUfxSpiTrade->m_szUserID ,m_szUserID);

	strcpy(m_pUfxSpiTrade->m_szPwd,m_szPwd);

	m_pUfxSpiTrade->m_pTrade = this;

	m_pUfxApiTrade->RegisterSpi(m_pUfxSpiTrade);

	
	m_pUfxApiTrade->RegisterFront(serverAddress.GetBuffer(0));

	serverAddress.ReleaseBuffer();

	char pwd[16];
	if(strcmp(m_szBroker,"-9999")==0)
		strcpy(pwd,"");
	else
		strcpy(pwd,m_szBroker);

	//����ʹ��m_szBroker ����������
	m_pUfxApiTrade->Init("license.dat",pwd);

}



int CUFXTradeEx::InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int orderref)
{
	m_pUfxSpiTrade->m_orderref = orderref;

	//�µ��ӿ��Ƿֿ���
	string code = instrument;
	if(code.find("&") !=-1)
		return m_pUfxSpiTrade->ReqArbOrderinsert("", instrument, direction, offsetflag, '0',count, price);
	else
		return m_pUfxSpiTrade->ReqOrderinsert("", instrument, direction, offsetflag, '0',count, price);

}



int CUFXTradeEx::WithDraw(int entrustno,char *sCode)
{

	return m_pUfxSpiTrade->ReqFuncWithdraw(entrustno);

}



int CUFXTradeEx::QueryEntrust(int entrustno,int reqid)
{
	return 1;

}



int CUFXTradeEx::QueryPosition(char *account)
{
	return 1;

}



void CUFXTradeEx::RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount)
{

	m_pTradeInterface->RtnOrder(orderref, ordersysid,status,bs,kp,ins);
}



void CUFXTradeEx::RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins)
{
	m_pTradeInterface->RtnTrade(userid,orderref,ordersysid,price,offsetflag,direction,tradetime,vol,ins);

}



void CUFXTradeEx::OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)
{
	m_pTradeInterface->OnRspOrderInsert(ref,entrustno,entrust_status,errorid);

}




	//����״̬��Ϣ
bool CUFXTradeEx::IsWeiBao(char status) 
{
	bool ret = false;
	if(status == '0')
		ret = true;

	return ret;

}


bool CUFXTradeEx::IsDaiBao(char status)
{
	bool ret = false;
	if(status == '1')
		ret = true;

	return ret;

}


bool CUFXTradeEx::IsYiBao(char status)
{
	bool ret = false;
	if(status == '2')
		ret = true;

	return ret;

}



bool CUFXTradeEx::IsBuChe(char status)
{
	bool ret = false;
	if(status == '5')
		ret = true;

	return ret;

}



bool CUFXTradeEx::IsYiChe(char status)
{
	bool ret = false;
	if(status == '6')
		ret = true;

	return ret;

}



bool CUFXTradeEx::IsBuCheng(char status)
{
	bool ret = false;
	if(status == '7')
		ret = true;

	return ret;

}



bool CUFXTradeEx::IsYiCheng(char status)
{
	bool ret = false;
	if(status == '8')
		ret = true;

	return ret;

}


bool CUFXTradeEx::IsFeiDan(char status)
{
	bool ret = false;
	if(status == '9')
		ret = true;

	return ret;

}


string CUFXTradeEx::TransBS(char bs)
{
	if(bs == UFX_ORDER_BUY)
		return "��";
	else if(bs== UFX_ORDER_SELL)
		return "��";
	else
		return "δ֪";

}



string CUFXTradeEx::TransKP(char kp)
{
	if(kp == UFX_SP_OPEN)
		return "����";
	else if(kp == UFX_SP_CLOSE)
		return "ƽ��";
	else
		return "δ֪";
}



string CUFXTradeEx::TransStatus(char st)
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


int CUFXTradeEx::GetInterfaceType()
{
	return TRADE_UFX_F;
}



