#include "stdafx.h"

#include "StrategyGridV1.h"

#include "..\hsquantrade\FunctionPub.h"


#include<time.h>

#include "..\hsquantrade\tool.h"

#include <string>


CStrategyGrid::CStrategyGrid(void)
{
	m_strStrategyName = "�����ײ���";
	
	m_nStrategyStatus = STRATEGY_STATUS_INIT;

	m_nTradeCountSum = 0;

	m_fyingkuiPosition = 0;

	m_lastPrice = 0;

	m_hignPrice = 0;

	m_follow = "-1";                   //�Ƿ����,Ĭ��д��-1

	m_timeOut = 0;                     //��ʱʱ��

	m_jiacha = 0;					       //�۲�

	m_kp = 0;

	m_fyingkuiClose = 0;

	m_nInstrumentValue = 1;

	m_maxProfit = 0;

	m_fProfitBackStop = 0;

	m_pricebid1 = 0;

	m_priceask1 = 0;

	m_N1 = 0;

	m_nCount = 0;

	m_strParamList = "���׺�Լ;��Լ����;����������A1;��ʼ�۸�P1;������J1;ÿ��������N1;���׷���;��ƽ����;���ɽ�����;��󳷵�����;��ʼ����;�ʽ�س�;�������;�˻�;";

	m_params.insert(pair<string, CString>("���׺�Լ",""));

	m_params.insert(pair<string, CString>("��Լ����",""));

	m_params.insert(pair<string, CString>("����������A1",""));

	m_params.insert(pair<string, CString>("��ʼ�۸�P1",""));

	m_params.insert(pair<string, CString>("������J1",""));

	m_params.insert(pair<string, CString>("ÿ��������N1",""));

	m_params.insert(pair<string, CString>("���׷���","0"));

	m_params.insert(pair<string, CString>("��ƽ����","0"));

	m_params.insert(pair<string, CString>("���ɽ�����",""));

	m_params.insert(pair<string, CString>("��󳷵�����","500"));

	m_params.insert(pair<string, CString>("��ʼ����","0"));

	m_params.insert(pair<string, CString>("�ʽ�س�","0"));

	m_params.insert(pair<string, CString>("�������","500"));

	m_params.insert(pair<string, CString>("�˻�",""));

	m_params.insert(pair<string, CString>("�Ƿ�׷��","-1"));

	m_params.insert(pair<string, CString>("��ʱʱ��","0"));

	m_params.insert(pair<string, CString>("�۲�","0"));

	isNewModel = true;

	isAnohterDay = false;

	isSave = false;

	isLoad = false;

	m_Exit = false;


}

CStrategyGrid::CStrategyGrid(int i)
{
	if(i==1)
	{
		setValue();
		m_tOrder = thread( bind( &CStrategyGrid::checkOrder, this ) );

		//ָ���̺߳���
		//m_tStore = thread( bind( &CStrategyGrid::process, this ) );

	}
}


CStrategyGrid::~CStrategyGrid(void)
{
	m_Exit = true;
	m_orderinfo.clear();
	m_mapOrder.clear();
	m_tOrder.join();
	m_tStore.join();
	
}


void  CStrategyGrid::Tick(CTickData * pt)
{

	if(strcmp(m_strInstrument,pt->szInstrument)!=0)
		return;

	m_lastPrice = pt->lastprice;

	//��Ϊ�յ���һ�����¼۾Ϳ��̻��߽��м��Ͼ�����
	if((data!=NULL) && ((*data).isReady) )
	{
		//����ֱ��д��m_tickcount > 1
		if(m_tickcount == 2)
		{	
			m_btick = true;
			//return ;
		}

		m_tickcount++;
	}
	else
	{
		return;
	}

	//m_hignPrice = pt->highestprice;

	m_pricebid1 = pt->bidprice[0];

	m_priceask1 = pt->askprice[0];

	m_fyingkuiPosition = 0;

	/*
	for(int i=0;i<m_arPosition.GetSize();i++)
	{
		TPosition * pos= (TPosition *)m_arPosition[i];

		double temp = 0 ;

		if( pos->direction == '0' )
		{
			temp = ( pt->lastprice - pos->price ) * pos->count * m_nInstrumentValue;
		}
		else if( pos->direction == '1' )
		{
			temp = ( pos->price - pt->lastprice) * pos->count * m_nInstrumentValue;
		}

		m_fyingkuiPosition += temp;
	}	

	//���պ��ʽ�������
	if( ( m_fyingkuiPosition + m_fyingkuiClose ) < m_nMaxLost && m_nMaxLost < -1 ) //0 ��ʱ������
	{
		m_bRun = 0;

		char msg[128];

		sprintf_s(msg,128,"������%.3f ��������ʼ�������ã���֣�ֹͣ��������",m_fyingkuiPosition + m_fyingkuiClose);

		AddLog(msg);

		ClearPosition();
	}

	if(  ( m_fyingkuiPosition + m_fyingkuiClose ) > m_maxProfit )
	{
		m_maxProfit = ( m_fyingkuiPosition + m_fyingkuiClose );
	}

	float back =  m_maxProfit - ( m_fyingkuiPosition + m_fyingkuiClose );

	if( back > m_fProfitBackStop  && m_fProfitBackStop > 1 ) //0 ��ʱ������
	{
		m_bRun = 0;

		char msg[128];

		sprintf_s(msg,128,"�ʽ�س�%.3f �����ʽ�س����ޣ���֣�ֹͣ��������",back);

		AddLog(msg);

		ClearPosition();
	}
	*/

}

void CStrategyGrid::BeginRun()
{
	//�������������ٵ������̻߳ᵼ�±��������ڹ��캯�����治��
	//t = thread( bind( &CStrategyGrid::checkOrder, this ) );
	m_tStore = thread( bind( &CStrategyGrid::process, this ) );
	
}

void CStrategyGrid::Order()
{
	char msg[256];

	//����
	if(m_kp==0)
	{
		//��
		if( m_TradeDirection == 0 )
		{
			double price = m_beginPrice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{
				int orderref = OpenBuy(m_strInstrument.GetBuffer(m_strInstrument.GetLength()),count,price,m_fundAccount.GetBuffer(m_fundAccount.GetLength()));

				sprintf_s(msg,256,"��ʼ����,����,��������:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'0','0');

				price = price - m_J1;

				countAll += count;

				if( countAll + count > m_nCount )
				{
					break;
				}

				//�жϼ۸��Ƿ񳬹���ͣ�۸�

			
			}
		}
		//����
		else
		{
			double price = m_beginPrice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{
				int orderref = OpenSale(m_strInstrument.GetBuffer(m_strInstrument.GetLength()),count,price,m_fundAccount.GetBuffer(m_fundAccount.GetLength()));

				sprintf_s(msg,256,"��ʼ����,����,��������:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'1','0');

				price = price + m_J1;

				countAll += count;

				if( countAll + count > m_nCount )
				{
					break;
				}

				//�жϼ۸��Ƿ񳬹���ͣ�۸�
			}
		}
	}
	//ƽ��
	else if(m_kp==1)
	{
		//��ƽ
		if( m_TradeDirection == 0 )
		{
			double price = m_beginPrice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{
				dealHoldsinfo(count,price,m_TradeDirection);

				price = price - m_J1;

				countAll += count;

				if( countAll + count > m_nCount )
				{
					break;
				}

				//�жϼ۸��Ƿ񳬹���ͣ�۸�

			
			}
		}
		//��ƽ
		else
		{
			double price = m_beginPrice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{

				dealHoldsinfo(count,price,m_TradeDirection);

				price = price + m_J1;

				countAll += count;

				if( countAll + count > m_nCount )
				{
					break;
				}

				//�жϼ۸��Ƿ񳬹���ͣ�۸�
			}
		}

	}


}

void CStrategyGrid::SetParam(CString paramname,CString value)
{

	//"���׺�Լ;����������A1;��ʼ�۸�P1;������J1;ÿ��������N1;���׷���;���ɽ�����;��󳷵�����;������;�˻�;";

	m_params[(LPCTSTR)paramname] = value;

	//����Ҫ����Ӧ��ֵ����
	if(paramname=="���׺�Լ")
	{
		m_strInstrument = value;
	}
	else if(paramname=="��Լ����")
	{
		m_nInstrumentValue =  atoi(value);
	}
	else if(paramname=="����������A1")
	{
		if(m_nCount==0)
		{
			m_nCount = atoi(value);
			return;
		}

		int newCount = atoi(value);

		char msg[256];
		//���в���
		if(!checkOrdertime())
		{
			
			sprintf(msg,"�ǽ���ʱ�䲻����ı�������...");
			AddLog(msg);
			return;
		}

		if(!m_bRun || m_Exit)
		{
			sprintf(msg,"ģ�ʹ��ڷǼ���״̬,������ı�������...");
			AddLog(msg);
			return;
		}


		if(newCount > m_nCount)
		{
			CString s;
			s.Format("������������Ϊ:%d",newCount);

			AddLog(s.GetBuffer(0));

			addGrid(newCount);
		}
		else if(newCount < m_nCount)
		{
			CString s;
			s.Format("������������Ϊ:%d",newCount);

			AddLog(s.GetBuffer(0));

			subGrid(newCount);
		}

		m_nCount = newCount;

	}
	else if(paramname=="��ʼ�۸�P1")
	{
		m_beginPrice = atof(value);
	}
	else if(paramname=="������J1")
	{
		m_J1 = atof(value);
	}
	else if(paramname=="ÿ��������N1")
	{
		m_N1 = atof(value);
	}
	else if(paramname=="���׷���")
	{
		m_TradeDirection = atoi(value);
	}
	else if(paramname=="��ƽ����")
	{
		m_kp = atoi(value);
	}
	else if(paramname=="���ɽ�����")
	{
		m_nMaxTradeCount = atoi(value);
	}
	else if(paramname=="��󳷵�����")
	{
		m_nMaxChedanCount = atoi(value);
	}
	else if(paramname=="��ʼ����")
	{
		m_nMaxLost = atoi(value);
	}
	else if(paramname=="�ʽ�س�")
	{
		m_fProfitBackStop = atof(value);
	}
	else if(paramname=="�˻�")
	{
		m_fundAccount = value;
	}
	else if(paramname=="�������")
	{
		m_iIntervalOrder = atoi(value);
	}
}


void CStrategyGrid::SetParam(CString value)
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

	//if( m_TradeDirection == 0 )
	//{
	//	if(m_kp==0)
	//		m_strStrategyName = m_strStrategyName + m_strInstrument + "����";     //modelname
	//	else if(m_kp==1)
	//		m_strStrategyName = m_strStrategyName + m_strInstrument + "ƽ��";     //modelname

	//}
	//else if( m_TradeDirection == 1 )
	//{
	//	if(m_kp==0)
	//		m_strStrategyName = m_strStrategyName + m_strInstrument + "����";     //modelname
	//	else if(m_kp==1)
	//		m_strStrategyName = m_strStrategyName + m_strInstrument + "ƽ��";     //modelname
	//}

}

CString CStrategyGrid::GetParamValue()
{
	CString str;

	str.Format("%s;%d;%d;%.4f;%.4f;%.2f;%d;%d;%d;%d;%d;%.2f;%d;%s;",m_strInstrument,m_nInstrumentValue,m_nCount,m_beginPrice,m_J1,m_N1,m_TradeDirection,m_kp,m_nMaxTradeCount,m_nMaxChedanCount,m_nMaxLost,m_fProfitBackStop,m_iIntervalOrder,m_fundAccount);

	return str;
}

CString CStrategyGrid::GetParamValue(CString paramname)
{
	return m_params[(LPCTSTR)paramname];
}

CString CStrategyGrid::GetStatusInfo(void)
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

		openrice.Format("%.4f",m_beginPrice);
	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_BK )
	{
		status = "��";

		openrice.Format("%.4f",m_beginPrice);

	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_SK )
	{
		status = "����";

		openrice.Format("%.4f",m_beginPrice);

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

	if( m_TradeDirection == 0 )
	{
		if(m_kp==0)
			status = status+"(����)";     //modelname
		else
			status = status+"(ƽ��)";     //modelname
	}
	else if( m_TradeDirection == 1 )
	{
		if(m_kp==0)
			status = status+"(����)";     //modelname
		else
			status = status+"(ƽ��)";     //modelname
	}
	

	result.Format("%s;%s;%s;%d;%s;%s;%s(%s);%.4f;%s;%.4f;%.4f(%s);%s;%s;",m_strStrategyName,m_strInstrument,codename,m_nCount,status,openrice,yingkuiPos,yinkuirate,m_fyingkuiClose,modelstatus,m_lastPrice,m_hignPrice,huitiaorate,m_fundAccount,m_strStrategyName);

	//result = "�����ײ���TF1512����;TF1512;TF1512;10;��;100;0;�ղ�;0;0;041249,;";
	return result;
}

