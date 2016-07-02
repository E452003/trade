#include "stdafx.h"

#include "COrderEx.h"

#include "BaseTradeApi.h"

#include "StrategyBase.h"

#include<windows.h>

#include "TradeInterface.h"

#include "tool.h"

#include "CTPTrade.h"

//////////////
//COrderEx
//////////////
COrderEx::COrderEx(char *account,char * instrument, int count, char direction, char offsetflag, double price,int ordertype,int timeout,double jiacha,CBaseTradeApi * api,CTradeInterface *ptrade,int orderref)
{
	
	strcpy(m_account,account);

	strcpy(m_szIntrument,instrument);

	m_count = count;

	m_direction = direction;

	m_offsetflag = offsetflag;

	m_price= price;

	m_nordertype = ordertype;

	m_ntimeout = timeout;

	m_jiacha = jiacha;

	m_pApi = api;

	m_ptrade = ptrade;

	m_nOrderRef = orderref;

	m_bWithDrawING = false;

	m_iTradeCount = 0;

	m_nEntrustNo = 0;

	m_status = 'w';				//��ʾ��ʼֵ

	m_bDone = false;

	m_bReOrder = false;

	time_t rawtime;
	time ( &rawtime );
	m_timestamp = (long)rawtime;

	
}


COrderEx::~COrderEx()
{
	m_bDone = true;
}


void COrderEx::RtnOrder(int orderref,int ordersysid,char status,char bs1,char kp1,char * ins)
{
	if(strcmp(m_szIntrument,ins) !=0)
		return ;

	if(m_nordertype==-1)
		return;

	if(m_bDone)
		return;

	char msg[128];

	CString code = ins;

	CString bs,kp,st;

	bs = Tool::TransBS(m_direction,m_pApi);

	kp = Tool::TransKP(m_offsetflag,m_pApi);

	st = Tool::TransStatus(status,m_pApi);

	m_status = status; 


	//���m_nEntrustNoΪ0 ctp�ӿ�ֱ�ӷ��� û��Ӧ��
	//���ctp�ӿ�
	if( (m_nEntrustNo==0) && (ordersysid >0) && (m_pApi->GetInterfaceType()==TRADE_CTP))
		m_nEntrustNo = orderref;


	sprintf(msg,"ί�лر�->��Լ %s  ί�к� %d  ״̬ %s  ״̬id %c",code,m_nEntrustNo,st,status);

	WRITE_LOG(1,msg);

 
	if(m_nordertype== 1 || m_nordertype== 2)  //6�ѳ�,5���� ��Ҫ׷��
	{
		if(m_pApi->IsBuChe(status) || m_pApi->IsYiChe(status))
		{
			sprintf(msg,"׷��->��Լ:%s,ί�к�:%d,�յ������ر�,׷��",m_szIntrument,m_nEntrustNo);

			WRITE_LOG(1,msg);

			//׷��
			int newcount = m_count - m_iTradeCount;
			
			sprintf(msg,"׷��->��Լ %s %s %s,ί�к� %d  ���� %d,�۸� %.4f ׷��...",m_szIntrument,bs,kp,m_nEntrustNo,newcount,m_price);

			WRITE_LOG(1,msg);

			m_ptrade->InsertOrder(m_szIntrument,newcount,m_direction,m_offsetflag,m_price,m_nordertype,m_ntimeout,m_jiacha,m_account);

			m_bDone = true;

			m_bWithDrawING = false;   //������ɣ�
		

		
		}

	}
	else if(m_nordertype==0)
	{	
		if(m_pApi->IsYiChe(m_status))
		{
			sprintf(msg,"��ʱ����->��Լ:%s,ί�к�:%d,�յ������ر�,������ֹ",m_szIntrument,m_nEntrustNo);
			WRITE_LOG(1,msg);
			m_bDone = true;
		}
		else if(m_pApi->IsBuChe(m_status) )
		{
			sprintf(msg,"��ʱ����->��Լ:%s,ί�к�:%d,�յ������ر�,������ֹ",m_szIntrument,m_nEntrustNo);
			WRITE_LOG(1,msg);
			m_bDone = true;
		}

	}

}

