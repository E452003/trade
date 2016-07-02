#include "stdafx.h"

#include "Order.h"

#include "BaseTradeApi.h"

#include "StrategyBase.h"

#include<windows.h>

#include "TradeInterface.h"

#include "tool.h"

#include "CTPTrade.h"

#include "FunctionPub.h"


COrder::COrder(char *account,char *instrument, int count, char direction, char offsetflag, double price,int ordertype,int timeout,double jiacha,CBaseTradeApi * api,CTradeInterface *ptrade,int orderref)
{

	strcpy(m_account,account);

	strcpy(m_szIntrument,instrument);

	m_count = count;

	m_direction = direction;

	m_offsetflag = offsetflag;

	m_price= price;

	m_nOrderType = ordertype;

	m_ntimeout = timeout;

	m_jiacha = jiacha;

	m_pApi = api;

	m_ptrade = ptrade;

	m_nOrderRef = orderref;

	m_bWithDrawING = false;

	m_iTradeCount = 0;

	m_nEntrustNo = 0;

	m_nOrderSysID = 0;

	m_status = 'w';				//��ʾ��ʼֵ

	m_bDone = false;

	//��ʱ����ģʽ
	if( m_nOrderType == ORDER_TYPE_TIMEOUT_ABORT )
	{

		m_tTrd = thread(std::bind(&COrder::DealOrder, this));

	}

	//��¼����ʱ��
	time_t rawtime;
	time ( &rawtime );
	m_timestamp = (long)rawtime;


}


COrder::~COrder(void)
{
	m_bDone = true;

	//WRITE_LOG(1,"����");

	if(m_nOrderType == ORDER_TYPE_TIMEOUT_ABORT)
	{
		m_tTrd.join();
	}
}


/*
//���Ƿ����µ�Ӧ�����洦��
�����֧�ֱ������õĽӿ�,orderref ���Ǳ�������, ordersysidһ��������
�����֧�ֱ������õĽӿ�,orderref ����batch_no, ordersysid��ί�к�
CTP���ڻ�UFX�����ر��ߵ���ί�лر�
��Ʊ��ȨUFX�����ɹ����ߵĳɽ��ر�
*/
void COrder::RtnOrder(int orderref,int ordersysid,char status,char bs1,char kp1,char *ins)
{

	if(m_nOrderType == ORDER_TYPE_DONOTHING )
		return;

	if(m_bDone)
		return;

	char msg[128];

	CString code = ins;

	//CString bs,kp,st;
	//bs = Tool::TransBS(m_direction,m_pApi);
	//kp = Tool::TransKP(m_offsetflag,m_pApi);
	//st = Tool::TransStatus(status,m_pApi);

	string bs,kp,st;
	bs = m_pApi->TransBS(m_direction);
	kp = m_pApi->TransKP(m_offsetflag);
	st = m_pApi->TransStatus(status);

	//���״̬�Ѿ�����ֹ״̬ ״̬�Ͳ��ܸı�  ���������ǰ״̬�ѳɣ������ڱ���ѱ�
	if(m_pApi->IsBuChe(m_status) || m_pApi->IsFeiDan(m_status) || m_pApi->IsYiChe(m_status) || m_pApi->IsYiCheng(m_status))
	{
		sprintf(msg,"ί�лر�->��Լ %s  ί�к� %d  ��ǰ״̬״̬ %s Ϊ��ֹ״̬ ���λر�״̬ %s",code,m_nEntrustNo,m_pApi->TransStatus(m_status).c_str(),m_pApi->TransStatus(status).c_str());
		WRITE_LOG(1,msg);
		m_bDone = true;

#ifdef _DEBUG
		int id = GetCurrentThreadId();
		sprintf(msg,"1111�߳�id  %d",id);
		WRITE_LOG(1,msg);

#endif
		return;
		//m_status = status; 
	}
	else
	{
		m_status = status;
	}

	m_nOrderSysID = ordersysid;

	//���m_nEntrustNoΪ0 ctp�ӿ�ֱ�ӷ��� û��Ӧ��
	//���ctp�ӿ�
	if( (m_nEntrustNo==0) && (ordersysid >0) && (m_pApi->GetInterfaceType()==TRADE_CTP))
		m_nEntrustNo = orderref;


	sprintf(msg,"ί�лر�->��Լ %s  ί�к� %d  ״̬ %s  ״̬id %c",code,m_nEntrustNo,st.c_str(),status);

	WRITE_LOG(1,msg);

 
	if(m_nOrderType== ORDER_TYPE_ZUIJIA_NEWPRICE || m_nOrderType == ORDER_TYPE_ZUIJIA_JIACHA )  //6�ѳ�,5���� ��Ҫ׷��
	{
		//if(Tool::isBuChe(m_status,m_pApi) ||  Tool::isYiChe(m_status,m_pApi))
		if(m_pApi->IsBuChe(m_status) || m_pApi->IsYiChe(m_status))
		{
			if(!m_bWithDrawING/* || (ordersysid <=0) */)
			{
				sprintf(msg,"�Ƕ���������  ��Լ %s  ί������ %d  ���ص��� %d",code,m_nEntrustNo,ordersysid);
				WRITE_LOG(1,msg);
				return;
			}

			sprintf(msg,"׷��->��Լ:%s,ί�к�:%d,�յ������ر�,׷��",m_szIntrument,m_nEntrustNo);

			WRITE_LOG(1,msg);

			//׷��
			int newcount = m_count - m_iTradeCount;
			
			sprintf(msg,"׷��->��Լ %s %c %c,ί�к� %d  ���� %d,�۸� %.4f ׷��...",m_szIntrument,bs1,kp1,m_nEntrustNo,newcount,m_price);

			WRITE_LOG(1,msg);

			int result = m_ptrade->InsertOrder(m_szIntrument,newcount,m_direction,m_offsetflag,m_price,m_nOrderType,m_ntimeout,m_jiacha,m_account);

			m_bDone = true;

			m_bWithDrawING = false;   //������ɣ�

			if(result == -1)
			{
				WRITE_LOG(1,"ί��ʧ��,ԭ��������˻������˱�֤���޶�...");
				//m_bDone = true;//��������״̬
			}

		
		}

	}
	else if( m_nOrderType == ORDER_TYPE_TIMEOUT_ABORT )
	{
		//if(Tool::isYiChe(m_status,m_pApi))	
		if(m_pApi->IsYiChe(m_status))
		{
			sprintf(msg,"��ʱ����->��Լ:%s,ί�к�:%d,�յ������ر�,������ֹ",m_szIntrument,m_nEntrustNo);
			WRITE_LOG(1,msg);
			m_bDone = true;		
		}
		//else if(Tool::isBuChe(m_status,m_pApi))
		else if(m_pApi->IsBuChe(m_status))
		{
			sprintf(msg,"��ʱ����->��Լ:%s,ί�к�:%d,�յ������ر�,������ֹ",m_szIntrument,m_nEntrustNo);
			WRITE_LOG(1,msg);
			m_bDone = true;			
		}

	}

}


