#include "stdafx.h"

#include "StrategyGrid.h"

#include "FunctionPub.h"

#include "TradeInterface.h"
#include<time.h>
#include "tool.h"
#include <string>
//using namespace std;


vector<CHoldsinfo> CStrategyGrid::m_holdsinfo;

map<string,CCodeStatus>CStrategyGrid::m_codestatus;

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

	jiacha = 0;					       //�۲�

	m_kp = 0;

	m_fyingkuiClose = 0;

	m_nInstrumentValue = 1;

	m_maxProfit = 0;

	m_fProfitBackStop = 0;

	m_pricebid1 = 0;

	m_priceask1 = 0;

	m_N1 = 0;

	m_nCount = 0;

	m_strParamList = "���׺�Լ;��Լ����;����������A1;��ʼ�۸�P1;������J1;ÿ��������N1;���׷���;��ƽ����;���ɽ�����;��󳷵�����;��ʼ����;�ʽ�س�;�˻�;";

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
		
		t = thread( bind( &CStrategyGrid::checkOrder, this ) );

	}
}


CStrategyGrid::~CStrategyGrid(void)
{
	m_bRun = false;
	m_Exit = true;
	m_orderinfo.clear();
	t.join();
	
}


void  CStrategyGrid::Tick(CTickData * pt)
{
	m_lastPrice = pt->lastprice;

	m_hignPrice = pt->highestprice;

	m_pricebid1 = pt->bidprice[0];

	m_priceask1 = pt->askprice[0];

	m_fyingkuiPosition = 0;

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


}

void CStrategyGrid::BeginRun()
{

	if( m_nStrategyStatus == 0 )
	{
		//��ʼ�ҵ�
		Order();
	}
	
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

				sprintf_s(msg,256,"����,��������:%d",orderref);

				AddLog(msg);

				m_listOrderRef.Add(orderref);

				COrderInfo *order = new COrderInfo();
				strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
				order->ref = orderref;
				order->ordercount = count;
				order->tradecount = 0;
				order->orderprice = price;
				order->tradeprice = 0;
				order->bs = '0';
				order->kp = '0';
				order->begin_kp = 0;    //��ʼ�ǿ�
				m_orderinfo[orderref] = *order;
				delete order;

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

				sprintf_s(msg,256,"����,��������:%d",orderref);

				AddLog(msg);

				m_listOrderRef.Add(orderref);

				COrderInfo *order = new COrderInfo();
				strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
				order->ref = orderref;
				order->ordercount = count;
				order->tradecount = 0;
				order->orderprice = price;
				order->tradeprice = 0;
				order->bs = '1';
				order->kp = '0';
				order->begin_kp = 0;    //��ʼ�ǿ�
				m_orderinfo[orderref] = *order;
				delete order;

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

	if( m_TradeDirection == 0 )
	{
		if(m_kp==0)
			m_strModelName = m_strStrategyName + m_strInstrument + "����";     //modelname
		else if(m_kp==1)
			m_strModelName = m_strStrategyName + m_strInstrument + "ƽ��";     //modelname

	}
	else if( m_TradeDirection == 1 )
	{
		if(m_kp==0)
			m_strModelName = m_strStrategyName + m_strInstrument + "����";     //modelname
		else if(m_kp==1)
			m_strModelName = m_strStrategyName + m_strInstrument + "ƽ��";     //modelname
	}

}

