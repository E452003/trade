#include "stdafx.h"
#include "StrategyLimitupFlucuateV1.h"
#include "..\HSQuanTrade\FunctionPub.h"
#include <string>
#include "..\hsquantrade\tool.h"

#define STOCK_TOTAL_CAPITAL 2000000000

CStrategyLimitupFlucuate::CStrategyLimitupFlucuate(void)
{
	m_dbPreVolume = 0.001;
	m_fyingkuiPosition = 0;

	m_fOpenPrice = 0;

	m_bRun = false;

	m_strStrategyName = "��ͣ��򿪲�������";

	//m_nStrategyStatus = STRATEGY_STATUS_BK;

	m_hignPrice = 0;

	m_lastPrice = 0;
	m_downRate1 = 0.05;
	m_downRate2 = 0.08;
	m_loseRate = 0.08;
	m_fyingkuiPosition = 0;
	//m_dbAvgPrice = 0.01;

	//m_nCount = 100;
	m_profitRate = 0.20;
	m_dbShiftLost = 0.02;

	m_strParamList = "��ʼȨ��;ֹ����;ֹӯ��;�ƶ�ֹ����;������;�������̼۱���1;�������̼۱���2;��ͣ����;�˻�;ͬʱ������;��������;�˳��ź�Ϊָ��ۻ����̼�;�Ƿ��µ�;�Ƿ�ο�20�վ���";


	m_params.insert(pair<string, CString>("��ʼȨ��",""));

	m_params.insert(pair<string, CString>("ֹ����",""));
	m_params.insert(pair<string, CString>("ֹӯ��",""));
	m_params.insert(pair<string, CString>("�ƶ�ֹ����",""));
	m_params.insert(pair<string, CString>("������",""));

	m_params.insert(pair<string, CString>("�������̼۱���1",""));
	m_params.insert(pair<string, CString>("�������̼۱���2",""));
	m_params.insert(pair<string, CString>("��ͣ����",""));

	m_params.insert(pair<string, CString>("�˻�",""));
	m_params.insert(pair<string, CString>("ͬʱ������",""));
	m_params.insert(pair<string, CString>("��������",""));
	m_params.insert(pair<string, CString>("�˳��ź�Ϊָ��ۻ����̼�", ""));
	m_params.insert(pair<string, CString>("�Ƿ��µ�", ""));//
	m_params.insert(pair<string, CString>("�Ƿ�ο�20�վ���", ""));//
	m_params.insert(pair<string, CString>("�Ƿ�׷��","-1"));
	m_params.insert(pair<string, CString>("��ʱʱ��","0"));
	m_params.insert(pair<string, CString>("�۲�","0"));

	m_pPolicy = CFundManager::GetPolicyInstance(FUND_POLICY_STOCK_AVG);


}

void CStrategyLimitupFlucuate::CreateStatisticObject()
{
	m_pStatistic = new CStockTradeStatistic();
}
CStrategyLimitupFlucuate::~CStrategyLimitupFlucuate(void)
{

}
void CStrategyLimitupFlucuate::SetParam(CString paramname,CString value)
{
	m_params[(LPCTSTR)paramname] = value;

	if (0 == paramname.Compare("���׺�Լ"))
	{
		if (value.GetLength() > 6)
		{
			value = value.Right(6);

		}
		m_strInstrument = value;
	}

	//����Ҫ����Ӧ��ֵ����
	else if(paramname=="ֹ����")
	{
		m_loseRate = atof(value);
	}
	else if (paramname=="ֹӯ��")
	{
		m_profitRate = atof(value);
	}
	else if (paramname == "�ƶ�ֹ����")
	{
		m_dbShiftLost = atof(value);
	}
	else if(paramname=="������")
	{
		m_changeRate = atof(value);
	}
	else if(paramname=="�������̼۱���1")
	{
		m_downRate1 = atof(value);
	}
	else if(paramname=="�������̼۱���2")
	{
		m_downRate2 = atof(value);
	}
	else if (paramname == "��ͣ����")
	{
		m_iStockType = atoi(value);
	}
	else if(paramname=="�˻�")
	{
		m_fundAccount = value;
	}
	else if(paramname=="��ʼȨ��")
	{
		double dbBeginQuanyi = atof(value);
		SetBeginQuanyi(dbBeginQuanyi);
	}
	else if (paramname == "ͬʱ������")
	{
		
		m_pPolicy->SetParams("ͬʱ������", value);
	}
	else if (paramname == "��������")
	{
		
		m_dbCostRate = atof(value);
		m_pStatistic->m_feilv = m_dbCostRate;
		m_pPolicy->SetCostRate(m_dbCostRate);
	}
	else if (paramname == "�˳��ź�Ϊָ��ۻ����̼�")
	{
		m_iPriceOrderType = atoi(value);
	}
	else if(paramname == "�Ƿ��µ�")
	{
		m_bRealTrade = (0 == atoi(value) ? false : true);
	}
	else if (paramname == "�Ƿ�ο�20�վ���")
	{
		m_bAboveIndexMa20 = (0 == atoi(value) ? false : true);
	}
}

