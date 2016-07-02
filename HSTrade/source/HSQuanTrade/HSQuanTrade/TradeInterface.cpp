#include "stdafx.h"

#include "TradeInterface.h"

#include "UFXTradeEx.h"

#include "UFXTrade.h"

#include "CTPTrade.h"

#include "FunctionPub.h"

#include "Order.h"

#include "tool.h"

#include "StragegyManager.h"

#include "StrategyBase.h"

#include "SocketTrade.h"

#include "UFXOptTradeApi.h" 

#include "COrderEx.h"



CTradeInterface::CTradeInterface(void)
{

	m_iOrderRef = Tool::getDayofWeek() * 10000;

	m_bStop = false;

	m_tTrd = thread(std::bind(&CTradeInterface::DealOrder, this));

	m_hThread = (HANDLE)_beginthreadex(NULL,0,ThreadRtnProcess,(void*)this,0,NULL);

}

 unsigned int _stdcall CTradeInterface::ThreadRtnProcess(void * pt)
 {
	 CTradeInterface * pTrade = (CTradeInterface *)pt;

	while(!pTrade->m_bStop)
	{
		 pTrade->m_lockRtn.Lock();

		 int size = pTrade->m_arListRtn.GetSize();

		 pTrade->m_lockRtn.UnLock();

		 if( size == 0 )
		 {
			Sleep(1);
			continue;
		 }

		  pTrade->m_lockRtn.Lock();

		 OrderRtn * pData = (OrderRtn *) pTrade->m_arListRtn[0];

		 pTrade->m_arListRtn.RemoveAt(0);

		 pTrade->m_lockRtn.UnLock();

		 switch(pData->returntype)
		 {
		 case RTN_TYPE_TRADE:
			 {
				 pTrade->RtnTradeEx(pData->account,pData->orderref,pData->ordersysid,pData->price,pData->offsetflag,pData->direction,pData->tradetime,pData->count,pData->instrument);
			 }
			 break;
		 case RTN_TYPE_ORDER:
			 {
				 pTrade->RtnOrderEx(pData->orderref,pData->ordersysid,pData->status,pData->direction,pData->offsetflag,pData->instrument);
			 }
			 break;
		 default:
			 break;
		 }

		 delete pData;

		 pData = NULL;
	}

	 return 0;
 }

CTradeInterface::~CTradeInterface(void)
{
	m_bStop = true;
	m_tTrd.join();
	Exit();

	WaitForSingleObject(m_hThread,INFINITE);

	CloseHandle(m_hThread);
}


//�����ʺ��ҵ���ӦAPI��Ȼ�����InsertOrder
int CTradeInterface::InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int ordertype,int timeout,double jiacha,char * account)
{
	int ref = -3333;

	if(strcmp("600570",instrument)==0)
	{
		return 0;
	}
	
	for(int i=0;i<m_arApiList.GetSize();i++)
	{
		CBaseTradeApi *api = (CBaseTradeApi *)m_arApiList[i];

		if( strcmp(api->m_szUserID,account) ==  0)
		{
			//��ؼ��
			if(!api->CheckRisk(instrument,count,price,direction,offsetflag,0))
			{
				return -1;
			}


			m_iOrderRef ++;

			//����ctp��ufx�ڻ�ref ���Ǳ������ã�����֤ȯufx���ֻ��T2ͨѶ�������,ͨ��Ͷ��Ӯ�ҽ������û��
			//֤ȯ������batch_no
			//��Ʊ��ȨҲ���õ�batch_no
			ref = api->InsertOrder(instrument,count,direction,offsetflag,price,m_iOrderRef);

			api->AddOrder(m_iOrderRef);

			//WRITE_LOG(1,"InsertOrder");

			//��������
			if (ordertype != ORDER_TYPE_DONOTHING )
			{				
				COrder * porder = new COrder(account,instrument,count,direction,offsetflag,price,ordertype,timeout,jiacha,api,this,ref);

				//m_LockOrder.lock();

				m_arListOrder.Add(porder);

				/*m_LockOrder.unlock();*/
			}

			break;

			
		}
	}
	return ref;
}


