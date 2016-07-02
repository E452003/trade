#include "HundsunMdCenterApi.h"
#include "mdSpi.h"

int main()
{
	//H5 && Futu һ����
	CHundsunMdCenterApi* lpUserMdApi = CHundsunMdCenterApi::CreateHundsunMdCenterApi();
	CMdSpi* lpUserMdSpi = new CMdSpi(lpUserMdApi);
	lpUserMdApi->RegisterSpi(lpUserMdSpi);

	//ע��֤ȯ
	//lpUserMdApi->RegisterDBFPath("D:/dbf");
	lpUserMdApi->RegisterH5("h5apilicense.xml", "h5config_lv2.xml", true);

	//ע���ڻ�
	lpUserMdApi->RegisterFutu("license/yaqh_hs_sim_2.0/license.dat");
	lpUserMdApi->RegisterFront("115.238.53.140:9106");

	//��ʼ��
	//lpUserMdApi->Init(HUNDSUN_FTDC_DBF);
	lpUserMdApi->Init(HUNDSUN_FTDC_H5);
	lpUserMdApi->Init(HUNDSUN_FTDC_Futu);

	char* Instrument[1];

	cerr<<"------------------------------------------------"<<endl;
	cerr<<" [1]  ����֤ȯ                                  "<<endl;
	cerr<<" [2]  �����ڻ�                                  "<<endl;
	cerr<<" [0]  Exit                                      "<<endl;
	cerr<<"------------------------------------------------"<<endl;

	while(1)
	{
		int cmd;  
		cin>>cmd;
		switch(cmd){
		case 1:
			Instrument[0] = "600570";
			lpUserMdApi->SubscribeMarketData(HUNDSUN_FTDC_SH, Instrument, sizeof(Instrument)/sizeof(char*));
			break;
		case 2:
			Instrument[0] = "IF1511";
			lpUserMdApi->SubscribeMarketData(HUNDSUN_FTDC_CFFEX, Instrument, sizeof(Instrument)/sizeof(char*));
			break;
		case 0: exit(0);
		}
	}
	lpUserMdApi->ReleaseApi();

	return 0;
}