#include "stdafx.h"
#include "data.h"
#include "string"
#include "tool.h"
#include "typedefine.h"

////
//CAccData
////
CAccData::CAccData()
{
	m_bReady = false;

	m_dEnableMoney = 0.0;

	m_hMsgWnd = 0;

	m_dMargin = 99999999;

	m_iCheDanCount = 500;

	m_dPL = 0;

	m_dUseMargin = 0;

	m_dUsePL = 0;
}


void CAccData::SetRiskInfo(double margin,int count,double pl)
{
	m_dMargin = margin;
	m_iCheDanCount = count;
	m_dPL = pl;

}


double CAccData::UpdateHoldsInfo(char *code,int vol,double price,char bs,char kp)
{
	char msg[256];

	double ret = 0; 
	//ͷһ�ο���
	if(m_sHoldsReal.find(code) == m_sHoldsReal.end())
	{
		CHoldsEx ohold;
		strcpy(ohold.szInstrument,code);
		//ohold.PosiDirection = bs;
		if(bs==POSTION_DIRECTION_BUY)
		{
			ohold.dLongTradePrice = price;
			ohold.LongPosition = vol;
			ohold.dLongMargin = vol * price  * CPubData::m_sFutuCodeInfo[code].iVolumeMultiple * CPubData::m_sFutuCodeInfo[code].LMarginRatio;
		}
		else if(bs==POSTION_DIRECTION_SELL)
		{
			ohold.dShortTradePrice = price;
			ohold.ShortPosition = vol;
			ohold.dShortMargin = vol * price  * CPubData::m_sFutuCodeInfo[code].iVolumeMultiple * CPubData::m_sFutuCodeInfo[code].SMarginRatio;
		}

		m_sHoldsReal[code] = ohold;

	}
	else
	{
		int newcount = 0;	
		//����
		if(kp == HS_THOST_FTDC_OF_Open)
		{
			//���㿪�־���	
			if(/*ohold.PosiDirection==bs && */bs == POSTION_DIRECTION_BUY)
			{
				m_sHoldsReal[code].dLongMargin += vol * price  * CPubData::m_sFutuCodeInfo[code].iVolumeMultiple * CPubData::m_sFutuCodeInfo[code].LMarginRatio;
				newcount = m_sHoldsReal[code].LongPosition + vol;
				m_sHoldsReal[code].dLongTradePrice = (m_sHoldsReal[code].LongPosition * m_sHoldsReal[code].dLongTradePrice + vol * price) / newcount;
				m_sHoldsReal[code].LongPosition = newcount;
			}
			else if(/*ohold.PosiDirection==bs && */bs == POSTION_DIRECTION_SELL)
			{
				m_sHoldsReal[code].dShortMargin += vol * price  * CPubData::m_sFutuCodeInfo[code].iVolumeMultiple * CPubData::m_sFutuCodeInfo[code].SMarginRatio;
				newcount = m_sHoldsReal[code].ShortPosition + vol;
				m_sHoldsReal[code].dShortTradePrice = (m_sHoldsReal[code].ShortPosition * m_sHoldsReal[code].dShortTradePrice + vol * price) / newcount;
				m_sHoldsReal[code].ShortPosition = newcount;
			}
		}
		//ƽ�� 
		else
		{
			double AveMargin = 0;
						
			//ƽ��
			if(/*ohold.PosiDirection==bs && */bs == POSTION_DIRECTION_BUY)
			{
				if(m_sHoldsReal[code].ShortPosition <=0)
				{
					WRITE_LOG(1,"�ֲֲ�ƥ��...");
					return 0;
				}

				//��֤��
				AveMargin = m_sHoldsReal[code].dShortMargin / m_sHoldsReal[code].ShortPosition;
				m_sHoldsReal[code].dShortMargin -= AveMargin * vol;

				newcount = m_sHoldsReal[code].ShortPosition - vol;	

				//ӯ��
				ret = (m_sHoldsReal[code].dShortTradePrice - price) * vol;

				//�ֲ־���
				if(newcount!=0)
					m_sHoldsReal[code].dShortTradePrice = (m_sHoldsReal[code].ShortPosition * m_sHoldsReal[code].dShortTradePrice - vol * price) / newcount;
				else
					m_sHoldsReal[code].dShortTradePrice = 0;

				//�ֲ���
				m_sHoldsReal[code].ShortPosition = newcount;
			}
			//ƽ��
			else if(/*ohold.PosiDirection==bs && */bs == POSTION_DIRECTION_SELL)
			{
				if(m_sHoldsReal[code].LongPosition <=0)
				{
					WRITE_LOG(1,"�ֲֲ�ƥ��...");
					return 0;
				}

				//��֤��
				AveMargin = m_sHoldsReal[code].dLongMargin / m_sHoldsReal[code].LongPosition;
				m_sHoldsReal[code].dLongMargin -= AveMargin * vol;

				newcount = m_sHoldsReal[code].LongPosition - vol;

				//ӯ��
				ret = (price- m_sHoldsReal[code].dLongTradePrice) * vol;

				//�ֲ־���
				if(newcount!=0)
					m_sHoldsReal[code].dLongTradePrice = (m_sHoldsReal[code].LongPosition * m_sHoldsReal[code].dLongTradePrice - vol * price) / newcount;
				else
					m_sHoldsReal[code].dLongTradePrice = 0;

				//�ֲ���
				m_sHoldsReal[code].LongPosition = newcount;
			}
		}
	
	}

#ifdef _DEBUG
	map<string,CHoldsEx> ::iterator it = m_sHoldsReal.begin();
	char heyue[32];
	int lcount = 0;
	double lprice = 0;
	int scount = 0;
	double sprice = 0;
	double lmargin = 0;
	double smargin = 0;
	for(;it != m_sHoldsReal.end();it++)
	{
		strcpy(heyue,it->second.szInstrument);
		lcount = it->second.LongPosition;
		lprice = it->second.dLongTradePrice;
		lmargin = it->second.dLongMargin;
		scount = it->second.ShortPosition;
		sprice = it->second.dShortTradePrice;
		smargin = it->second.dShortMargin;
		sprintf(msg,"�ֲ���Ϣ  ��Լ %s  ��� %d  ��ͷ���� %.4f  ��ͷ��֤�� %.4f  �ղ� %d  ��ͷ���� %.4f  ��ͷ��֤�� %.4f",heyue,lcount,lprice,lmargin,scount,sprice,smargin);
		WRITE_LOG(1,msg);
	}

#endif

	return ret;

}