/*
����ɽ��ر�
���뷽ʽ��
���ֳɽ�-------------������ֹӮ��
ƽ�ֳɽ�-------------�����ҿ��ֵ�
*/
void CStrategyGrid::OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price)
{
	//�ж��Ƿ����ڱ����Եı���
	bool bt = false;
	
	bt = isExistRef(orderref,direction,offsetflag,ins);

	int notTradeCount = 0;            //���λر�֮ǰδ�ɽ�����

	char msg[256];

	if( bt )
	{
		notTradeCount = m_orderinfo[orderref].ordercount - m_orderinfo[orderref].tradecount;

		if( offsetflag == '0' )//����
		{
			if(direction == '0')
			{		
				sprintf_s(msg,256,"[�ر�]���ֳɽ� �ɽ��۸�%.3f,����%d,��������:%d",price,vol,orderref);

				AddLog(msg);

				if(m_kp==0)
					m_nTradeCountSum += vol;

				m_CheckSelfMutex.lock();

				double newprice = m_orderinfo[orderref].orderprice + m_J1;
				
				vol = dealTrade(orderref,vol,notTradeCount);

				if(vol == -1)
					return;

				/*
				//��Ҫ�ۼӳɽ�����
				m_orderinfo[orderref].tradecount += vol;
				//�Ѿ�ȫ���ɽ���ɾ��
				if(m_orderinfo[orderref].ordercount==m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);	
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"ȫ���ɽ�,ɾ���б�����:%d",orderref);
					AddLog(msg);

				}
				//���ֹ��ر����������ڱ������������
				else if(m_orderinfo[orderref].ordercount < m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);	
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"�ɽ�(%d)���ڱ���(%d),ȫ���ɽ�,ɾ���б�����:%d",m_orderinfo[orderref].tradecount,m_orderinfo[orderref].ordercount,orderref);
					AddLog(msg);

					vol = notTradeCount;

				}
				*/
				m_CheckSelfMutex.unlock();


				//�����ж���ƽ�󿪵����ɽ�����
				if( m_nTradeCountSum  >= m_nMaxTradeCount && m_kp==1)
				{
					sprintf_s(msg,256,"�ɽ����� %d ��������%d,�����µ� ",m_nTradeCountSum,m_nMaxTradeCount);

					AddLog(msg);

					return;
				}

				if(m_bRun)
				{
					//ƽ��
					JoinOrder(orderref,vol,newprice,'1',CLOSEFLAG_CLOSE_TODAY);

					//���ֻر������϶���ƽ��
					//int closeorderref = CloseSale(m_strInstrument.GetBuffer(0),vol,newprice,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);

					//AddOrderInfo(closeorderref,vol,newprice,'1',CLOSEFLAG_CLOSE_TODAY);

					//��¼���ֵĲ�λ
					//AddOpenPosition(closeorderref,vol,price,direction,ins);

					//sprintf_s(msg,256,"ֹӯ������ �۸�%.3f ����%d orderref=%d ",newprice,vol,closeorderref);

					//AddLog(msg);
				}

			}
			else
			{
				sprintf_s(msg,256,"[�ر�]���ֳɽ� �ɽ��۸�%.3f,����%d,��������:%d",price,vol,orderref);

				AddLog(msg);

				if(m_kp==0)
					m_nTradeCountSum += vol;


				m_CheckSelfMutex.lock();
				double newprice = m_orderinfo[orderref].orderprice - m_J1;

				vol = dealTrade(orderref,vol,notTradeCount);

				if(vol == -1)
					return;

				/*
				//��Ҫ�ۼӳɽ�����
				m_orderinfo[orderref].tradecount += vol;
				//�Ѿ�ȫ���ɽ���ɾ��
				if(m_orderinfo[orderref].ordercount==m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);	
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"ȫ���ɽ�,ɾ���б�����:%d��",orderref);
					AddLog(msg);
				}
				//���ֹ��ر����������ڱ������������
				else if(m_orderinfo[orderref].ordercount < m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);	
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"�ɽ�(%d)���ڱ���(%d),ȫ���ɽ�,ɾ���б�����:%d",m_orderinfo[orderref].tradecount,m_orderinfo[orderref].ordercount,orderref);
					AddLog(msg);

					vol = notTradeCount;

				}
				*/
				m_CheckSelfMutex.unlock();


				//�����жϳ�ʼƽ�ֵ����ɽ�����
				if( m_nTradeCountSum  >= m_nMaxTradeCount && m_kp==1)
				{
					sprintf_s(msg,256,"�ɽ����� %d ��������%d,�����µ� ",m_nTradeCountSum,m_nMaxTradeCount);

					AddLog(msg);

					return;
				}

				if(m_bRun)
				{
					//ƽ��
					JoinOrder(orderref,vol,newprice,'0',CLOSEFLAG_CLOSE_TODAY);

					//int closeorderref = CloseBuy(m_strInstrument.GetBuffer(0),vol,newprice,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);

					//AddOrderInfo(closeorderref,vol,newprice,'0',CLOSEFLAG_CLOSE_TODAY);

					////��¼���ֵĲ�λ
					//AddOpenPosition(closeorderref,vol,price,direction,ins);
					//	
					//sprintf_s(msg,256,"ֹӯ������ �۸�%.3f ����%d orderref=%d ",newprice,vol,closeorderref);

					//AddLog(msg);

				}

			}

		}
		else//ƽ��
		{
			CalCloseProfit(direction,price,vol);

			if(direction == '1')
			{
				sprintf_s(msg,256,"[�ر�]ƽ�ֳɽ� �ɽ��۸�%.3f,����%d,��������:%d",price,vol,orderref);

				AddLog(msg);

				if(m_kp==1)
					m_nTradeCountSum += vol;

				m_CheckSelfMutex.lock();
				double newprice = m_orderinfo[orderref].orderprice - m_J1;

				vol = dealTrade(orderref,vol,notTradeCount);

				if(vol == -1)
					return;

				/*
				//��Ҫ�ۼӳɽ�����
				m_orderinfo[orderref].tradecount += vol;
				//�Ѿ�ȫ���ɽ���ɾ��
				if(m_orderinfo[orderref].ordercount==m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"ȫ���ɽ�,ɾ���б�����:%d��",orderref);
					AddLog(msg);
				}
				//���ֹ��ر����������ڱ������������
				else if(m_orderinfo[orderref].ordercount < m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);	
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"�ɽ�(%d)���ڱ���(%d),ȫ���ɽ�,ɾ���б�����:%d",m_orderinfo[orderref].tradecount,m_orderinfo[orderref].ordercount,orderref);
					AddLog(msg);

					vol = notTradeCount;

				}
				*/
				m_CheckSelfMutex.unlock();


				//�����жϳ�ʼ���ֵ����ɽ�����
				if( m_nTradeCountSum  >= m_nMaxTradeCount && m_kp==0)
				{
					sprintf_s(msg,256,"�ɽ����� %d ��������%d,�����µ� ",m_nTradeCountSum,m_nMaxTradeCount);

					AddLog(msg);

					return;
				}

				if( m_bRun )
				{
					//����
					JoinOrder(orderref,vol,newprice,'0','0');
					//int ref = OpenBuy(m_strInstrument.GetBuffer(0),vol,newprice,m_fundAccount.GetBuffer(0));

					//AddOrderInfo(ref,vol,newprice,'0','0');

					//sprintf_s(msg,256,"���ֵ����� �۸�%.2f ����%d orderref=%d ",newprice,vol,ref);

					//AddLog(msg);
				}


			}
			else
			{
				sprintf_s(msg,256,"[�ر�]ƽ�ֳɽ� �ɽ��۸�%.2f,����%d,��������:%d",price,vol,orderref);

				AddLog(msg);

				if(m_kp==1)
					m_nTradeCountSum += vol;

				m_CheckSelfMutex.lock();
				double newprice = m_orderinfo[orderref].orderprice + m_J1;

				vol = dealTrade(orderref,vol,notTradeCount);

				if(vol == -1)
					return;

				/*
				//��Ҫ�ۼӳɽ�����
				m_orderinfo[orderref].tradecount += vol;
				//�Ѿ�ȫ���ɽ���ɾ��
				if(m_orderinfo[orderref].ordercount==m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"ȫ���ɽ�,ɾ���б�����:%d��",orderref);
					AddLog(msg);
				}
				//���ֹ��ر����������ڱ������������
				else if(m_orderinfo[orderref].ordercount < m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);	
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"�ɽ�(%d)���ڱ���(%d),ȫ���ɽ�,ɾ���б�����:%d",m_orderinfo[orderref].tradecount,m_orderinfo[orderref].ordercount,orderref);
					AddLog(msg);

					vol = notTradeCount;

				}*/
				m_CheckSelfMutex.unlock();

				//�����жϳ�ʼ���ֵ����ɽ�����
				if( m_nTradeCountSum  >= m_nMaxTradeCount && m_kp==0)
				{
					sprintf_s(msg,256,"�ɽ����� %d ��������%d,�����µ� ",m_nTradeCountSum,m_nMaxTradeCount);

					AddLog(msg);

					return;
				}

				if( m_bRun )
				{
					//����
					JoinOrder(orderref,vol,newprice,'1','0');

					//int ref = OpenSale(m_strInstrument.GetBuffer(0),vol,newprice,m_fundAccount.GetBuffer(0));

					//AddOrderInfo(ref,vol,newprice,'1','0');

					//sprintf_s(msg,256,"���ֵ����� �۸�%.3f ����%d orderref=%d ",newprice,vol,ref);

					//AddLog(msg);
				}

			}
		}
	}
}



