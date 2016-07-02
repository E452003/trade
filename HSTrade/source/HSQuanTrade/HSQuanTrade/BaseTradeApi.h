#pragma once
#include <string>
#include "data.h"


#define USERTOKENLEN	256		///���Ƴ���


class CTradeInterface;

class CStrategyManager;

class CBaseTradeApi
{
public:
	CBaseTradeApi();

	~CBaseTradeApi(void);

	virtual void init(CString serverAddress) = 0;

	char m_szTradeServer[256];

	char m_szPwd[16];

	char m_szUserID[16];

	char m_szBroker[16];

	int m_nOrderRefOffset;

	//һЩ�ʺ������Ϣ
	int g_iSystemNo;

	int g_iBranchNo;

	bool g_bLogin;

	CString m_exchAccount;

	char g_cUserToken[USERTOKENLEN];

	HWND m_hMsgWnd;

	int m_bRisk;		//�Ƿ����÷��

	void Log(char * msg);

	virtual int InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int orderref)=0;

	virtual int WithDraw(int entrustno,char *sCode)=0;

	virtual int QueryEntrust(int entrustno,int reqid)=0;

	virtual int QueryPosition(char *account){return 0;};

	virtual int QueryTrade(char *account){return 0;};

	virtual int QueryFund(char *account){return 0;};

	virtual void RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount)=0;

	virtual void RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins)=0;
	
	void SetInterface(CTradeInterface * pt);

	virtual void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)=0; 

	//����״̬��Ϣ
	virtual bool IsWeiBao(char status)=0;    //δ��

	virtual bool IsDaiBao(char status)=0;    //����

	virtual bool IsYiBao(char status)=0;     //�ѱ�

	virtual bool IsBuChe(char status)=0;     //����

	virtual bool IsYiChe(char status)=0;     //�ѳ�

	virtual bool IsBuCheng(char status)=0;   //����

	virtual bool IsYiCheng(char status)=0;   //�ѳ�

	virtual bool IsFeiDan(char status)=0;    //�ϵ�

	virtual std::string TransBS(char bs)=0;

	virtual std::string TransKP(char kp)=0;

	virtual std::string TransStatus(char st)=0;


	//���
	void SetRiskInfo(double margin,int count,double pl,int open);

	bool CheckRisk(char *code,int count,double price,char bs,char kp,int type); //type=0 �µ�  type=1 ����

	void UpdateRiskInfo(char *code,int vol,double price,char bs,char kp);

	bool ExistOrder(int orderref);

	void AddOrder(int orderref);


	//�жϽӿ�����  ��Ϊ��ͬ�ӿ�������Щ�ֶ�  ���籨��״̬����ϴ�
	//ctp	0;	ufx_f	1;	ufx_s	2;	ufx_auto	3;	ufx_opt		4  
	virtual int GetInterfaceType()=0;


protected:

public:

	CTradeInterface * m_pTradeInterface;

	//ÿ���˻�������
	CAccData data;

};

