#include "stdafx.h"

#include "StrategyLimitupSale.h"
#include "TradeInterface.h"
#include "FunctionPub.h"


CStrategyLimitupSale::CStrategyLimitupSale(void)
{
	m_fyingkuiPosition = 0;

	m_fOpenPrice = 0;

	m_bRun = false;

	m_strStrategyName = "��ͣ���Ʊ��������";

	m_nStrategyStatus = STRATEGY_STATUS_BK;
	
    m_hignPrice = 0;

	m_lastPrice = 0;

	m_isToHighPrice =false;

	m_strParamList = "���׺�Լ;��������;���ּ�;ֹ����;������;�ص���;�Ƿ�׷��;��ʱʱ��;�˻�;";

	m_params.insert(pair<string, CString>("���׺�Լ",""));

	m_params.insert(pair<string, CString>("��������",""));

	m_params.insert(pair<string, CString>("���ּ�",""));

	m_params.insert(pair<string, CString>("ֹ����",""));

	m_params.insert(pair<string, CString>("������",""));

	m_params.insert(pair<string, CString>("�ص���",""));

	m_params.insert(pair<string, CString>("�Ƿ�׷��",""));

	m_params.insert(pair<string, CString>("��ʱʱ��",""));

	m_params.insert(pair<string, CString>("�˻�",""));

}


CStrategyLimitupSale::~CStrategyLimitupSale(void)
{
}



void  CStrategyLimitupSale::Tick(CTickData * pt)
{

	if(strcmp(m_strInstrument,pt->szInstrument)!=0)
		return;

	m_hignPrice = pt->highestprice;                                  //��¼��߼�

	if(pt->lastprice==pt->UpperLimitPrice)
	{
		m_isToHighPrice = true;                                       //�ﵽ����ͣ��
	}


	if(m_nStrategyStatus!=STRATEGY_STATUS_BK)
		return;

	CString log = "";
	int iCount = GetStockCount(pt->szInstrument);
	m_fyingkuiPosition = (pt->lastprice - m_fOpenPrice) * iCount;   //����ӯ��

	m_lastPrice = pt->lastprice;                                      //��¼�����¼�

	//����ͣ��
	if((pt->lastprice < pt->UpperLimitPrice) && (m_isToHighPrice == true))
	{
		//CMediaAux::PlayWavSound();

		//ƽ��
		if(CloseSale(m_strInstrument.GetBuffer(0),iCount,pt->lastprice,m_fundAccount.GetBuffer(0),'1')>=0)
		{

			log = pt->szInstrument;

			log = log + ":����ͣ��->ƽ�� ";

			AddLog(log.GetBuffer(0));

		}

		m_nStrategyStatus = STRATEGY_STATUS_INIT;

		return;
		
	}
	
	//����%��ƽ��
	if( m_fOpenPrice*(1-m_loseRate) > pt->lastprice )  
	{
		//CMediaAux::PlayWavSound();

		//ƽ��
		if(CloseSale(m_strInstrument.GetBuffer(0),iCount,pt->lastprice,m_fundAccount.GetBuffer(0),'1')>=0)
		{

			log = pt->szInstrument;

			CString rate;

			rate.Format("%f",m_loseRate);

			log = log + ":����" +rate + "->ƽ��";

			AddLog(log.GetBuffer(0));

		}

		m_nStrategyStatus = STRATEGY_STATUS_INIT;

		return;
	}
	
	//��ͣ�壬�����ʴ���%
	double rate = pt->Volume/(m_pStrategyCodeInfo->getActiveCapital(pt->szInstrument));

	if((pt->UpperLimitPrice == pt->lastprice) &&(  rate> m_changeRate) )
	{
		//CMediaAux::PlayWavSound();

		//ƽ��
		if(CloseSale(m_strInstrument.GetBuffer(0),iCount,pt->lastprice,m_fundAccount.GetBuffer(0),'1')>=0)
		{
			
			log = pt->szInstrument;

			CString rate;

			rate.Format("%f",m_changeRate);

			log = log + ":�����ʴ���" +rate + "->ƽ��";

			AddLog(log.GetBuffer(0));

			
		}

		m_nStrategyStatus = STRATEGY_STATUS_INIT;

		return;
	}

	//�������߼ۻص�%
	if( (m_hignPrice * (1-m_downRate)) > pt->lastprice)
	{
		CMediaAux::PlayWavSound();
	

		if(CloseSale(m_strInstrument.GetBuffer(0),iCount,pt->lastprice,m_fundAccount.GetBuffer(0),'1')>=0)
		{
			

			log = pt->szInstrument;

			CString rate;

			rate.Format("%f",m_downRate);

			log = log + ":�ص�" +rate + "->ƽ��";

			AddLog(log.GetBuffer(0));

		}

		m_nStrategyStatus = STRATEGY_STATUS_INIT;

		return;
	}
	
	return;
}


