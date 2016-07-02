#include "stdafx.h"
#include "StrategyLimitupBuy_imp.h"
#include <map>
#include <string>
#include "..\hsquantrade\FunctionPub.h"
#include<time.h>


CStrategyLimitupBuy::CStrategyLimitupBuy()
{

	m_strStrategyName = "����ͣ����������";
	
	m_nStrategyStatus = STRATEGY_STATUS_INIT;

	m_fyingkuiPosition = 0;

	m_lastPrice = 0;

	m_hignPrice = 0;

	m_follow = "-1";                   //�Ƿ����,Ĭ��д��-1

	m_timeOut = 0;                     //��ʱʱ��

	jiacha = 0;					       //�۲�

	m_nCount = 0;

	m_sEntrustInfo = "";

	m_rate = 0.5;

	m_dChangeRate = 0.01;

	m_Exit = false;

	m_bRun = false;

	m_done = false;

	m_huigou = false;

	m_strParamList = "���׺�Լ;����;�۸�;���׷���;��ƽ����;��һ�����;������;ί����Ϣ;�˻�;";

	m_params.insert(pair<string, CString>("���׺�Լ",""));

	m_params.insert(pair<string, CString>("����","0"));

	m_params.insert(pair<string, CString>("�۸�","0"));

	m_params.insert(pair<string, CString>("���׷���","0"));

	m_params.insert(pair<string, CString>("��ƽ����","0"));

	m_params.insert(pair<string, CString>("ί����Ϣ","0"));

	m_params.insert(pair<string, CString>("��һ�����","0.5"));

	m_params.insert(pair<string, CString>("������","0.01"));

	m_params.insert(pair<string, CString>("�˻�",""));

	m_params.insert(pair<string, CString>("�Ƿ�׷��","-1"));

	m_params.insert(pair<string, CString>("��ʱʱ��","0"));

	m_params.insert(pair<string, CString>("�۲�","0"));

	
}

CStrategyLimitupBuy::~CStrategyLimitupBuy()
{

	m_bRun = false;

	m_tWorkThread.join();

}