void COrderEx::RtnTrade(int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins)
{
	if(strcmp(m_szIntrument,ins) !=0)
		return ;

	if(m_nordertype==-1)
		return;

	char msg[128];

	CString code = ins;

	m_iTradeCount = m_iTradeCount + vol;
	
	sprintf(msg,"�ɽ��ر�->��Լ %s  ί������ %d  ί������ %d  �ɽ����� %d  �ɽ��۸� %.4f",code,m_nEntrustNo,m_count,vol,price);

	WRITE_LOG(1,msg);

	if( (m_iTradeCount==m_count)  && (price > 0.00001) )    //�ܳɽ�����==�µ�����
	{
		sprintf(msg,"�ɽ��ر�->ί������ %d  ȫ���ɽ� ������ֹ",m_nEntrustNo);

		m_bDone = true;

		WRITE_LOG(1,msg);

	}
	else if( (m_iTradeCount < m_count) && (price > 0.00001) )
	{
		sprintf(msg,"�ɽ��ر�->��Լ %s  ί������ %d  ���ֳɽ�",code,m_nEntrustNo);

		WRITE_LOG(1,msg);

		//m_bDone = true;
	}

	if(m_bDone)
		return;

	//��Ϊ�ɽ��۸�Ϊ0  �ǳ����ر� 
	if(price < 0.00001)
	{
		if( m_nordertype==1 || m_nordertype==2)
		{

			sprintf(msg,"׷��->��Լ %s  ί�к� %d  �յ������ر� ׷��",m_szIntrument,m_nEntrustNo);

			WRITE_LOG(1,msg);

			//׷�� ����Ϊ�˴γɽ�����
			int newcount = vol;

			double old_price = m_price;

			double newprice = 0.0;

			if(m_direction == '0')
				newprice = m_price /*+ m_jiacha*/;
			else if(m_direction == '1')
				newprice = m_price /*- m_jiacha*/;
			
			sprintf(msg,"׷��->��Լ %s  ί�к� %d  ���� %d  �۸� %.4f(ԭ�۸� %.4f)  ׷��...",m_szIntrument,m_nEntrustNo,newcount,newprice,old_price);

			WRITE_LOG(1,msg);

			//test
			//if(m_nordertype!=2)
			m_ptrade->InsertOrder(m_szIntrument,newcount,m_direction,m_offsetflag,m_price,m_nordertype,m_ntimeout,m_jiacha,m_account);

			m_bDone = true;

			m_bWithDrawING = false;   //������ɣ�
				
		}
		else if(m_nordertype==0)
		{
			sprintf(msg,"��ʱ����->��Լ %s  ί�к� %d  �յ������ر� ������ֹ",m_szIntrument,m_nEntrustNo);
			WRITE_LOG(1,msg);
			m_bDone = true;
		}
	
	}

}

/*�����֧�ֱ������õĽӿ�
	  entrustno != -9999 ��ʾ��֧�ֱ���������Ҫ����entrustno
*/
void COrderEx::OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)
{
	/*if(strcmp(m_szIntrument,ins) !=0)
		return ;*/

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
		//Ͷ��Ӯ�һر�����ֻ��ί�к� ���������ί�����ø���Ϊί�к� 
		//����ʹ�������������ر����ڱ���Ӧ����� ί�����þ���ԭ���ģ�����ί�к� �޷�ƥ��
		//m_nOrderRef = entrustno;

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

	}

	m_status = entrust_status;

}


