#include "mdSpi.h"
#include "DBSpi.h"

bool CMdSpi::IsErrorRspInfo(CHundsunFtdcRspInfoField *pRspInfo)
{
	bool ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (ret)
	{
		printf("IsErrorRspInfo\n");
		cerr<<" ��Ӧ | "<<pRspInfo->ErrorMsg<<endl;
	}
	return ret;
}

void CMdSpi::SubAll()
{
	char* Instrument[1];
	char sTemp[1024] = {};
	map<pair<int, string>, CHundsunFtdcInstrumentField*>::iterator it;
	for(it = m_InstrumentMap.begin(); it != m_InstrumentMap.end(); it++)
	{
		memcpy(sTemp, it->second->Instrument, 2);

		if(it->second->ExchangeID = HUNDSUN_FTDC_SH)
		{	
			if(strcmp(sTemp, "60") != 0)
				continue;

			Instrument[0] = it->second->Instrument;
			lpUserApi->SubscribeMarketData(it->second->ExchangeID, Instrument, sizeof(Instrument)/sizeof(char*));
		}
		else if(it->second->ExchangeID = HUNDSUN_FTDC_SZ)
		{
			if(!(strcmp(sTemp, "00") == 0 || strcmp(sTemp, "30") == 0))
				continue;

			Instrument[0] = it->second->Instrument;
			lpUserApi->SubscribeMarketData(it->second->ExchangeID, Instrument, sizeof(Instrument)/sizeof(char*));
		}
	}
}

CHundsunFtdcInstrumentField* CMdSpi::GetInstrument(THundsunExchangeType ExchangeID, THundsunFtdcInstrument pszInstrument)
{
	pair<int, string> pr(ExchangeID, pszInstrument);
	map<pair<int, string>, CHundsunFtdcInstrumentField*>::iterator it = m_InstrumentMap.find(pr);
	if( it != m_InstrumentMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void CMdSpi::UpdateInstrument(CHundsunFtdcInstrumentField *pInstrument)
{
	CHundsunFtdcInstrumentField* lpInstrument = GetInstrument(pInstrument->ExchangeID, pInstrument->Instrument);
	if(!lpInstrument)
	{
		lpInstrument = new CHundsunFtdcInstrumentField();
		pair<int, string> pr(pInstrument->ExchangeID, pInstrument->Instrument);
		m_InstrumentMap[pr] = lpInstrument;
	}
	memcpy(lpInstrument, pInstrument, sizeof(CHundsunFtdcInstrumentField));
}

void CMdSpi::OnFrontConnected()
{
	printf("[����]���ӳɹ�...\n");
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	printf("[����]���ӶϿ�...\n");
}

void CMdSpi::OnRspUserLogin(CHundsunFtdcRspUserLoginField *pRspUserLogin, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		cout<<pRspUserLogin->MarkerDataType<<"|"<<pRspUserLogin->FundAccount<<"��¼�ɹ�..."<<endl;
	}
}

void CMdSpi::OnRspQryInstrument(CHundsunFtdcInstrumentField *pInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pInstrument)
	{
		UpdateInstrument(pInstrument);
		cout<<pInstrument->ExchangeID<<"|"<<pInstrument->Instrument << "|" <<  pInstrument->InstrumentName<<endl;

		if( strcmp(pInstrument->Instrument,"10000380")==0 )
		{
			int i = 0;
			i++;
		}
	}



	if(bIsLast)
		cout<<"��Լ��ѯ�ɹ�"<<endl;
}

void CMdSpi::OnRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		if(pSpecificInstrument)
			cout<<"["<<pSpecificInstrument->Instrument<<"] ���ĳɹ�"<<endl;
	}
}

void CMdSpi::OnRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData)
{
	cout<<"["<<pDepthMarketData->Instrument<<"] LastPrice:"<<pDepthMarketData->LastPrice<<endl;
	cout<<"��5�ۣ�"<<pDepthMarketData->SalePrice5<<"    ��5����"<<pDepthMarketData->SaleAmount5/100<<endl;
	cout<<"��4�ۣ�"<<pDepthMarketData->SalePrice4<<"    ��4����"<<pDepthMarketData->SaleAmount4/100<<endl;
	cout<<"��3�ۣ�"<<pDepthMarketData->SalePrice3<<"    ��3����"<<pDepthMarketData->SaleAmount3/100<<endl;
	cout<<"��2�ۣ�"<<pDepthMarketData->SalePrice2<<"    ��2����"<<pDepthMarketData->SaleAmount2/100<<endl;
	cout<<"��1�ۣ�"<<pDepthMarketData->SalePrice1<<"    ��1����"<<pDepthMarketData->SaleAmount1/100<<endl;
	cout<<"��1�ۣ�"<<pDepthMarketData->BuyPrice1<<"    ��1����"<<pDepthMarketData->BuyAmount1/100<<endl;
	cout<<"��2�ۣ�"<<pDepthMarketData->BuyPrice2<<"    ��2����"<<pDepthMarketData->BuyAmount2/100<<endl;
	cout<<"��3�ۣ�"<<pDepthMarketData->BuyPrice3<<"    ��3����"<<pDepthMarketData->BuyAmount3/100<<endl;
	cout<<"��4�ۣ�"<<pDepthMarketData->BuyPrice4<<"    ��4����"<<pDepthMarketData->BuyAmount4/100<<endl;
	cout<<"��5�ۣ�"<<pDepthMarketData->BuyPrice5<<"    ��5����"<<pDepthMarketData->BuyAmount5/100<<endl;

	if(lpUserDBSpi)
	{
		lpUserDBSpi->MarkdataInsert(pDepthMarketData);
	}
}
