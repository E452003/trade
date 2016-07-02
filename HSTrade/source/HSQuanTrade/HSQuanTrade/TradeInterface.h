#include <atomic> 
#include <string>
#include "typedefine.h"
#include "MgServerSocket.h"
#include <thread>
#include <mutex>
#include "public.h"


#pragma once


struct OrderRtn
{
	int returntype;
	char account[32];
	int orderref;
	int ordersysid;
	char status;
	char direction;
	char offsetflag;
	char instrument[32];
	double price;
	int   count;
	char  tradetime[32];
	OrderRtn()
	{
		memset(this,0,sizeof(OrderRtn));
	}
};




class CStrategyManager;

class CTradeInterface
{
public:
	CTradeInterface(void);

	~CTradeInterface(void);

	std::atomic_int m_iOrderRef;

	int InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int ordertype,int timeout,double jiacha,char * account);

	//�����µ�
	int InsertOrderGroup(char * instrument, int count, char direction, char offsetflag, double price,int ordertype,int timeout,double jiacha,char * account,int group);

	int WithDraw(char*account,int entrustno,char *sCode);

	void QueryPosition(char *account);

	void QueryEntrust(char*account,int entrustno);

	void QueryTrade(char*account);

	void QueryFund(char*account);


	//����˻�
	int OpenAccount(CString account,  CString pwd,CString serverip, CString apitype,CString brokeid);

	//�����˻������Ϣ
	void SetRiskInfo(char* account,double margin,int count,double pl,int open);

	void init();

	HWND m_hMsgWnd;

	bool m_bStop;

	std::thread m_tTrd;

	std::mutex m_LockOrder;


	/*
	�ڶ����ֶ���Ϊ�˼��ݲ�֧�ֱ������õĽӿ�
	*/
	void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid);

	void OnRspWithdraw(int ref);

	void RtnOrder(int orderref,int ordersysid,char status,char bs ,char kp,char * ins);

	void RtnOrderEx(int orderref,int ordersysid,char status,char bs ,char kp,char * ins);
		
	void RtnTrade(char *account,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins);

	void RtnTradeEx(char *account,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins);

    CString  getAccount();            //��ȡ�Ѽ�����˻�

	void DealOrder();               //ͨ��һ���̶߳�ʱˢ�¶�����״̬

	void OrderTick(CTickData *pd);

	void Login();

	void LogOut();

	void Exit();

	void SetAccountInfo(char *account,bool isQueryHold);

private:

	CPtrArray m_arApiList;

public:

	CPtrArray m_arListOrder;

	CStrategyManager* m_strManger;

	//�����Ҫ�����µ�
	CPtrArray m_arListOrderGroup;

//���socket�ӿ�
public:
	
	bool InitSock();

	void InitListen();

	CMgServerSocket m_ServerSock;

	void getQueryInfo(char* buf,char *account);       //socket��ѯ��Ӧ���޷���socket���׽ӿ��л�ȡ,

	int m_iRestLength;								  //����Ҫ���յĳ���

	std::string buf;								  //������յ���socket����

	bool m_bEnd;									  //���������Ƿ�������

	int m_port ;									  //�����˿�

	CPtrArray m_arListRtn;  //�洢�����ͳɽ��ر�

	ManualLock m_lockRtn;

	static unsigned int _stdcall ThreadRtnProcess(void * pt);

	HANDLE m_hThread;

};