void CStrategyLimitupFlucuate::SetParam(CString value)
{
	//��ʽ��600570;10;40;0.01;0.02;
#ifdef _DEBUG
	AddLog("��ͣ��򿪲�������������...", TRADE_MSG_TYPE_LOG_PRV);
#endif
	CStringArray list;

	CFunctionPub pub;

	pub.ParseParam(value,list,';');

	if( list.GetSize() >= 3 )
	{

		double dbBeginQuanyi = atof(list[0]);
		SetBeginQuanyi(dbBeginQuanyi);

		m_params["��ʼȨ��"] = list[0];

		m_loseRate = atof(list[1]);

		m_params["ֹ����"] = list[1];                      //������

		m_profitRate = atof(list[2]);
		m_params["ֹӯ��"] = list[2];

		m_params["�ƶ�ֹ����"] = list[3];
		m_dbShiftLost = atof(list[3]);

		m_changeRate = atof(list[4]);
		m_params["������"] = list[4];		               //������


		m_downRate1 = atof(list[5]);

		m_params["�������̼۱���1"] = list[5];						//�������̼۱���1

		m_downRate2 = atof(list[6]);
		m_params["�������̼۱���2"] = list[6];						//�������̼۱���2

		m_params["��ͣ����"] = list[7];
		m_iStockType = atoi(list[7]);


		m_fundAccount = list[8];							//�ʺ�   �ö��Ÿ�����  a,b,c,d,
		m_params["�˻�"] = list[8];				

		double nPostionCount = atoi(list[9]);
		m_pPolicy->SetParams("ͬʱ������", list[9]);
		m_params["ͬʱ������"] = list[9];

		m_dbCostRate = atof(list[10]);
		m_params["��������"] = list[10];
		m_pStatistic->m_feilv = m_dbCostRate;
		m_pPolicy->SetCostRate(m_dbCostRate);
		//m_strModelName = m_strStrategyName + m_strInstrument;     //modelname

		m_params["�˳��ź�Ϊָ��ۻ����̼�"] = list[11];
		m_iPriceOrderType = atoi(list[11]);

		m_params["�Ƿ��µ�"] = list[12];
		m_bRealTrade = (0 == atoi(list[12]) ? false : true);

		m_params["�Ƿ�ο�20�վ���"] = list[13];
		m_bAboveIndexMa20 = (0 == atoi(list[13]) ? false : true);
	}

}
//
//double CStrategyLimitupFlucuate::getPreDayHighPrice(const char* szCurDate)
//{
//	double dbPreDayHighPrice = 0.01;
//	if (m_pKManager)
//	{
//		//to be continued...
//		CKStrdata kData;
//
//		if (m_pKManager->getDayKData(m_strInstrument.GetBuffer(), szCurDate, kData))
//		{
//			dbPreDayHighPrice = kData.priceH;
//		}
//		m_strInstrument.ReleaseBuffer();
//
//	}
//
//	return dbPreDayHighPrice;
//}
//
//double CStrategyLimitupFlucuate::getPreDayVolume(const char* szCurDate)
//{
//	if (0.001 == m_dbPreVolume)
//	{
//		if (m_pKManager)
//		{
//			//to be continued...
//			CKStrdata kData;
//
//			if (m_pKManager->getDayKData(m_strInstrument.GetBuffer(), szCurDate, kData))
//			{
//				m_dbPreVolume = kData.vol;
//			}
//			m_strInstrument.ReleaseBuffer();
//			
//			
//		}
//	}
//
//	return m_dbPreVolume;
//}

bool CStrategyLimitupFlucuate::GetDayKData(const char* szCode, const char* szCurDate, CKStrdata &oKData)
{
	ZeroMemory(&oKData, sizeof(oKData));
	if (m_pKManager)
	{
		m_pKManager->getDayKData(szCode, szCurDate, oKData);
		return true;
	}

	return false;
}

bool CStrategyLimitupFlucuate::GetPreNDayKData(const char* szCode, const char* szCurDate, CKStrdata &oKData, int iDay)
{
	ZeroMemory(&oKData, sizeof(oKData));
	if (m_pKManager)
	{
		return m_pKManager->getNDayKData(szCode, szCurDate, oKData, iDay);
	}

	return false;
}

