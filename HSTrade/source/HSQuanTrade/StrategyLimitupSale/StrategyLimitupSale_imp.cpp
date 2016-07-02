#include "stdafx.h"
#include "StrategyLimitupSale_imp.h"
#include <map>
#include <string>
#include "..\HSQuanTrade\FunctionPub.h"
#include <time.h>
#include "..\HSQuanTrade\tool.h"


CStrategyLimitupSale::CStrategyLimitupSale()
{

	m_strStrategyName = "����ͣ������";
	
	m_nStrategyStatus = STRATEGY_STATUS_BK;

	m_fyingkuiPosition = 0;

	m_lastPrice = 0;

	m_hignPrice = 0;

	m_follow = "-1";                   //�Ƿ����,Ĭ��д��-1

	m_timeOut = 0;                     //��ʱʱ��

	jiacha = 0;					       //�۲�

	m_nCount = 0;

	m_fLoseRate = 0.05;               //ֹ���

	m_fGetRate = 0.25;

	m_fBackRate = 0.05;               //��߼ۻص���

	m_Exit = false;

	m_bRun = false;

	m_done = false;

	m_dTradePrice = 0.0;                 //�ɱ���

	m_iDurationTime = 60;

	m_strParamList = "���׺�Լ;����;���ּ۸�;�ص���;ֹ����;ֹӯ��;����ʱ��;�˻�;";

	m_params.insert(pair<string, CString>("���׺�Լ",""));

	m_params.insert(pair<string, CString>("����","0"));

	m_params.insert(pair<string, CString>("���ּ۸�","0"));

	m_params.insert(pair<string, CString>("�ص���","0.05"));

	m_params.insert(pair<string, CString>("ֹ����","0.05"));

	m_params.insert(pair<string, CString>("ֹӯ��","0.25"));

	m_params.insert(pair<string, CString>("����ʱ��","60"));

	m_params.insert(pair<string, CString>("�˻�",""));

	m_params.insert(pair<string, CString>("�Ƿ�׷��","-1"));

	m_params.insert(pair<string, CString>("��ʱʱ��","0"));

	m_params.insert(pair<string, CString>("�۲�","0"));

	
}

CStrategyLimitupSale::~CStrategyLimitupSale()
{
	m_bRun = false;

	saveModel();

}