bool COrderEx::DealPrice(CTickData *pd)
{
	if(strcmp(m_szIntrument,pd->szInstrument) !=0)
		return true;

	bool ret = true;

	double bid1 = pd->bidprice[0];
	double bid2 = pd->bidprice[1];
	double ask1 = pd->askprice[0];
	double ask2 = pd->askprice[1];

	if(m_nordertype !=ORDER_TYPE_ZUIJIA_NEWPRICE && m_nordertype !=ORDER_TYPE_ZUIJIA_JIACHA)   //�µ�����
		return true;

	if (m_bWithDrawING)    //����δ��� �������ٴγ���;
		return true;

	if(m_bDone)
		return true;

	//�����ȵ�ί��Ӧ����߻ر�����
	if(m_nEntrustNo==0)
		return true;

	char msg[128];

	//��Щ�����б�������ʱ�������
	time_t rawtime;
	time ( &rawtime );
	long timestamp = (long)rawtime;

	if(timestamp - m_timestamp <m_ntimeout)
	{
		sprintf(msg,"����ʱ���뵱ǰʱ����С�� %d",m_ntimeout);

		WRITE_LOG(1,msg);
		//return true;
	}


	//�ѳɣ��ѳ����������ϵ�  ��������
	if( m_pApi->IsYiCheng(m_status) || m_pApi->IsFeiDan(m_status)|| m_pApi->IsBuChe(m_status) || m_pApi->IsYiChe(m_status) )
	{
		m_bDone = true;
		return true;
	}

	

	//�Ƿ���������
	bool need = false; 

	double old_price = m_price;

	if(m_nordertype == ORDER_TYPE_ZUIJIA_NEWPRICE)
	{
		if(m_direction == '0')
		{
			if( (ask1 > m_price) ) 
			{	
				//�۲��Ƿ����
				if( (ask1 - m_price) >= (2*m_jiacha))
				{
					sprintf(msg,"���� %d  ���� %.4f  ����۲� %.4f",m_nEntrustNo,ask1 - m_price,2*m_jiacha);
					WRITE_LOG(1,msg);
					ret = false;
				}

				m_price = ask1;

				need = true;
			}
		}
		else if(m_direction == '1')
		{
			if( (bid1 < m_price) ) 
			{	
				//�۲��Ƿ����
				if( (m_price - bid1) >= (2*m_jiacha))
				{
					sprintf(msg,"���� %d  ���� %.4f  ����۲� %.4f",m_nEntrustNo,m_price - bid1,2*m_jiacha);
					WRITE_LOG(1,msg);
					ret = false;
				}

				m_price = bid1;

				need = true;
			}
		}
	}
	else if(m_nordertype == ORDER_TYPE_ZUIJIA_JIACHA)
	{
		if(m_direction == '0')
		{
			if( (ask1 - m_price) > m_jiacha ) 
			{								
				sprintf(msg,"ģʽ2 ��һ����ί�м۲� %.4f  �۲���� %.4f",(ask1 - m_price),m_jiacha);

				WRITE_LOG(1,msg);

				m_price = ask1;

				need = true;
			}
		}
		else if(m_direction == '1')
		{
			if( (m_price - bid1) > m_jiacha ) 
			{							
				sprintf(msg,"ģʽ2 ί�м�����һ�۲� %.4f  �۲���� %.4f",(m_price - bid1),m_jiacha);

				WRITE_LOG(1,msg);

				m_price = bid1;

				need = true;

			}
		}
	
	}

	if(need)
	{
		if(!m_bWithDrawING)
		{

			//m_bWithDrawING = true;

			//m_ptrade->WithDraw(m_account,m_nEntrustNo,m_szIntrument);

			if(WithDraw(m_nEntrustNo))
			{
				sprintf(msg,"׷��->��������ָ��  ί������ %d  ��Լ %s  ԭ�۸� %.4f  �¼۸� %.4f ����һ�� %.4f ����һ�� %.4f",m_nEntrustNo,m_szIntrument,old_price,m_price,bid1,ask1);
		
				WRITE_LOG(1,msg);
			}

		}
		else
		{
			sprintf(msg,"���� %d  ׷�۳���ʧ�ܣ��ñ����Ѿ����ڳ���״̬",m_nEntrustNo);
			WRITE_LOG(1,msg);
		}

	}

	return ret;
}


bool COrderEx::IsDone()
{
	return m_bDone;
}


void COrderEx::SetDone(bool done)
{
	m_bDone = done;

}


bool COrderEx::IsWithDraw()
{

	return m_bWithDrawING;
}


void COrderEx::SetWithDraw(bool withdraw)
{
	m_bWithDrawING = withdraw;
}


bool COrderEx::IsReOrder()
{
	return m_bReOrder;
}


void COrderEx::SetReOrder(bool order)
{
	m_bReOrder = order;
}