/*
//��Ʊ��Ȩ�����ɹ����ߵĳɽ��ر�
//�����ӿ����ߵ�ί�лر�
//�����жϳɽ��۸����ж��Ƿ��ǳɽ��ر����ǳ����ر�
CTP�ӿ������������ɽ��ر��ǵ���Լ��������
*/
void COrder::RtnTrade(int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins)
{
	if( m_nOrderType == ORDER_TYPE_DONOTHING )
		return;

	if(m_bDone)
		return;

	char msg[128];

	string code = ins;

	string heyue = m_szIntrument;

	bool taoli = false;
	//������Լ �ر�ȡ�ڶ���
	if(heyue.find("&") != -1)
	{
		int index = heyue.find("&");

		string substr = heyue.substr(index+1);

		strcpy(m_szLeg2,substr.c_str());

		taoli = true;
	}

	//ֻ���յ��ڶ��Ȳ��ۼӳɽ�����
	if(taoli)
	{
		if(strcmp(m_szLeg2,ins)==0)
		{
			m_iTradeCount = m_iTradeCount + vol;
		}
		//�յ���һ�Ⱦ�ֻ�Ǽ�¼�¾ͷ���
		else
		{
			sprintf(msg,"�ɽ��ر�->��Լ %s  ί������ %d  ί������ %d  �ɽ����� %d  �ɽ��۸� %.4f",code.c_str(),m_nEntrustNo,m_count,vol,price);
			WRITE_LOG(1,msg);
			return;
		}
	
	}
	else
		m_iTradeCount = m_iTradeCount + vol;
	

	//�ɽ��ر�
	if(price > 0.00001)
	{
		sprintf(msg,"�ɽ��ر�->��Լ %s  ί������ %d  ί������ %d  �ɽ����� %d  �ɽ��۸� %.4f",code.c_str(),m_nEntrustNo,m_count,vol,price);

		WRITE_LOG(1,msg);

		if( (m_iTradeCount==m_count))    //�ܳɽ�����==�µ�����
		{

			//if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(m_pApi))
			if(m_pApi->GetInterfaceType()==TRADE_CTP)
				m_status = '0';     //�ѳ�
			else
				m_status = '8';

			sprintf(msg,"�ɽ��ر�->��Լ %s  ί������ %d  ȫ���ɽ� ������ֹ",m_szIntrument,m_nEntrustNo);

			m_bDone = true;
			WRITE_LOG(1,msg);			

		}
		else if( (m_iTradeCount < m_count) )
		{

			//if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(m_pApi))
			if(m_pApi->GetInterfaceType()==TRADE_CTP)
				m_status = '1';     //���� ��ʻ�������Ҫ�ж��ܳɽ�����
			else
				m_status = '7';     

			sprintf(msg,"�ɽ��ر�->��Լ %s  ί������ %d  ���ֳɽ�",m_szIntrument,m_nEntrustNo);

			WRITE_LOG(1,msg);

		}

	}
	//��Ϊ�ɽ��۸�Ϊ0  �ǳ����ر� 
	else if(price < 0.00001)
	{
		sprintf(msg,"�����ر�->��Լ %s  ί������ %d  ί������ %d  �������� %d  �ɽ��۸� %.4f",code.c_str(),m_nEntrustNo,m_count,vol,price);

		WRITE_LOG(1,msg);

		if( m_nOrderType == ORDER_TYPE_ZUIJIA_NEWPRICE || m_nOrderType == ORDER_TYPE_ZUIJIA_JIACHA)
		{
			sprintf(msg,"׷��->��Լ %s  ί�к� %d  �յ������ر� ׷��",m_szIntrument,m_nEntrustNo);

			WRITE_LOG(1,msg);

			//����Ϊ�˴γɽ�����
			int newcount = vol;

			double newprice = m_price;
		
			sprintf(msg,"׷��->��Լ %s  ί�к� %d  ���� %d  �۸� %.4f  ׷��...",m_szIntrument,m_nEntrustNo,newcount,newprice);

			WRITE_LOG(1,msg);

			//test
			//if(m_nordertype!=2)
			int result=m_ptrade->InsertOrder(m_szIntrument,newcount,m_direction,m_offsetflag,m_price,m_nOrderType,m_ntimeout,m_jiacha,m_account);

			m_bDone = true;		

			m_bWithDrawING = false;   //������ɣ�

			if(result == -1)
			{
				WRITE_LOG(1,"ί��ʧ��,ԭ��������˻������˱�֤���޶�...");
				//m_bDone = true;//��������״̬
			}	
		
		}
		else if( m_nOrderType == ORDER_TYPE_TIMEOUT_ABORT )
		{
			sprintf(msg,"��ʱ����->��Լ %s  ί�к� %d  �յ������ر� ������ֹ",m_szIntrument,m_nEntrustNo);
			WRITE_LOG(1,msg);
			m_bDone = true;
			
		}
	
	}
}