CString CStrategyGrid::GetParamValue()
{
	CString str;

	str.Format("%s;%d;%d;%.4f;%.4f;%.2f;%d;%d;%d;%d;%d;%.2f;%s;",m_strInstrument,m_nInstrumentValue,m_nCount,m_beginPrice,m_J1,m_N1,m_TradeDirection,m_kp,m_nMaxTradeCount,m_nMaxChedanCount,m_nMaxLost,m_fProfitBackStop,m_fundAccount);

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

	codename = CStockInfo::getfutuName(m_strInstrument.GetBuffer(0));

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
	

	result.Format("%s;%s;%s;%d;%s;%s;%s(%s);%.4f;%s;%.4f;%.4f(%s);%s;%s;",m_strStrategyName,m_strInstrument,codename,m_nCount,status,openrice,yingkuiPos,yinkuirate,m_fyingkuiClose,modelstatus,m_lastPrice,m_hignPrice,huitiaorate,m_fundAccount,m_strModelName);

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

	for(int i = 0;i<m_listOrderRef.GetSize();i++)
	{
		if( orderref == m_listOrderRef[i] )
		{
			bt = true;
			break;
		}
	}

	bt = isExistRef(orderref,direction,offsetflag,ins);

	char msg[256];

	if( bt )
	{
		if( offsetflag == '0' )//����
		{
			//if( m_TradeDirection == 0 )
			if(direction == '0')
			{		
				
				sprintf_s(msg,256,"[�ر�]���ֳɽ� �ɽ��۸�%.3f,����%d,��������:%d",price,vol,orderref);

				AddLog(msg);

				if(m_kp==0)
					m_nTradeCountSum += vol;

				double newprice = m_orderinfo[orderref].orderprice + m_J1;

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


				//�����ж���ƽ�󿪵����ɽ�����
				if( m_nTradeCountSum  >= m_nMaxTradeCount && m_kp==1)
				{
					sprintf_s(msg,256,"�ɽ����� %d ��������%d,�����µ� ",m_nTradeCountSum,m_nMaxTradeCount);

					AddLog(msg);

					return;
				}

				if(m_bRun)
				{
					//���ֻر������϶���ƽ��
					int closeorderref = CloseSale(m_strInstrument.GetBuffer(0),vol,newprice,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);

					COrderInfo *order = new COrderInfo();
					strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
					order->ref = closeorderref;
					order->ordercount = vol;
					order->orderprice = newprice;
					order->bs = '1';
					order->kp = '3';
					order->tradecount = 0;
					m_orderinfo[closeorderref] = *order;
					delete order;
				

					//��¼���ֵĲ�λ
					TPosition * pos = new TPosition;

					m_arPosition.Add(pos);

					strcpy_s(pos->ins,ins);

					pos->count = vol;

					pos->price  = price;

					pos->direction = direction;

					m_listOrderRef.Add(closeorderref);

					sprintf_s(msg,256,"ֹӯ������ �۸�%.3f ����%d orderref=%d ",newprice,vol,closeorderref);

					AddLog(msg);
				}

			}
			else
			{
				sprintf_s(msg,256,"[�ر�]���ֳɽ� �ɽ��۸�%.3f,����%d,��������:%d",price,vol,orderref);

				AddLog(msg);

				if(m_kp==0)
					m_nTradeCountSum += vol;

				double newprice = m_orderinfo[orderref].orderprice - m_J1;

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


				//�����жϳ�ʼƽ�ֵ����ɽ�����
				if( m_nTradeCountSum  >= m_nMaxTradeCount && m_kp==1)
				{
					sprintf_s(msg,256,"�ɽ����� %d ��������%d,�����µ� ",m_nTradeCountSum,m_nMaxTradeCount);

					AddLog(msg);

					return;
				}

				if(m_bRun)
				{
					int closeorderref = CloseBuy(m_strInstrument.GetBuffer(0),vol,newprice,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);

					COrderInfo *order = new COrderInfo();
					strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
					order->ref = closeorderref;
					order->ordercount = vol;
					order->orderprice = newprice;
					order->bs = '0';
					order->kp = '3';
					order->tradecount = 0;
					m_orderinfo[closeorderref] = *order;
					delete order;

					//��¼���ֵĲ�λ
					TPosition * pos = new TPosition;

					m_arPosition.Add(pos);

					strcpy_s(pos->ins,ins);

					pos->count = vol;

					pos->price  = price;

					pos->direction = direction;
				
					m_listOrderRef.Add(closeorderref);

					sprintf_s(msg,256,"ֹӯ������ �۸�%.3f ����%d orderref=%d ",newprice,vol,closeorderref);

					AddLog(msg);
				}

			}

		}
		else//ƽ��
		{
			CalCloseProfit(direction,price,vol);

			//if( m_TradeDirection == 0 )
			if(direction == '1')
			{
				sprintf_s(msg,256,"[�ر�]ƽ�ֳɽ� �ɽ��۸�%.3f,����%d,��������:%d",price,vol,orderref);

				AddLog(msg);

				if(m_kp==1)
					m_nTradeCountSum += vol;

				double newprice = m_orderinfo[orderref].orderprice - m_J1;

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

				//�����жϳ�ʼ���ֵ����ɽ�����
				if( m_nTradeCountSum  >= m_nMaxTradeCount && m_kp==0)
				{
					sprintf_s(msg,256,"�ɽ����� %d ��������%d,�����µ� ",m_nTradeCountSum,m_nMaxTradeCount);

					AddLog(msg);

					return;
				}

				if( m_bRun )
				{
					int orderref = OpenBuy(m_strInstrument.GetBuffer(0),vol,newprice,m_fundAccount.GetBuffer(0));

					m_listOrderRef.Add(orderref);

					COrderInfo *order = new COrderInfo();
					strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
					order->ref = orderref;
					order->ordercount = vol;
					order->orderprice = newprice;
					order->bs = '0';
					order->kp = '0';
					order->tradecount = 0;
					m_orderinfo[orderref] = *order;
					delete order;


					sprintf_s(msg,256,"���ֵ����� �۸�%.2f ����%d orderref=%d ",newprice,vol,orderref);

					AddLog(msg);
				}


			}
			else
			{
				sprintf_s(msg,256,"[�ر�]ƽ�ֳɽ� �ɽ��۸�%.2f,����%d,��������:%d",price,vol,orderref);

				AddLog(msg);

				if(m_kp==1)
					m_nTradeCountSum += vol;

				double newprice = m_orderinfo[orderref].orderprice + m_J1;

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

				//�����жϳ�ʼ���ֵ����ɽ�����
				if( m_nTradeCountSum  >= m_nMaxTradeCount && m_kp==0)
				{
					sprintf_s(msg,256,"�ɽ����� %d ��������%d,�����µ� ",m_nTradeCountSum,m_nMaxTradeCount);

					AddLog(msg);

					return;
				}

				if( m_bRun )
				{
					int orderref = OpenSale(m_strInstrument.GetBuffer(0),vol,newprice,m_fundAccount.GetBuffer(0));

					m_listOrderRef.Add(orderref);

					COrderInfo *order = new COrderInfo();
					strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
					order->ref = orderref;
					order->ordercount = vol;
					order->orderprice = newprice;
					order->bs = '1';
					order->kp = '0';
					order->tradecount = 0;
					m_orderinfo[orderref] = *order;
					delete order;

					sprintf_s(msg,256,"���ֵ����� �۸�%.3f ����%d orderref=%d ",newprice,vol,orderref);

					AddLog(msg);
				}

			}
		}
	}
}