void COrderEx::RervseOrder(CTickData *pd)
{

	if(strcmp(m_szIntrument,pd->szInstrument) !=0)
		return ;

	//�µ������ǳɽ�����
	//int count = m_iTradeCount;
	//������ֹ�׷�� �����ǳ��� ���򿪲���������óɽ�������0
	int count = m_count;

	char bs = '0';
	char kp = '0';

	double newprice = 0;

	if(m_direction == '0')
	{
		bs = '1';
		newprice = pd->bidprice[0];

	}
	else if(m_direction == '1')
	{
		bs = '0';
		newprice = pd->askprice[0];
	}

	if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(m_pApi))
	{
		if(m_offsetflag == '0')
			kp = '3';
	}
	else
	{
		if(m_offsetflag == '0')
				kp = '1';
	}

	char msg[256];
	sprintf(msg,"ƽ��->���� %d  ��Լ %s  ԭ�۸� %.4f  ԭ���� %d  �¿��ּ۸� %.4f  �¿������� %d",m_nOrderRef,m_szIntrument,m_price,m_count,newprice,count);
	WRITE_LOG(1,msg);
	
	m_ptrade->InsertOrder(m_szIntrument,count,bs,kp,newprice,m_nordertype,m_ntimeout,m_jiacha,m_account);

	m_bReOrder = true;
}



int COrderEx::WithDraw(int ref)
{
	int ret = 1;
	m_lockWithDraw.lock();

	if( (ref == m_nEntrustNo) && (!m_bWithDrawING))
	{
		m_ptrade->WithDraw(m_account,m_nEntrustNo,m_szIntrument);
		char msg[128];
		sprintf(msg,"���� %d",m_nEntrustNo);
		WRITE_LOG(1,msg);
		m_bWithDrawING = true;
		ret = -1;
	}

	m_lockWithDraw.unlock();
	return ret;
}


int COrderEx::GetEntrustAmount()
{
	return m_count;
}


int COrderEx::GetTradeAmount()
{
	return m_iTradeCount;
}


double COrderEx::GetEntrustPrice()
{
	return m_price;
}



//////////////////////
//COrderGroup
//////////////////////
COrderGroup::COrderGroup(int id)
{
	m_iGrpId = id;
}



COrderGroup::~COrderGroup()
{


}


void COrderGroup:: OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)
{

	bool found = false;
	if( m_mapOrder.end() != m_mapOrder.find(ref) )
	{
		m_mapOrder[ref]->OnRspOrderInsert(ref,entrustno,entrust_status,errorid);
		found = true;
	}

	//20160615
	//Ϊ�˼���Ͷ��Ӯ�ҵĽӿ� ʲô���ɵĳ�����
	//ctp�ӿڱ����������û��OnRspOrderInsert����


	//20160629  ԭ���Ǽ���Ͷ��Ӯ��m_nOrderRef = entrustno;
	//����ֻ����ֻ�ܽӿ�������  �����ò���������  ��ע�͵�

	//if(found && errorid==0)
	//{
	//	m_mapOrder[entrustno] =  m_mapOrder[ref];
	//	m_mapOrder.erase(ref);
	//	char msg[128];
	//	sprintf(msg,"ԭ���� %d ����Ϊ %d",ref,entrustno);
	//	WRITE_LOG(1,msg);
	//}

}


