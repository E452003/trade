#pragma once
#include<vector>
#include<map>
#include"typedefine.h"
#include<string>
#include "public.h"
#include <afxmt.h>
using namespace std;


class CKdata
{

public:

	CKdata();

public:

	char code[31];

	std::vector<CKStrdata> m_kMin1;     //һ����K��

	std::vector<CKStrdata> m_kDay;
	//...

	std::map<string, double> m_hsMa20; //20�վ���
public:
	
	bool getDayKData(const char* szCurDate, CKStrdata &oKdata);
	//�������������ȡǰһ��K�����ݣ��������ĳһ���Ƿ��ǽ�����
	bool getNDayKData(const char* szCurDate, CKStrdata &oKdata, int iDays);
	int getNDaySumKData(const char* szCurDate,std::vector<CKStrdata>* pvKData,int iDays);

	bool CalcMa20(const char* szTime, double& dbMa20);

	bool GetLastKDataDate(CTime &tmLast);

	int CalDaysBetween2DayK(const char* szDate1, const char* szDate2);//����ʷ��K�����ݼ�������K�߼����������

protected:
	int ReadDayKData();
};



class CKdataManager{

public:
	
	std::map<string, CKdata*> m_kData;

	void Init();
	bool getDayKData(const char* sCode, const char* szDate, CKStrdata &kData);			//��ȡ��ԼK��
	bool getNDayKData(const char* sCode, const char* szCurDate, CKStrdata &oKdata, int iDays);
	int getNDaySumKData(const char* sCode,const char* szCurDate,std::vector<CKStrdata>* pvKData,int iDays);

	bool getMinKData(char *sCode, std::vector<CKStrdata>** pvKData);
	bool CalcMa20(const char *sCode, const char* szTime, double& dbMa20);
	bool GetLastKDataDate(const char* szCode, CTime &tmLast);

	static void Task_SupplementDayKData(void *lpData);
	static void Task_Aux(void *lpData);
	CThread m_thdSupplement;
	CThread m_thdAux;
	bool m_bSupplementToday;
	CMutex m_mtxSupplement;
	vector<string> m_arCodes;

	int LoadStockList();

	int GetCodeList(vector<string>& arCodes);
	int CalDaysBetween2DayK(const char* szCode, const char* szDate1, const char* szDate2);//����ʷ��K�����ݼ�������K�߼����������
	HWND m_hMsgWnd;
public:

	CKdataManager();

	~CKdataManager();

	void ReadFile(int iType);								//��ȡȫ�����ݣ�0�������ߣ�1����1min

	void ReadKdata(char *sCode,char *sPath,int iType);		//iType, 0�������ߣ�1����1min

	string GetDir();										//��ȡ��ִ���ļ�·

	void UnLoadData(const char* szCode);

	void FreeData();

};

