#include "stdafx.h"
#include "mdSpi.h"
#include "StragegyManager.h"
#include "StrategyBase.h"
#include "typedefine.h"
#include "TradeInterface.h"
#include "Order.h"


bool CMdSpi::IsErrorRspInfo(CHundsunFtdcRspInfoField *pRspInfo)
{
	bool ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (ret)
	{
		//printf("IsErrorRspInfo\n");
		cerr<<" ��Ӧ | "<<pRspInfo->ErrorMsg<<endl;
		m_MdStrategy->Log(pRspInfo->ErrorMsg);
	}
	return ret;

}

void CMdSpi::OnFrontConnected()
{
	m_MdStrategy->Log("[����]���ӳɹ�...\n");
	CTime tmNow = CTime::GetCurrentTime();
	CString strTime = tmNow.Format("%Y%m%d");
	strcpy(m_szDate, strTime);
	SubscribeMarketData();
	
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	m_MdStrategy->Log("[����]���ӶϿ�...\n");
}

void CMdSpi::OnRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CString log = pSpecificInstrument->Instrument;
	log = "UFX:"+log + "���ĳɹ�";
	//m_MdStrategy->Log("xxxxx");
	if(!IsErrorRspInfo(pRspInfo))
	{
		if(pSpecificInstrument)
		{
			m_MdStrategy->Log(log.GetBuffer(0));
			cout<<"["<<pSpecificInstrument->Instrument<<"] ���ĳɹ�"<<endl;
		}
	}
	
}

///ȡ����������Ӧ��
void CMdSpi::OnRspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CString log = pSpecificInstrument->Instrument;
	log = "UFX:"+log + "ȡ�����ĳɹ�";
	//m_MdStrategy->Log("xxxxx");
	if(!IsErrorRspInfo(pRspInfo))
	{
		if(pSpecificInstrument)
		{
			m_MdStrategy->Log(log.GetBuffer(0));
			cout<<"["<<pSpecificInstrument->Instrument<<"] ȡ�����ĳɹ�"<<endl;
		}
	}

}



