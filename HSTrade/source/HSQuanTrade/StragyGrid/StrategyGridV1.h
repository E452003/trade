#pragma once

#include "..\hsquantrade\strategybase.h"

#include<thread>
#include<mutex>

#include "..\hsquantrade\typedefine.h"

#include<vector>

#include <map>


struct TPosition
{
	char  ins[32];
	float price;
	int   count;
	char  direction;

	TPosition()
	{
		memset(this,0,sizeof(TPosition));
	}
};


class CStrategyGrid :	public CStrategyBase
{
public:
	CStrategyGrid(void);

	CStrategyGrid(int i);

	~CStrategyGrid(void);

	virtual void  Tick(CTickData * pt);

	virtual CStrategyBase * NewInstance() { return new CStrategyGrid(1) ; };

	virtual void SetParam(CString paramname,CString value);    

	virtual void SetParam(CString value);

	virtual CString GetParamValue();

	virtual CString GetParamValue(CString paramname);

	virtual CString GetStatusInfo(void);

	virtual void BeginRun();

	virtual void OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price);

	virtual void OnErrorRtn(int code,char * errormsg) ;

	virtual void OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins) ;

	virtual void OnRspOrderInsert(int ref,char entrust_status,int errorid);

	virtual void reload();                //���¼���

	virtual void saveModel() ;             //����ģ��



public:

	void Order();

	void CalCloseProfit(char direction,double price,int count);

	void ClearPosition(void);


	CUIntArray m_listOrderRef;

	CPtrArray m_arPosition;


	double m_beginPrice;

	double m_J1;

	double m_N1;

	int m_TradeDirection;

	int m_kp;

	int m_nMaxTradeCount;

	int m_nMaxChedanCount;

	int m_nMaxLost;

	int m_nTradeCountSum;

	float m_fProfitBackStop;

	float m_fyingkuiClose;

	float m_maxProfit;

	double m_pricebid1;

	double m_priceask1;

	int	m_nInstrumentValue;//��Լ����


public:

	void RemoveOrder(int orderref);

	bool isExistRef(int ref,char bs,char kp,char * ins);

	void addGrid(int count);            //��������

	void subGrid(int count){};

	void newOrder(int count,double price);

	void setValue();


	void AddOrderInfo(int ref,int count,double price,char bs,char kp);       //��¼ָ����Ϣ

	void AddOpenPosition(int ref,int count,double price,char bs,char *code);  //��¼������Ϣ


	//-------------------------------------------------------------------------
	// �����������ź�
	//-------------------------------------------------------------------------
	
	bool checkOrdertime();             //�Ƿ���

	bool checkothertime();             //�Ƿ�ǽ���ʱ��

	bool checksavetime();              //���̱���ģ��

	bool reLogintime();                //������µ�¼ʱ��

	bool checkMatchTime();             //����Ƿ��Ǵ�ϳɽ���ʱ��


	//-------------------------------------------------------------------------
	// ״̬λ
	//-------------------------------------------------------------------------
	
	bool isAnohterDay;                   //�Ƿ�ڶ���

	bool isNewModel;                     //�Ƿ��½�ģ��

	bool isSave;                         //�Ƿ��Ѿ�����

	bool isLoad;                         //�Ƿ��Ѿ�����

	bool m_btick;                        //����һ�������־

	int m_tickcount;                     

	
	thread m_tOrder;                     //�̰߳󶨺���

	void checkOrder();


	//-------------------------------------------------------------------------
	// �ֲִ�����
	//-------------------------------------------------------------------------

	void dealHoldsinfo(int count,double price,int bs);           //ģ�ͳ�ʼ�׶��µ�

	void setHoldinfo();                                          //���ֲֽ̳�ֱ��ϲ�

	void updateHoldinfo(char bs,char kp,char *ins,int count);    //�յ��ر����³ֲ�

	std::mutex m_QueryHoldMutex;


	//-------------------------------------------------------------------------
	// �ҵ�δ�ɹ��ĵ���
	//-------------------------------------------------------------------------
	
	map<int,COrderInfo> m_failOrder;  

	std::mutex m_reOrderMutex;

	void reOrderAgain();

	int dealTrade(int orderref,int vol,int iNotTradeCount);			//����ɽ��ر�


	//-------------------------------------------------------------------------
	// ����Ƶ���ķֱʳɽ�
	//-------------------------------------------------------------------------

	map<int,COrderInfo> m_mapOrder;                         //�ر������������ı�����Ϣ

	thread m_tStore;

	void process();                                         //�̺߳���

	void OrderTogether();                                   //�ֱʳɽ��ۼ�һ�𱨵�

	void JoinOrder(int ref,int vol,double orderprice,char bs,char kp);     //����ref��ͬ�ֱʳɽ��ۼ� 

	bool JoinDiffRef(int ref,int vol,double orderprice,char bs,char kp);   //ref����ͬͬ����Ҫ�ϲ�

	std::mutex m_JoinMutex;

	int m_iIntervalOrder;                                   //�µ��ļ��


	//-------------------------------------------------------------------------
	// �Գɽ�����
	//-------------------------------------------------------------------------
	map<int,COrderInfo> m_mapSelfOrder;                 //��ſ����Գɽ��ı���

	bool CheckSelfNotOrder(int ref,double price,char bs,char kp,int count);    //���������Գɽ�

	bool CheckSelfOrder(int ref,double price,char bs,char kp);      //���ҵ��Ƿ�����Գɽ�

	void OrderSelfColse();                              //�µ�

	std::mutex m_CheckSelfMutex;

	bool ReverseOrder(int ref,int type,int count);    //�������Գɽ��ĵ����ڲ��Գɽ�Ȼ��ת

	void OrderByMap(map<int,COrderInfo> orderMap);							//����map����

	void OrderByInfo(COrderInfo orderinfo);									
	
};


