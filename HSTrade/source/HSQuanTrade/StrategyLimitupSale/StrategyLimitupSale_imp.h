#pragma once

#include "..\hsquantrade\strategybase.h"
#include <thread>
#include <mutex>


class CStrategyLimitupSale : public CStrategyBase
{

public:

	CStrategyLimitupSale();

	~CStrategyLimitupSale();

	virtual void  Tick(CTickData * pt);

	virtual CStrategyBase * NewInstance() { 
		
		
		CStrategyBase* pStrategy = new CStrategyLimitupSale();
		if (pStrategy)
		{
			pStrategy->CreateStatisticObject();
		}
		
		return new CStrategyLimitupSale();  
	
	
	
	};

	virtual void SetParam(CString paramname,CString value);    

	virtual void SetParam(CString value);

	virtual CString GetParamValue();

	virtual CString GetParamValue(CString paramname);

	virtual CString GetStatusInfo(void);

	virtual void BeginRun();

	virtual void Pause();

	virtual void OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price);

	virtual void OnErrorRtn(int code,char * errormsg){} ;

	virtual void OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins);

	virtual void reload();                //���¼���

	virtual void saveModel() ;             //����ģ��


public:

	double m_fBackRate;                   //��߼ۻص�����

	double m_fLoseRate;                   //�������

	double m_fGetRate;					  //ֹӯ����

	double m_dTradePrice;                 //�ɱ���

	bool m_done;                           //�Ƿ����

	int m_iDurationTime;                   //���������ĳ���ʱ��,��λ��
	
public:

	bool isDoNothingTime();					//������������ʱ��

	bool isCloeTime();						//��������ƽ�ֵ�ʱ��

	bool isSaveTime();						//�̺󱣴���Ϣ

	map<string,long> m_sSignalTime;         //��Լ��Ӧ����������ʱ���


};