int CTradeInterface::InsertOrderGroup(char * instrument, int count, char direction, char offsetflag, double price,int ordertype,int timeout,double jiacha,char * account,int group)
{
	int ref = 0;

	if(strcmp("600570",instrument)==0)
		return 0;
	
	for(int i=0;i<m_arApiList.GetSize();i++)
	{
		CBaseTradeApi *api = (CBaseTradeApi *)m_arApiList[i];

		if( strcmp(api->m_szUserID,account) ==  0)
		{
			
			m_iOrderRef ++;

			ref = api->InsertOrder(instrument,count,direction,offsetflag,price,m_iOrderRef);

			//WRITE_LOG(1,"InsertOrder");

			//��������
			if (ordertype != ORDER_TYPE_DONOTHING)
			{	
				COrderGroup *orderGrp = NULL;
				for(int k=0; k < m_arListOrderGroup.GetSize(); k++)
				{
					COrderGroup *Grp = (COrderGroup *)m_arListOrderGroup[k];
					if(Grp->GetGrpId() == group)
					{
						orderGrp = Grp;
					}
				}

				if(!orderGrp)
				{
					orderGrp = new COrderGroup(group);

					m_arListOrderGroup.Add(orderGrp);
				}

				COrderEx *porder = new COrderEx(account,instrument,count,direction,offsetflag,price,ordertype,timeout,jiacha,api,this,ref);
				
				//ͬһ����ϵ����
				if(orderGrp->GetGrpId() == group)
				{					
					orderGrp->AddOrder(ref,porder);
				}
				
			}

			break;

			
		}
	}


	return ref;
}

//����
int CTradeInterface::WithDraw(char*account,int entrustno,char *sCode)
{
	for(int i=0;i<m_arApiList.GetSize();i++)
	{
		CBaseTradeApi * api = (CBaseTradeApi *)m_arApiList[i];

		if( strcmp(api->m_szUserID,account) ==  0)
		{
			if(!api->CheckRisk(sCode,0,0,'0','0',1))
			{
				return -1;
			}

			api->WithDraw(entrustno,sCode);

		}
	}
	return 0;

}


void CTradeInterface::QueryPosition(char *account)
{
	for(int i=0;i<m_arApiList.GetSize();i++)
	{
		CBaseTradeApi * api = (CBaseTradeApi *)m_arApiList[i];

		if( strcmp(api->m_szUserID,account) ==  0)
		{		
			api->QueryPosition(account);

		}
	}

}


void CTradeInterface::QueryEntrust(char*account,int entrustno)
{
	for(int i=0;i<m_arApiList.GetSize();i++)
	{
		CBaseTradeApi * api = (CBaseTradeApi *)m_arApiList[i];

		if( strcmp(api->m_szUserID,account) ==  0)
		{		
			api->QueryEntrust(entrustno,entrustno);

		}
	}

}


void CTradeInterface::QueryTrade(char*account)
{
	for(int i=0;i<m_arApiList.GetSize();i++)
	{
		CBaseTradeApi * api = (CBaseTradeApi *)m_arApiList[i];

		if( strcmp(api->m_szUserID,account) ==  0)
		{		
			//api->QueryPosition(account);

		}
	}

}


void CTradeInterface::QueryFund(char*account)
{
	for(int i=0;i<m_arApiList.GetSize();i++)
	{
		CBaseTradeApi * api = (CBaseTradeApi *)m_arApiList[i];

		if( strcmp(api->m_szUserID,account) ==  0)
		{		
			//api->QueryPosition(account);
			api->QueryFund(account);

		}
	}

}