void CStrategyLimitupSale::SetParam(CString paramname,CString value)
{
	m_params[(LPCTSTR)paramname] = value;

	int iCount;
	//����Ҫ����Ӧ��ֵ����
	if(paramname=="ֹ����")
	{
		m_loseRate = atof(value);
	}
	else if(paramname=="������")
	{
		m_changeRate = atof(value);
	}
	else if(paramname=="�ص���")
	{
		m_downRate = atof(value);
	}
	else if(paramname=="�Ƿ�׷��")
	{
		m_follow = value; 
	}
	else if(paramname=="��ʱʱ��")
	{
		m_timeOut = atoi(value);
	}
	else if(paramname=="�˻�")
	{
		m_fundAccount = value;
	}
	else if(paramname=="��������")
	{
		iCount = atoi(value);
		SetStockCount(m_strInstrument, iCount);
	}
	else if(paramname=="�۲�")
	{
		m_jiacha = atof(value);
	}else if(paramname=="���ּ�")
	{
		m_fOpenPrice = atof(value);
	}

}

void CStrategyLimitupSale::SetParam(CString value)
{
	
	//��ʽ��600570;10;40;0.01;0.02;

	CStringArray list;

	CFunctionPub pub;

	pub.ParseParam(value,list,';');

	if( list.GetSize() >= 3 )
	{

		m_strInstrument = list[0];

		m_params["���׺�Լ"] = list[0];

		int iCount;
		iCount = atoi(list[1]);
		SetStockCount(m_strInstrument, iCount);
		m_params["��������"] = list[1];


		m_fOpenPrice = atof(list[2]);

		m_params["���ּ�"] = list[2];


		m_loseRate = atof(list[3]);
			
		m_params["ֹ����"] = list[3];                      //������


		m_changeRate = atof(list[4]);
			
	    m_params["������"] = list[4];		               //������


		m_downRate = atof(list[5]);
		
		m_params["�ص���"] = list[5];						//�ص���


		m_follow = list[6];                                 //�Ƿ����

		m_params["�Ƿ�׷��"] = list[6];


		m_timeOut = atoi(list[7]);							//��ʱʱ��

		m_params["��ʱʱ��"] = list[7];


		m_fundAccount = list[8];							//�ʺ�   �ö��Ÿ�����  a,b,c,d,

		m_params["�˻�"] = list[8];				


		//m_strModelName = m_strStrategyName + m_strInstrument;     //modelname

		
	}
}


CString CStrategyLimitupSale::GetParamValue()
{

	CString str;
	int iCount = GetStockCount(m_strInstrument);
	str.Format("%s;%d;%.4f;%.2f;%.2f;%.2f;%s;%d;%s;",m_strInstrument,iCount,m_fOpenPrice,m_loseRate,m_changeRate,m_downRate,m_follow,m_timeOut,m_fundAccount);

	return str;

}

CString CStrategyLimitupSale:: GetParamValue(CString paramname)
{

	return m_params[(LPCTSTR)paramname];
}

CString CStrategyLimitupSale::GetStatusInfo(void)
{
	CString result;

	CString status;

	CString openrice;

	CString yingkuiPos;

	CString modelstatus;

	CString codename;

	CString yinkuirate;

	CString huitiaorate;

	if( m_nStrategyStatus ==  STRATEGY_STATUS_INIT )
	{
		status = "�ղ�";

		openrice.Format("%.4f",m_fOpenPrice);
	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_BK )
	{
		status = "��";

		openrice.Format("%.4f",m_fOpenPrice);

		//yingkuiPos.Format("%.2f",m_fyingkuiPosition);
	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_SK )
	{
		status = "����";

		openrice.Format("%.4f",m_fOpenPrice);

		//yingkuiPos.Format("%.2f",m_fyingkuiPosition);
	}

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


 void CStrategyLimitupSale::OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price)
 {
 }