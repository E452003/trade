#include "h5sdkSpi.h"
#include "HundsunH5sdkApi.h"
#include "public.h"

void ShowList()
{
	cerr<<"------------------------------------------------"<<endl;
	cerr<<" [0]  ��ʾĿ¼                                  "<<endl;
	cerr<<"------------------------------------------------"<<endl;
	cerr<<" [1]  ��ѯ��Լ��Ϣ                              "<<endl;
	cerr<<" [2]  ��ѯ�г���Ϣ                              "<<endl;
	cerr<<" [3]  ��������                                  "<<endl;
	cerr<<" [10] ��ѯ�������                              "<<endl;
	cerr<<" [20] ��ѯK�߷���                               "<<endl;
	cerr<<" [99]  Exit                                      "<<endl;
	cerr<<"------------------------------------------------"<<endl;
}

int main()
{
	char HqTypeCode[255];
	char ProdCode[255];
	char QueryDate[255];
	int CandlePeriod;
	char HSExchangeType[255];

	CHundsunH5sdkApi* lpUserApi = CHundsunH5sdkApi::CreateHundsunH5sdkApi();
	CH5sdkSpi* lpUserSpi = new CH5sdkSpi(lpUserApi);
	lpUserApi->RegisterSpi(lpUserSpi);
	lpUserApi->RegisterFront("112.124.211.5:9999");
	lpUserApi->Init();

	ShowList();

	while(1)
	{
		int cmd;  
		cin>>cmd;

		if(lpUserSpi->GetQueryStatus())
		{
			cerr<<"���ڲ�ѯ...����!"<<endl;
			continue;
		}

		switch(cmd){
			case 0:
				ShowList();
				break;
			case 1:
				lpUserSpi->ReqQryInstrument();
				break;
			case 2:
				lpUserSpi->ReqQryMarket();
				break;
			case 3:
				lpUserSpi->ReqH5sdkSubscribe();
				break;
			case 10:
				lpUserSpi->ReqH5sdkSnapshot();
				break;
			case 20:
				cerr<<" ���棺�벻Ҫ�ڽ���ʱ���ڵ����ݣ� "<<endl;
				system_pause();

				cerr<<" �����뽻�������룺HqTypeCode "<<endl;
				cerr<<"     ֤ȯ�г�����֤-->XSHG ��֤-->XSHE"<<endl;
				cerr<<"     �ڻ��г����н�-->CCFX ����-->XSGE ����-->XDCE ֣��-->XZCE"<<endl;
				cin>>HqTypeCode; 

				cerr<<" �������Լ���룺ProdCode "<<endl;
				cin>>ProdCode; 

				cerr<<" ������K�����ͣ�CandlePeriod "<<endl;
				cerr<<"     1����K��-->1 "<<endl;
				cerr<<"     5����K��-->2 "<<endl;
				cerr<<"     15����K��-->3 "<<endl;
				cerr<<"     30����K��-->4 "<<endl;
				cerr<<"     60����K��-->5 "<<endl;
				cerr<<"     ��K��-->6 "<<endl;
				cerr<<"     ��K��-->7 "<<endl;
				cerr<<"     ��K��-->8 "<<endl;
				cerr<<"     ��K��-->9 "<<endl;
				cin>>CandlePeriod; 

				cerr<<" �����뿪ʼ��ѯ��־��QueryDate����ʽ��19700101�� "<<endl;
				cin>>QueryDate; 

				cerr<<" HqTypeCode = "<<HqTypeCode<<" ProdCode = "<<ProdCode<<" CandlePeriod = "<<CandlePeriod<<" QueryDate = "<<QueryDate<<endl;
				system_pause();

				lpUserSpi->SetHqTypeCode(HqTypeCode);
				lpUserSpi->SetProdCode(ProdCode);
				lpUserSpi->SetCandlePeriod(CandlePeriod);
				lpUserSpi->SetQueryDate(QueryDate);

				hs_strncpy(HSExchangeType, GetHSExchagneType(HqTypeCode), sizeof(HSExchangeType));

				cerr<<" ����ɾ�������ļ�... "<<endl;
				char path[1024];
				mkdirFullPath("./Data");
				sprintf(path,"./Data/%s.txt", ProdCode);
				DeleteFile(path);

				cerr<<" ��ʼ������... "<<endl;
				lpUserSpi->ReqH5sdkCandleByRange();
				lpUserSpi->SetQueryStatus(true);
				break;
			case 99: exit(0);
		}
	}

	lpUserApi->ReleaseApi();

	return 0;
}