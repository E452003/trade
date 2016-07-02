#pragma once
#include "strategybase.h"
class CStrategyLimitupSale :
	public CStrategyBase
{
public:
	CStrategyLimitupSale(void);

	~CStrategyLimitupSale(void);

	virtual void  Tick(CTickData * pt);
	virtual void BackTest(CKStrdata *pk, const char* sCode){}
	virtual CStrategyBase * NewInstance() {

		CStrategyBase* pStrategy = new CStrategyLimitupSale();
		if (pStrategy)
		{
			pStrategy->CreateStatisticObject();
		}
		return pStrategy;
		
	};

	virtual void SetParam(CString paramname,CString value);    

	virtual void SetParam(CString value);

	virtual CString GetParamValue();

	virtual CString GetParamValue(CString paramname);

	virtual CString GetStatusInfo(void);

	virtual void BeginRun(){};

	virtual void OnTradeReturn(char * ins,char * time,int orderref,int ordersystemid,char direction,char offsetflag,int vol,double price);

	virtual void OnErrorRtn(int code,char * errormsg) {};

	virtual void OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins) {};

	virtual void reload() {};                //���¼���

	virtual void saveModel() {};             //����ģ��

	virtual void setData(const vector<CHoldsinfo> * phold,const std::map<std::string,CCodeStatus> * pcode,HWND m_hMsgWnd){};             //��������

public:
	double m_loseRate;              //ֹ����

	double m_downRate;			    //��߼ۻص���

	double m_changeRate;            //������

	bool m_isToHighPrice;           //�Ƿ�ﵽ����߼�

};

