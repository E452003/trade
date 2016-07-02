#include <mutex>
#include <string>
#include <thread>

using namespace std;

#pragma once


class CBaseTradeApi;

class CTradeInterface;

class COrder
{
public:
	COrder(char *account,char * instrument, int count, char direction, char offsetflag, double price,int ordertype,int timeout,double jiacha,CBaseTradeApi * api,CTradeInterface *ptrade,int orderref);

	~COrder(void);

	CBaseTradeApi *m_pApi;

	CTradeInterface *m_ptrade;

	char m_account[15];

	int m_nOrderRef;

	int m_nEntrustNo;

	int m_nOrderSysID;

	char m_szIntrument[16];

	char m_szLeg2[16];

	bool m_bDone;			//��ֹ״̬:�ɽ����ѳ����Ѿ�������������

	long m_timestamp;		 //����ʱ��
	
	void RtnOrder(int orderref,int ordersysid,char status,char bs1,char kp1,char * ins);

	void RtnTrade(int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins);

	/*�����֧�ֱ������õĽӿ�
	  entrustno != -9999 ��ʾ��֧�ֱ���������Ҫ����entrustno
	*/
	void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid);

private:

	int  m_count;			 //ί������

	int m_iTradeCount;	     //�ɽ�����

	char m_direction;

	char m_offsetflag;

	double m_price;

	int  m_nOrderType;       //-1�����ж������� 0��ʱ���� 1��һ���������˻�û�ɽ������Լ�׷�� 2 �̿ڼ۲���ϲ��� ׷�� 

	int  m_ntimeout;

	double m_jiacha;

	char m_status;			 //����״̬  

	bool m_bWithDrawING;     //�Ƿ����ڳ���  

	std::thread m_tTrd;

	std::thread m_tTrdQuery; //��ѯ�߳�


public:

	void DealOrder();							  //ģʽ0	��ʱ�򳷵�

	void DealPrice(double buy1,double sale1);     //ģʽ1	��Ҫ׷�� 

	void QueryOrder();							  //��ѯί��	

	void DealOrderEx();							  //����ģʽ Ҳ��Ҫ��ʱ������������


};