void  CStrategyLimitupFlucuate::Tick(CTickData * pt)
{
	if (m_arCodeSubs.end() == m_arCodeSubs.find(pt->szInstrument))
	{
		return;
	}
	/*if (0 == strcmp(pt->szInstrument, "131810"))
	{
	return;
	}*/
	
	if (!IsRightTickTime(pt->szTime))
	{
		return;
	}
	int iSignalStatus = GetSignalStatus(pt->szInstrument);

	//��¼��߼�
	double dbHighPrice = GetHighPrice(pt->szInstrument);
	if (TRADE_STATUS_INIT == iSignalStatus)
	{
		dbHighPrice = 0;
		SetHighPrice(pt->szInstrument, dbHighPrice);
	}
	else if (TRADE_STATUS_BK_TRADE == iSignalStatus && dbHighPrice < pt->highestprice)
	{
		dbHighPrice = pt->highestprice;
		SetHighPrice(pt->szInstrument, dbHighPrice);
	}

	int iCount = GetStockCount(pt->szInstrument);
	//m_fyingkuiPosition = (pt->lastprice - m_fOpenPrice) * iCount;   //����ӯ��

	//m_lastPrice = pt->lastprice;                                      //��¼�����¼�

	CTime tmLastTradeDate;
	if (!m_pKManager->GetLastKDataDate("999999", tmLastTradeDate))
	{
		AddLog("��ָ֤����ʷ�������ݲ�����", TRADE_MSG_TYPE_LOG_PRV);
		return;
	}
	CString strLastTradeDate = tmLastTradeDate.Format("%Y%m%d");


	CKStrdata oPreKData1;
	if (!GetPreNDayKData(pt->szInstrument, strLastTradeDate, oPreKData1, 0))
	{
		WRITE_LOG(1, "��ȡǰһ��K������ʧ��");
		return;
	}
	
	double dbActiveCaptital = m_pStrategyCodeInfo->getActiveCapital(pt->szInstrument);
	if (dbActiveCaptital > STOCK_TOTAL_CAPITAL) //��ͨ�ɱ�����20�ڹɻ�����ͨ��ֵ����1000�ڵ�Ʊ�ų���
	{
		return;
	}

	double dbPreRate = oPreKData1.vol / dbActiveCaptital;

	char sMsg[256] = {0};
	double dbOpenPrice = 0.0;
	int iOrderRef;

	strcpy(m_szTickTime, pt->szDate);
	strcat(m_szTickTime, " ");
	strcat(m_szTickTime, pt->szTime);
#ifdef _DEBUG
	if (0 == strcmp(pt->szDate, "20150119") )
	{
		AddLog("�����ź�20150119");
	}

#endif

	
	

	//����
	if (TRADE_STATUS_INIT == iSignalStatus)
	{
		CKStrdata oIndexData;
		bool bSuc = m_pKManager->getDayKData("999999", strLastTradeDate, oIndexData);

		if (m_bAboveIndexMa20)
		{
			double dbMa20 = 0;
			if (!m_pKManager->CalcMa20("999999", strLastTradeDate, dbMa20))
			{
				return;
			}
			if (!bSuc || oIndexData.priceS <= dbMa20)
			{
				return;
			}
		}
		

		double dbPreDayClosePrice = oPreKData1.priceS;
		CKStrdata oPreKData2;
		if (!GetPreNDayKData(pt->szInstrument, strLastTradeDate, oPreKData2, -1))
		{
			WRITE_LOG(1, "��ȡǰ����K������ʧ��");
			return;
		}
		double dbPre2DayClosePrice = oPreKData2.priceS;
		if(0 == m_iStockType)
		{
			if (dbPreRate < m_changeRate)
			{

				dbOpenPrice = pt->preClosePrice * (1 - m_downRate1/*0.05*/);

			}
			else//����ѡ��Ĺ�Ʊ���й۲⣬��������������̼ۣ���ô���������޼��µ���
			{
				dbOpenPrice = pt->preClosePrice * (1 - m_downRate2/*0.08*/);

			}

			if (pt->lastprice < pt->preClosePrice 
				&& dbPre2DayClosePrice * 1.1 - dbPreDayClosePrice < 0.001 //������ͣ 
				&& pt->lastprice > dbPreDayClosePrice * 0.9)//�ǵ�ͣ��
			{
				//�������ջ����ʵ͵�(<1%)��Ʊ�����յ������̼�5%�������롣����Ʊ���յ���8%�������롣
				

				if (pt->lastprice <= dbOpenPrice )
				{
					double dbFund;
					if (!ApplyForFund(dbFund))
					{
						sprintf_s(sMsg,256,"%s ���ź�,���ʽ𲻹��� ��ǰ������: %d", pt->szInstrument, GetPositionCount());
						AddLog(sMsg);
						return;
					}

					iCount = ((int)(dbFund / pt->lastprice) / 100 ) * 100;
					SetStockCount(pt->szInstrument, iCount);
					
					
					iOrderRef = OpenBuy(pt->szInstrument, iCount,pt->askprice[1], m_fundAccount.GetBuffer(0));
					sprintf_s(sMsg, 256, "%s ���ź�, ��һ��:%.2f,������:%.2f,������:%.2f,���ļ�:%.2f,�����:%.2f, ��һ��%.2f,�����%.2f,������%.2f,���ļ�%.2f,�����%.2f,",
						pt->szInstrument, pt->askprice[0], pt->askprice[1],pt->askprice[2],pt->askprice[3],pt->askprice[4],
						pt->bidprice[0],pt->bidprice[1],pt->bidprice[2],pt->bidprice[3],pt->bidprice[4]);
					
					AddLog(sMsg, TRADE_MSG_TYPE_LOG_PRV);

					SetAvgPrice(pt->szInstrument, pt->lastprice);
					sprintf_s(sMsg,256,"����,��������:%d",iOrderRef);
					AddLog(sMsg);
					AddOrderInfo(iOrderRef,iCount,dbOpenPrice,'0','0', pt->szInstrument);
				}

			}
		}
		else if(1 == m_iStockType) //һ����ͣ
		{
			if (oPreKData1.priceH == oPreKData1.priceS && 
				oPreKData1.priceH == oPreKData1.priceL &&
				oPreKData1.priceH == oPreKData1.priceK &&
				dbPre2DayClosePrice * 1.1 - dbPreDayClosePrice < 0.001 &&
				pt->lastprice < pt->preClosePrice &&
				pt->lastprice > dbPreDayClosePrice * 0.9)//��20�վ���֮��
			{
				dbOpenPrice = pt->preClosePrice * (1 - m_downRate1/*0.05*/);
				if (pt->lastprice <= dbOpenPrice )
				{

					double dbFund;
					if (!ApplyForFund(dbFund))
					{
						sprintf_s(sMsg,256,"%s ���ź�,���ʽ𲻹��� ��ǰ������: %d", pt->szInstrument, GetPositionCount());
						AddLog(sMsg);
						return;
					}


					iCount = ((int)(dbFund / pt->lastprice) / 100 ) * 100;
					SetStockCount(pt->szInstrument, iCount);
					iOrderRef = OpenBuy(pt->szInstrument, iCount, pt->askprice[1], m_fundAccount.GetBuffer(0));
					sprintf_s(sMsg, 256, "%s ���ź�, ��һ��:%.2f,������:%.2f,������:%.2f,���ļ�:%.2f,�����:%.2f, ��һ��%.2f,�����%.2f,������%.2f,���ļ�%.2f,�����%.2f,",
						pt->szInstrument, pt->askprice[0], pt->askprice[1],pt->askprice[2],pt->askprice[3],pt->askprice[4],
						pt->bidprice[0],pt->bidprice[1],pt->bidprice[2],pt->bidprice[3],pt->bidprice[4]);
					AddLog(sMsg, TRADE_MSG_TYPE_LOG_PRV);

					SetAvgPrice(pt->szInstrument, pt->lastprice);
					sprintf_s(sMsg,256,"����,��������:%d",iOrderRef);
					AddLog(sMsg);
					AddOrderInfo(iOrderRef,iCount,dbOpenPrice,'0','0', pt->szInstrument);
				}
			}
		}
		
	}

	else if (TRADE_STATUS_BK_TRADE == iSignalStatus)
	{
		iCount = GetStockCount(pt->szInstrument);
#ifdef _DEBUG
		if (0 == strcmp(pt->szDate, "20151228"))
		{
			AddLog("�����ź�20151228");
		}

#endif		
		time_t tNow;
		CFunctionPub::String2Time(pt->szDate, tNow);

		time_t tLastTrade = GetOpenPostionTime(pt->szInstrument);
		int iTradeDays = CalcPositionDays(tLastTrade, tNow) + 1;
		if (iTradeDays > 1)
		{

			CKStrdata oPreKData2;
			if (!GetPreNDayKData(pt->szInstrument, strLastTradeDate, oPreKData2, -1))
			{
				AddLog("��ȡǰ����K������ʧ��");
				return;
			}
			double dbPreDayClosePrice = oPreKData1.priceS;
			//double dbPre2DayClosePrice = oPreKData2.priceS;
			//��ͣ������
			if (pt->lastprice - dbPreDayClosePrice * 0.9 <= 0.001)
			{
				AddLog("��ͣ��������");
				return;
			}
			
			//ֹ��
			double dbAvgPrice = GetAvgPrice(pt->szInstrument);
			if (dbAvgPrice > 0.01 && dbAvgPrice * (1 - m_loseRate) > pt->lastprice )
			{
				
				CloseSale(pt->szInstrument, iCount, pt->lastprice, m_fundAccount.GetBuffer(0),'1');
			}
			else
			{
				

				double dbLimitUpPrice = dbPreDayClosePrice * 1.1;
				
				//����
				if (/*iTradeDays >= 3 &&*/ pt->lastprice < dbHighPrice * (1- m_dbShiftLost))
				{
					CloseSale(pt->szInstrument, iCount, pt->lastprice, m_fundAccount.GetBuffer(0),'1');
				}
				else if (/*iTradeDays >= 3 && */dbAvgPrice * ( 1 + m_profitRate) <= pt->lastprice && pt->lastprice < dbLimitUpPrice)
				{
					CloseSale(pt->szInstrument, iCount, pt->lastprice, m_fundAccount.GetBuffer(0),'1');
				}
			}
			
			

			
		}
		

	

	}
	
	


}