int CTradeInterface::OpenAccount(CString account,  CString pwd,CString serverip, CString apitype,CString brokeid)
{
	CFunctionPub fun;

	if( apitype == "CTP")
	{

		CBaseTradeApi * pApi = new CCTPTrade();

		fun.Copy(pApi->data.m_szUserID,16,account);

		pApi->SetInterface(this);

		pApi->data.m_hMsgWnd = this->m_hMsgWnd;

		pApi->m_hMsgWnd = m_hMsgWnd;

		fun.Copy(pApi->m_szBroker,16,brokeid);

		fun.Copy(pApi->m_szUserID,16,account);

		fun.Copy(pApi->m_szPwd,16,pwd);

		fun.Copy(pApi->m_szTradeServer,256,serverip);

		m_arApiList.Add(pApi);

		pApi->init(serverip);
	}
	else if( apitype == "UFX_F" )
	{
		CBaseTradeApi * pApi = new CUFXTradeEx();

		fun.Copy(pApi->data.m_szUserID,16,account);

		pApi->SetInterface(this);

		pApi->data.m_hMsgWnd = this->m_hMsgWnd;

		pApi->m_hMsgWnd = m_hMsgWnd;

		fun.Copy(pApi->m_szBroker,16,brokeid);

		fun.Copy(pApi->m_szUserID,16,account);

		fun.Copy(pApi->m_szPwd,16,pwd);

		fun.Copy(pApi->m_szTradeServer,256,serverip);

		m_arApiList.Add(pApi);

		pApi->init(serverip);
	}
	else if(apitype == "UFX_S")
	{
				
		CBaseTradeApi * pApi = new CSECUTrade();

		fun.Copy(pApi->data.m_szUserID,16,account);

		pApi->SetInterface(this);

		pApi->data.m_hMsgWnd = this->m_hMsgWnd;

		pApi->m_hMsgWnd = m_hMsgWnd;

		fun.Copy(pApi->m_szBroker,16,brokeid);

		fun.Copy(pApi->m_szUserID,16,account);

		fun.Copy(pApi->m_szPwd,16,pwd);

		fun.Copy(pApi->m_szTradeServer,256,serverip);

		m_arApiList.Add(pApi);

		pApi->init(serverip);
	
	}
	else if(apitype == "UFX_AUTO")
	{
				
		CBaseTradeApi * pApi = new CSocketTrade();

		fun.Copy(pApi->data.m_szUserID,16,account);

		pApi->SetInterface(this);

		pApi->data.m_hMsgWnd = m_hMsgWnd;

		pApi->m_hMsgWnd = m_hMsgWnd;

		fun.Copy(pApi->m_szBroker,16,brokeid);

		fun.Copy(pApi->m_szUserID,16,account);

		fun.Copy(pApi->m_szPwd,16,pwd);

		fun.Copy(pApi->m_szTradeServer,256,serverip);

		m_arApiList.Add(pApi);

		pApi->init(serverip);
	
	}
	else if(apitype == "UFX_OPT")
	{
		CBaseTradeApi *pApi = new CUFXOptTradeApi();

		fun.Copy(pApi->data.m_szUserID,16,account);

		pApi->SetInterface(this);

		pApi->data.m_hMsgWnd = this->m_hMsgWnd;

		pApi->m_hMsgWnd = m_hMsgWnd;

		fun.Copy(pApi->m_szBroker,16,brokeid);

		fun.Copy(pApi->m_szUserID,16,account);

		fun.Copy(pApi->m_szPwd,16,pwd);

		fun.Copy(pApi->m_szTradeServer,256,serverip);

		m_arApiList.Add(pApi);

		pApi->init(serverip);

	
	}

	return 0;
}


//�����˻������Ϣ
void CTradeInterface::SetRiskInfo(char* account,double margin,int count,double pl,int open)
{
	for(int i=0;i<m_arApiList.GetSize();i++)
	{
		CBaseTradeApi * api = (CBaseTradeApi *)m_arApiList[i];

		if( strcmp(api->m_szUserID,account) ==  0)
		{		
			api->SetRiskInfo(margin,count,pl,open);
		}
	}

}



void CTradeInterface::init()
{		

	m_bEnd = true;

	m_iRestLength = 0;

	buf = "";

	InitListen();


}


void CTradeInterface::RtnTrade(char *account,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins)
{
	//�ѻر������б�
	OrderRtn * pt = new OrderRtn;

	m_lockRtn.Lock();

	m_arListRtn.Add(pt);

	pt->returntype = RTN_TYPE_TRADE;

	pt->orderref = orderref;

	pt->ordersysid = ordersysid;

	pt->price = price;

	pt->count = vol;

	pt->direction = direction;

	pt->offsetflag = offsetflag;

	strcpy(pt->instrument,ins);

	strcpy(pt->tradetime,tradetime);

	strcpy(pt->account,account);
	
	m_lockRtn.UnLock();
	
	CMediaAux::PlayWavSound(E_HS_SOUND_TRADE_RTN);
	
}