void CStrategyLimitupBuy::Tick(CTickData * pt)
{
	//if(strcmp(m_strInstrument,pt->szInstrument)!=0)
	//	return;

	time_t rawtime;
	time ( &rawtime );
	long timestamp = (long)rawtime;


	double buy1 = pt->bidprice[0];
	double buy2 = pt->bidprice[1];
	double buy5 = pt->bidprice[4];
	double sale1 = pt->askprice[0];
	double sale2 = pt->askprice[1];
	double limitup = pt->UpperLimitPrice;
	double lastprice = pt->lastprice;
	char*code = pt->szInstrument;
	double buy1count = pt->bidvol[0];
	double buy2count = pt->bidvol[1];
	int sale1count = pt->askvol[0];

	m_hignPrice = pt->highestprice;  //��߼�

	CString price;

	price.Format("��Լ:%s,���¼�:%.2f,��һ��:%.2f,��һ��:%.1f,�����:%.2f,�����:%.1f,��һ��:%.2f,��һ��:%d,������:%.2f,�ɽ���:%ld"
			,code,lastprice,buy1,buy1count,buy2,buy2count,sale1,sale1count,sale2,pt->Volume);

	WRITE_LOG(1,price.GetBuffer(0));

	if( (data==NULL) || (!(*data).isReady) )
		return;

	//9:15����û�̿ڲ�����
	if(buy1 - 0.00 < 0.1)
		return;

	//9:20~9:30
	if(isCollectiveBidTime())
		return;

	std::vector<CEntrustInfo>::iterator it;
	for(it=(*data).m_sEntrustInfo.begin();it!=(*data).m_sEntrustInfo.end();it++)
	{
		if(strcmp( it->szInstrument,code)==0)
		{
			if(lastprice<limitup)
			{
				//����
				CString log;
				log.Format("��Լ:%s,���¼�:%.3f,������ͣ��,����",code,lastprice);
				AddLog(log.GetBuffer(0));
				OrderAction(it->entrustno);
				(*data).m_sEntrustInfo.erase(it);
				break;
			}

			if(sale2>0)
			{
				//����
				CString log;
				log.Format("��Լ:%s,������(%.3f) > 0,����",code,sale2);
				AddLog(log.GetBuffer(0));
				OrderAction(it->entrustno);
				(*data).m_sEntrustInfo.erase(it);
				break;
			}

			if(buy1==sale1 && sale1==limitup && (buy1count/buy2count>=m_rate))
			{

				//����
				CString log;
				log.Format("��Լ:%s,��һ(%f)/���(%f)ͻ�Ʊ���(%.3f),����",code,buy1count,buy2count,m_rate);
				AddLog(log.GetBuffer(0));
				OrderAction(it->entrustno);
				(*data).m_sEntrustInfo.erase(it);
				break;

			}

			//��ͣ���ϣ������ʴ���1%
			//��ֹ��������û���µ����
			double rate = 0.0;
			if(m_pStrategyCodeInfo->getActiveCapital(pt->szInstrument) > 10)
			{
				rate = pt->Volume/(m_pStrategyCodeInfo->getActiveCapital(pt->szInstrument));
			}
			if((pt->UpperLimitPrice == pt->lastprice) &&(  rate> m_dChangeRate) )
			{
				//����
				CString log;
				log.Format("��Լ:%s,��ǰ������(%.3f)����%.3f,����",code,rate,m_dChangeRate);
				AddLog(log.GetBuffer(0));
				OrderAction(it->entrustno);
				(*data).m_sEntrustInfo.erase(it);
				break;

			}
		
		}
	}

	//ͨ����ѯ�ֲ�����������dll�ˣ�����ע�͵�
	/*
	//9:35ǰ������
	if(isDoNothingTime())
	{
		return;
	}

	char msg[256];
	std::vector<CHoldsinfo>::iterator ithold;
	for(ithold=(*data).m_sHoldsInfo.begin();ithold!=(*data).m_sHoldsInfo.end();ithold++)
	{
		if(strcmp( ithold->szInstrument,code)==0)
		{
			//14:55������5%�м�����
			if( (isCloeTime()) && ( ithold->dTradePrice*(1-0.05) > pt->lastprice) )
			{
				playsound();

				//OpenSale(code,ithold->TdAvaiable,buy5,m_fundAccount.GetBuffer(0));

				sprintf(msg,"14:55,��Լ:%s,�������0.05,�м�����...�ɱ���%.4f,��ǰ�۸�%.4f",code,ithold->dTradePrice,pt->lastprice);

				AddLog(msg);

				(*data).m_sHoldsInfo.erase(ithold);

				break;
			}

			//��߼ۻص�5%
			//if( (m_hignPrice * (1-0.05)) > pt->lastprice )
			if( (m_hignPrice * (1-0.05)) > pt->lastprice )
			{
				sprintf(msg,"��Լ:%s,��߼�:%.4f,���¼�:%.4f,�ص���������,��¼...",code,m_hignPrice,pt->lastprice);

				AddLog(msg);

				if(m_sSignalTime[code] == -9999 || m_sSignalTime[code] == 0 )
				{
					m_sSignalTime[code] = timestamp;
					break;
				}

				//����һ���ӵ������ź�
				if( (timestamp - m_sSignalTime[code] > 60) && (m_sSignalTime[code]!=0) )
				{
					playsound();

					sprintf(msg,"��Լ:%s,��ǰʱ��:%ld,���ʱ��%ld,����һ����,����...",code,timestamp,m_sSignalTime[code]);

					AddLog(msg);

					//OpenSale(code,ithold->TdAvaiable,pt->lastprice,m_fundAccount.GetBuffer(0));

					(*data).m_sHoldsInfo.erase(ithold);
				}

				break;
			}
			//û�������ź�
			else
			{
				m_sSignalTime[code] = -9999;

				break;
			}
		}

	}
	*/


	//�ع�������
	if(m_huigou)
	{
		if(strcmp("131810",code)==0)
		{
			//1000��������
			int count = (int(data->m_dEnableMoney / 1000)) * 10;

			AddLog("�ع�...");

			OpenSale(code,count,buy2,m_fundAccount.GetBuffer(0));

			m_huigou = false;
		}
	}


}