void CStrategyGrid::OnErrorRtn(int code,char * errormsg) 
{
	char msg[128];

	
}

void CStrategyGrid::OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins) 
{
	//����г�����Ҫɾ����Ӧ�ı�������
	if(isExistRef(orderref,bs,kp,ins))
	{
		if(orderstatus=='5' || orderstatus=='6')
		{
			CString s;
			s.Format("[�����ر�] ɾ����Ӧָ��,��������:%d",orderref);
			m_orderinfo.erase(orderref);
			RemoveOrder(orderref);
			AddLog(s.GetBuffer(0));
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

	jiacha = 2 * m_J1;

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
	iter = m_orderinfo.find(ref);
	if(iter!=m_orderinfo.end())
	{
		//�Ȱ�kpȥ��������1 ��3 ��4 Ҳ������
		if( (strcmp(iter->second.szInstrument,ins)==0) && (iter->second.bs==bs) /*&& (iter->second.kp==kp)*/ )
		{
			return true;
		}
	 }
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

	newOrder(addcount,price);

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

				m_listOrderRef.Add(orderref);

				COrderInfo *order = new COrderInfo();
				strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
				order->ref = orderref;
				order->ordercount = count;
				order->tradecount = 0;
				order->orderprice = price;
				order->tradeprice = 0;
				order->bs = '0';
				order->kp = '0';
				order->begin_kp = 0;    //��ʼ�ǿ�
				m_orderinfo[orderref] = *order;
				delete order;

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

				m_listOrderRef.Add(orderref);

				COrderInfo *order = new COrderInfo();
				strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
				order->ref = orderref;
				order->ordercount = count;
				order->tradecount = 0;
				order->orderprice = price;
				order->tradeprice = 0;
				order->bs = '1';
				order->kp = '0';
				order->begin_kp = 0;    //��ʼ�ǿ�
				m_orderinfo[orderref] = *order;
				delete order;

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

	path = path + m_strModelName + ".ini";
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

					m_listOrderRef.Add(ref);

					COrderInfo *order = new COrderInfo();
					strcpy(order->szInstrument,code.GetBuffer(0));
					order->ref = ref;
					order->ordercount = cc;
					order->tradecount = 0;
					order->orderprice = pp;
					order->tradeprice = 0;
					order->bs = '0';
					order->kp = '0';
					m_orderinfo[ref] = *order;
					delete order;
				}
				else if(v4=="1" || v4=="3")
				{
					int ref = CloseBuy(code.GetBuffer(0),cc,pp,account.GetBuffer(0),CLOSEFLAG_CLOSE);

					m_listOrderRef.Add(ref);

					COrderInfo *order = new COrderInfo();
					strcpy(order->szInstrument,code.GetBuffer(0));
					order->ref = ref;
					order->ordercount = cc;
					order->tradecount = 0;
					order->orderprice = pp;
					order->tradeprice = 0;
					order->bs = '0';
					order->kp = '1';
					m_orderinfo[ref] = *order;
					delete order;
				}
			}
			else if(v3=="1")
			{
				if(v4=="0")
				{
					int ref = OpenSale(code.GetBuffer(0),cc,pp,account.GetBuffer(0));

					m_listOrderRef.Add(ref);

					COrderInfo *order = new COrderInfo();
					strcpy(order->szInstrument,code.GetBuffer(0));
					order->ref = ref;
					order->ordercount = cc;
					order->tradecount = 0;
					order->orderprice = pp;
					order->tradeprice = 0;
					order->bs = '1';
					order->kp = '0';
					m_orderinfo[ref] = *order;
					delete order;
				}
				else if(v4=="1" || v4=="3")
				{
					int ref = CloseSale(code.GetBuffer(0),cc,pp,account.GetBuffer(0),CLOSEFLAG_CLOSE);

					m_listOrderRef.Add(ref);

					COrderInfo *order = new COrderInfo();
					strcpy(order->szInstrument,code.GetBuffer(0));
					order->ref = ref;
					order->ordercount = cc;
					order->tradecount = 0;
					order->orderprice = pp;
					order->tradeprice = 0;
					order->bs = '1';
					order->kp = '1';
					m_orderinfo[ref] = *order;
					delete order;
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

	path = path + m_strModelName + ".ini";


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


bool CStrategyGrid::check_b_jihe()
{
	bool ret =false;
	bool ret1 = false;

	string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));

	//if(prefix=="ru" || prefix=="rb" || prefix=="p")
	//{
	//	const time_t t = time(NULL);

	//	struct tm* current_time = localtime(&t);

	//	int h = current_time->tm_hour;
	//	int m = current_time->tm_min;
	//	int s = current_time->tm_sec;

	//	//���Ͼ���֮ǰ�µ����
	//	if( (h==8) && (m<55) )
	//		ret1 =  true;

	//	if( (h==20) && (m<55) )
	//		ret1 =  true;
	//}


	char a = m_codestatus[prefix].status;

	if(a=='0')
		ret =true;

	return (ret|ret1);


}

bool CStrategyGrid::check_in_jihe()
{
	bool ret = false;
	bool ret1 = false;

	string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));

	//if(prefix=="ru" || prefix=="rb" || prefix=="p")
	//{
	//	const time_t t = time(NULL);

	//	struct tm* current_time = localtime(&t);

	//	int h = current_time->tm_hour;
	//	int m = current_time->tm_min;
	//	int s = current_time->tm_sec;

	//	//���Ͼ���֮ǰ�µ����
	//	if( (h==20) && (m>=55) )
	//		ret1 = true;

	//}
	

	char a = m_codestatus[prefix].status;

	if(a=='3' || a=='5')
		ret =true;

	return (ret|ret1);

}


