#pragma once
#include "strategybase.h"
#include<thread>
#include "typedefine.h"
#include<vector>

//�ֲ���Ϣ
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

	virtual void reload();                //���¼���

	virtual void saveModel() ;             //����ģ��


private:

	double m_beginPrice;

	double m_J1;

	double m_N1;

	int m_TradeDirection;

	int m_kp;

	void Order();

	int m_nMaxTradeCount;

	int m_nMaxChedanCount;

	int m_nMaxLost;

	int m_nTradeCountSum;

	CUIntArray m_listOrderRef;

	float m_fProfitBackStop;

	CPtrArray m_arPosition;
	
	float m_fyingkuiClose;

	void CalCloseProfit(char direction,double price,int count);

	void ClearPosition(void);

	float m_maxProfit;

	double m_pricebid1;

	double m_priceask1;


public:

	void RemoveOrder(int orderref);

	bool isExistRef(int ref,char bs,char kp,char * ins);

	void addGrid(int count);

	void subGrid(int count){};

	void newOrder(int count,double price);

	void setValue();
	//������ʱ���

	bool check_b_jihe();

	bool check_in_jihe();

	bool checkpanzhong();

	bool checkothertime();

	bool checksavetime();

	//������µ�¼ʱ��
	bool reLogintime();
	//����Ƿ�쵽����ǰ�ˣ��ǵĻ�˯��ʱ�����
	bool checkbefore();

	//״̬λ
	bool isAnohterDay;

	bool isNewModel;

	bool isSave;

	bool isLoad;

	thread t;

	void checkOrder();

	//����ֲ�
	static vector<CHoldsinfo> m_holdsinfo;

	void dealHoldsinfo(int count,double price,int bs);

	void setHoldinfo();

	//�����Լ״̬��Ϣ
	//static map<string,vector<CCodeStatus> >m_codestatus;
	static map<string,CCodeStatus>m_codestatus;


};