void CTradeInterface::RtnTradeEx(char *account,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins)
{
#ifdef _DEBUG
	char msg[256];
	sprintf(msg,"�ɽ�  %d",orderref),
	WRITE_LOG(1,msg);
#endif


	COrder *tempOrder = NULL;

	m_strManger->m_LockOrder.lock();

	for(int i=0;i<m_arListOrder.GetSize();i++)
	{
		COrder * order = (COrder *)m_arListOrder[i];

		if(orderref==order->m_nOrderRef && ordersysid == order->m_nOrderSysID  )//����m_nOrderSysID�ıȽϣ�ctp��ͬ�����ӱ���orderref����һ��������UFX�ӿ���Ҫ���м��ݴ���
		{
			//order->RtnTrade(orderref,ordersysid,price,offsetflag,direction,tradetime,vol,ins);
			tempOrder = order;

			break;
		}


	}
	m_strManger->m_LockOrder.unlock();

	//��ʱtemp
	if(tempOrder)
		tempOrder->RtnTrade(orderref,ordersysid,price,offsetflag,direction,tradetime,vol,ins);

	//������
	for(int i=0;i<m_strManger->m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_strManger->m_arModelRun[i];
		
		pt->OnTradeReturn(ins,tradetime,orderref,ordersysid,direction,offsetflag,vol,price);
	}

	//��������
	for(int i=0;i<m_arListOrderGroup.GetSize();i++)
	{
		COrderGroup * grp = (COrderGroup *)m_arListOrderGroup[i];

		//if(orderref==grp->m_nOrderRef)
		{
			grp->RtnTrade(orderref,ordersysid,price,offsetflag,direction,tradetime,vol,ins);

			//break;
		}
	}

	//��������Ϣ
	for(int i=0;i<m_arApiList.GetSize();i++)
	{
		CBaseTradeApi *api = (CBaseTradeApi*)m_arApiList[i];
		if(strcmp(api->m_szUserID,account)==0 && api->ExistOrder(orderref))
		{	
			api->UpdateRiskInfo(ins,vol,price,direction,offsetflag);
		}
	}
}

void CTradeInterface::RtnOrder(int orderref,int ordersysid,char status,char bs ,char kp,char * ins)
{
	//�ѻر������б�
	OrderRtn * pt = new OrderRtn;

	m_lockRtn.Lock();

	m_arListRtn.Add(pt);

	pt->returntype = RTN_TYPE_ORDER;

	pt->orderref = orderref;

	pt->ordersysid = ordersysid;

	pt->status = status;

	pt->direction = bs;

	pt->offsetflag = kp;
	
	strcpy(pt->instrument,ins);
		
	m_lockRtn.UnLock();

	if('5' == status) //cancel
	{
		CMediaAux::PlayWavSound(E_HS_SOUND_TRADE_CANCEL);
	}
}

void CTradeInterface::RtnOrderEx(int orderref,int ordersysid,char status,char bs ,char kp,char * ins)
{	

#ifdef _DEBUG
	char msg[256];
	sprintf(msg,"ί�� %d",orderref),
	WRITE_LOG(1,msg);
#endif


	COrder *tempOrder = NULL;

	m_strManger->m_LockOrder.lock();
	for(int i=0;i<m_arListOrder.GetSize();i++)
	{
		COrder * order = (COrder *)m_arListOrder[i];

		if(orderref==order->m_nOrderRef)
		{
			//order->RtnOrder(orderref, ordersysid,status,bs,kp,ins);
			tempOrder = order;
			break;
		}
	}
	m_strManger->m_LockOrder.unlock();

	//��ʱtemp
	if(tempOrder)
		tempOrder->RtnOrder(orderref, ordersysid,status,bs,kp,ins);

	//������
	for(int i=0;i<m_strManger->m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_strManger->m_arModelRun[i];
		
		pt->OnOrderReturn(orderref, ordersysid,status,bs,kp,ins);
	}

	for(int i=0;i<m_arListOrderGroup.GetSize();i++)
	{
		COrderGroup * grp = (COrderGroup *)m_arListOrderGroup[i];

		//if(orderref==grp->m_nOrderRef)
		{
			grp->RtnOrder(orderref, ordersysid,status,bs,kp,ins);

			//break;
		}
	}
	
}


