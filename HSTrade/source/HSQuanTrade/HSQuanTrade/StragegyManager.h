#pragma once
#include "public.h"
#include "typedefine.h"
#include "data.h"
#include "CKdata.h"
#include "TradeStatistic2.h"
#include "KDataEx.h"
#include "EasyThread.h"


class CStrategyBase;

class CTradeInterface;

class CHundsunMdCenterApi;

class CMdSpi;

class CMdSpiCTP;

class CThostFtdcMdApi;

typedef enum tagTestMode
{
	E_TICK_TEST = 0,
	E_K_1_MIN_TEST,
	E_K_3_MIN_TEST,
	E_K_5_MIN_TEST,
	E_K_15_MIN_TEST,
	E_K_1_HOUR_TEST,
	E_K_DAY_TEST,
}E_BACK_TEST_MODE;

typedef struct St_BackTest_Step{
	CString m_sCode;
	int m_iTotalCount;
	int m_iTotalStep;
	int m_iSubCount;
	int m_iSubStep;
}St_BackTest_Step;


class CStrategyManager
{
public:
	CStrategyManager(void);

	~CStrategyManager(void);

public:
	CPtrArray m_arModel;

	CPtrArray m_arModelRun;
	CPtrArray m_arModelRemove;

	CMutex m_mtxModelRun;
	CMutex m_mtxModelRemove;

	void init(CString ufxhqip,CString ctphqip);

	void Exit(void);

	CString GetAllStrategyName(void);

	CString GetStrategyParamInfo(CString name);         //��ȡ���ԵĲ����б�

	CString GetStrategyParamValue(CString modelname);   //��ȡ����״̬ģ�͵Ĳ���ֵ

	CString GetStrategyParamValue(CString modelname,CString paramname);    //

	void SetStrategyParam(CString modelname,CString paramname, CString value);

	CString AddRunStragegy(CString param);

	void ParseParam(CString param, CStringArray & list,char ch);

	CStrategyBase * GetStrategyInstance(CString name);

	CString GetStrategyStatus(CString modelname);

	CString GetStragyLog(CString modelname);

	CString GetStrategySignal(CString modelname);

	int PauseModel(CString modelname);

	int StopModel(CString modelname);

	int RunModel(CString modelname);

	void Log(char * msg);

	//��������
	void register_ufx(CString ip);

	void register_ctp(CString ip);

	//��������
	void ModelTick(CTickData *pd);

	//�Զ�����ͼ���
	void SaveModels();

	void ReLoad();

	void SetAccountInfo(char *account,CAccData *data);

	void StragegyBackTest(LPCTSTR sName, LPCTSTR sParamList, LONG lTimeBegin, LONG lTimeEnd, LPCTSTR sCodeRanger, LPCTSTR sCycle);

	void UnLoad();     //ж�ؼ��ص�dll

	CString GetStrategyContext(CString sStrategyId);
	void SetStrategyContext(CString strStrategyId, CString strContext);

public:

	CTradeInterface *m_pTradeInterface;

	CHundsunMdCenterApi* lpUserMdApi;

	CMdSpi* lpUserMdSpi;

	CThostFtdcMdApi *lpUserMdApi_CTP; 

	CMdSpiCTP *lpUserMdSpi_CTP;

	HWND m_hMsgWnd;

	CPtrArray m_code;

	std::vector<HINSTANCE> m_vDll;

	CString GetAppPath();

	CStockInfo *m_pGetCodeInfo;                    //s��ȡ��Լ����Ϣ�Ľӿ�

	std::mutex m_LockOrder;
	
	CKdataManager m_oKManager;
	CKDataManagerEx m_oKManagerEx;
	CStringArray m_arTestCodes;

	LONG m_lTestTimeBegin, m_lTestTimeEnd;

	BOOL HasBackTestOver()
	{
		if (m_thdBackTest.IsRunning())
		{
			return FALSE;
		}
		return TRUE;
	}
	E_BACK_TEST_MODE m_eBackTestMode;

	

protected:
	int ReadHistoryData(CStrategyBase* pStrategy, CString strCode, long lTimeBegin, long lTimeEnd,int iTotalCount, int iTotalStep);

	CThread m_thdBackTest;

	
	static void ProcessBackTest(void * lpData);

	void OnTest1Min(CStrategyBase* pStrategy);
	void OnTest1Day(CStrategyBase* pStrategy);

	vector<string> m_vCodesHasSub;

   
	CEasyThread *m_pthdSubcode;
	CEasyThread m_thdAux;
	static void Task_Subcode(void *lpData);
	static void Task_Aux(void *lpData);
	bool m_bSubcodeEveryday;//ÿ��һ��
	bool m_bSubcodeByMannul;//�ֶ�
	CMutex m_mtxSubCheck;
};