void CStrategyLimitupBuy::SetParam(CString paramname,CString value)
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
	else if(paramname=="�۸�")
	{
		m_fOpenPrice = atof(value);
	}
	else if(paramname=="��������")
	{
		m_kp = ( (char*)value.GetBuffer())[0];
	}
	else if(paramname=="��ƽ����")
	{
		m_kp = ( (char*)value.GetBuffer())[0];
	}
	else if(paramname=="ί����Ϣ")
	{
		m_sEntrustInfo = value;
	}
	else if(paramname=="��һ�����")
	{
		m_rate = atof(value);
	}
	else if(paramname=="������")
	{
		m_dChangeRate = atof(value);
	}
	else if(paramname=="�˻�")
	{
		m_fundAccount = value;
	}

}


void CStrategyLimitupBuy::SetParam(CString value)
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

	//m_strModelName = m_strStrategyName;
	//m_sModuleId = m_strStrategyName;

}

//"���׺�Լ;����;�۸�;���׷���;��ƽ����;��һ�����;������;ί����Ϣ;�˻�;";
CString CStrategyLimitupBuy::GetParamValue()
{
	CString str;

	str.Format("%s;%d;%.4f;%d;%d;%.3f;%.3f;%s;%s;",m_strInstrument,m_nCount,0.0,0,0,m_rate,m_dChangeRate,m_sEntrustInfo,m_fundAccount);

	return str;
}


CString CStrategyLimitupBuy::GetParamValue(CString paramname)
{
	return m_params[(LPCTSTR)paramname];
}

CString CStrategyLimitupBuy::GetStatusInfo(void)
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

	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_SK )
	{
		status = "����";

		openrice.Format("%.4f",m_fOpenPrice);

	}

	if( m_bRun == false )
		modelstatus = "��ͣ";
	else
		modelstatus = "����";

	yingkuiPos.Format("%.4f",m_fyingkuiPosition);

	codename = m_pStrategyCodeInfo->getfutuName(m_strInstrument.GetBuffer(0));

	m_strInstrument.ReleaseBuffer();

	//yinkuirate.Format("%.2f",(m_lastPrice - atof(openrice)) / atof(openrice));

	yinkuirate = "0";

	if(m_hignPrice < 0.001)
	{
		huitiaorate = "0.00";
	}
	else
	{
		huitiaorate.Format("%.2f",((m_hignPrice - m_lastPrice) / m_hignPrice));
	}

	if( m_bs == 0 )
	{
		
	}
	else if( m_bs == 1 )
	{
		
	}
	

	result.Format("%s;%s;%s;%d;%s;%s;%s(%s);%.4f;%s;%.4f;%.4f(%s);%s;%s;",m_strStrategyName,m_strInstrument,codename,
		m_nCount,status,"0","0","0",0.0,modelstatus,0.0,0.0,"-",m_fundAccount,m_sStrategyId);

	//result = "�����ײ���TF1512����;TF1512;TF1512;10;��;100;0;�ղ�;0;0;041249,;";
	return result;

}

void CStrategyLimitupBuy::BeginRun()
{
	
	//�µ�
	if(strcmp(m_strInstrument,"-6666")==0)
	{
		m_strType = 1;
	}
	//���
	else if(strcmp(m_strInstrument,"-9999")==0)
	{
		m_strType = 2;
	}
	//�ع�
	else if(strcmp(m_sEntrustInfo,"-1")==0 )
	{
		m_strType = 3;
	}
	//����
	else
	{
		m_strType = -1;
	}


	m_tWorkThread = thread(bind(&CStrategyLimitupBuy::process,this));

}


void CStrategyLimitupBuy::order()
{

	//����ί����Ϣ��,��ʽ:code,count,price|code,count,price|

	if(m_sEntrustInfo=="")
		return;
	
	CStringArray list;

	CFunctionPub pub;

	CString code,count,price;

	pub.ParseParam(m_sEntrustInfo,list,'|');

	for(int i = 0;i<list.GetSize();i++)
	{
		CString str = list[i];
		CStringArray orderinfo;

		pub.ParseParam(str,orderinfo,',');

		code = orderinfo[0];

		count = orderinfo[1];

		price = orderinfo[2]; 

		if(code=="" || count=="" || price=="")
			continue;

		int orderref = OpenBuy(code.GetBuffer(0),atoi(count),atof(price),m_fundAccount.GetBuffer(0));

		COrderInfo *order = new COrderInfo();
		strcpy(order->szInstrument,code.GetBuffer(0));
		order->ref = orderref;
		order->ordercount = atoi(count);
		order->tradecount = 0;
		order->orderprice = atof(price);
		order->tradeprice = 0;
		order->bs = '0';
		order->kp = '0';
		m_orderinfo[orderref] = *order;
		delete order;

		std::this_thread::sleep_for(std::chrono::milliseconds(2*1000));
		
	}

}