void COrderGroup::RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins)
{
	char msg[256];

	if( m_mapOrder.end() != m_mapOrder.find(orderref) )
	{
		//ctp�ӿڳ�����ί�лر�	
		bool need = false;
		if(m_mapOrder[orderref]->m_pApi->IsYiChe(status))
		{
			for(unsigned int i=0;i<m_vecChedan.size(); i++)
			{
				//���ڳ�����׷������
				if(orderref == m_vecChedan[i])
				{
					sprintf(msg,"���� %d ���ڼ۲���󣬳���������׷��...",orderref);
					WRITE_LOG(1,msg);
					m_mapOrder[orderref]->SetDone(true);//��ֹ״̬
					m_mapOrder[orderref]->SetWithDraw(false);
					need = true;
				}
			}

			m_lockNotTrade.lock();
			for(unsigned int i=0;i<m_vecNotTrade.size(); i++)
			{
				if(orderref == m_vecNotTrade[i])
				{
					sprintf(msg,"���� %d δ�ɽ����������ɹ�,����������׷��...",orderref);
					WRITE_LOG(1,msg);
					m_mapOrder[orderref]->SetDone(true);//��ֹ״̬
					m_mapOrder[orderref]->SetWithDraw(false);
					m_lockNotTrade.unlock();
					return;
				}		
			}
			m_lockNotTrade.unlock();


			if(need)
			{
				DealReverse(orderref);
				return;
			}
		}


		//���δ�ɽ���������ʧ�ܣ������ñ����ɽ���	
		/*
		if(m_mapOrder[orderref]->m_pApi->IsYiCheng(status))
		{
			m_lockNotTrade.lock();
			for(unsigned int i=0;i<m_vecNotTrade.size(); i++)
			{			
				if(orderref == m_vecNotTrade[i] && m_mapOrder[orderref]->IsWithDraw())
				{
					sprintf(msg,"���� %d δ�ɽ���������ʧ�ܣ��ñ����ɽ���...",orderref);
					WRITE_LOG(1,msg);
					m_mapOrder[orderref]->SetDone(true);//��ֹ״̬
					m_mapOrder[orderref]->SetWithDraw(false);
				
					if(!m_mapOrder[orderref]->IsReOrder())
					{
						string code = m_mapOrder[orderref]->m_szIntrument;
						m_mapOrder[orderref]->RervseOrder(&m_mapTick[code]);
					}
					m_lockNotTrade.unlock();
					return;
				}
				
			}

			m_lockNotTrade.unlock();
		}*/
		

		m_mapOrder[orderref]->RtnOrder(orderref,ordersysid,status,bs,kp,ins);
	}

}


void COrderGroup::RtnTrade(int orderref,int ordersysid,double price,char bs,char kp,char * tradetime,int vol,char *ins)
{
	char msg[256];

	if( m_mapOrder.end() != m_mapOrder.find(orderref) )
	{

		bool need = false;

		//��ʾ�����ر�
		if(price < 0.0000001)
		{
			for(unsigned int i=0;i<m_vecChedan.size(); i++)
			{
				//���ڳ�����׷������
				if(orderref == m_vecChedan[i])
				{
					sprintf(msg,"���� %d ���ڼ۲���󣬳���������׷��...",orderref);
					WRITE_LOG(1,msg);
					m_mapOrder[orderref]->SetDone(true);//��ֹ״̬
					m_mapOrder[orderref]->SetWithDraw(false);
					need = true;
				}
			}

			m_lockNotTrade.lock();
			for(unsigned int i=0;i<m_vecNotTrade.size(); i++)
			{
				if(orderref == m_vecNotTrade[i])
				{
					sprintf(msg,"���� %d δ�ɽ����������ɹ�,��ֹ...",orderref);
					WRITE_LOG(1,msg);
					m_mapOrder[orderref]->SetDone(true);//��ֹ״̬
					m_mapOrder[orderref]->SetWithDraw(false);					
					m_lockNotTrade.unlock();
					return;
				}
			
			}
			m_lockNotTrade.unlock();

		}

		if(need)
		{
			DealReverse(orderref);
			return;
		}

		//���δ�ɽ���������ʧ�ܣ������ñ����ɽ���
		int temp = m_mapOrder[orderref]->GetTradeAmount();
		temp += vol;
		//�ɽ��ر� �����µ�ֻ����һ��
		if(/*temp==m_mapOrder[orderref]->GetTradeAmount() &&*/ (price  > 0.000001) )
		{
			m_lockNotTrade.lock();
			for(unsigned int i=0;i<m_vecNotTrade.size(); i++)
			{
				if(orderref == m_vecNotTrade[i] && m_mapOrder[orderref]->IsWithDraw())
				{
					sprintf(msg,"���� %d δ�ɽ���������ʧ�ܣ��ñ����ɽ���...",orderref);
					WRITE_LOG(1,msg);
					m_mapOrder[orderref]->SetDone(true);//��ֹ״̬
					m_mapOrder[orderref]->SetWithDraw(false);
				
					if(!m_mapOrder[orderref]->IsReOrder())
					{
						string code = m_mapOrder[orderref]->m_szIntrument;
						m_mapOrder[orderref]->RervseOrder(&m_mapTick[code]);
					}

					m_lockNotTrade.unlock();
					return;
				}
			
			}
			m_lockNotTrade.unlock();

			//���ǰ�涼�����ˣ�����ɽ����� ��Ҫ����ƽ��
			if( (m_vecNotTrade.size()>0) || (m_vecChedan.size() >0) )
			{
				if(!m_mapOrder[orderref]->IsReOrder())
				{
					string code = ins;
					m_mapOrder[orderref]->RervseOrder(&m_mapTick[code]);
					sprintf(msg,"���� %d  ���汨���ɽ� ��Ҫ����ƽ��",orderref);
					WRITE_LOG(1,msg);
				}

			}
		}


		m_mapOrder[orderref]->RtnTrade(orderref,ordersysid,price,bs,kp,tradetime,vol,ins);
	}

}