/*
ί��Ӧ��Ҳ�����ί�к�
*/
void COrder::OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)
{

	char msg[256];

	if(errorid !=0 )
	{
		m_bDone = true;	
		return;
	}

	//֧�ֱ������õĽӿ�
	if(entrustno==-9999)
	{
		sprintf(msg,"����Ӧ��->�������� %d  ״̬ %c ",m_nOrderRef,entrust_status);
		WRITE_LOG(1,msg);
		m_nEntrustNo = m_nOrderRef;
	}
	else
	{
		sprintf(msg,"����Ӧ��->ί�к� %d  ״̬ %c ",entrustno,entrust_status);
		WRITE_LOG(1,msg);
		m_nEntrustNo = entrustno;

		//201604   begin
		//Ͷ��Ӯ�һر�����ֻ��ί�к� ���������ί�����ø���Ϊί�к� 
		//����ʹ�������������ر����ڱ���Ӧ����� ί�����þ���ԭ���ģ�����ί�к� �޷�ƥ��
		//m_nOrderRef = entrustno;
		//201604   end

		//20160628 begin
		//֮ǰ����Ϊ�˼���Ͷ��Ӯ��  ��������m_nOrderRef = entrustno �����ƻص����� ���ص������� entrustno  entrustno���ж�
		//�������� �޸�Ϊ����

		//Ͷ��Ӯ�����ܽӿ�
		if(m_pApi->GetInterfaceType()==TRADE_UFX_AUTO)
		{
			m_nOrderRef = entrustno;
		}
		else
		{
		
		}

		//���ص���
		m_nOrderSysID = entrustno;
		

	}

	m_status = entrust_status;

}