int CStrategyLimitupFlucuate::CalcPositionDays(time_t tOpen, time_t tNow)
{
	struct tm *tmFmt = localtime(&tOpen);
	struct tm tmOpen = *tmFmt;

	tmFmt = localtime(&tNow);
	struct tm tmNow = *tmFmt;

	int iDaySpan = 0;
	if (tmOpen.tm_year == tmNow.tm_year) //20151223 vs 20151225
	{
		iDaySpan = tmNow.tm_yday - tmOpen.tm_yday;

	}
	else if (tmOpen.tm_year < tmNow.tm_year) //20151231 vs 20160104
	{
		if (0 == tmOpen.tm_year % 4) //����
		{
			iDaySpan = 366 - tmOpen.tm_yday + tmNow.tm_yday;
		}
		else
		{
			iDaySpan = 365 - tmOpen.tm_yday + tmNow.tm_yday;
		}
	}


	int iWorkDaySpan = 0;
	time_t tNextDay = tOpen;
	for (int i = 0; i < iDaySpan; i++)
	{
		tNextDay = tNextDay + 60*60*24;
		struct tm* tmNext = localtime(&tNextDay);
		if (tmNext->tm_wday > 0 && tmNext->tm_wday < 6)
		{
			iWorkDaySpan++;
		}
	}

	return iWorkDaySpan;

}

void CStrategyLimitupFlucuate::AddOrderInfo(int ref,int count,double price,char bs,char kp, const char* szCode)
{
	m_listOrderRef.Add(ref);

	m_CheckSelfMutex.lock();

	COrderInfo order;
	strcpy(order.szInstrument, szCode);
	order.ref = ref;
	order.ordercount = count;
	order.tradecount = 0;
	order.orderprice = price;
	order.bs = bs;
	order.kp = kp;
	m_orderinfo[ref] = order;
	

	m_CheckSelfMutex.unlock();

}