void CStrategyGrid::OnErrorRtn(int code,char * errormsg) 
{
	char msg[128];

	
}


void CStrategyGrid::OnRspOrderInsert(int ref,char entrust_status,int errorid)
{

	/*

	//���ж��Ƿ��Ǹ�ģ�͵ı���
	m_CheckSelfMutex.lock();

	if(m_orderinfo.end() == m_orderinfo.find(ref))
	{
		m_CheckSelfMutex.unlock();

		return;
	}

	m_CheckSelfMutex.unlock();

	//�������ı���
	if(errorid !=0 )
	{
		CString s;
		s.Format("����ʧ��,������:%d,���Ӳ���,����ί������:%d",errorid,ref);
		AddLog(s.GetBuffer(0));

		m_reOrderMutex.lock();

		m_failOrder[ref] = m_orderinfo[ref];

		m_reOrderMutex.unlock();
		//ɾ��ָ��

		m_CheckSelfMutex.lock();

		m_orderinfo.erase(ref);

		m_CheckSelfMutex.unlock();
	}


	*/
}


void CStrategyGrid::OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins) 
{
	//����г�����Ҫɾ����Ӧ�ı�������
	if(isExistRef(orderref,bs,kp,ins))
	{
		if((orderstatus=='5' || orderstatus=='6') && (ordersystemid!=0))
		{
			CString s;
			s.Format("[�����ر�] ɾ����Ӧָ��,��������:%d",orderref);

			m_CheckSelfMutex.lock();

			//201604 begin						
			//�ж��Ƿ����ڲ��ɽ��ĳ����ر�
			map<int,COrderInfo>::iterator it = m_orderinfo.find(orderref);
			if(it != m_orderinfo.end())
			{
				if(it->second.selfclose == '1')
				{
					s.Format("[�����ر�] �ڲ��ɽ�������������,��������:%d",orderref);
					//JoinOrder�������Ѿ�����������������������ɵ�
					//m_JoinMutex.lock();

					int newCount = it->second.ordercount - it->second.tradecount;
					double newPrice = it->second.orderprice;
					char bs = it->second.bs;
					char kp = it->second.kp;
					JoinOrder(orderref,newCount,newPrice,bs,kp);
					char msg[256];
					sprintf_s(msg,256,"�������뱨������,����:%d,�۸�:%.3f,����:%d,����:%c,��ƽ:%c...",orderref,newPrice,newCount,bs,kp);
					AddLog(msg);

					//m_JoinMutex.unlock();	
				}
			}			
			//201604 end

			m_orderinfo.erase(orderref);

			m_CheckSelfMutex.unlock();

			RemoveOrder(orderref);
			AddLog(s.GetBuffer(0));
		}
		//���������ı���
		else if(ordersystemid==0 && orderstatus=='5')
		{
			/*
			CString s;
			s.Format("���Ӳ���,ԭ��������:%d",orderref);
			AddLog(s.GetBuffer(0));

			m_reOrderMutex.lock();
			m_failOrder[orderref] = m_orderinfo[orderref];
			m_reOrderMutex.unlock();
			//ɾ��ָ��

			m_CheckSelfMutex.lock();
			m_orderinfo.erase(orderref);
			m_CheckSelfMutex.unlock();
			*/
		
		}
	}

}


void CStrategyGrid::CalCloseProfit(char direction,double price,int count)
{

	if( direction == '0' )
	{
		
		//BP-��һ�����յĳֲ�
		for(int i=0;i<m_arPosition.GetSize();i++)
		{
			TPosition * pos = (TPosition *)m_arPosition[i];

			if( pos->direction =='1' )
			{
				m_arPosition.RemoveAt(i);
				
				m_fyingkuiClose += (pos->price - price ) * count * m_nInstrumentValue;

				delete pos;

				break;
			}
		}

	}
	else
	{
		//SP
		for(int i=0;i<m_arPosition.GetSize();i++)
		{
			TPosition * pos = (TPosition *)m_arPosition[i];

			if( pos->direction =='0' )
			{
				m_arPosition.RemoveAt(i);
				
				m_fyingkuiClose += (price - pos->price) * count * m_nInstrumentValue;

				delete pos;

				break;
			}
		}
	}
}