double CAccData::GetMargin(char *code)
{
	double ret = 0;
	map<string,CHoldsEx> ::iterator it = m_sHoldsReal.begin();

	for(;it != m_sHoldsReal.end();it++)
	{
		//if(bs == POSTION_DIRECTION_BUY)
		//{
		//	if(it->second.LongPosition >=0)
		//		ret = it->second.dLongMargin;
		//}
		//else if(bs == POSTION_DIRECTION_SELL)
		//{
		//	if(it->second.ShortPosition >=0)
		//		ret = it->second.dShortMargin;
		//}

		ret = ret = it->second.dLongMargin + it->second.dShortMargin;

	}

	return ret;
}


void CAccData::AddOrder(int orderref)
{
	m_vOrderRef.push_back(orderref);
}


bool CAccData::ExistOrder(int orderref)
{
	bool ret = false;
	for(size_t i=0; i< m_vOrderRef.size(); i++)
	{
		if(orderref == m_vOrderRef[i])
		{
			ret = true;
			break;
		}
	}

	return ret;
}


void CAccData::UpdateRiskInfo(char *code,int vol,double price,char bs,char kp)
{
	char msg[256];

#ifdef _DEBUG
	WRITE_LOG(1,"---------------------------riskinfo--------------------------");

	sprintf(msg,"�ɽ���Ϣ  ��Լ %s  ���� %d  �ɽ��۸� %.4f  ���� %c  ��ƽ %c",code,vol,price,bs,kp);
	WRITE_LOG(1,msg);
#endif


	//��ȡ��֤����
	double rate = 0;
	if(bs == POSTION_DIRECTION_BUY)
	{
		rate = CPubData::m_sFutuCodeInfo[code].LMarginRatio;
	}
	else if(bs == POSTION_DIRECTION_SELL)
	{
		rate = CPubData::m_sFutuCodeInfo[code].SMarginRatio;
	}

	//���³ֲ���Ϣ
	double pl = UpdateHoldsInfo(code,vol,price,bs,kp) * CPubData::m_sFutuCodeInfo[code].iVolumeMultiple;

	//�ۼ�ӯ��
	double oldpl = m_dUsePL;
	m_dUsePL += pl;

	double oldmargin = m_dUseMargin;
	//���� ��
	if(kp == HS_THOST_FTDC_OF_Open)
	{
		//m_dUseMargin += rate * price * vol; 
		m_dUseMargin = GetMargin(code);
		sprintf(msg,"ԭ���ñ�֤�� %.4f  �������ӱ�֤�� %.4f  �����ñ�֤�� %.4f",oldmargin,m_dUseMargin-oldmargin,m_dUseMargin);
	}
	//ƽ�� ��
	else/* if(kp == HS_THOST_FTDC_OF_Close || kp == HS_THOST_FTDC_OF_CloseToday)*/
	{
		//m_dUseMargin -= rate * price * vol;
		m_dUseMargin = GetMargin(code);
		sprintf(msg,"ԭ���ñ�֤�� %.4f  �����ͷű�֤�� %.4f  �����ñ�֤�� %.4f",oldmargin,oldmargin-m_dUseMargin,m_dUseMargin);
	}

#ifdef _DEBUG	
	WRITE_LOG(1,msg);

	sprintf(msg,"ԭƽ��ӯ�� %.4f  ���γɽ�ӯ�� %.4f  ��ƽ��ӯ�� %.4f",oldpl,pl,m_dUsePL);
	WRITE_LOG(1,msg);

	WRITE_LOG(1,"---------------------------riskinfo--------------------------");
#endif
}