CString CStrategyLimitupFlucuate::GetParamValue()
{
	CString str;
	

	CString strValue = m_pPolicy->GetParams("ͬʱ������");
	
	int nPostionCount = atoi(strValue);
	//"��ʼȨ��;ֹ����;ֹӯ��;�ƶ�ֹ����;������;�������̼۱���1;�������̼۱���2;��ͣ����;�˻�;ͬʱ������;��������;�˳��ź�Ϊָ��ۻ����̼�;�Ƿ��µ�";
	double dbFund = m_pPolicy->GetCurrentFund();
	str.Format("%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%d;%s;%d;%.2f;%d;%d",
		m_dbBeginQuanyi,//��ʼȨ��
		m_loseRate,//ֹ����
		m_profitRate,//ֹӯ��
		m_dbShiftLost,//�ƶ�ֹ����
		m_changeRate,//������
		m_downRate1, //�������̼۱���1
		m_downRate2,//�������̼۱���2

		m_iStockType,//��ͣ����
		m_fundAccount,//�˻�

		nPostionCount,//ͬʱ������
		m_dbCostRate,//��������

		m_iPriceOrderType,//�˳��ź�Ϊָ��ۻ����̼�
		m_bRealTrade);//�Ƿ��µ�

	return str;
}

CString CStrategyLimitupFlucuate::GetParamValue(CString paramname)
{
		return m_params[(LPCTSTR)paramname];
}

CString CStrategyLimitupFlucuate::GetStatusInfo(void)
{
	CString result;

	CString status;

	CString openrice;

	CString yingkuiPos;

	CString modelstatus;

	CString codename;

	CString yinkuirate;

	CString huitiaorate;

	/*
	if( m_nStrategyStatus ==  STRATEGY_STATUS_INIT )
	{
	status = "�ղ�";

	openrice.Format("%.4f",m_fOpenPrice);
	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_BK )
	{
	status = "��";

	openrice.Format("%.4f",m_fOpenPrice);

	yingkuiPos.Format("%.2f",m_fyingkuiPosition);
	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_SK )
	{
	status = "����";

	openrice.Format("%.4f",m_fOpenPrice);

	yingkuiPos.Format("%.2f",m_fyingkuiPosition);
	}
	*/
	if( m_bRun == false )
		modelstatus = "��ͣ";
	else
		modelstatus = "����";

	yingkuiPos.Format("%.4f",m_fyingkuiPosition);

	codename = m_pStrategyCodeInfo->getName(m_strInstrument.GetBuffer(0));

	m_strInstrument.ReleaseBuffer();

	yinkuirate.Format("%.2f",(m_lastPrice - atof(openrice)) / atof(openrice));

	if(m_hignPrice < 0.001)
	{
		huitiaorate = "0.00";
	}
	else
	{
		huitiaorate.Format("%.2f",((m_hignPrice - m_lastPrice) / m_hignPrice));
	}
	int iCount = GetStockCount(m_strInstrument);
	result.Format("%s;%s;%s;%d;%s;%s;%s(%s);%s;%s;%.4f;%.4f(%s);%s;",m_strStrategyName,m_strInstrument,codename,iCount,status,openrice,yingkuiPos,yinkuirate,"0",modelstatus,m_lastPrice,m_hignPrice,huitiaorate,m_fundAccount);

	return result;
}

void CStrategyLimitupFlucuate::BeginRun()
{

}

void CStrategyLimitupFlucuate::OnTradeReturn(char * ins,char * sTime,int orderref,int localorderid,char direction,char offsetflag,int vol,double price)
{
	bool bt = false;

	bt = isExistRef(orderref,direction,offsetflag,ins);
	if (bt)
	{
		if ('0' == offsetflag && '0' == direction)
		{
			time_t tLastTrade = time(NULL);
			SetOpenPostionTime(ins, tLastTrade);
			m_orderinfo[orderref].tradecount += vol;
			if (m_orderinfo[orderref].ordercount == m_orderinfo[orderref].tradecount)
			{
				//m_dbAvgPrice = price;
				SetAvgPrice(ins, price);
			}
			
		}
		else if ('1' == offsetflag && '1' == direction)
		{
			
		}
	}
	
}

void CStrategyLimitupFlucuate::OnErrorRtn(int code,char * errormsg)
{

}

void CStrategyLimitupFlucuate::OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins)
{

}

void CStrategyLimitupFlucuate::reload()
{

}

void CStrategyLimitupFlucuate::saveModel()
{

}


bool CStrategyLimitupFlucuate::isExistRef(int ref,char bs,char kp,char * ins)
{
	map<int,COrderInfo> ::iterator iter;

	m_CheckSelfMutex.lock();

	iter = m_orderinfo.find(ref);
	if(iter!=m_orderinfo.end())
	{
		//�Ȱ�kpȥ��������1 ��3 ��4 Ҳ������
		if( (strcmp(iter->second.szInstrument,ins)==0) && (iter->second.bs==bs) /*&& (iter->second.kp==kp)*/ )
		{
			m_CheckSelfMutex.unlock();
			return true;
		}
	}
	m_CheckSelfMutex.unlock();
	return false;

}

int CStrategyLimitupFlucuate::CloseAll(char* szInstrument, double dbPrice)
{ 
	int iSignalStatus = GetSignalStatus(szInstrument);
	if (TRADE_STATUS_BK_TRADE == iSignalStatus)
	{
		int iCount = GetStockCount(szInstrument);
		CloseSale(szInstrument, iCount, dbPrice, m_fundAccount.GetBuffer(0),'1');
	}
	
	return 0;
}

