#pragma once

#include "..\hsquantrade\strategybase.h"
#include <thread>
#include <mutex>
#include <map>

#define STOCK_TOTAL_MAX 1000000000

#define STOCK_TOTAL_MIN 200000000


class CStrategyBreakHigh : public CStrategyBase
{

public:

	CStrategyBreakHigh();

	~CStrategyBreakHigh();

	virtual void  Tick(CTickData * pt);

	virtual void BackTest(CKStrdata *pk, const char* sCode);

	//virtual CStrategyBase * NewInstance() { return new CStrategyBreakHigh(); };

	virtual CStrategyBase * NewInstance() { 
		
		CStrategyBase* pStrategy = new CStrategyBreakHigh();
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

	virtual void BeginRun();

	virtual void Pause();

	virtual void OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price);

	virtual void OnErrorRtn(int code,char * errormsg){} ;

	virtual void OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins);

	virtual void reload();                //���¼���

	virtual void reload(CString strParam);

	virtual void saveModel() ;             //����ģ��

	virtual int CloseAll(char* szInstrument, double dbPrice);

	virtual void CreateStatisticObject();

	virtual void OnSubcode(); 



/*
//	���Բ���
*/

public:

	int m_iCycle;                                       //��������

	int m_iPriceDay;									//�������¸�

	double m_dFluctuateRange;							//��������

	double m_dAbsoluteStop;                             //����ֹ��

	double m_dMoveStop;									//�ƶ�ֹ��

	double m_dStartFund;								//��ʼ�ʽ�

	double m_dTradePrice;                               //�ɱ���

	int m_iTradeCount;									//�ɽ�����

	double m_preHignPrice;								//ǰN����߼�

	char m_sCurCode[7];									//��ǰ��Լ

	//map<string,double> m_mapHighPrice;					//��¼����������߼�

	map<string,double> m_mapPreHighPrice;				//ǰN�����߼�

	map<string,int> m_mapIN;							//-1 ��ʾ����֮ǰ���ڲ��������0��ʾ��ʼֵ��1��ʾ�������,-2��ʾ���쿪�ֲ���ƽ��

	int m_iInType;										//0�¸����룬1��ͣ����

	double m_dPrePriceS;								//����������ǰһ������̼�

	double m_dGet;										//ֹӯ��

	//int m_xiaopan;										//�Ƿ���С�̹�

	int m_junxian;										//�Ƿ񿴾���

/*
//	�������
//
*/
	vector<CKStrdata> m_v30KData;						//ǰ30���K������

	char m_sCurDate[9];									//��ǰ����

	bool m_bDaPan;										//����ָ���Ƿ���20�վ�������

	vector<string> m_vHold;						//����ֲ���Ϣ


public:
	int GetPreNDaySumKData(const char* szCurDate,vector<CKStrdata>*vk,int iDay);

	bool GetPreNDayKData(const char* szCurDate, CKStrdata &oKData, int iDay);

	bool CheckZhenFu(char *code,char *curDate);

	bool OrderBuy(char *code,double dPrice,double dStartFund);
	
	bool FilterSub(char *code);					//���˲�����������Լ

	bool DaPan();								//20�վ���

	void SetPreHgPrice(const char *code,double price);

	double GetPreHgPrice(const char *code);

	//���¶��ĺ�Լ
	CEasyThread *m_pThdSubcode;

	void ReSubCode();

	static void Task_Subcode(void *lpData);



};