//����ʱ�򳷵�����
void COrder::DealOrder()
{
	//�����ʱʱ����0������Ϊ�ǵ�����Ч������
	if(m_ntimeout==0)
		return;

	if(m_nOrderType != ORDER_TYPE_TIMEOUT_ABORT )
		return;

	if(m_bDone)
		return;

	char msg[256];

	while (!m_bDone)
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(m_ntimeout*1000));

		if(m_bDone)
			return;

		//�����ȵ�ί��Ӧ����߻ر�����
		if(m_nEntrustNo==0)
			continue;

		int ref = m_nOrderRef;

		//������Ҫ�ж��¶���״̬
		//�ѳɣ��ϵ�;�������ѳ����ǲ�׷��
		//if( Tool::isYiCheng(m_status,m_pApi) || Tool::isFeiDan(m_status,m_pApi)|| Tool::isBuChe(m_status,m_pApi) || Tool::isYiChe(m_status,m_pApi) )
		if(m_pApi->IsYiCheng(m_status) || m_pApi->IsFeiDan(m_status) || m_pApi->IsBuChe(m_status) || m_pApi->IsYiChe(m_status))
		{
			sprintf(msg,"��ʱ����->ί������ %d ��Լ %s %c %c  �۸� %.4f  ���� %d  ״̬ %s  �߳���ֹ",m_nEntrustNo,m_szIntrument,/*Tool::TransBS(m_direction,m_pApi)*/m_direction,/*Tool::TransKP(m_offsetflag,m_pApi)*/m_offsetflag,m_price,m_count,/*Tool::TransStatus(m_status,m_pApi)*/m_pApi->TransStatus(m_status).c_str());

			WRITE_LOG(1,msg);

			m_bDone = true;			

			return;    //�ѳɻ��߳����Ļ����˳��߳�

		}

		//����
		if(!m_bWithDrawING)
		{
			int result = m_ptrade->WithDraw(m_account,m_nEntrustNo,m_szIntrument);

			m_bWithDrawING = true;

			sprintf(msg,"��ʱ����->ί������ %d ��Լ %s ��������ָ��",m_nEntrustNo,m_szIntrument);

			WRITE_LOG(1,msg);

			if(result==-1)
			{
				WRITE_LOG(1,"����ʧ��,�������˻������˳�����������");
				m_bDone = true;//��������״̬				
			}
		}
	}
	
}

/*
1 �����µ�֮����һ����������δ�ɽ� �ͳ��� Ȼ��Լ�׷��
2 �����µ�֮������۲�ﵽָ������ֵ���ͳ��� Ȼ��Լ�׷��
*/
void COrder::DealPrice(double buy1,double sale1)
{

	if(m_nOrderType != ORDER_TYPE_ZUIJIA_NEWPRICE && m_nOrderType != ORDER_TYPE_ZUIJIA_JIACHA)   //�µ�����
		return;

	if (m_bWithDrawING)    //����δ��� �������ٴγ���;
		return;

	if(m_bDone)
		return;

	//�����ȵ�ί��Ӧ����߻ر�����
	if(m_nEntrustNo==0)
		return;

	char msg[128];

	//��Щ�����б�������ʱ�������
	time_t rawtime;
	time ( &rawtime );
	long timestamp = (long)rawtime;
	bool timeout = false;

	if(timestamp - m_timestamp <m_ntimeout)
	{
		sprintf(msg,"����ʱ���뵱ǰʱ����С�� %d",m_ntimeout);

		WRITE_LOG(1,msg);
		//return;
	}
	{
		timeout = true;
	}

#ifdef _DEBUG
		int id = GetCurrentThreadId();
		sprintf(msg,"2222�߳�id  %d",id);
		WRITE_LOG(1,msg);

#endif


	//�ѳɣ��ѳ����������ϵ�  ��������
	//if( Tool::isYiCheng(m_status,m_pApi) || Tool::isFeiDan(m_status,m_pApi)|| Tool::isBuChe(m_status,m_pApi) || Tool::isYiChe(m_status,m_pApi) )
	if( (m_pApi->IsYiCheng(m_status) && m_count==m_iTradeCount) || m_pApi->IsFeiDan(m_status) || m_pApi->IsBuChe(m_status) || m_pApi->IsYiChe(m_status))
	{
		m_bDone = true;		
		return;
	}

	

	//�Ƿ���������
	bool need = false; 

	double old_price = m_price;

	if(m_nOrderType == ORDER_TYPE_ZUIJIA_NEWPRICE)
	{
		if(m_direction == '0')
		{
			if( (sale1 > m_price) || timeout) 
			{				
				m_price = sale1;

				need = true;
			}
		}
		else if(m_direction == '1')
		{
			if( (buy1 < m_price) || timeout) 
			{				
				m_price = buy1;

				need = true;

			}
		}
	}
	else if( m_nOrderType == ORDER_TYPE_ZUIJIA_JIACHA )
	{
		if(m_direction == '0')
		{
			if( (sale1 - m_price) > m_jiacha ) 
			{								
				sprintf(msg,"ģʽ2 ��һ����ί�м۲� %.4f  �۲���� %.4f",(sale1 - m_price),m_jiacha);

				WRITE_LOG(1,msg);

				m_price = sale1;

				need = true;
			}
		}
		else if(m_direction == '1')
		{
			if( (m_price - buy1) > m_jiacha ) 
			{							
				sprintf(msg,"ģʽ2 ί�м�����һ�۲� %.4f  �۲���� %.4f",(m_price - buy1),m_jiacha);

				WRITE_LOG(1,msg);

				m_price = buy1;

				need = true;

			}
		}
	
	}

	if(need)
	{
		m_bWithDrawING = true;

		//test
		//if(m_nordertype!=2)
		int result=m_ptrade->WithDraw(m_account,m_nEntrustNo,m_szIntrument);

		sprintf(msg,"׷��->��������ָ��  ί������ %d  ��Լ %s  ԭ�۸� %.4f  �¼۸� %.4f ����һ�� %.4f ����һ�� %.4f",m_nEntrustNo,m_szIntrument,old_price,m_price,buy1,sale1);
		
		WRITE_LOG(1,msg);

		if(result==-1)
		{
			WRITE_LOG(1,"����ʧ��,�������˻������˳�����������");
			m_bDone = true;//��������״̬			
		}

#ifdef _DEBUG
		if(strcmp(m_szIntrument,"rb1610")==0)
		{
			int u =0;
			u++;	
		}
		Sleep(20);
#endif
		
	}


}


