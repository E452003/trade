#pragma once
#include <string>
#include "basetradeapi.h"
#include "UFXTradeSpiEx.h"

using namespace std;


class CUFXTradeEx :public CBaseTradeApi
{

public:

	CUFXTradeEx();

	~CUFXTradeEx();


	CUFXTraderSpiEx *m_pUfxSpiTrade;

	CHundsunTraderApi *m_pUfxApiTrade;


	virtual void init(CString serverAddress);

	virtual int InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int orderref);

	virtual int WithDraw(int entrustno,char *sCode);

	virtual int QueryEntrust(int entrustno,int reqid);

	virtual int QueryPosition(char *account);

	virtual void RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount);

	virtual void RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins);

	virtual void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid);


	//����״̬��Ϣ
	virtual bool IsWeiBao(char status);    //δ��

	virtual bool IsDaiBao(char status);    //����

	virtual bool IsYiBao(char status);     //�ѱ�

	virtual bool IsBuChe(char status);     //����

	virtual bool IsYiChe(char status);     //�ѳ�

	virtual bool IsBuCheng(char status);   //����

	virtual bool IsYiCheng(char status);   //�ѳ�

	virtual bool IsFeiDan(char status);    //�ϵ�

	virtual string TransBS(char bs);

	virtual string TransKP(char kp);

	virtual string TransStatus(char st);


	virtual int GetInterfaceType();



};