void CTradeInterface::OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)
{
	m_strManger->m_LockOrder.lock();
    //��Ҫ��������
	for(int i=0;i<m_arListOrder.GetSize();i++)
	{
			COrder * order = (COrder *)m_arListOrder[i];

			if(order->m_nOrderRef == ref)
			{	     
				order->OnRspOrderInsert(ref,entrustno,entrust_status,errorid);

				break;
			}	
	}
	m_strManger->m_LockOrder.unlock();

	//ͬʱ��Ҫ�����ԣ���ֹ�µ�ʧ���쳣
	for(int i=0;i<m_strManger->m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_strManger->m_arModelRun[i];
		
		pt->OnRspOrderInsert(ref,entrust_status,errorid);
	}


	//��Ҫ��������
	for(int i=0;i<m_arListOrderGroup.GetSize();i++)
	{
		COrderGroup * grp = (COrderGroup *)m_arListOrderGroup[i];

		//if(grp->m_nOrderRef == ref)
		{	     
			grp->OnRspOrderInsert(ref,entrustno,entrust_status,errorid);

			//break;
		}	
	}

	return;

}




void CTradeInterface::OnRspWithdraw(int ref)
{

}


CString  CTradeInterface:: getAccount()
{
	CString accountInfo = "";
	for(int i=0;i<m_arApiList.GetSize();i++)
	{	
		CBaseTradeApi * api = (CBaseTradeApi *)m_arApiList[i];

		accountInfo = accountInfo + api->m_szUserID + ",";

	}
	return accountInfo;

	
}



void CTradeInterface::DealOrder()
{

	std::this_thread::get_id();

	int count = 0;

	char msg[128];

	//��¼����ʱ��
	time_t rawtime;
	time ( &rawtime );
	long lastcheck  = (long)rawtime;

	while (!m_bStop)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1*1000));

		if(m_arListOrder.GetSize()!=0)
		{
			time_t rawtime;
			time ( &rawtime );
			long now = (long)rawtime;

			if(now - lastcheck < 15)
				continue;
		}
		else
			continue;

		m_strManger->m_LockOrder.lock();

#ifdef _DEBUG
		char temp[128];
		sprintf(temp,"DealOrder lock");
		WRITE_LOG(1,temp);
#endif 

		for(int i=0;i<m_arListOrder.GetSize();i++)
		{
			COrder *order = (COrder *)m_arListOrder[i];
			if(order)
			{
				if(order->m_bDone)          //����������״̬ ɾ������
				{
					count++;	
#ifdef _DEBUG
					sprintf(msg,"������ֹ  %d  %d",order->m_nOrderRef,order->m_nOrderSysID);
					WRITE_LOG(1,msg);
#endif 
				}
				//�����ѯһ��״̬
				else
				{
					/*int ref = order->m_nOrderRef;
					time_t rawtime;
					time ( &rawtime );
					long timestamp = (long)rawtime;
					
					if(timestamp - order->m_timestamp > 30)
						QueryEntrust(order->m_account,ref);*/

				}
			}

		}
 
		//ɾ���б�
		for(int i = 0; i< count; i++)
		{
			for(int k=0;k<m_arListOrder.GetSize();k++)
			{
				COrder *order = (COrder *)m_arListOrder[k];
			 
				if( (order) && (order->m_bDone) )          //����������״̬ ɾ������
				{

#ifdef _DEBUG
					sprintf(msg,"ɾ������  %d  %d",order->m_nOrderRef,order->m_nOrderSysID);
					WRITE_LOG(1,msg);
#endif 
					delete order;						 //�ͷ��ڴ�

					order = NULL;

					m_arListOrder.RemoveAt(k);

					break;	
				}

			}
		}

		count = 0;

		m_strManger->m_LockOrder.unlock();

#ifdef _DEBUG
		sprintf(temp,"DealOrder unlock");
		WRITE_LOG(1,temp);
#endif 
		time_t rawtime;
		time ( &rawtime );
		lastcheck = (long)rawtime;

		
	}



}