bool CStrategyGrid::checkpanzhong()
{

	bool ret = false;
	bool ret1 = false;

	string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));

	//if(prefix=="ru" || prefix=="rb" || prefix=="p")
	//{
	//	const time_t t = time(NULL);

	//	struct tm* current_time = localtime(&t);

	//	int h = current_time->tm_hour;
	//	int m = current_time->tm_min;
	//	int s = current_time->tm_sec;

	//	if(h==9)
	//		ret1 = true;
	//	else if( (h==10) && ((m<15) || (m>=30)) )
	//		ret1 = true;
	//	else if(h==11 && m <30)
	//		ret1 = true;
	//	else if(h==13 && m >30)
	//		ret1 = true;
	//	else if(h==14)
	//		ret1 = true;
	//	else if(h==21 /*|| h==22 || h==23*/)
	//		ret1 = true;

	//}
	

	char a = m_codestatus[prefix].status;

	if(a=='2')
		ret =true;

	return (ret|ret1);

	//return true;


}

bool CStrategyGrid::checkothertime()
{
	bool ret =false;

	//const time_t t = time(NULL);

	//struct tm* current_time = localtime(&t);

	//int h = current_time->tm_hour;
	//int m = current_time->tm_min;
	//int s = current_time->tm_sec;

	//if(2<=h && h<=7)
	//	return true;
	//else if( h==11 && 30<m && m<=60 )
	//	return true;
	//else if(h==12 )
	//	return true;
	//else if(h==13 && m<30)
	//	return true;
	//else if(h==15 && m>=0)
	//	return true;
	//else if(h>15 && h<20)
	//	return true;


	string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));

	char a = m_codestatus[prefix].status;

	if(a=='1')
		ret =true;

	return ret;


}