//ƽ�����в�λ����������δ�ɽ���
void CStrategyGrid::ClearPosition(void)
{	
	//�ȳ���
	for(int i=0;i<m_listOrderRef.GetSize();i++)
	{
		int orderref = m_listOrderRef[i];

		char msg[128];

		sprintf_s(msg,128,"���� orderref = %d",orderref);

		AddLog(msg);

		OrderAction(orderref);
	}
	
	//�����µ���ʽ����ֵ�ʱ����Ҫʹ��׷��
	m_follow = "1";

	m_jiacha = 2 * m_J1;

	//ƽ��
	for(int i=0;i<m_arPosition.GetSize();i++)
	{
		TPosition * pos = (TPosition *)m_arPosition[i];

		if( pos->direction == '0' )
		{			
			CloseSale(m_strInstrument.GetBuffer(0),pos->count,m_pricebid1-m_J1,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
		}
		else
		{
			CloseBuy(m_strInstrument.GetBuffer(0),pos->count,m_priceask1+m_J1,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
		}
	}
}


void CStrategyGrid::RemoveOrder(int orderref)
{
	for(int i=0;i<m_listOrderRef.GetSize();i++)
	{
		if( orderref == m_listOrderRef[i] )
		{
			m_listOrderRef.RemoveAt(i);
			break;
		}
	}
}

bool CStrategyGrid::isExistRef(int ref,char bs,char kp,char * ins)
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

void CStrategyGrid::addGrid(int count)
{
	int addcount = count - m_nCount;

	double price = 0;

	if(m_TradeDirection==0)
	{
		if(m_kp==0)
		{
			 price = m_beginPrice - (m_nCount / m_N1) * m_J1;
		}
		else if(m_kp==1)
		{
			 price = m_beginPrice + (m_nCount / m_N1) * m_J1;
		}
	}
	else if(m_TradeDirection==1)
	{
		if(m_kp==0)
		{
			 price = m_beginPrice + (m_nCount / m_N1) * m_J1;
		}
		else if(m_kp==1)
		{
			 price = m_beginPrice - (m_nCount / m_N1) * m_J1;
		}
	}

	//���в���
	if(checkOrdertime())
	{
		newOrder(addcount,price);
	}
	else
	{
		char msg[256];
		sprintf(msg,"�ǽ���ʱ�䲻����ı�������...");
		AddLog(msg);
	}

}


void CStrategyGrid::newOrder(int newcount,double newprice)
{

	char msg[256];

	//����
	if(m_kp==0)
	{
		//��
		if( m_TradeDirection == 0 )
		{
			double price = newprice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{
				int orderref = OpenBuy(m_strInstrument.GetBuffer(m_strInstrument.GetLength()),count,price,m_fundAccount.GetBuffer(m_fundAccount.GetLength()));

				sprintf_s(msg,256,"����,����,��������:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'0','0');

				price = price - m_J1;

				countAll += count;

				if( countAll + count > newcount )
				{
					break;
				}

				//�жϼ۸��Ƿ񳬹���ͣ�۸�

			
			}
		}
		//����
		else
		{
			double price = newprice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{
				int orderref = OpenSale(m_strInstrument.GetBuffer(m_strInstrument.GetLength()),count,price,m_fundAccount.GetBuffer(m_fundAccount.GetLength()));

				sprintf_s(msg,256,"����,����,��������:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'1','0');

				m_listOrderRef.Add(orderref);

				price = price + m_J1;

				countAll += count;

				if( countAll + count > newcount )
				{
					break;
				}

				//�жϼ۸��Ƿ񳬹���ͣ�۸�
			}
		}
	}
	//ƽ��
	else if(m_kp==1)
	{
		//��ƽ
		if( m_TradeDirection == 0 )
		{
			double price = newprice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{
				dealHoldsinfo(count,price,m_TradeDirection);

				price = price - m_J1;

				countAll += count;

				if( countAll + count > newcount )
				{
					break;
				}

				//�жϼ۸��Ƿ񳬹���ͣ�۸�

			
			}
		}
		//��ƽ
		else
		{
			double price = newprice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{

				dealHoldsinfo(count,price,m_TradeDirection);

				price = price + m_J1;

				countAll += count;

				if( countAll + count > newcount )
				{
					break;
				}

				//�жϼ۸��Ƿ񳬹���ͣ�۸�
			}
		}

	}

}


void CStrategyGrid::reload()                //���¼���
{
	//ԭʼ�ַ��� c++11
	CString path ;

	string s= Tool::GetModelDir();

	path = s.c_str();	

	path = path + m_strStrategyName + ".ini";
	//path = path  + "�����ײ���ru1605����3023422.ini";

	int amount  = GetPrivateProfileInt("orderinfo", "amount", 0, path);

	CString code,account,count,price,kp,bs;
	string code1,account1;

	//һֱ����-1
	//GetPrivateProfileString("modelparams", "���׺�Լ", "", code1.GetBuffer(0), 16, path);

	GetPrivateProfileString("modelparams", "���׺�Լ", "", (char*)code1.c_str(), 16, path);

	//GetPrivateProfileString("modelparams", "�˻�", "", account1.GetBuffer(0), 16, path);

	GetPrivateProfileString("modelparams", "�˻�", "", (char*)account1.c_str(), 16, path);

	code = code1.c_str();
	account = account1.c_str();

	for(int i=0;i<amount;i++)
	{
	
		CString k1,k2,k3,k4,v2,v3,v4;

		string a,b,c;

		k1.Format("count%d",i);
		k2.Format("price%d",i);
		k3.Format("bs%d",i);
		k4.Format("kp%d",i);
		
		int v1 = GetPrivateProfileInt("orderinfo", k1, 0, path);
		//GetPrivateProfileString("orderinfo", k2, "", v2.GetBuffer(0), 20, path); 
		//GetPrivateProfileString("orderinfo", k3, "", v3.GetBuffer(0), 2, path); 
		//GetPrivateProfileString("orderinfo", k4, "", v4.GetBuffer(0), 2, path); 

		GetPrivateProfileString("orderinfo", k2, "", (char*)a.c_str(), 20, path); 
		GetPrivateProfileString("orderinfo", k3, "", (char*)b.c_str(), 2, path); 
		GetPrivateProfileString("orderinfo", k4, "", (char*)c.c_str(), 2, path);

		v2 = a.c_str();
		v3 = b.c_str();
		v4 = c.c_str();


		if(v1==0)
			continue;

			int cc = v1;

			double pp = atof(v2);

			if(v3=="0")
			{
				if(v4=="0")
				{
					int ref = OpenBuy(code.GetBuffer(0),cc,pp,account.GetBuffer(0));

					AddOrderInfo(ref,cc,pp,'0','0');

				}
				else if(v4=="1" || v4=="3")
				{
					int ref = CloseBuy(code.GetBuffer(0),cc,pp,account.GetBuffer(0),CLOSEFLAG_CLOSE);

					AddOrderInfo(ref,cc,pp,'0','1');

				}
			}
			else if(v3=="1")
			{
				if(v4=="0")
				{
					int ref = OpenSale(code.GetBuffer(0),cc,pp,account.GetBuffer(0));

					AddOrderInfo(ref,cc,pp,'1','0');

				}
				else if(v4=="1" || v4=="3")
				{
					int ref = CloseSale(code.GetBuffer(0),cc,pp,account.GetBuffer(0),CLOSEFLAG_CLOSE);

					AddOrderInfo(ref,cc,pp,'1','1');

				}
			
			}

		}
	

}


void CStrategyGrid::saveModel()            //����ģ��
{

	if(m_orderinfo.size()<=0)
		return;

	//ԭʼ�ַ��� c++11
	CString path ;

	string s= Tool::GetModelDir();

	path = s.c_str();	

	path = path + m_strStrategyName + ".ini";


	std::map<std::string,CString>::iterator it; 

	//���׺�Լ;��Լ����;����������A1;��ʼ�۸�P1;������J1;ÿ��������N1;���׷���;���ɽ�����;��󳷵�����;��ʼ����;�ʽ�س�;�˻�;
	for(it=m_params.begin();it!=m_params.end();it++)
	{
		string key = it->first;
		CString value = it->second;
		WritePrivateProfileString("modelparams", key.c_str(), value, path);
	}

	//����δ�ɽ��Ŀ��ֺ�ƽ��ί��
	CString c,count,price,bs,kp;
	c.Format("%d",m_orderinfo.size());
	WritePrivateProfileString("orderinfo", "amount", c, path);

	int i=0;

	map<int,COrderInfo>::iterator it_order;
	for(it_order=m_orderinfo.begin();it_order!=m_orderinfo.end();it_order++)
	{
		COrderInfo c = it_order->second;
		CString k1,k2,k3,k4,v1,v2,v3,v4;

		k1.Format("count%d",i);
		k2.Format("price%d",i); 
		k3.Format("bs%d",i); 
		k4.Format("kp%d",i); 

		v1.Format("%d",c.ordercount-c.tradecount);
		v2.Format("%.4f",c.orderprice);
		v3.Format("%c",c.bs);
		v4.Format("%c",c.kp);

		WritePrivateProfileString("orderinfo", k1, v1, path);
		WritePrivateProfileString("orderinfo", k2, v2, path);
		WritePrivateProfileString("orderinfo", k3, v3, path);
		WritePrivateProfileString("orderinfo", k4, v4, path);

		i++;
	}

	//���������Ҫ���m_orderinfo
	m_orderinfo.clear();

}


bool CStrategyGrid::checkOrdertime()
{
	
	bool ret = false;
	
	string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));

	char a = (*data).m_sCodeStatus[prefix].status;
	if(a=='3' /*|| a=='5'*/ || a=='2')
		ret =true;

	return ret;

	//bool ret =false;

	//const time_t t = time(NULL);

	//struct tm* current_time = localtime(&t);

	//int h = current_time->tm_hour;
	//int m = current_time->tm_min;
	//int s = current_time->tm_sec;

	//if( (h==16) && (m>=35) )
	//{
	//	ret =  true;
	//}
	//return ret;
}

bool CStrategyGrid::checkMatchTime()
{
	bool ret = false;
	
	string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));

	char a = (*data).m_sCodeStatus[prefix].status;
	if(a=='5')
		ret =true;

	return ret;
}

bool CStrategyGrid::checkothertime()
{
	bool ret =false;

	string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));

	char a = (*data).m_sCodeStatus[prefix].status;
	if(a=='1' || a=='0')
		ret =true;

	return ret;

	//return false;

}

bool CStrategyGrid::checksavetime()
{
	bool ret = false;

	string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));

	char a = (*data).m_sCodeStatus[prefix].status;
	if(a=='6')
		ret =true;

	return ret;

}

bool CStrategyGrid::reLogintime()
{
	bool ret =false;

	const time_t t = time(NULL);

	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;

	if( (h==8 || h==20) && (20<m && m<50) )
	{
		ret =  true;
	}
	else if( (h==12 ) && (55<m && m<59) )
	{
		ret =  true;
	}
	else if( (h==13 ) && (25<m && m<29) )
	{
		ret =  true;
	}

	return ret;

}

//bool CStrategyGrid::checkbefore()
//{
//	bool ret =false;
//
//	const time_t t = time(NULL);
//
//	struct tm* current_time = localtime(&t);
//
//	int h = current_time->tm_hour;
//	int m = current_time->tm_min;
//	int s = current_time->tm_sec;
//
//	if( (h==8 || h==20) && (m>=50) )
//	{
//		ret =  true;
//	}
//	else if(h==9 || h==21)
//	{
//		ret =  true;
//	}
//	else if( (h==10) &&(h>=25) )
//	{
//		ret =  true;
//	}
//	else if( (h==12) &&(h>=55) )
//	{
//		ret =  true;
//	}
//	else if( (h==13) &&(h>=25) )
//	{
//		ret =  true;
//	}
//
//	return ret;
//
//}