double CStrategyLimitupFlucuate::GetHighPrice(const char* szCode)
{
	double dbHigh = 0.0;
	if (m_mapHighPrice.find(szCode) != m_mapHighPrice.end())
	{
		dbHigh = m_mapHighPrice[szCode];
	}

	return dbHigh;
}
void CStrategyLimitupFlucuate::SetHighPrice(const char* szCode, double dbPrice)
{
	m_mapHighPrice[szCode] = dbPrice;
}

void CStrategyLimitupFlucuate::BackTest(CKStrdata *pk, const char* sCode)
{

	//�����̼�
	CKStrdata oKData;
	double dbPreClosePrice;
	if (m_pKManager->getNDayKData(sCode, pk->curDate, oKData, -1))
	{
		dbPreClosePrice = oKData.priceS;

	}

	int iSignalStatus = GetSignalStatus(sCode);

#ifdef _DEBUG
	if (0 == strcmp(pk->curDate, "20150115") )
	{
		AddLog("�����ź�20150115");
	}
#endif

	//��¼��߼�
	double dbHighPrice = GetHighPrice(sCode);
	if (TRADE_STATUS_INIT == iSignalStatus)
	{
		dbHighPrice = 0;
		SetHighPrice(sCode, dbHighPrice);
	}
	else if (TRADE_STATUS_BK_TRADE == iSignalStatus && dbHighPrice < pk->priceH)
	{
		dbHighPrice = pk->priceH;
		SetHighPrice(sCode, dbHighPrice);
	}

	int iCount = GetStockCount(sCode);
	//m_fyingkuiPosition = (pt->lastprice - m_fOpenPrice) * iCount;   //����ӯ��

	//m_lastPrice = pt->lastprice;                                      //��¼�����¼�

	CKStrdata oPreKData1;
	if (!GetPreNDayKData(sCode, pk->curDate, oPreKData1, -1))
	{
		AddLog("��ȡǰһ��K������ʧ��");
		return;
	}

	double dbActiveCaptital = m_pStrategyCodeInfo->getActiveCapital((char*)sCode);
	if (dbActiveCaptital > STOCK_TOTAL_CAPITAL) //��ͨ�ɱ�����20�ڹɻ�����ͨ��ֵ����1000�ڵ�Ʊ�ų���
	{
		return;
	}

	double dbPreRate = oPreKData1.vol / dbActiveCaptital;

	char sMsg[256] = {0};
	double dbOpenPrice = 0.0;
	int iOrderRef;



	double dbMa20 = 0;

	if (!m_pKManager->CalcMa20("999999", pk->curDate, dbMa20))
	{
		return;
	}

	
	//����
	if (TRADE_STATUS_INIT == iSignalStatus)
	{
		CKStrdata oIndexData;
		bool bSuc = m_pKManager->getDayKData("999999", pk->curDate, oIndexData);

		if (!bSuc || oIndexData.priceS <= dbMa20)
		{
			return;
		}

		double dbPreDayClosePrice = oPreKData1.priceS;
		CKStrdata oPreKData2;
		if (!GetPreNDayKData(sCode, pk->curDate, oPreKData2, -2))
		{
			AddLog("��ȡǰ����K������ʧ��");
			return;
		}
		double dbPre2DayClosePrice = oPreKData2.priceS;
		if(0 == m_iStockType)
		{
			if (dbPreRate < m_changeRate)
			{

				dbOpenPrice = dbPreClosePrice * (1 - m_downRate1/*0.05*/);

			}
			else//����ѡ��Ĺ�Ʊ���й۲⣬��������������̼ۣ���ô���������޼��µ���
			{
				dbOpenPrice = dbPreClosePrice * (1 - m_downRate2/*0.08*/);

			}
			double dbOrderPrice;
			if (pk->priceL <= dbOpenPrice && pk->priceH >= dbOpenPrice)//ָ����볡
			{
				dbOrderPrice = dbOpenPrice;
			}
			
			if (dbOrderPrice < dbPreClosePrice 
				&& dbPre2DayClosePrice * 1.1 - dbPreDayClosePrice < 0.001 //������ͣ 
				&& dbOrderPrice > dbPreDayClosePrice * 0.9)//�ǵ�ͣ��
			{
				//�������ջ����ʵ͵�(<1%)��Ʊ�����յ������̼�5%�������롣����Ʊ���յ���8%�������롣


				if (dbOrderPrice <= dbOpenPrice )
				{
					double dbFund;
					if (!ApplyForFund(dbFund))
					{
						sprintf_s(sMsg,256,"%s ���ź�,���ʽ𲻹��� ��ǰ������: %d", sCode, GetPositionCount());
						AddLog(sMsg);
						return;
					}

					iCount = ((int)(dbFund / dbOrderPrice) / 100 ) * 100;
					SetStockCount(sCode, iCount);
					iOrderRef = OpenBuy((char*)sCode, iCount, dbOrderPrice, m_fundAccount.GetBuffer(0));
					if(m_bBackTest || false == m_bRun)
					{
						//	m_dbAvgPrice = pt->lastprice;
						SetAvgPrice(sCode, dbOrderPrice);
					}

					sprintf_s(sMsg,256,"����,��������:%d",iOrderRef);
					AddLog(sMsg);
					AddOrderInfo(iOrderRef,iCount,dbOpenPrice,'0','0', sCode);
				}

			}
		}
		else if(1 == m_iStockType) //һ����ͣ
		{
			dbOpenPrice = dbPreClosePrice * (1 - m_downRate1/*0.05*/);
			double dbOrderPrice;
			if (pk->priceL <= dbOpenPrice && pk->priceH >= dbOpenPrice)//ָ����볡
			{
				dbOrderPrice = dbOpenPrice;
			}

			if (oPreKData1.priceH == oPreKData1.priceS && 
				oPreKData1.priceH == oPreKData1.priceL &&
				oPreKData1.priceH == oPreKData1.priceK &&
				dbPre2DayClosePrice * 1.1 - dbPreDayClosePrice < 0.001 &&
				dbOrderPrice <dbPreClosePrice &&
				dbOrderPrice > dbPreDayClosePrice * 0.9)//��20�վ���֮��
			{
				
				if (dbOrderPrice <= dbOpenPrice )
				{

					double dbFund;
					if (!ApplyForFund(dbFund))
					{
						sprintf_s(sMsg,256,"%s ���ź�,���ʽ𲻹��� ��ǰ������: %d", sCode, GetPositionCount());
						AddLog(sMsg);
						return;
					}


					iCount = ((int)(dbFund / dbOrderPrice) / 100 ) * 100;
					SetStockCount(sCode, iCount);
					iOrderRef = OpenBuy((char*)sCode, iCount, dbOrderPrice, m_fundAccount.GetBuffer(0));
					if(m_bBackTest || false == m_bRun)
					{
						//m_dbAvgPrice = pt->lastprice;
						SetAvgPrice(sCode, dbOrderPrice);
					}

					sprintf_s(sMsg,256,"����,��������:%d",iOrderRef);
					AddLog(sMsg);
					AddOrderInfo(iOrderRef,iCount,dbOrderPrice,'0','0', sCode);
				}
			}
		}

	}

	else if (TRADE_STATUS_BK_TRADE == iSignalStatus)
	{
		iCount = GetStockCount(sCode);
#ifdef _DEBUG
		if (0 == strcmp(pk->curDate, "20150115"))
		{
			WRITE_LOG(1, "�����ź�20150115");
		}

#endif		
		time_t tNow;
		CFunctionPub::String2Time(pk->curDate, tNow);

		time_t tLastTrade = GetOpenPostionTime(sCode);
		int iTradeDays = CalcPositionDays(tLastTrade, tNow) + 1;
		if (iTradeDays > 1)
		{

			CKStrdata oPreKData2;
			if (!GetPreNDayKData(sCode, pk->curDate, oPreKData2, -2))
			{
				WRITE_LOG(1, "��ȡǰ����K������ʧ��");
				return;
			}
			double dbPreDayClosePrice = oPreKData1.priceS;
			
			//��ͣ������
			if (pk->priceL - dbPreDayClosePrice * 0.9 <= 0.001 &&
				pk->priceH == pk->priceS && 
				pk->priceH ==pk->priceL &&
				pk->priceH == pk->priceK )
			{
				AddLog("��ͣ��������");
				return;
			}

			
			double dbAvgPrice = GetAvgPrice(sCode);
			double dbLimitUpPrice = dbPreDayClosePrice * 1.1;
			double dbLostPrice = dbAvgPrice * (1 - m_loseRate);
			double dbZhiyinPrice = dbAvgPrice * ( 1 + m_profitRate);
			double dbHuitiaoPrice = dbHighPrice * (1- m_dbShiftLost);

			if (pk->priceL <= dbLostPrice)
			{
				CloseSale((char*)sCode, iCount, dbLostPrice, m_fundAccount.GetBuffer(0),'1');
			}
			else
			{
				if (0 == m_iPriceOrderType)
				{
					//ֹӯ
					if (pk->priceH > dbZhiyinPrice)
					{
						CloseSale((char*)sCode, iCount, dbZhiyinPrice, m_fundAccount.GetBuffer(0),'1');
					}
					else//�ص�
					{
						if (pk->priceS <= dbHuitiaoPrice)
						{
							CloseSale((char*)sCode, iCount, dbHuitiaoPrice, m_fundAccount.GetBuffer(0),'1');
						}
					}
					
				}
				else //���̼�
				{
					//ֹӯ
					if (pk->priceS > dbZhiyinPrice)
					{
						CloseSale((char*)sCode, iCount, pk->priceS, m_fundAccount.GetBuffer(0),'1');
					}
					else//�ص�
					{
						if (pk->priceS <= dbHuitiaoPrice)
						{
							CloseSale((char*)sCode, iCount, pk->priceS, m_fundAccount.GetBuffer(0),'1');
						}
						
					}
				}
			}
#if 0
			if (0 == m_iPriceOrderType) //ָ���
			{
				//if (pk->priceL < dbLostPrice)
				//{
				//	CloseSale((char*)sCode, iCount, pk->priceL, m_fundAccount.GetBuffer(0),'1');
				//}
				if (pk->priceL <= dbLostPrice && pk->priceH >= dbLostPrice) //ֹ��
				{
					
					CloseSale((char*)sCode, iCount, dbLostPrice, m_fundAccount.GetBuffer(0),'1');
				}
				else
				{
					//ֹӯ
					if (pk->priceH > dbZhiyinPrice)
					{
						CloseSale((char*)sCode, iCount, dbZhiyinPrice, m_fundAccount.GetBuffer(0),'1');
					}
					else//��߼ۻص�
					{
						if (pk->priceH > dbHuitiaoPrice && pk->priceL < dbHuitiaoPrice)
						{
							CloseSale((char*)sCode, iCount, dbHuitiaoPrice, m_fundAccount.GetBuffer(0),'1');
						}
						else if (pk->priceH < dbHuitiaoPrice) //����
						{
							CloseSale((char*)sCode, iCount, pk->priceH, m_fundAccount.GetBuffer(0),'1');
						}
					}
					
				}
			}
			else //���̼�
			{
				if (pk->priceS < dbLostPrice)
				{
					CloseSale((char*)sCode, iCount, pk->priceS, m_fundAccount.GetBuffer(0),'1');
				}
				else
				{
					
					//����
					if (/*iTradeDays >= 3 &&*/ pk->priceS < dbHuitiaoPrice)
					{
						CloseSale((char*)sCode, iCount, pk->priceS, m_fundAccount.GetBuffer(0),'1');
					}
					else if (/*iTradeDays >= 3 && */dbZhiyinPrice <= pk->priceS && pk->priceS < dbLimitUpPrice)
					{
						CloseSale((char*)sCode, iCount, pk->priceS, m_fundAccount.GetBuffer(0),'1');
					}
				}
			}

#endif

		}
	}

}


