#pragma once

#include "..\hsquantrade\strategybase.h"
#include <thread>
#include <mutex>


class CStrategyLimitupBuy : public CStrategyBase
{

public:

	CStrategyLimitupBuy();

	~CStrategyLimitupBuy();

	virtual void  Tick(CTickData * pt);

	virtual CStrategyBase * NewInstance() { 
		
		CStrategyBase* pStrategy = new CStrategyLimitupBuy();
		if (pStrategy)
		{
			pStrategy->CreateStatisticObject();
		}
		
		return new CStrategyLimitupBuy(); 
	
	};

	virtual void SetParam(CString paramname,CString value);    

	virtual void SetParam(CString value);

	virtual CString GetParamValue();

	virtual CString GetParamValue(CString paramname);

	virtual CString GetStatusInfo(void);

	virtual void BeginRun();

	virtual void OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price);

	virtual void OnErrorRtn(int code,char * errormsg){} ;

	virtual void OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins);

	virtual void reload(){};                //���¼���

	virtual void saveModel(){} ;             //����ģ��


public:

	//ί����Ϣ��,��ʽ:code,count,price|code,count,price|
	CString m_sEntrustInfo;

	double m_rate;                        //��һ�����

	double m_dChangeRate;                 //������

	char m_bs;

	char m_kp; 

	bool m_done;

	int m_strType;                         //1 �����µ� ����ع���2 ����ع���3 ������ͣ����Ժ�Լ����

	bool m_huigou;                         //�Ƿ���Իع�

	thread m_tWorkThread;

	std::mutex m_entrustMutex;


public:

	void order();

	void process();

	bool isOrderTime();                     //����Ľ���ʱ��

	bool isWithDrawTime();					//����ĳ���ʱ��

	bool isCollectiveBidTime();             //9:20~9:30

	bool isDoNothingTime();					//������������ʱ��

	bool isCloeTime();						//��������ƽ�ֵ�ʱ��

	void withDraw();                        //ȫ������

	map<string,long> m_sSignalTime;         //��Լ��Ӧ����������ʱ���

	void BussInessDeal(int type);



};