void CStrategyGrid::checkOrder()
{

	//���ﲻ���ڸ��˻�Ҳ��ֱ�ӽ����ѭ��
	while( (data==NULL) || !(*data).isReady)
	{
		AddLog("wait...");
		if((data!=NULL))
		{
			if( (m_kp==1) && ( checkOrdertime()) )
			{
				CString str;
				str.Format("���ڲ�ѯ�ֲ�...");
				AddLog(str.GetBuffer(0));
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(5));

		if(data==NULL)
		{
			//AddLog("�޴��ʺ�...");
			//return;
		}

	}

	while(!m_Exit)
	{
		
		//AddLog("222...");
		if(m_Exit)
		{
			CString str;

			str.Format("%s:������ֹ���˳�",m_strStrategyName);

			this->AddLog(str.GetBuffer(0));

			return;
		}

		if(!m_bRun)
		{
			CString str;

			str.Format("%s:������ͣ���ȴ�",m_strStrategyName);

			this->AddLog(str.GetBuffer(0));

			std::this_thread::sleep_for(std::chrono::milliseconds(3*1000));

			continue;
		}

		if(reLogintime())
		{
			::SendMessage(m_hMsgWnd,WM_MESSAGE_ENTRUST,3,0);

			std::this_thread::sleep_for(std::chrono::milliseconds(3*1000));
		}


		if(checkOrdertime() || m_btick)
		{
			//AddLog("333...");
			m_btick = false;

			if(isNewModel)
			{
				AddLog("ģ����ӳɹ�...");

				if( m_nStrategyStatus == 0 )
				{
					////ָ���̺߳���
					//m_tStore = thread( bind( &CStrategyGrid::process, this ) );

					Order();
				}

				isNewModel = false;

				std::this_thread::sleep_for(std::chrono::milliseconds(100));

				continue;
			}

			//����ʧ�ܵĵ����ٴα���
			reOrderAgain();

			if(isAnohterDay && !isNewModel && !isLoad)
			{
				this->AddLog("reload...");

				reload();

				isLoad = true;

				isSave = false;

				isAnohterDay = false;

				std::this_thread::sleep_for(std::chrono::milliseconds(10*1000));

			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}


		//����
		if(checksavetime())
		{
			//����̺������ȴ�
			if(isNewModel)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10*1000));
				continue;
			}

			if(!isSave && !isAnohterDay)
			{
				AddLog("save...");
				saveModel();
				isSave = true;
				isAnohterDay = true;
				isLoad = false;
				m_listOrderRef.RemoveAll();
				m_orderinfo.clear();

			}
		
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

		}

		//һ��ǽ���ʱ��
		if(checkothertime())
		{
			//AddLog("444...");
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			continue;
			
		}

		//û�о�ǿ��˯3��
		string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));
		if((*data).m_sCodeStatus[prefix].status==0 && !m_btick)
			std::this_thread::sleep_for(std::chrono::milliseconds(1*1000));

		//AddLog("555...");
		
	}

	if(m_Exit)
	{
		CString str;

		str.Format("%s:������ֹ���˳�",m_strStrategyName);

		this->AddLog(str.GetBuffer(0));

		return;
	}


}

void CStrategyGrid::setValue()
{
	m_strStrategyName = "�����ײ���";
	
	m_nStrategyStatus = STRATEGY_STATUS_INIT;

	m_nTradeCountSum = 0;

	m_fyingkuiPosition = 0;

	m_lastPrice = 0;

	m_hignPrice = 0;

	m_follow = "-1";                   //�Ƿ����,Ĭ��д��-1

	m_timeOut = 0;                     //��ʱʱ��

	m_jiacha = 0;					       //�۲�

	m_fyingkuiClose = 0;

	m_nInstrumentValue = 1;

	m_maxProfit = 0;

	m_fProfitBackStop = 0;

	m_pricebid1 = 0;

	m_priceask1 = 0;

	m_N1 = 0;

	m_kp = 0;

	m_nCount = 0;

	m_btick = false;

	m_tickcount = 0;

	m_iIntervalOrder = 500;

	m_strParamList = "���׺�Լ;��Լ����;����������A1;��ʼ�۸�P1;������J1;ÿ��������N1;���׷���;��ƽ����;���ɽ�����;��󳷵�����;��ʼ����;�ʽ�س�;�������;�˻�;";

	m_params.insert(pair<string, CString>("���׺�Լ",""));

	m_params.insert(pair<string, CString>("��Լ����",""));

	m_params.insert(pair<string, CString>("����������A1",""));

	m_params.insert(pair<string, CString>("��ʼ�۸�P1",""));

	m_params.insert(pair<string, CString>("������J1",""));

	m_params.insert(pair<string, CString>("ÿ��������N1",""));

	m_params.insert(pair<string, CString>("���׷���","0"));

	m_params.insert(pair<string, CString>("��ƽ����","0"));

	m_params.insert(pair<string, CString>("���ɽ�����",""));

	m_params.insert(pair<string, CString>("��󳷵�����","500"));

	m_params.insert(pair<string, CString>("��ʼ����","0"));

	m_params.insert(pair<string, CString>("�ʽ�س�","0"));

	m_params.insert(pair<string, CString>("�˻�",""));

	m_params.insert(pair<string, CString>("�Ƿ�׷��","-1"));

	m_params.insert(pair<string, CString>("��ʱʱ��","0"));

	m_params.insert(pair<string, CString>("�۲�","0"));

	m_params.insert(pair<string, CString>("�������","500"));

	isNewModel = true;

	isAnohterDay = false;

	isSave = false;

	isLoad = false;

	m_Exit = false;


}