void COrder::QueryOrder()
{

	//��˯100ms
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	//��Ȼ�ǳ�ʼֵ ��Ҫ��ѯί��
	if(m_status == 'w')
	{
		m_ptrade->QueryEntrust(m_account,0);
	}

}



void COrder::DealOrderEx()
{

	if(m_nOrderType != ORDER_TYPE_ZUIJIA_NEWPRICE &&  m_nOrderType != ORDER_TYPE_ZUIJIA_JIACHA)
		return;

	if(m_bDone)
		return;

	char msg[256];

	while (!m_bDone)
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(m_ntimeout*1000));

		//�����ȵ�ί��Ӧ����߻ر�����
		if(m_nEntrustNo==0)
			continue;

		int ref = m_nOrderRef;

		//������Ҫ�ж��¶���״̬
		//�ѳɣ��ϵ�;�������ѳ����ǲ�׷��
		//if( Tool::isYiCheng(m_status,m_pApi) || Tool::isFeiDan(m_status,m_pApi)|| Tool::isBuChe(m_status,m_pApi) || Tool::isYiChe(m_status,m_pApi) )
		if(m_pApi->IsYiCheng(m_status) || m_pApi->IsFeiDan(m_status) || m_pApi->IsBuChe(m_status) || m_pApi->IsYiChe(m_status))
		{
			sprintf(msg,"����ģʽ��ʱ����->ί������ %d ��Լ %s %c %c  �۸� %.4f  ���� %d  ״̬ %s  �߳���ֹ",m_nEntrustNo,m_szIntrument,m_direction,m_offsetflag,m_price,m_count,m_pApi->TransStatus(m_status).c_str());

			WRITE_LOG(1,msg);

			m_bDone = true;

			return;    //�ѳɻ��߳����Ļ����˳��߳�

		}

		//����
		if(!m_bWithDrawING)
		{
			int result = m_ptrade->WithDraw(m_account,m_nEntrustNo,m_szIntrument);

			m_bWithDrawING = true;

			sprintf(msg,"��ʱ����->ί������ %d ��Լ %s ��������ָ��",m_nEntrustNo,m_szIntrument);

			WRITE_LOG(1,msg);

			//���¼۸�
			if(m_direction==POSTION_DIRECTION_BUY)
				m_price = m_price + CPubData::m_sFutuCodeInfo[m_szIntrument].PriceTick;
			else if(m_direction==POSTION_DIRECTION_SELL)
				m_price = m_price - CPubData::m_sFutuCodeInfo[m_szIntrument].PriceTick;

			if(result==-1)
			{
				WRITE_LOG(1,"����ʧ��,�������˻������˳�����������");
				m_bDone = true;//��������״̬
				sprintf(msg,"%d 9",m_nOrderRef);
				WRITE_LOG(1,msg);
			}
		}
	}

}