void CStrategyLimitupBuy::process()
{

	if(data==NULL)
	{
		AddLog("�ʺ�û�ҵ�...");
		return;

	}
	while(!m_Exit)
	{
		if(m_strType==1)
		{
			if(isOrderTime())
			{
				if(!m_done)
				{
					//�µ�
					order();  
					m_done = true;
					std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));

					//��ί��
					(*data).isReady = false;
					QueryOrder();      
					std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));

					//��ѯ�ֲֲ����
					/*(*data).isReady = false;
					QueryPosition();
					std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));*/

				}
			}

			if(isWithDrawTime())
			{
				BussInessDeal(0);

				return;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(10*1000));
		}
		//��عҵ�
		else if(m_strType==2)
		{
			if(!m_done)
			{
				m_done = true;
				//��ί��
				(*data).isReady = false;
				QueryOrder();      
				std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));

				//��ѯ�ֲֲ����
				/*(*data).isReady = false;
				QueryPosition();
				std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));*/

			}

			if(isWithDrawTime())
			{
				BussInessDeal(0);

				return;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(10*1000));

		}

		//����ع�
		else if(m_strType==3)
		{
			BussInessDeal(0);

			return;

		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(3*1000));
		}

		//���Ӷ�ʱ��ѯ�������̩��ʱ�䲻�������·������Ͽ�����
		{
			//AddLog("��500s,��ʱ��ѯ...");
			//QueryFund();
			//std::this_thread::sleep_for(std::chrono::milliseconds(500*1000));

		}
	}


}

bool CStrategyLimitupBuy::isOrderTime()
{

	bool ret =false;

	const time_t t = time(NULL);

	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;

	if(h==8)
	{
		ret =  true;
	}
	else if(h==9 && m<12)
	{
		ret =  true;
	}

	//return ret;

	return true;

}

bool CStrategyLimitupBuy::isWithDrawTime()
{
	bool ret =false;

	const time_t t = time(NULL);

	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;

	if( (h==14) && (0<m) && (m<10) )
	{
		ret =  true;
	}


	return ret;
	//return true;

}


bool CStrategyLimitupBuy::isDoNothingTime()
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

bool CStrategyLimitupBuy::isCloeTime()
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

bool CStrategyLimitupBuy::isCollectiveBidTime()
{
	bool ret =false;

	const time_t t = time(NULL);

	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;

    if(h==9 && m>=20 && m<30)
	{
		ret =  true;
	}

	return ret;
}


//����ر�
void CStrategyLimitupBuy::OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price)
{


}


void CStrategyLimitupBuy::OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins)
{

}


void CStrategyLimitupBuy::withDraw()
{
	m_entrustMutex.lock();

	std::vector<CEntrustInfo>::iterator it;
	for(it=(*data).m_sEntrustInfo.begin();it!=(*data).m_sEntrustInfo.end();it++)
	{
		int entrustno = it->entrustno;

		CString log;
		log.Format("�ع�ǰ����,Ȼ��ɾ��,ί�к�:%d...",entrustno);
		AddLog(log.GetBuffer(0));
		
		OrderAction(entrustno);

		//(*data).m_sEntrustInfo.erase(it);

		std::this_thread::sleep_for(std::chrono::milliseconds(1*1000));
	}

	(*data).m_sEntrustInfo.clear();

	m_entrustMutex.unlock();

}

void CStrategyLimitupBuy::BussInessDeal(int type)
{
	//��ѯδ�ɽ�ί��
	(*data).isReady = false;
	QueryOrder();
	std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));

	//����
	withDraw();
	std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));

	//��ѯ�ʽ�
	QueryFund();
	std::this_thread::sleep_for(std::chrono::milliseconds(3*1000));

	//���Իع�
	m_huigou = true;

	return;
}