void CStrategyLimitupSale::Tick(CTickData * pt)
{
	if(strcmp(m_strInstrument,pt->szInstrument)!=0)
		return;

	time_t rawtime;
	time ( &rawtime );
	long timestamp = (long)rawtime;


	double buy1 = pt->bidprice[0];
	double buy2 = pt->bidprice[1];
	double buy5 = pt->bidprice[4];
	double sale1 = pt->askprice[0];
	double sale2 = pt->askprice[1];
	double limitup = pt->UpperLimitPrice;
	double limitlow = pt->LowerLimitPrice;
	double lastprice = pt->lastprice;
	char*code = pt->szInstrument;
	int buy1count = pt->bidvol[0];
	int buy2count = pt->bidvol[1];
	int sale1count = pt->askvol[0];

	//��¼��߼�
	if(pt->highestprice > m_hignPrice)
	{
		m_hignPrice = pt->highestprice;  //��߼�
	}

	//�ֲ�ӯ��
	m_fyingkuiPosition = (pt->lastprice - m_dTradePrice) * m_nCount;

	m_lastPrice = pt->lastprice;

	
	CString price;

	price.Format("��Լ:%s,���¼�:%.3f,��߼�:%.3f",code,lastprice,m_hignPrice);

	WRITE_LOG(1,price.GetBuffer(0));

	if( (data==NULL) || (!(*data).isReady) )
		return;


	char msg[256];

	//9:35ǰ������
	if(isDoNothingTime() || (m_done))
	{
		return;
	}


	//14:55������5%�м�����
	if( (isCloeTime()) && (m_lastPrice < m_dTradePrice* (1- m_fLoseRate)) )
	{
		playsound();

		OpenSale(code,m_nCount,limitlow,m_fundAccount.GetBuffer(0));

		sprintf(msg,"14:55,��Լ:%s,�������%.3f,�м�����...�ɱ���%.3f,��ǰ�۸�%.3f",code,m_fLoseRate,m_dTradePrice,pt->lastprice);

		AddLog(msg);

		m_done = true;

		m_nStrategyStatus =  STRATEGY_STATUS_INIT;

	}

	//ֹ��ֹӯ
	//����֮ǰ�и�14:55ֹӯ���������������������������⴦��ֻ��������ֹӯ�ʺ�ֹ��ֹӯ��������
	if( m_fGetRate > 0.001)
	{
		if( (m_lastPrice < m_dTradePrice* (1 - m_fLoseRate) ) || (m_lastPrice > m_dTradePrice* (1 +  m_fGetRate)) ) 
		{
			playsound();

			OpenSale(code,m_nCount,limitlow,m_fundAccount.GetBuffer(0));

			sprintf(msg,"14:55,��Լ:%s,�ﵽֹӯ(%.3f)ֹ���(%.3f),�м�����...�ɱ���%.3f,��ǰ�۸�%.3f",code,m_fGetRate,m_fLoseRate,m_dTradePrice,pt->lastprice);

			AddLog(msg);

			m_done = true;

			m_nStrategyStatus =  STRATEGY_STATUS_INIT;
		
		}

	}

	//��߼ۻص�5%
	if( m_lastPrice  < (m_hignPrice * (1 - m_fBackRate)) )
	{
		sprintf(msg,"��Լ:%s,��߼�:%.4f,���¼�:%.4f,�ص�����%.3f,��¼...",code,m_hignPrice,m_lastPrice,m_fBackRate);

		AddLog(msg);

		if(m_sSignalTime[code] == -9999 || m_sSignalTime[code] == 0 )
		{
			m_sSignalTime[code] = timestamp;

		}

		//����һ���ӵ������ź�
		if( (timestamp - m_sSignalTime[code] > m_iDurationTime) && (m_sSignalTime[code]!=0) )
		{
			playsound();

			sprintf(msg,"��Լ:%s,��ǰʱ��:%ld,���ʱ��%ld,����һ����,����...",code,timestamp,m_sSignalTime[code]);

			AddLog(msg);

			OpenSale(code,m_nCount,limitlow,m_fundAccount.GetBuffer(0));

			m_done = true;

			m_nStrategyStatus =  STRATEGY_STATUS_INIT;
		}

	}
	//û�������ź�
	else
	{
		m_sSignalTime[code] = -9999;

	}

	if(isSaveTime())
	{
		saveModel();
	
	}

}


void CStrategyLimitupSale::SetParam(CString paramname,CString value)
{
	//"���׺�Լ;����;�۸�;��������;��ƽ����;ί����Ϣ;�˻�;";

	m_params[(LPCTSTR)paramname] = value;

	//����Ҫ����Ӧ��ֵ����
	if(paramname=="���׺�Լ")
	{
		m_strInstrument = value;
	}
	else if(paramname=="����")
	{
		m_nCount = atoi(value);
	}
	else if(paramname=="���ּ۸�")
	{
		m_dTradePrice = atof(value);
	}
	else if(paramname=="�ص���")
	{
		m_fBackRate = atof(value);
	}
	else if(paramname=="ֹ����")
	{
		m_fLoseRate = atof(value);
	}
	else if(paramname=="ֹӯ��")
	{
		m_fGetRate = atof(value);
	}
	else if(paramname=="����ʱ��")
	{
		m_iDurationTime = atoi(value);
	}
	else if(paramname=="�˻�")
	{
		m_fundAccount = value;
	}

}


void CStrategyLimitupSale::SetParam(CString value)
{

	CStringArray list;

	CFunctionPub pub;

	pub.ParseParam(value,list,';');

	CStringArray listField;

	pub.ParseParam(	m_strParamList,listField,';');

	for(int i=0;i<listField.GetSize()&&i<list.GetSize();i++)
	{
		SetParam(listField[i],list[i]);
	}

	/*m_strModelName = m_strStrategyName;*/

}


//"���׺�Լ;����;�۸�;�ص�����;ֹ�����;�˻�;";
CString CStrategyLimitupSale::GetParamValue()
{
	CString str;

	str.Format("%s;%d;%.3f;%.3f;%.3f;%.3f;%d;%s;",m_strInstrument,m_nCount,m_dTradePrice,m_fBackRate,m_fLoseRate,m_fGetRate,m_iDurationTime,m_fundAccount);

	return str;
}