void CStrategyLimitupFlucuate::OnSubcode()
{
	vector<string> arCodes;
	char sMsg[256];

#ifdef TEST //_DEBUG
	char sSubcode[256];

	sprintf(sSubcode, "subcode#%s;STOCK;UFX;#", "300028");
	sprintf(sMsg, "�Ѷ��� %s ����", "300028");
	::SendMessage(m_hMsgWnd, WM_MESSAGE_SUBCODE, 0, (LPARAM)sSubcode);
	AddLog(sMsg, TRADE_MSG_TYPE_LOG_PRV);
#else

	//step1: ��鿪�ֹ�Ʊ
	m_arCodeSubs.clear();
	std::map<string, int>::const_iterator itSignal = m_mapSignalStatus.cbegin();
	while (itSignal != m_mapSignalStatus.cend())
	{
		if (TRADE_STATUS_BK_TRADE == itSignal->second || TRADE_STATUS_SK_TRADE == itSignal->second)
		{

			 char sSubcode[256];

			 sprintf(sSubcode, "subcode#%s;STOCK;UFX;#",  itSignal->first.c_str());
			 sprintf(sMsg, "�Ѷ��� %s ����",  itSignal->first.c_str());
			 ::SendMessage(m_hMsgWnd, WM_MESSAGE_SUBCODE, 0, (LPARAM)sSubcode);
			 
			 m_arCodeSubs[itSignal->first] = itSignal->second;
		}
		itSignal++;
	}

	if (m_pKManager->GetCodeList(arCodes) > 0)
	{
		vector<string>::const_iterator itCode = arCodes.cbegin();

		while (itCode != arCodes.cend())
		{
		
			if (m_arCodeSubs.end() != m_arCodeSubs.find(*itCode))
			{
				itCode++;
				continue;
			}
			double dbActiveCaptital = m_pStrategyCodeInfo->getActiveCapital((char*)itCode->c_str());
			if (dbActiveCaptital > STOCK_TOTAL_CAPITAL) //��ͨ�ɱ�����20�ڹɻ�����ͨ��ֵ����1000�ڵ�Ʊ�ų���
			{
				itCode++;
				continue;
			}
			CTime tmLastTradeDate;
			if (!m_pKManager->GetLastKDataDate("999999", tmLastTradeDate))
			{
				WRITE_LOG(1, "��ָ֤����ʷ�������ݲ�����", TRADE_MSG_TYPE_LOG_PRV);
				break;
			}


			CString strTime = tmLastTradeDate.Format("%Y%m%d");
			CKStrdata oPreKData, oPreKData2;
			if (!GetPreNDayKData(itCode->c_str(), strTime, oPreKData, 0))
			{
				sprintf(sMsg, "��Ʊ���벻����:%s", itCode->c_str());
				WRITE_LOG(1, sMsg);

				itCode++;
				continue;
			}

			if (!GetPreNDayKData(itCode->c_str(), strTime, oPreKData2, -1))
			{
				sprintf(sMsg, "��ȡǰ����K������ʧ��:%s", itCode->c_str());
				WRITE_LOG(1, sMsg);
				itCode++;
				continue;
			}
			if (oPreKData2.priceS * 1.1 - oPreKData.priceS < 0.001)//������ͣ 
			{
				//��������
				char sSubcode[256];

				sprintf(sSubcode, "subcode#%s;STOCK;UFX;#", itCode->c_str());
				sprintf(sMsg, "�Ѷ��� %s ����", itCode->c_str());
				::SendMessage(m_hMsgWnd, WM_MESSAGE_SUBCODE, 0, (LPARAM)sSubcode);
				WRITE_LOG(1, sMsg);

				m_arCodeSubs[*itCode] = 0;
			}



			itCode++;
		}

		sprintf(sMsg, "���ζ��Ĺ�������Ʊ%d��, ���гɹ����Ĺ�Ʊ%d��", arCodes.size(), m_arCodeSubs.size());
		AddLog(sMsg, TRADE_MSG_TYPE_LOG_PRV);
	}

#endif
	
}