void CStrategyGrid::dealHoldsinfo(int count,double price,int bs)
{
	
	if(data == NULL)
		return;

	int yp = 0;  
	int p = 0;
	int index = 0;

	CString msg;

	m_QueryHoldMutex.lock();

	for(int i=0;i<(*data).m_sHoldsInfo.size();i++)
	{
		if (strcmp(((*data).m_sHoldsInfo)[i].szInstrument,m_strInstrument)!=0)
			continue;

		if(((*data).m_sHoldsInfo)[i].PosiDirection + bs== 1 )
		{
			yp = ((*data).m_sHoldsInfo)[i].YdAvaiable;
			p = ((*data).m_sHoldsInfo)[i].TdAvaiable;
			msg.Format("�˻�:%s,��ǰ���ý��:%d,�������%d...",m_fundAccount,p,yp);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();
			index = i;
			break;
		}
	
	}

	

	if(yp==0 && p==0)
	{
		msg.Format("���óֲֲ���...");
		AddLog(msg.GetBuffer(0));
		msg.ReleaseBuffer();

		m_QueryHoldMutex.unlock();
		return;
	}


	if(bs==0)
	{
		if(p>=count)    //��ִ����µ�����
		{
			int orderref = CloseBuy(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);

			AddOrderInfo(orderref,count,price,'0',CLOSEFLAG_CLOSE_TODAY);

			((*data).m_sHoldsInfo)[index].TdAvaiable = ((*data).m_sHoldsInfo)[index].TdAvaiable - count;

			int rest = ((*data).m_sHoldsInfo)[index].TdAvaiable;

			msg.Format("ƽ���:%d��,ʣ����:%d��,��������:%d",count,rest,orderref);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();

		}
		else if(yp + p >=count && p<count)
		{
			//��ƽ���
			int orderref = 0;

			if(p>0)
			{
				orderref =CloseBuy(m_strInstrument.GetBuffer(0),p,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
							
				AddOrderInfo(orderref,p,price,'0',CLOSEFLAG_CLOSE_TODAY);
				
			}

			int yorderref=CloseBuy(m_strInstrument.GetBuffer(0),count-p,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE);

			AddOrderInfo(yorderref,count-p,price,'0',CLOSEFLAG_CLOSE);

			((*data).m_sHoldsInfo)[index].TdAvaiable = 0;

			((*data).m_sHoldsInfo)[index].YdAvaiable = ((*data).m_sHoldsInfo)[index].YdAvaiable - (count-p);

			int rest = ((*data).m_sHoldsInfo)[index].YdAvaiable;

			msg.Format("����ƽ���:%d��,ʣ����:0��,��������:%d;��ƽ�ϲ�:%d��,ʣ���ϲ�:%d��,��������:%d",p,orderref,(count-p),rest,yorderref);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();
		}
	}
	else if(bs==1)
	{
		if(p>=count)
		{
			int orderref = CloseSale(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);

			AddOrderInfo(orderref,count,price,'1',CLOSEFLAG_CLOSE_TODAY);

			((*data).m_sHoldsInfo)[index].TdAvaiable = ((*data).m_sHoldsInfo)[index].TdAvaiable - count;

			int rest = ((*data).m_sHoldsInfo)[index].TdAvaiable;

			msg.Format("ƽ���:%d��,ʣ����:%d��,��������:%d",count,rest,orderref);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();

		}
		else if(yp + p>=count && p<count)
		{
			//��ƽ���
			int orderref = 0;
			if(p>0)
			{
				orderref =CloseSale(m_strInstrument.GetBuffer(0),p,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);

				AddOrderInfo(orderref,p,price,'1',CLOSEFLAG_CLOSE_TODAY);

			}

			int yorderref =CloseSale(m_strInstrument.GetBuffer(0),count-p,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE);

			AddOrderInfo(yorderref,count-p,price,'1',CLOSEFLAG_CLOSE);

			((*data).m_sHoldsInfo)[index].TdAvaiable = 0;

			((*data).m_sHoldsInfo)[index].YdAvaiable = ((*data).m_sHoldsInfo)[index].YdAvaiable - (count-p);

			int rest = ((*data).m_sHoldsInfo)[index].YdAvaiable;

			msg.Format("����ƽ���:%d��,ʣ����:0��,��������:%d;��ƽ�ϲ�:%d��,ʣ���ϲ�:%d��,��������:%d",p,orderref,(count-p),rest,yorderref);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();
		}

	}

	m_QueryHoldMutex.unlock();

}



void CStrategyGrid::setHoldinfo()
{
	CString msg;
	int p = 0;
	int yp = 0;

	for(int i=0;i<(*data).m_sHoldsInfo.size();i++)
	{
		if (strcmp((*data).m_sHoldsInfo[i].szInstrument,m_strInstrument)!=0)
			continue;

		if((*data).m_sHoldsInfo[i].PosiDirection == 0 )
		{
			yp = (*data).m_sHoldsInfo[i].YdAvaiable = (*data).m_sHoldsInfo[i].TdAvaiable + (*data).m_sHoldsInfo[i].YdAvaiable;
			p = (*data).m_sHoldsInfo[i].TdAvaiable = 0;
			msg.Format("�̺�,�˻�:%s,��ֹ��ϲ�,��ǰ���ý��:%d,�������%d...",m_fundAccount,p,yp);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();

			break;
		}

		if(((*data).m_sHoldsInfo)[i].PosiDirection == 1 )
		{
			yp = (*data).m_sHoldsInfo[i].YdAvaiable = (*data).m_sHoldsInfo[i].TdAvaiable + (*data).m_sHoldsInfo[i].YdAvaiable;
			p = (*data).m_sHoldsInfo[i].TdAvaiable = 0;
			msg.Format("�̺�,�˻�:%s,��ֹ��ϲ�,��ǰ���ý��:%d,�������%d...",m_fundAccount,p,yp);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();

			break;
		}
	
	}
	
}

void CStrategyGrid::updateHoldinfo(char bs,char kp,char *ins,int count)
{

	CString msg;
	int oldnum = 0;
	int newnum = 0;

	int ibs = bs - 48;
	int ikp = kp - 48;

	for(int i=0;i<(*data).m_sHoldsInfo.size();i++)
	{
		if (strcmp((*data).m_sHoldsInfo[i].szInstrument,ins)!=0)
			continue;

		if(ikp==0)
		{
			if((*data).m_sHoldsInfo[i].PosiDirection == ibs )
			{
				oldnum = (*data).m_sHoldsInfo[i].TdAvaiable ;
				newnum = (*data).m_sHoldsInfo[i].TdAvaiable + count;
				(*data).m_sHoldsInfo[i].TdAvaiable = newnum;

				msg.Format("�˻�:%s,���ֻر�,���¿��óֲ�.ԭ����%d,����%d,�¿���%d",m_fundAccount,oldnum,count,newnum);
				AddLog(msg.GetBuffer(0));
				msg.ReleaseBuffer();

				break;
			}

		}
		else if(ikp==1)
		{
			if(((*data).m_sHoldsInfo)[i].PosiDirection == ibs )
			{
				oldnum = (*data).m_sHoldsInfo[i].TdAvaiable ;
				newnum = (*data).m_sHoldsInfo[i].TdAvaiable - count;
				(*data).m_sHoldsInfo[i].TdAvaiable = newnum;

				msg.Format("�˻�:%s,ƽ�ֻر�,���¿��óֲ�.ԭ����%d,����%d,�¿���%d",m_fundAccount,oldnum,count,newnum);
				AddLog(msg.GetBuffer(0));
				msg.ReleaseBuffer();

				break;
			}

		}

	
	}

}



void CStrategyGrid::reOrderAgain()
{

	int count = 0;
	double price = 0;
	char msg[256];

	m_reOrderMutex.lock();

	if(m_failOrder.size()==0)
	{
		m_reOrderMutex.unlock();
		return;
	}

	map<int,COrderInfo>::iterator it;
	for(it=m_failOrder.begin();it!=m_failOrder.end();it++)
	{
		count = it->second.ordercount;
		price = it->second.orderprice;

		if(it->second.bs=='0' && it->second.kp=='0')
		{
			int orderref = OpenBuy(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0));
			sprintf_s(msg,256,"����,����,��������:%d",orderref);

			AddLog(msg);

			AddOrderInfo(orderref,count,price,'0','0');		
		
		}
		else if(it->second.bs=='1' && it->second.kp=='0')
		{
			int orderref = OpenSale(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0));
			sprintf_s(msg,256,"����,����,��������:%d",orderref);

			AddLog(msg);

			AddOrderInfo(orderref,count,price,'1','0');	
			
		}
		else if(it->second.bs=='0' && it->second.kp=='1')
		{
			int orderref = CloseBuy(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE);
			sprintf_s(msg,256,"����,ƽ��,��������:%d",orderref);
			AddLog(msg);

			AddOrderInfo(orderref,count,price,'0',CLOSEFLAG_CLOSE);	
		
		}
		else if(it->second.bs=='1' && it->second.kp=='1')
		{
			int orderref = CloseSale(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE);
			sprintf_s(msg,256,"����,ƽ��,��������:%d",orderref);
			AddLog(msg);

			AddOrderInfo(orderref,count,price,'1',CLOSEFLAG_CLOSE);	
			
		}
		else if(it->second.bs=='0' && it->second.kp=='3')
		{
			if(isAnohterDay)
			{
				int orderref = CloseBuy(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE);
				sprintf_s(msg,256,"����,ƽ��,��������:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'0',CLOSEFLAG_CLOSE);	
		
			}
			else
			{
				int orderref = CloseBuy(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
				sprintf_s(msg,256,"����,ƽ���ͷ,��������:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'0',CLOSEFLAG_CLOSE_TODAY);
				
			}
		}
		else if(it->second.bs=='1' && it->second.kp=='3')
		{
			if(isAnohterDay)
			{
				int orderref = CloseSale(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE);
				sprintf_s(msg,256,"����,ƽ��,��������:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'1',CLOSEFLAG_CLOSE);

			}
			else
			{
				int orderref = CloseSale(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
				
				sprintf_s(msg,256,"����,ƽ���ͷ,��������:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'1',CLOSEFLAG_CLOSE_TODAY);

			}
			
		}


	}
	
	m_failOrder.clear();

	m_reOrderMutex.unlock();

}


/*
selfclose ==0 ��������״̬
selfclose ==1 �����Գɽ����ҽ���ȴ����У����β�����
selfclose ==2 ���ο����Գɽ���δ�����������´α�������
*/

void CStrategyGrid::OrderTogether()
{

	m_JoinMutex.lock();

	//��ʱmap�������Ҫ�ȴ������ɹ��ı���
	map<int,COrderInfo> tempWait;

	if(m_mapOrder.size()==0)
	{
		m_JoinMutex.unlock();
		return;
	}

	int count = 0;
	double price = 0;
	char msg[256];
	int ref = 0;
	int orderref = 0;

	map<int,COrderInfo>::iterator it;

	//�Ƚ����Գɽ����
	for(it=m_mapOrder.begin();it!=m_mapOrder.end();it++)
	{
		orderref = it->second.ref;

		//�Ѿ��������ҽ�����һ�α������еı���������
		if(it->second.selfclose == '1')
		{
			continue;
		}

		//�������Գɽ����
		while( CheckSelfNotOrder(orderref,it->second.orderprice,it->second.bs,it->second.kp,it->second.ordercount) )
		{
			sprintf_s(msg,256,"ѭ�����,��������:%d,�۸�:%.3f,����:%c,��ƽ:%c ",orderref,it->second.orderprice,it->second.bs,it->second.kp);
			AddLog(msg);

			//������Ϊ�Ǽ۸���ȿ��ܷ������Գɽ����
			if(it->second.selfclose == '0')
			{
				sprintf_s(msg,256,"���ڼ۲�,���ܷ����Գɽ�,���沢������һ�α���,��������:%d...",orderref);
				AddLog(msg);

				//�����ָ������Գɽ�,����Ҫ�ȴ�����
				it->second.selfclose = '1';

				//��¼�����Գɽ��ı�����Ϣ
				COrderInfo *order = new COrderInfo();
				strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
				order->ref = orderref;
				order->ordercount = it->second.ordercount;
				order->tradecount = 0;              
				order->orderprice = it->second.orderprice;
				order->bs = it->second.bs;
				order->kp = it->second.kp;
				order->selfclose = '2';                  
				tempWait[orderref] = *order;
				delete order;
				

			}
			
		}

		if(it->second.selfclose == '1')
		{
			//˵�������������Ѿ������Գɽ��������Ƿ�����һ�α������У�����
			continue;
		}

		//�ҵ��Գɽ����
		if( CheckSelfOrder(orderref,it->second.orderprice,it->second.bs,it->second.kp) )
		{
			
			sprintf_s(msg,256,"��鵽��ҵ��Գɽ�,�ȴ�����,��������:%d,�۸�:%.3f,����:%c,��ƽ:%c ",orderref,it->second.orderprice,it->second.bs,it->second.kp);
			AddLog(msg);

			//�����ָ������Գɽ�,����Ҫ�ȴ�����
			it->second.selfclose = '1';

			//��¼�����Գɽ��ı�����Ϣ
			COrderInfo *order = new COrderInfo();
			strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
			order->ref = orderref;
			order->ordercount = it->second.ordercount;
			order->tradecount = 0;              
			order->orderprice = it->second.orderprice;
			order->bs = it->second.bs;
			order->kp = it->second.kp;
			order->selfclose = '2';                  
			tempWait[orderref] = *order;
			delete order;

	
		}

	}

	//�������ٱ���
	OrderByMap(m_mapOrder);

	m_mapOrder.clear();

	map<int,COrderInfo>::iterator iter; 
	for(iter=tempWait.begin();iter!=tempWait.end();iter++)
	{
		int ref = iter->second.ref;

		m_mapOrder[ref] = iter->second;

		sprintf(msg,"�����Գɽ�����ת�뱨������,�ȴ��������,�ٽ����´α���:%d",ref);
		AddLog(msg);

	}
	

	m_JoinMutex.unlock();

}


/*
**1.һ�ʱ����ֱʳɽ�������Ҫ���
**2.��ͬ�����������ǿ�ƽ�����������򡢼۸�һֱҲ��Ҫ���
*/
void CStrategyGrid::JoinOrder(int ref,int vol,double orderprice,char bs,char kp)
{

	m_JoinMutex.lock();

	char msg[256];

	sprintf(msg,"��������,�ɽ�����:%d,�ɽ���:%d,�µ�ί�м۸�:%.3f",ref,vol,orderprice);

	AddLog(msg);

	map<int,COrderInfo>::iterator it; 

	it = m_mapOrder.find(ref);

	int newCount = 0;
	double newPrice = 0.0;

	if(it!=m_mapOrder.end())
	{
		int count = m_mapOrder[ref].ordercount;

		m_mapOrder[ref].ordercount = vol + count;

		sprintf(msg,"���±���,ԭ��������:%d,������������:%d,�ܹ�:%d,�����۸�:%.3f,�ɽ�����:%d",count,vol,(vol + count),orderprice,ref);

		AddLog(msg);
	}
	else
	{
		//��Ѱ��֮ǰ�������кϲ�
		if( JoinDiffRef(ref,vol,orderprice,bs,kp) )
		{	
		}
		else
		{
			//֮ǰû�кϲ��ı�����������
			COrderInfo order;
			strcpy(order.szInstrument,m_strInstrument);
			order.ref = ref;
			order.ordercount = vol;
			order.orderprice = orderprice;
			order.tradecount = 0;
			order.bs = bs;
			order.kp = kp;

			m_mapOrder[ref] = order;

			sprintf(msg,"���ɱ���,��������:%d,�����۸�:%.3f,�ɽ�����:%d",vol,orderprice,ref);

			AddLog(msg);
		}
	
	}

	m_JoinMutex.unlock();

}

/*
**���ref��ͬ�ı���
*/
bool CStrategyGrid::JoinDiffRef(int ref,int vol,double orderprice,char bs,char kp)
{
	bool ret = false;
	char msg[256];

	map<int,COrderInfo>::iterator iter;
	for(iter = m_mapOrder.begin();iter!= m_mapOrder.end(); iter++)
	{
		int orderref = iter->second.ref;

		if(ref!= orderref && iter->second.orderprice == orderprice && iter->second.bs == bs && iter->second.kp == kp)
		{
			ret = true;

			int oldcount = iter->second.ordercount;
			int newcount = vol + oldcount;

			iter->second.ordercount = newcount;

			sprintf(msg,"�ϲ�,���α���:%d,����:%d,ԭ����:%d,����:%d",ref,vol,orderref,oldcount);
			AddLog(msg);

			sprintf(msg,"�±�������,��������:%d,�۸�:%.4f,����:%d,����:%c,��ƽ:%c",orderref,orderprice,newcount,bs,kp);
			AddLog(msg);
	
			break;
		}
	}

	return ret;

}


void CStrategyGrid::AddOrderInfo(int ref,int count,double price,char bs,char kp)
{
		m_listOrderRef.Add(ref);

		m_CheckSelfMutex.lock();

		COrderInfo *order = new COrderInfo();
		strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
		order->ref = ref;
		order->ordercount = count;
		order->tradecount = 0;
		order->orderprice = price;
		order->bs = bs;
		order->kp = kp;
		order->selfclose = '0';
		m_orderinfo[ref] = *order;
		delete order;

		m_CheckSelfMutex.unlock();

}

void CStrategyGrid::AddOpenPosition(int ref,int count,double price,char bs,char *code)
{
		//��¼���ֵĲ�λ
		TPosition * pos = new TPosition;

		m_arPosition.Add(pos);

		strcpy_s(pos->ins,code);

		pos->count = count;

		pos->price  = price;

		pos->direction = bs;

}

void CStrategyGrid::process()
{

	//���ﲻ���ڸ��˻�Ҳ��ֱ�ӽ����ѭ��
	while( (data==NULL) || !(*data).isReady)
	{
		AddLog("wait...");
		if((data!=NULL))
		{
			if( (m_kp==1) && ( checkOrdertime()) )
			{
				CString str;
				str.Format("���ڲ�ѯ�ֲ�...");
				AddLog(str.GetBuffer(0));
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(5));

		if(data==NULL)
		{
			//AddLog("�޴��ʺ�...");
			//return;
		}

	}


	while(!m_Exit)
	{
		if(m_Exit)
			return;

		//����Ƿǽ���ʱ����ȴ�
		if(!checkOrdertime())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			continue;
		}
		//������Ͼ��۴�Ͻ׶γɽ��ˣ���ȴ�
		if(checkMatchTime())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			continue;
		}

		//test
		//std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		//OrderAction(20007);
		//return;

		//�ֱʳɽ�����
		OrderTogether();

		std::this_thread::sleep_for(std::chrono::milliseconds(m_iIntervalOrder));

		//AddLog("process...");

		//�Գɽ�����
		//OrderSelfColse();

	}

}


bool CStrategyGrid::CheckSelfNotOrder(int ref,double price,char bs,char kp,int count)
{
	bool ret = false;

	if(m_mapOrder.size()==0)
		return ret;

	char msg[256];

	double lastPrice = 0.0;

	char lastBS = '0';

	char lastKP = '0';

	int lastRef = 0;

	int lastCount = 0;


	map<int,COrderInfo> ::iterator it;
	for(it = m_mapOrder.begin();it!=m_mapOrder.end();it++)
	{
		lastRef = it->second.ref;
		lastPrice = it->second.orderprice;
		lastBS = it->second.bs;
		lastKP = it->second.kp;
		lastCount = it->second.ordercount;

		//���lastRef�Ѿ������˾�����
		//������������ڲ��ɽ��ͼ������
		if( lastRef==ref)
		{
			continue;
		}

		//�Ѿ��������ҽ�����һ�α������еı���������
		if(it->second.selfclose == '1')
		{
			continue;
		}


		//�� > ��ƽ
		if( (bs=='0') && (kp=='0') && (price>=lastPrice) && (lastBS=='1') && (lastKP == '1' || lastKP == '3')  )
		{
			ret = true;
			break;
		}
		//���� < ��ƽ
		else if( (bs=='1') && (kp=='0') && (price<=lastPrice) && (lastBS=='0') && (lastKP == '1' || lastKP == '3') )
		{
			ret = true;
			break;
		}
		//��ƽ > ����
		else if( (bs=='0') && (kp == '1' || kp == '3') && (price>=lastPrice) && (lastBS=='1') && (lastKP=='0') )
		{
			ret = true;
			break;
		}
		//��ƽ < ��
		else if( (bs=='1') && (kp == '1' || kp == '3') && (price<=lastPrice) && (lastBS=='0') && (lastKP=='0') )
		{
			ret = true;
			break;
		}

	}

	if(ret)
	{
		sprintf(msg,"��δ����У��(������,��У��),�۸�(%.3f,%.3f),����(%c,%c),��ƽ(%c,%c),����(%d,%d),��������(%d,%d)",lastPrice,price,lastBS,bs,lastKP,kp,lastCount,count,lastRef,ref);

		AddLog(msg);

		//����۸���ͬ�����������¸���һ��
		//bug...
		//if( (price - lastPrice < 0.0000001) ||  (lastPrice - price < 0.0000001 ) )
		//if( (price - lastPrice < 0.0000001) ||  (price - lastPrice > -0.0000001 ) )
		if( price == lastPrice  )
		{

			if(count < lastCount)
			{
				ReverseOrder(ref,0,count);
				ReverseOrder(lastRef,0,count);
			}
			else if(count > lastCount)
			{
				ReverseOrder(ref,0,lastCount);
				ReverseOrder(lastRef,0,lastCount);
			}
			else
			{
				ReverseOrder(ref,0,count);
				ReverseOrder(lastRef,0,count);
			}

		}
		else
		{
			//201603  begin
			//��Ϊ�۸�һ�²����Գɽ�
			//ret = false;
			//201603 end

			//201604 begin
			//��ǳ��۸���ȵĿ����Գɽ�����
			//���retΪtrue���Ҽ۸���ȣ�ReverseOrde������  ���� it->second.selfclose = '1';
			//������ ret=true,it->second.selfclose = '0'  �ж��Ƿ��Ǽ۸���ȿ��ܷ������Գɽ�
			//201604 end
		}

	}


	return ret;

}

bool CStrategyGrid::CheckSelfOrder(int ref,double price,char bs,char kp)
{
	m_CheckSelfMutex.lock();

	bool ret = false;

	if(m_orderinfo.size()==0)
	{
		m_CheckSelfMutex.unlock();
		return ret;
	}

	char msg[256];

	double lastPrice = 0.0;

	char lastBS = '0';

	char lastKP = '0';

	int lastRef = 0;

	int lastCount = 0;

	map<int,COrderInfo> ::iterator iter;
	for(iter = m_orderinfo.begin();iter!=m_orderinfo.end();iter++)
	{
		lastRef = iter->second.ref;
		lastPrice = iter->second.orderprice;
		lastBS = iter->second.bs;
		lastKP = iter->second.kp;
		lastCount = iter->second.ordercount;

		//201604 begin  
		//���ǲ����ӳٱ����Ĵ�����Ϊ���ڳ���·�ϳɽ������.����ά��ԭ��������Ϊ�ɽ���û��ϵ	
		//Ϊ1������ǰ�������Ѿ��������������ˡ�����Ӧ�𣬻�ɾ����orderinfo
		if ( iter->second.selfclose == '1' )
		{
			sprintf_s(msg,256,"�ùҵ�(%d)�Ѿ������Գɽ�,�Ѿ�������������...",lastRef);
			AddLog(msg);
			continue;
		}
		

		//�� > ��ƽ
		if( (bs=='0') && (kp=='0') && (price>=lastPrice) && (lastBS=='1') && (lastKP == '1' || lastKP == '3')  )
		{
			ret = true;
			break;
		}
		//���� < ��ƽ
		else if( (bs=='1') && (kp=='0') && (price<=lastPrice) && (lastBS=='0') && (lastKP == '1' || lastKP == '3') )
		{
			ret = true;
			break;
		}
		//��ƽ > ����
		else if( (bs=='0') && (kp == '1' || kp == '3') && (price>=lastPrice) && (lastBS=='1') && (lastKP=='0') )
		{
			ret = true;
			break;
		}
		//��ƽ < ��
		else if( (bs=='1') && (kp == '1' || kp == '3') && (price<=lastPrice) && (lastBS=='0') && (lastKP=='0') )
		//else if( bs=='1' && (kp == '1' || kp == '3') && (price>=lastPrice) && lastBS=='0' && lastKP=='0'  )
		{
			ret = true;
			break;
		}

	}

	if(ret)
	{
		sprintf(msg,"��ҵ�У��(�ҵ�,��У��),�۸�(%.3f,%.3f),����(%c,%c),��ƽ(%c,%c),��������(%d,%d)",lastPrice,price,lastBS,bs,lastKP,kp,lastRef,ref);

		AddLog(msg);

		
		//if( (price - lastPrice < 0.0000001) ||  (lastPrice - price < 0.0000001 ) )
		//if( (price - lastPrice < 0.0000001) ||  (price - lastPrice > -0.0000001 ) )
		//����ֱ�ӳ���,�������ɹ�û��ϵ
		//if( price == lastPrice  )
		{
			
			sprintf_s(msg,256,"���ҵ�,ʹ����뱨������,��������:%d",lastRef);

			OrderAction(lastRef);

			iter->second.selfclose = '1';

			AddLog(msg);

		}
		/*
		else
		{
			//�۸�����ӳٴ���
			//201603 begin
			//ret = false;
			//201603 end

			//201604 begin
			//���ڻ��Ƿ���true
			//201604 end


		}
		*/

	}


	m_CheckSelfMutex.unlock();

	return ret;

}

void CStrategyGrid::OrderSelfColse()
{

	if(m_mapSelfOrder.size()==0)
	{
		return;
	}

	int count = 0;
	double price = 0;
	char msg[256];
	int ref = 0;
	int orderref = 0;

	map<int,COrderInfo>::iterator it; 
	for(it=m_mapSelfOrder.begin();it!=m_mapSelfOrder.end();it++)
	{
		count = it->second.ordercount;
		price = it->second.orderprice;
			
		if(it->second.kp == '0' && it->second.bs == '0')
		{
			ref = OpenBuy(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0));
			AddOrderInfo(ref,count,price,'0','0');
				
		}
		else if(it->second.kp=='0' && it->second.bs=='1')
		{
			OpenSale(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0));
			AddOrderInfo(ref,count,price,'1','0');
		}
		else if(it->second.kp!='0' && it->second.bs=='0')
		{
			CloseBuy(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
			AddOrderInfo(ref,count,price,'0',CLOSEFLAG_CLOSE_TODAY);
			
		}
		else if(it->second.kp!='0' && it->second.bs=='1')
		{
			CloseSale(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
			AddOrderInfo(ref,count,price,'1',CLOSEFLAG_CLOSE_TODAY);
		}

		sprintf(msg,"����Գɽ�����,��������:%d",ref);
		AddLog(msg);
	}

	m_mapSelfOrder.clear();

}



int CStrategyGrid::dealTrade(int orderref,int vol,int iNotTradeCount)
{
	char msg[256];

	map<int,COrderInfo> ::iterator it = m_orderinfo.find(orderref);

	if(it == m_orderinfo.end())
	{
		sprintf_s(msg,256,"�����ڱ�������:%d...",orderref);
		AddLog(msg);
		return -1;
	}

	//��Ҫ�ۼӳɽ�����
	m_orderinfo[orderref].tradecount += vol;
	//�Ѿ�ȫ���ɽ���ɾ��
	if(m_orderinfo[orderref].ordercount == m_orderinfo[orderref].tradecount)
	{
		RemoveOrder(orderref);	
		m_orderinfo.erase(orderref);
		sprintf_s(msg,256,"ȫ���ɽ�,ɾ���б�����:%d",orderref);
		AddLog(msg);

	}
	//���ֹ��ر����������ڱ������������
	else if(m_orderinfo[orderref].ordercount < m_orderinfo[orderref].tradecount)
	{
		RemoveOrder(orderref);	
		m_orderinfo.erase(orderref);
		sprintf_s(msg,256,"�ɽ�(%d)���ڱ�������(%d),ȫ���ɽ�,ɾ���б�����:%d",m_orderinfo[orderref].tradecount,m_orderinfo[orderref].ordercount,orderref);
		AddLog(msg);

		sprintf_s(msg,256,"δ�ɽ�����:%d",iNotTradeCount);
		AddLog(msg);

		vol = iNotTradeCount;

	}

	return vol;
}


bool CStrategyGrid::ReverseOrder(int ref,int type,int count)
{

	char msg[256];

	if(type == 0)
	{
		map<int,COrderInfo> ::iterator it = m_mapOrder.find(ref);

		if(it != m_mapOrder.end())
		{
			char bs = it->second.bs;
			char kp = it->second.kp;
			double price = it->second.orderprice;

			char newBs = '0';
			char newKp = '0';
			double newPrice = 0;

			//�򿪳ɽ�
			if(bs == '0' && kp == '0')
			{
				newBs = '1';
				newKp = '3';
				newPrice = price + m_J1;
			}
			//�����ɽ�
			else if(bs == '1' && kp == '0')
			{
				newBs = '0';
				newKp = '3';
				newPrice = price - m_J1;
			}
			//��ƽ�ɽ�
			else if(bs == '0' && (kp == '3' || kp == '1') )
			{
				newBs = '1';
				newKp = '0';
				newPrice = price + m_J1;
			}
			//��ƽ�ɽ�
			else if(bs == '1' && (kp == '3' || kp == '1') )
			{
				newBs = '0';
				newKp = '0';
				newPrice = price - m_J1;
			}

		
			//���
			if(count < it->second.ordercount)
			{	
						
				COrderInfo order;
				strcpy(order.szInstrument,m_strInstrument.GetBuffer(0));
				order.ref = it->second.ref * 10;
				order.ordercount = it->second.ordercount - count;
				order.tradecount = 0;
				order.orderprice = it->second.orderprice;
				order.bs = it->second.bs;
				order.kp = it->second.kp;
				order.selfclose = '0';
    
				//����ֱ�ӱ���
				OrderByInfo(order);

				
				//�ڲ��ɽ�����
				it->second.ordercount = count;
				it->second.bs = newBs;
				it->second.kp = newKp;
				it->second.orderprice = newPrice;
				it->second.tradecount = 0;                     

				sprintf_s(msg,256,"�������,����ԭ�ƻ��ȱ���:%d,ʣ��:%d �����ڲ��ɽ�...",order.ordercount,count);

				AddLog(msg);

			}
			else if(count == it->second.ordercount)
			{
				//��������
				it->second.bs = newBs;
				it->second.kp = newKp; 
				it->second.orderprice = newPrice;
				it->second.tradecount = 0;						
			}
			
			it->second.selfclose = '2';   //���︳ֵ2����ʾ�����µ�

			sprintf_s(msg,256,"�ڲ��Գɽ�(ԭ,��),��������:%d,����(%c,%c),��ƽ(%c,%c),�۸�(%.3f,%.3f)",ref,bs,it->second.bs,kp,it->second.kp,price,it->second.orderprice);

			AddLog(msg);

		}
	}
	//����������������
	else if(type == 1)
	{
		map<int,COrderInfo> ::iterator it = m_orderinfo.find(ref);

		if(it != m_orderinfo.end())
		{
			char bs = it->second.bs;
			char kp = it->second.kp;
			double price = it->second.orderprice;

			COrderInfo order;
			strcpy(order.szInstrument,m_strInstrument);
			order.ref = ref;
			order.ordercount = it->second.ordercount;
			order.orderprice = price;
			order.tradecount = 0;
			order.bs = bs;
			order.kp = kp;
			order.selfclose = '0';

			m_mapOrder[ref] = order;

			//�򿪳ɽ�
			if(bs == '0' && kp == '0')
			{
				order.bs = '1';
				order.kp = '3';
				order.orderprice = price + m_J1;
			}
			//�����ɽ�
			else if(bs == '1' && kp == '0')
			{
				order.bs = '0';
				order.kp = '3';
				order.orderprice = price - m_J1;
			}
			//��ƽ�ɽ�
			else if(bs == '0' && (kp == '3' || kp == '1') )
			{
				order.bs = '1';
				order.kp = '0';
				order.orderprice = price + m_J1;
			}
			//��ƽ�ɽ�
			else if(bs == '1' && (kp == '3' || kp == '1'))
			{
				order.bs = '0';
				order.kp = '0';
				order.orderprice = price - m_J1;
			}

			sprintf_s(msg,256,"���ҵ�,�ڲ��Գɽ�(ԭ,��),��������:%d,����(%c,%c),��ƽ(%c,%c),�۸�(%.3f,%.3f)",ref,bs,order.bs,kp,order.kp,price,order.orderprice);

			AddLog(msg);

		}
	}

	return true;

}


void CStrategyGrid::OrderByMap(map<int,COrderInfo> orderMap)
{
	char msg[256];

	map<int,COrderInfo> ::iterator it;

	int ref = 0;

	for(it = orderMap.begin(); it!= orderMap.end();it++)
	{

		if(it->second.selfclose == '1')
		{
			sprintf(msg,"�����Գɽ�,����������,��������%d",it->second.ref);
			AddLog(msg);
			continue;
		}
	
		int count = it->second.ordercount;
		double price = it->second.orderprice;
		int orderref = it->second.ref;

		if(it->second.bs == '0' && it->second.kp == '0')
		{
			ref = OpenBuy(m_strInstrument.GetBuffer(0),count,it->second.orderprice,m_fundAccount.GetBuffer(0));
			AddOrderInfo(ref,count,it->second.orderprice,'0','0');
				
		}
		else if(it->second.bs=='1' && it->second.kp=='0' )
		{
			ref = OpenSale(m_strInstrument.GetBuffer(0),count,it->second.orderprice,m_fundAccount.GetBuffer(0));
			AddOrderInfo(ref,count,it->second.orderprice,'1','0');
		}
		else if(it->second.bs=='0' && it->second.kp!='0')
		{
			ref = CloseBuy(m_strInstrument.GetBuffer(0),count,it->second.orderprice,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
			AddOrderInfo(ref,count,it->second.orderprice,'0',CLOSEFLAG_CLOSE_TODAY);
			
		}
		else if(it->second.bs=='1' && it->second.kp!='0')
		{
			ref = CloseSale(m_strInstrument.GetBuffer(0),count,it->second.orderprice,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
			AddOrderInfo(ref,count,it->second.orderprice,'1',CLOSEFLAG_CLOSE_TODAY);
		}

		if(it->second.selfclose == '2')
		{
			sprintf(msg,"�Գɽ������б���,ԭ��������:%d,�����ɱ�������%d,ɾ��...",orderref,ref);
			AddLog(msg);
			it->second.selfclose = '0';
		}

		sprintf(msg,"ֹӯ������,��������:%d",ref);
		AddLog(msg);

	}


}


void CStrategyGrid::OrderByInfo(COrderInfo orderinfo)
{
	map<int,COrderInfo> map;

	map[orderinfo.ref] = orderinfo;

	OrderByMap(map);

}