bool CStrategyGrid::checksavetime()
{
	bool ret = false;
	bool ret1 = false;

	string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));

	//if(prefix=="ru" || prefix=="rb" || prefix=="p")
	//{
	//	const time_t t = time(NULL);

	//	struct tm* current_time = localtime(&t);

	//	int h = current_time->tm_hour;
	//	int m = current_time->tm_min;
	//	int s = current_time->tm_sec;

	//	if(h==15 && m==1)
	//		ret =  true;
	//}
	

	char a = m_codestatus[prefix].status;

	if(a=='6')
		ret =true;

	return (ret|ret1);

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

	//if( (h==14 || h==20) && (30<m && m<40) )
	//{
	//	ret =  true;
	//}

	return ret;

}

bool CStrategyGrid::checkbefore()
{
	bool ret =false;

	const time_t t = time(NULL);

	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;

	if( (h==8 || h==20) && (m>=53) )
	{
		ret =  true;
	}
	else if(h==9 || h==21)
	{
		ret =  true;
	}
	else if( (h==10) &&(h>=28) )
	{
		ret =  true;
	}
	else if( (h==12) &&(h==58) )
	{
		ret =  true;
	}
	else if( (h==13) &&(h==28) )
	{
		ret =  true;
	}

	return ret;

}

void CStrategyGrid::checkOrder()
{

	//����������Ҫ��˯�ߣ���Ϊ������δ����
	std::this_thread::sleep_for(std::chrono::milliseconds(100));


	while(!m_Exit)
	{
		//BeginRun();
		//return;
		if(m_Exit)
		{
			CString str;

			str.Format("%s:������ֹ���˳�",m_strModelName);

			this->AddLog(str.GetBuffer(0));

			return;
		}

		if(!m_bRun)
		{
			CString str;

			str.Format("%s:������ͣ���ȴ�",m_strModelName);

			this->AddLog(str.GetBuffer(0));

			std::this_thread::sleep_for(std::chrono::milliseconds(3*1000));

			continue;
		
		}

		if(reLogintime())
		{
			//m_SPTrade->relogin();
			::SendMessage(CStrategyBase::m_hMsgWnd,WM_MESSAGE_ENTRUST,3,0);

			std::this_thread::sleep_for(std::chrono::milliseconds(3*1000));
		}


		if(check_in_jihe())
		{

			if(isNewModel)
			{
				this->AddLog("���Ͼ����µ�,�ȴ�����");

				BeginRun();
				isNewModel = false;

				std::this_thread::sleep_for(std::chrono::milliseconds(10*1000));

				continue;
			}


			if(isAnohterDay && !isNewModel && !isLoad)
			{
				this->AddLog("reload...");

				reload();

				isLoad = true;

				isSave = false;

				isAnohterDay = false;

				std::this_thread::sleep_for(std::chrono::milliseconds(10*1000));

			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1*1000));
		}


		if( checkpanzhong() )
		{
			if(isNewModel)
			{
				this->AddLog("������Ӳ���");

				BeginRun();

				isNewModel = false;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(3*1000));
		
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
				this->AddLog("save...");
				saveModel();
				isSave = true;
				isAnohterDay = true;
				isLoad = false;
				this->m_listOrderRef.RemoveAll();
				this->m_orderinfo.clear();

				//if(m_kp==1)
				setHoldinfo();
			}
		
			std::this_thread::sleep_for(std::chrono::milliseconds(10*1000));

		}


		//�ӽ����Ͼ��ۻ��߿���ʱ����Ӳ��ԣ�����100ms������ 8:50����
		if(checkbefore())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			continue;
	
		}


		if(check_b_jihe())
		{
			this->AddLog("���Ͼ���ǰ�µ�,�ȴ�����");

			std::this_thread::sleep_for(std::chrono::milliseconds(3*1000));

			continue;
	
		}

		//һ��ǽ���ʱ��
		if(checkothertime())
		{

			std::this_thread::sleep_for(std::chrono::milliseconds(3*1000));

			//continue;
			
		}

		//û�о�ǿ��˯3��
		string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));
		if(m_codestatus[prefix].status==0)
			std::this_thread::sleep_for(std::chrono::milliseconds(3*1000));


	}


	if(m_Exit)
	{
		CString str;

		str.Format("%s:������ֹ���˳�",m_strModelName);

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

	jiacha = 0;					       //�۲�

	m_fyingkuiClose = 0;

	m_nInstrumentValue = 1;

	m_maxProfit = 0;

	m_fProfitBackStop = 0;

	m_pricebid1 = 0;

	m_priceask1 = 0;

	m_N1 = 0;

	m_kp = 0;

	m_nCount = 0;

	m_strParamList = "���׺�Լ;��Լ����;����������A1;��ʼ�۸�P1;������J1;ÿ��������N1;���׷���;��ƽ����;���ɽ�����;��󳷵�����;��ʼ����;�ʽ�س�;�˻�;";

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

	isNewModel = true;

	isAnohterDay = false;

	isSave = false;

	isLoad = false;

	m_Exit = false;



}