void CTradeInterface::OrderTick(CTickData *pd)
{
	//��������Ϊ1�Ķ���
	m_strManger->m_LockOrder.lock();
	for(int i =0;i<m_arListOrder.GetSize();i++)
	{
		COrder *order = (COrder *)m_arListOrder[i];

		if(strcmp(order->m_szIntrument,pd->szInstrument)==0)
		{
			order->DealPrice(pd->bidprice[0],pd->askprice[0]);
		}

	}
	m_strManger->m_LockOrder.unlock();


	//�������
	for(int i =0;i<m_arListOrderGroup.GetSize();i++)
	{
		COrderGroup *grp = (COrderGroup *)m_arListOrderGroup[i];

		//if(strcmp(grp->m_szIntrument,pd->szInstrument)==0)
		{
			grp->Tick(pd);
		}

	}


}


void CTradeInterface::Login()
{
	for(int i=0;i<m_arApiList.GetSize();i++)
	{	
		CBaseTradeApi * api = (CBaseTradeApi *)m_arApiList[i];

		if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(api))
		{
			c->Login();
		}

	}

}


void CTradeInterface::LogOut()
{
	for(int i=0;i<m_arApiList.GetSize();i++)
	{	
		CBaseTradeApi * api = (CBaseTradeApi *)m_arApiList[i];

		if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(api))
		{
			c->LogOut();
		}

	}

}


void CTradeInterface::SetAccountInfo(char *account,bool isQueryHold)
{
	for(int i=0;i<m_arApiList.GetSize();i++)
	{	
		CBaseTradeApi * api = (CBaseTradeApi *)m_arApiList[i];

		if(strcmp(api->m_szUserID,account)==0)
		{

			if(isQueryHold)
			{
				api->data.m_bReady = false;
				//��Ҫ�����������Ҫ��ѯ�³ֲ�
				QueryPosition(account);
			}
			else
			{
				api->data.m_bReady = true;
			}

			m_strManger->SetAccountInfo(account,&(api->data) );

			break;
		}

	}

}



//socket�ӿ�
//
bool CTradeInterface::InitSock()
{
	if (!m_ServerSock.CreateSockListen())
	{
		return FALSE;
	}
	//if (!m_ServerSock.Bind())
	//{
	//	return FALSE;
	//}
	if (!m_ServerSock.Bind(m_port))
	{
		return FALSE;
	}
	if (!m_ServerSock.Listen(1))
	{
		return FALSE;
	}
	return TRUE;

}


void CTradeInterface::InitListen()
{

	// TODO: ���������߳�
	if (!InitSock())
	{
		CTime tm = CTime::GetCurrentTime();
		CString strTime = tm.Format("%Y-%m-%d %H:%M:%S");
		CString str;
		str.Format(_T("%s    ��������ʧ�ܣ�\n"), strTime);
		return;
	}
	if (WSAAsyncSelect(m_ServerSock.GetSockListen(), m_hMsgWnd, WM_MESSAGE_SOCK, FD_ACCEPT | FD_CLOSE) == 0)
	{
		CTime tm = CTime::GetCurrentTime();
		CString strTime = tm.Format("%Y-%m-%d %H:%M:%S");
		CString str;
		str.Format(_T("%s    ��������ɹ���\n"), strTime);
		//isStart= true;
	}

}

//socket�ӿ�


void CTradeInterface::Exit()
{

	for(int i=0;i<m_arApiList.GetSize();i++)
	{
		CBaseTradeApi * pApi = (CBaseTradeApi *)m_arApiList[i];
		delete pApi;
		pApi = NULL;
	}
	m_arApiList.RemoveAll();

	m_strManger->m_LockOrder.lock();
	for(int i=0;i<m_arListOrder.GetSize();i++)
	{
		COrder * order = (COrder *)m_arListOrder[i];
		//m_arListOrder.RemoveAt(i);
		delete order;
		order = NULL;
	}
	m_arListOrder.RemoveAll();

	m_strManger->m_LockOrder.unlock();

}


void CTradeInterface::getQueryInfo(char* buf,char *account)
{

	for(int i=0;i<m_arApiList.GetSize();i++)
	{
		CBaseTradeApi * pApi = (CBaseTradeApi *)m_arApiList[i];

		if(strcmp(pApi->m_szUserID,account)==0)
		{
			Tool::TransRspQuery(buf,account,&pApi->data);
			//string s(buf);
			//WRITE_LOG(1,s);
		}
	}

}