void CMdSpi::OnRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData)
{
	cout<<"["<<pDepthMarketData->Instrument<<"] LastPrice:"<<pDepthMarketData->LastPrice<<" BuyPrice1:"<<pDepthMarketData->BuyPrice1<<endl;
	
	//m_MdStrategy->Log(pDepthMarketData->Instrument);
	CTickData  pd ;
	memset(&pd, 0, sizeof(CTickData));
	
	strcpy(pd.szInstrument, pDepthMarketData->Instrument);
	//strcpy(pd.szTime, intt(pDepthMarketData->UpdateTime));
	itoa(pDepthMarketData->UpdateTime,pd.szTime,10);
	strcpy(pd.szDate, m_szDate);
	//int x = pDepthMarketData->UpdateMillisec;
	pd.bidprice[0] = pDepthMarketData->BuyPrice1;
	pd.bidprice[1] = pDepthMarketData->BuyPrice2;
	pd.bidprice[2] = pDepthMarketData->BuyPrice3;
	pd.bidprice[3] = pDepthMarketData->BuyPrice4;
	pd.bidprice[4] = pDepthMarketData->BuyPrice5;
	pd.bidvol[0] = pDepthMarketData->BuyAmount1;
	pd.bidvol[1] = pDepthMarketData->BuyAmount2;
	pd.bidvol[2] = pDepthMarketData->BuyAmount3;
	pd.bidvol[3] = pDepthMarketData->BuyAmount4;
	pd.bidvol[4] = pDepthMarketData->BuyAmount5;

	pd.askprice[0] = pDepthMarketData->SalePrice1;
	pd.askprice[1] = pDepthMarketData->SalePrice2;
	pd.askprice[2] = pDepthMarketData->SalePrice3;
	pd.askprice[3] = pDepthMarketData->SalePrice4;
	pd.askprice[4] = pDepthMarketData->SalePrice5;
	pd.askvol[0] = pDepthMarketData->SaleAmount1;
	pd.askvol[1] = pDepthMarketData->SaleAmount2;
	pd.askvol[2] = pDepthMarketData->SaleAmount3;
	pd.askvol[3] = pDepthMarketData->SaleAmount4;
	pd.askvol[4] = pDepthMarketData->SaleAmount5;

	pd.UpperLimitPrice = pDepthMarketData->UpperLimitPrice;
	pd.LowerLimitPrice = pDepthMarketData->LowerLimitPrice;
	pd.Volume = pDepthMarketData->Volume;
	pd.lastprice = pDepthMarketData->LastPrice;
	pd.highestprice = pDepthMarketData->HighestPrice;
	pd.preClosePrice = pDepthMarketData->PreClosePrice;
	
	if(m_MdStrategy)
	{
		/*
		SYSTEMTIME st;
		GetLocalTime(&st);
		int m_ihour = st.wHour;
		int m_iMinutes = st.wMinute;
		int m_iSeconds = st.wSecond;
		int m_Milliseconds = st.wMilliseconds;		


		char time[24];
		sprintf(time,"����%d:%d:%d:%d",m_ihour,m_iMinutes,m_iSeconds,m_Milliseconds);
		strcpy(pd.time,time);*/
		m_MdStrategy->ModelTick(&pd);

		//��������Ϊ1�Ķ���
		m_MdStrategy->m_pTradeInterface->OrderTick(&pd);

		//�Ƿ�������Ȩ����
		std::string strCode(pd.szInstrument);
		if((strCode[0] == '1' && strCode.length() >6) || (strcmp("510050",pd.szInstrument)==0) )
		{
			string strTime = pd.szTime;
			int iTime =atoi(pd.szTime);
			if(iTime==93000 || iTime==93001)
				return;

#if 0
			char test[128];
			sprintf(test,"��Ȩʱ�� %s",pd.szTime);
			WRITE_LOG(1,test);

			//string heyue = "sp rb1609&rb1701";
			//int index = heyue.find("&");

			//string substr = heyue.substr(index+1);

#endif

			char msg[256];
			sprintf_s(msg,256,"TICK#%s;%.4f;%.4f;%d;%.4f;%d;%s",pd.szInstrument,pd.lastprice,pd.bidprice[0],pd.bidvol[0],pd.askprice[0],pd.askvol[0],pd.szTime);

			::SendMessage(m_MdStrategy->m_hMsgWnd,WM_MESSAGE_TRADE,3,(long)msg);
		}


	}

	
}

void CMdSpi::SubscribeMarketData()
{
	//lpUserApi->SubscribeMarketData(1,m_ppInstrumentID,m_nInsNum);

	char* Instrument[1];
	//Instrument[0]="131810";
	//lpUserApi->SubscribeMarketData(2,Instrument,1);

	//char* Instrument[1];
	std::map<string,int>::iterator itcode;
	for( itcode = m_UfxCode.begin(); itcode != m_UfxCode.end(); itcode++)
	{
		int exchid = itcode->second;
		Instrument[0] = (char*)itcode->first.c_str();
		lpUserApi->SubscribeMarketData(exchid,Instrument,sizeof(Instrument)/sizeof(char*));
	}


}

void CMdSpi::OnRspQryInstrument(CHundsunFtdcInstrumentField *pInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	if(pInstrument)
	{
		cout<<pInstrument->ExchangeID<<"|"<<pInstrument->Instrument << "|" <<  pInstrument->InstrumentName<<endl;
		StockOpt opt;
		strcpy(opt.code,pInstrument->Instrument);
		opt.exchid = pInstrument->ExchangeID;
		strcpy(opt.name,pInstrument->InstrumentName);
		m_MdStrategy->m_pGetCodeInfo->m_PubData.m_mapOptCode[pInstrument->Instrument] = opt;

	}

	if(bIsLast)
	{
		cout<<"��Լ��ѯ�ɹ�"<<endl;
		std::map<std::string,StockOpt>::iterator it = m_MdStrategy->m_pGetCodeInfo->m_PubData.m_mapOptCode.begin();
		string opt = "QUERYOPT#";
		for(;it != m_MdStrategy->m_pGetCodeInfo->m_PubData.m_mapOptCode.end(); it++)
		{
			string code = it->first;
			string name = it->second.name;
			code.append(";") .append(name).append(";#");

			opt.append(code);
		}

		::SendMessage(m_MdStrategy->m_hMsgWnd,WM_MESSAGE_TRADE,4,(long)opt.c_str());

	}

	

}
