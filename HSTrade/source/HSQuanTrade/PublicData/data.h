#pragma once

#include "CinfoLogger.h"
#include "typedefine.h"
#include "string"
#include <map>
#include "vector"
#include <string>

/*
**ÿ���˻�������,�ֲ�,ί��,�ʽ��
*/
class AFX_EXT_CLASS CAccData
{
	//����һ����������
public:

	CAccData();

public:

	char m_szUserID[16];

	bool m_bReady;               //�����Ƿ�׼����

	double m_dEnableMoney;		 //��ѯ�����Ŀ����ʽ�

	HWND m_hMsgWnd;

	std::map<std::string,CCodeStatus> m_sCodeStatus;   //��Լ״̬��Ϣ

	std::vector<CHoldsinfo> m_sHoldsInfo;              //��ѯ�����ĳֲ���Ϣ

	std::vector<CEntrustInfo>m_sEntrustInfo;           //ί����Ϣ

	std::map<std::string,CHoldsEx> m_sHoldsReal;       //ʵʱ���µĳֲ���Ϣ


	//�˻������Ϣ��ֵ	
	double m_dMargin;									//��֤��ռ�ö��

	int m_iCheDanCount;									//��������

	double m_dPL;										//�ﵽ��ӯ����ֵ�Ͳ��ٿ���

private:

	std::vector<int> m_vOrderRef;

	//��ǰ������Ϣ
	double m_dUseMargin;								//�Ѿ�ռ�õı�֤��

	std::map<std::string,int> m_mapUseCheDan;			//�Ѿ���������

	double m_dUsePL;									//��ǰӯ��


public:
	void SetRiskInfo(double margin,int count,double pl);

	bool CheckRisk(char *code,int count,double price,char bs,char kp,int type); //type=0 �µ�  type=1 ����

	double UpdateHoldsInfo(char *code,int vol,double price,char bs,char kp);

	void UpdateRiskInfo(char *code,int vol,double price,char bs,char kp);

	double GetMargin(char *code);

	void AddOrder(int orderref);

	bool ExistOrder(int orderref);

};


/*
**����������
*/

class AFX_EXT_CLASS CPubData
{
public:

	CPubData();

public:

	 static  std::map<std::string,StockInfo> m_sCodeInfo;           //��Ʊ��Լ

	 static std::map<std::string,FutuCodeInfo> m_sFutuCodeInfo;    //�ڻ���Լ

	 static std::vector<ArbitrageCodeInfo> s_vFutuCodeInfo;

	 static std::map<std::string,StockOpt> m_mapOptCode;			//��Ʊ��Ȩ����

	 static std::map<std::string, ArbitrageCodeInfo> m_mapArbitrageCode;		//��׼������Լ

	 static std::map<std::string, int> m_mapExchaneId;
};


/*
**��ȡ��Լ��Ϣ
*/


class AFX_EXT_CLASS CStockInfo
{

public:
	CStockInfo();

	CPubData m_PubData;                                //����������

public:
	
	char* getCode(char* name)
	{
		return "";
	}

	char* getName(const char* code)
	{
		return m_PubData.m_sCodeInfo[code].Name;
	}

	long getStart(char* code)
	{
		return m_PubData.m_sCodeInfo[code].J_start;
	}

	double getActiveCapital(char* code)
	{
		return m_PubData.m_sCodeInfo[code].ActiveCapital;
	}

	int getExchid(char* code)
	{
		std::string str(code);
		//��Ϊ�ǹ�Ʊ��Ȩ��Լ
		if(str[0] == '1' && str.length() >6)
			return 3;

		//50etf
		if(strcmp("510050",code) ==0)
			return 1;

		//131810 �ع�
		if(strcmp("131810",code) ==0)
			return 2;

		return m_PubData.m_sCodeInfo[code].exchid;
	}


	char* getfutuName(char* code)
	{
		return m_PubData.m_sFutuCodeInfo[code].name;
	
	}

	char* getfutuexchid(char* code)
	{
		return m_PubData.m_sFutuCodeInfo[code].exchid;
	}

	void LoadCodeInfo();			//����txt��Լ��Ϣ
	

};