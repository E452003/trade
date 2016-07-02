#include "mdSpi.h"

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

void CMdSpi::OnFrontConnected()
{
	printf("[����]���ӳɹ�...\n");
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	printf("[����]���ӶϿ�...\n");
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
	cout<<"��10�ۣ�"<<pDepthMarketData->SalePrice10<<"  ��10����"<<pDepthMarketData->SaleAmount10/100<<endl;
	cout<<"��9�ۣ�"<<pDepthMarketData->SalePrice9<<"    ��9����"<<pDepthMarketData->SaleAmount9/100<<endl;
	cout<<"��8�ۣ�"<<pDepthMarketData->SalePrice8<<"    ��8����"<<pDepthMarketData->SaleAmount8/100<<endl;
	cout<<"��7�ۣ�"<<pDepthMarketData->SalePrice7<<"    ��7����"<<pDepthMarketData->SaleAmount7/100<<endl;
	cout<<"��6�ۣ�"<<pDepthMarketData->SalePrice6<<"    ��6����"<<pDepthMarketData->SaleAmount6/100<<endl;
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
	cout<<"��6�ۣ�"<<pDepthMarketData->BuyPrice6<<"    ��6����"<<pDepthMarketData->BuyAmount6/100<<endl;
	cout<<"��7�ۣ�"<<pDepthMarketData->BuyPrice7<<"    ��7����"<<pDepthMarketData->BuyAmount7/100<<endl;
	cout<<"��8�ۣ�"<<pDepthMarketData->BuyPrice8<<"    ��8����"<<pDepthMarketData->BuyAmount8/100<<endl;
	cout<<"��9�ۣ�"<<pDepthMarketData->BuyPrice9<<"    ��9����"<<pDepthMarketData->BuyAmount9/100<<endl;
	cout<<"��10�ۣ�"<<pDepthMarketData->BuyPrice10<<"  ��10����"<<pDepthMarketData->BuyAmount10/100<<endl;
}