bool CAccData::CheckRisk(char *code,int count,double price,char bs,char kp,int type) //type=0 �µ�  type=1 ����
{
	bool ret = true;
	char msg[256];

	//��¼����Լ��������
	if(type == 1)
	{
		if(m_mapUseCheDan.find(code)==m_mapUseCheDan.end())
			m_mapUseCheDan[code] = 1;
		else
		{		
			if(m_mapUseCheDan[code] >= m_iCheDanCount)		
				ret = false;
			if(ret)
				m_mapUseCheDan[code]++;

		}

		sprintf(msg,"��Լ %s  �������� %d  ������������ %d",code,m_mapUseCheDan[code],m_iCheDanCount);
		WRITE_LOG(1,msg);

	}
	//�µ�
	else if(type == 0)
	{
		//ƽ��
		if(kp == HS_THOST_FTDC_OF_Close)
			return ret;

		double rate = 0;
		if(bs == POSTION_DIRECTION_BUY)
		{
			rate = CPubData::m_sFutuCodeInfo[code].LMarginRatio;
		}
		else if(bs == POSTION_DIRECTION_SELL)
		{
			rate = CPubData::m_sFutuCodeInfo[code].SMarginRatio;
		}

		double prefreez = price * count * rate * CPubData::m_sFutuCodeInfo[code].iVolumeMultiple;

		//��鱣֤��ռ��
		if(prefreez + m_dUseMargin >= m_dMargin)
		{
			sprintf(msg,"����ί��Ԥ���� %.4f  ��ǰռ�ñ�֤�� %.4f  ������֤���޶�  %.4f",prefreez,prefreez + m_dUseMargin,m_dMargin);
			WRITE_LOG(1,msg);
			ret = false;
		}

		//���ӯ��
		if(m_dUsePL > m_dPL)
		{
			sprintf(msg,"��ǰƽ��ӯ�� %.4f  ����ƽ��ӯ���޶� %.4f",m_dUsePL,m_dPL);
			WRITE_LOG(1,msg);
			ret = false;
		}
	}

	return ret;

}



////
//CPubData
////
CPubData::CPubData()
{


}



////
//CStockInfo
////
CStockInfo::CStockInfo()
{

}

void CStockInfo::LoadCodeInfo()
{
	CStdioFile file;

	std::string path = Tool::GetProgramDir() + "\\StockInfo.txt";

	CString pathstr = path.c_str();

	if(!file.Open(pathstr,CFile::modeRead))
		return;

	CString strCodeInfo;
	CString strSubString0 =_T(",");
	CString strSubString1 =_T(",");
	CString strSubString2 =_T(",");
	CString strSubString3 =_T(",");

	while(file.ReadString(strCodeInfo))
	{

		AfxExtractSubString (strSubString0,(LPCTSTR)strCodeInfo,0,',');
		AfxExtractSubString (strSubString1,(LPCTSTR)strCodeInfo,1,',');
		AfxExtractSubString (strSubString2,(LPCTSTR)strCodeInfo,2,',');
		AfxExtractSubString (strSubString3,(LPCTSTR)strCodeInfo,6,',');


		StockInfo * sc = new StockInfo();        //����ṹ��
		memset(sc,0,sizeof(StockInfo));
		strcpy(sc->code,(LPCTSTR)strSubString0);
		strcpy(sc->Name,(LPCTSTR)strSubString1);
		sc->J_start = atol(strSubString2);
		sc->ActiveCapital = atof(strSubString3);
		//�ж�һ��code������һ���г�
		if (sc->code[0]=='6' && sc->code[1]=='0')
		{
			sc->exchid = 1;
		}
		else
		{
			sc->exchid = 2;
		}

		m_PubData.m_sCodeInfo[sc->code] = *sc;
		delete sc;
		
	}
	file.Close();

}


std::map<std::string,StockInfo> CPubData::m_sCodeInfo;           //��Ʊ��Լ

std::map<std::string,FutuCodeInfo> CPubData::m_sFutuCodeInfo;    //�ڻ���Լ
std::vector<ArbitrageCodeInfo> CPubData::s_vFutuCodeInfo;

std::map<std::string,StockOpt> CPubData::m_mapOptCode;			//��Ʊ��Ȩ����

std::map<std::string, ArbitrageCodeInfo> CPubData::m_mapArbitrageCode;		//��׼������Լ

std::map<std::string, int> CPubData::m_mapExchaneId;