/*
������Ҫ׷�������������۲���󣬰������´���
1.�������������ɽ��� ��ƽ��
2.���û�ɽ��ͳ���
*/
void COrderGroup:: Tick(CTickData *pd)
{

#ifdef _DEBUG

	char msg[256];
	sprintf(msg,"�յ�tick  %s",pd->szInstrument);
	//WRITE_LOG(1,msg);

#endif 
	string code = pd->szInstrument;
	m_mapTick[code] = (*pd);

	map<int,COrderEx*>::iterator it = m_mapOrder.begin();
	for(;it != m_mapOrder.end();it++)
	{
		if(strcmp(pd->szInstrument,it->second->m_szIntrument)!=0)
			continue;

		//������ֹ״̬
		if(!it->second->IsDone())
		{
			if(!it->second->DealPrice(pd))
			{
				//׷�ۼ۲�ܴ�
				m_vecChedan.push_back(it->first);

				return;
			}
		}
	}

}


void COrderGroup::AddOrder(int ref, COrderEx *order)
{
	m_mapOrder[ref] = order;
}


int COrderGroup::GetGrpId()
{
	return m_iGrpId;
}


void COrderGroup::DealReverse(int orderref)
{
	char msg[256];

	//�������������г������߷��򿪲�
	map<int,COrderEx*>::iterator iter = m_mapOrder.begin();
	for(;iter != m_mapOrder.end();iter++)
	{
		int ref = iter->first;

		//����ֹ״̬���������ҵ�ǰû�д��ڳ���״̬
		if(!(iter->second->IsDone()))
		{
			if(!m_mapOrder[ref]->IsWithDraw())
			{
				iter->second->WithDraw(ref);
				m_lockNotTrade.lock();
				m_vecNotTrade.push_back(ref);	
				m_lockNotTrade.unlock();
				sprintf(msg,"���� %d  ��ӽ���δ�ɽ���������...",ref);
				WRITE_LOG(1,msg);
			}
			else
			{
				sprintf(msg,"���� %d  ���ڳ���...",ref);
				WRITE_LOG(1,msg);
				continue;
			}
		}
		//��ֹ״̬ƽ��
		else
		{
			bool found = false;
			for(unsigned int k=0;k<m_vecChedan.size(); k++)
			{
				if( (ref == m_vecChedan[k]) )
				{
					found = true;
					break;
				}
			}

			m_lockNotTrade.lock();
			for(unsigned int k=0;k<m_vecNotTrade.size(); k++)
			{
				if( (ref == m_vecNotTrade[k]) )
				{
					found = true;
					break;
				}
			}
			m_lockNotTrade.unlock();

			if(found)
				continue;

			//���ڳ������о�ƽ��
			if(!m_mapOrder[ref]->IsReOrder())
			{
				string code = m_mapOrder[ref]->m_szIntrument;
				if( m_mapTick.end() !=m_mapTick.find(code) )
					m_mapOrder[ref]->RervseOrder(&m_mapTick[code]);
				else
				{
					CTickData pd;
					strcpy(pd.szInstrument,code.c_str());
					pd.askprice[0] = m_mapOrder[ref]->GetEntrustPrice();
					pd.bidprice[0] = m_mapOrder[ref]->GetEntrustPrice();
					m_mapOrder[ref]->RervseOrder(&pd);
					sprintf(msg,"���� %d  ��ǰû�� %s �����飬����ԭί�м�  %.4f  ����...",ref,code.c_str(),m_mapOrder[ref]->GetEntrustPrice());
					WRITE_LOG(1,msg);
				}
			}

		}
			
	}

}


