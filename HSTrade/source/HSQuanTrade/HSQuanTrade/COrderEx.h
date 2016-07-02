#include <vector>
#include <mutex>
#include <string>
#include <thread>
#include "typedefine.h"
#include <map>


class CBaseTradeApi;

class CTradeInterface;


class COrderEx
{
public:
	COrderEx(char *account,char * instrument, int count, char direction, char offsetflag, double price,int ordertype,int timeout,double jiacha,CBaseTradeApi * api,CTradeInterface *ptrade,int orderref);

	~COrderEx(void);

	CBaseTradeApi *m_pApi;

	CTradeInterface *m_ptrade;

	char m_account[15];

	int m_nOrderRef;

	int m_nEntrustNo;

	char m_szIntrument[16];

	bool m_bDone;			//��ֹ״̬:�ɽ����ѳ����Ѿ�������������

	bool m_bReOrder;		//�Ƿ���Ҫƽ��

	void RtnOrder(int orderref,int ordersysid,char status,char bs1,char kp1,char * ins);

	void RtnTrade(int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins);

	void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid);

private:

	int  m_count;			 //ί������

	int m_iTradeCount;	     //�ɽ�����

	char m_direction;

	char m_offsetflag;

	double m_price;


	int  m_nordertype;       //-1�����ж������� 0��ʱ���� 1��һ���������˻�û�ɽ������Լ�׷�� 2 �̿ڼ۲���ϲ��� ׷�� 

	int  m_ntimeout;

	double m_jiacha;

	long m_timestamp;		 //����ʱ��


	char m_status;			 //����״̬  

	bool m_bWithDrawING;     //�Ƿ����ڳ���  

	std::mutex m_lockWithDraw;


public:

	bool DealPrice(CTickData *pd);     //ģʽ1	��Ҫ׷�� 

	bool IsDone();

	void SetDone(bool done);

	bool IsWithDraw();

	void SetWithDraw(bool withdraw);

	bool IsReOrder();					//�Ƿ��򿪲�

	void SetReOrder(bool order);

	void RervseOrder(CTickData *pd);					//������

	int WithDraw(int ref);		

	int GetEntrustAmount();

	int GetTradeAmount();

	double GetEntrustPrice();
};



class COrderGroup
{

public:
	COrderGroup(int id);

	~COrderGroup();

	void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid);

	void RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins);

	void RtnTrade(int orderref,int ordersysid,double price,char bs,char kp,char * tradetime,int vol,char *ins);

	void Tick(CTickData *pd);

	void AddOrder(int ref, COrderEx *order);

	int GetGrpId();

	void DealReverse(int orderref);


private:

	std::map<int,COrderEx*> m_mapOrder;				//����map

	std::vector<int> m_vecChedan;					//׷��ʱ���۲���󣬳�������׷���ı���

	std::map<std::string,CTickData> m_mapTick;		//��������

	std::vector<int> m_vecNotTrade;					//����δ�ɽ��ı��� ��������

	long m_timestamp;								//����ʱ��

	int m_iGrpId;


	std::mutex m_lockNotTrade;
};