void CStrategyGrid::dealHoldsinfo(int count,double price,int bs)
{

	int yp = 0;
	int p = 0;
	int index = 0;

	CString msg;

	for(int i=0;i<m_holdsinfo.size();i++)
	{
		if (strcmp(m_holdsinfo[i].szInstrument,m_strInstrument)!=0)
			continue;

		if(m_holdsinfo[i].PosiDirection + bs== 1 )
		{
			//yp = m_holdsinfo[i].YdPosition - m_holdsinfo[i].YdFrozen;    //�޷���ȡ�񶳽���򶳽�
			//p = m_holdsinfo[i].TdPosition - m_holdsinfo[i].TdFrozen;
			yp = m_holdsinfo[i].YdAvaiable;
			p = m_holdsinfo[i].TdAvaiable;
			msg.Format("��ǰ���ý��:%d,�������%d...",p,yp);
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
		return;
	}


	if(bs==0)
	{
		if(p>=count)    //��ִ����µ�����
		{
			int orderref = CloseBuy(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);


			m_listOrderRef.Add(orderref);

			COrderInfo *order = new COrderInfo();
			strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
			order->ref = orderref;
			order->ordercount = count;
			order->tradecount = 0;
			order->orderprice = price;
			order->tradeprice = 0;
			order->bs = '0';
			order->kp = '3';
			order->begin_kp = 1;    //��ʼ��ƽ
			m_orderinfo[orderref] = *order;
			delete order;

			m_holdsinfo[index].TdAvaiable = m_holdsinfo[index].TdAvaiable - count;

			msg.Format("ƽ���:%d��,ʣ����:%d��,��������:%d",count,m_holdsinfo[index].TdAvaiable,orderref);
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
				m_listOrderRef.Add(orderref);

				COrderInfo *order = new COrderInfo();
				strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
				order->ref = orderref;
				order->ordercount = p;
				order->tradecount = 0;
				order->orderprice = price;
				order->tradeprice = 0;
				order->bs = '0';
				order->kp = '3';
				order->begin_kp = 1;    //��ʼ��ƽ
				m_orderinfo[orderref] = *order;
				delete order;
			}

			m_holdsinfo[index].TdAvaiable = 0;


			int yorderref=CloseBuy(m_strInstrument.GetBuffer(0),count-p,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE);

			COrderInfo *order = new COrderInfo();
			strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
			order->ref = yorderref;
			order->ordercount = count-p;
			order->tradecount = 0;
			order->orderprice = price;
			order->tradeprice = 0;
			order->bs = '0';
			order->kp = '1';
			order->begin_kp = 1;    //��ʼ��ƽ
			m_orderinfo[yorderref] = *order;
			delete order;

			m_holdsinfo[index].YdAvaiable = m_holdsinfo[index].YdAvaiable - (count-p);

			msg.Format("����ƽ���:%d��,ʣ����:0��,��������:%d;��ƽ�ϲ�:%d��,ʣ���ϲ�:%d��,��������:%d",p,orderref,(count-p),m_holdsinfo[index].YdAvaiable,yorderref);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();
		}
	}
	else if(bs==1)
	{
		if(p>=count)
		{
			int orderref = CloseSale(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);

			COrderInfo *order = new COrderInfo();
			strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
			order->ref = orderref;
			order->ordercount = count;
			order->tradecount = 0;
			order->orderprice = price;
			order->tradeprice = 0;
			order->bs = '1';
			order->kp = '3';
			order->begin_kp = 1;    //��ʼ��ƽ
			m_orderinfo[orderref] = *order;
			delete order;

			m_holdsinfo[index].TdAvaiable = m_holdsinfo[index].TdAvaiable - count;

			msg.Format("ƽ���:%d��,ʣ����:%d��,��������:%d",count,m_holdsinfo[index].TdAvaiable,orderref);
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

				COrderInfo *order = new COrderInfo();
				strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
				order->ref = orderref;
				order->ordercount = p;
				order->tradecount = 0;
				order->orderprice = price;
				order->tradeprice = 0;
				order->bs = '1';
				order->kp = '3';
				order->begin_kp = 1;    //��ʼ��ƽ
				m_orderinfo[orderref] = *order;
				delete order;
			}

			m_holdsinfo[index].TdAvaiable = 0;

			int yorderref =CloseSale(m_strInstrument.GetBuffer(0),count-p,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE);

			COrderInfo *order = new COrderInfo();
			strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
			order->ref = yorderref;
			order->ordercount = count-p;
			order->tradecount = 0;
			order->orderprice = price;
			order->tradeprice = 0;
			order->bs = '1';
			order->kp = '1';
			order->begin_kp = 1;    //��ʼ��ƽ
			m_orderinfo[yorderref] = *order;
			delete order;

			m_holdsinfo[index].YdAvaiable = m_holdsinfo[index].YdAvaiable - (count-p);

			msg.Format("����ƽ���:%d��,ʣ����:0��,��������:%d;��ƽ�ϲ�:%d��,ʣ���ϲ�:%d��,��������:%d",p,orderref,(count-p),m_holdsinfo[index].YdAvaiable,yorderref);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();
		}

	}


}

void CStrategyGrid::setHoldinfo()
{
	CString msg;
	int p = 0;
	int yp = 0;

	for(int i=0;i<m_holdsinfo.size();i++)
	{
		if (strcmp(m_holdsinfo[i].szInstrument,m_strInstrument)!=0)
			continue;

		if(m_holdsinfo[i].PosiDirection == 0 )
		{
			yp = m_holdsinfo[i].YdAvaiable = m_holdsinfo[i].TdAvaiable + m_holdsinfo[i].YdAvaiable;
			p = m_holdsinfo[i].TdAvaiable = 0;
			msg.Format("�̺�,��ֹ��ϲ�,��ǰ���ý��:%d,�������%d...",p,yp);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();

			break;
		}

		if(m_holdsinfo[i].PosiDirection == 1 )
		{
			yp = m_holdsinfo[i].YdAvaiable = m_holdsinfo[i].TdAvaiable + m_holdsinfo[i].YdAvaiable;
			p = m_holdsinfo[i].TdAvaiable = 0;
			msg.Format("�̺�,��ֹ��ϲ�,��ǰ���ý��:%d,�������%d...",p,yp);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();

			break;
		}
	
	}

}