CString CStrategyLimitupSale::GetParamValue(CString paramname)
{
	return m_params[(LPCTSTR)paramname];
}

CString CStrategyLimitupSale::GetStatusInfo(void)
{
	CString result;

	CString status;

	CString yingkuiPos;

	CString modelstatus;

	CString codename;

	double yinkuirate;

	double huitiaorate;

	if( m_nStrategyStatus ==  STRATEGY_STATUS_INIT )
	{
		status = "�ղ�";

	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_BK )
	{
		status = "��";

	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_SK )
	{
		status = "����";

	}


	if( m_bRun == false )
		modelstatus = "��ͣ";
	else
		modelstatus = "����";


	yingkuiPos.Format("%.4f",m_fyingkuiPosition);

	codename = m_pStrategyCodeInfo->getName(m_strInstrument.GetBuffer(0));

	if(m_lastPrice < 0.001)
	{
		yinkuirate = 0.0;
	}
	else
	{
		yinkuirate = (m_lastPrice - m_dTradePrice) / m_dTradePrice;
	}
	


	if(m_hignPrice < 0.001)
	{
		huitiaorate = 0.00;
	}
	else
	{
		huitiaorate =( m_hignPrice - m_lastPrice) / m_hignPrice;
	}


	result.Format("%s;%s;%s;%d;%s;%.3f;%.4f(%.3f);%.4f;%s;%.4f;%.4f(%.3f);%s;%s;",m_strStrategyName,m_strInstrument,codename,
		m_nCount,status,m_dTradePrice,m_fyingkuiPosition,yinkuirate,0.0,modelstatus,m_lastPrice,m_hignPrice,huitiaorate,m_fundAccount,/*m_strModelName*/GetStrategyId());

	return result;

}

void CStrategyLimitupSale::BeginRun()
{
	reload();

	//���ĺ�Լ
	//"subcode#" + "131810" + ";" + "STOCK;UFX;" + "#"
	string subInfo;
	subInfo.append("subcode#").append(m_strInstrument).append(";STOCK;UFX;#");

	::SendMessage(m_hMsgWnd,WM_MESSAGE_ENTRUST,7,(LPARAM)subInfo.c_str());
}

void CStrategyLimitupSale::Pause()
{
	char msg[128];

	if(!m_bRun)
	{
		sprintf(msg,"��ͣ...");

		AddLog(msg);

		m_sSignalTime[m_strInstrument.GetBuffer(0)] = -9999;

		return;
		
	}
}

bool CStrategyLimitupSale::isDoNothingTime()
{
	bool ret =false;

	const time_t t = time(NULL);

	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;


	if(h==9 && m<=35)
	{
		ret =  true;
	}

	return ret;

}

bool CStrategyLimitupSale::isCloeTime()
{
	bool ret =false;

	const time_t t = time(NULL);

	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;

    if(h==14 && m>=55)
	{
		ret =  true;
	}

	return ret;

}


bool CStrategyLimitupSale::isSaveTime()
{

	bool ret = false;

	const time_t t = time(NULL);
	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;

	if(h == 14 && m >= 58)
	{
		ret = true;
	}

	return ret;

}

//����ر�
void CStrategyLimitupSale::OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price)
{


}

void CStrategyLimitupSale::OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins)
{

}


void CStrategyLimitupSale::reload()
{
	//ԭʼ�ַ��� c++11
	string path = Tool::GetModelDir() + "StrategyLimitupSale.ini";
	
	char hignprice[32];

	GetPrivateProfileString( "HighestPrice", m_strInstrument, "0.0", hignprice,16 ,(char*)path.c_str() );

	m_hignPrice = atof(hignprice);

	
}

void CStrategyLimitupSale::saveModel()
{
	//ԭʼ�ַ��� c++11
	CString path ;

	string s= Tool::GetModelDir();

	path = s.c_str();	

	path = path + "StrategyLimitupSale.ini";

	char sHighPrice[12];

	sprintf(sHighPrice,"%.3f",m_hignPrice);

	WritePrivateProfileString("HighestPrice", m_strInstrument, sHighPrice, path);

}

