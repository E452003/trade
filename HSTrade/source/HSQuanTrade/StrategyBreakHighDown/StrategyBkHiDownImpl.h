#pragma once
#include "..\HSQuanTrade\StrategyBase.h"
#include<vector>
#include <map>

using namespace std;

typedef enum tagSigStatus
{
	E_SEEK_HIGH = 0,
	E_WAIT_FALL,
	E_WAIT_ARISE,
}E_Bk_Hi_DownStep;

class CStrategyBkHiDownImpl :
	public CStrategyBase
{
public:
	CStrategyBkHiDownImpl(void);
	~CStrategyBkHiDownImpl(void);

	virtual void OnSubcode();
	virtual void  Tick(CTickData * pt);
	virtual void BackTest(CKStrdata *pk, const char* sCode);
	virtual CStrategyBase * NewInstance() { 

		CStrategyBase* pStrategy = new CStrategyBkHiDownImpl();
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

	virtual void OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price);

	virtual void OnErrorRtn(int code,char * errormsg) ;

	virtual void OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins) ;

	virtual int CloseAll(char* szInstrument, double dbPrice);
	virtual void CreateStatisticObject();
	virtual void reload();                //���¼���
	virtual void saveModel() ;             //����ģ��

protected:
	CUIntArray m_listOrderRef;
	std::mutex m_CheckSelfMutex;
	std::map<string, int> m_arCodeSubs;

	bool GetDayKData(const char* szCode, const char* szCurDate, CKStrdata &oKData);
	bool GetPreNDayKData(const char* szCode, const char* szCurDate, CKStrdata &oKData, int iDay);
	void AddOrderInfo(int ref,int count,double price,char bs,char kp, const char* szCode);

	bool isExistRef(int ref,char bs,char kp,char * ins);
	int CalcPositionDays(time_t tOpen, time_t tNow);
	

	std::map<string, double> m_mapHighPrice;

	double GetHighPrice(const char* szCode);
	void SetHighPrice(const char* szCode, double dbPrice);


	double m_dbLoseRate;              //ֹ����
	double m_dbProfitRate;			//ֹӯ��

	double m_dbChangeRate;            //������

	double m_dbShiftLost;			//�ƶ�ֹ��
	double m_dbDownRate;			    //�ص��ʣ����¸߻ص���
	int m_iRefHighDay;				//��N���¸�
	int m_iFallDays;				//N��ص�
	double m_dbFallRate;				//�������

	int m_iWaitBreakDownDays;		//ͻ���µ���������
	bool m_bStrictlyFall;			//��������

	bool m_bMa20Ref;				//�Ƿ�ο�20�վ���

	bool GetHighPriceKData(const char* szCode, CKStrdata& oKData);
	void SetHighPriceKData(const char* szCode, CKStrdata oKData);
	void RemoveHighPriceKData(const char* szCode);
	std::map<string, CKStrdata> m_mapHighPriceKData;

	bool GetLowPriceKData(const char* szCode, CKStrdata& oKData);
	void SetLowPriceKData(const char*szCode, CKStrdata oKData);
	void RemoveLowPriceKData(const char* szCode);
	std::map<string, CKStrdata> m_mapLowPriceKData;

	std::map<string, E_Bk_Hi_DownStep> m_mapSigStatus;

	E_Bk_Hi_DownStep GetSigStatus(const char* szCode);

	void SetSigStatus(const char* szCode, E_Bk_Hi_DownStep eStatus);
	//E_Bk_Hi_DownStep m_eSigStep;
};

