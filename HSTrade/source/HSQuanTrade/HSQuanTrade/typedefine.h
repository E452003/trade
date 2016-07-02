#pragma once
#define TRADE_STATUS_INIT 0
#define TRADE_STATUS_BK_SIGNAL 1
#define TRADE_STATUS_BK_TRADE  2
#define TRADE_STATUS_BK_CANCEL 3
#define TRADE_STATUS_SP_SIGNAL 4
#define TRADE_STATUS_SP_TRADE  5
#define TRADE_STATUS_SP_CANCEL 6
#define TRADE_STATUS_SK_SIGNAL 7
#define TRADE_STATUS_SK_TRADE  8
#define TRADE_STATUS_SK_CANCEL 9
#define TRADE_STATUS_BP_SIGNAL 10
#define TRADE_STATUS_BP_TRADE 11
#define TRADE_STATUS_BP_CANCEL 12


#define	WRITE_LOG			CinfoLogger::instance()->LogInfo

#define	STRING_FORMAT		CinfoLogger::instance()->LogInfo

#define	ENTER_FUNC			CinfoLogger::instance()->EnterFunction

#define	LEAVE_FUNC			CinfoLogger::instance()->LeaveFunction

#define	GET_TIME_STRING		CinfoLogger::instance()->GetCurTimeToString


#define TRADE_MSG_TYPE_LOG 0
#define TRADE_MSG_TYPE_LOG_PRV 1
#define TRADE_MSG_TYPE_OTHER 2
#define TRADE_MSG_TYPE_HQ 3
#define TRADE_MSG_TYPE_QUERY 4
#define TRADE_MSG_TYPE_SIG 5
#define TRADE_MSG_TYPE_ARBITRAGE_HQ 6
#define TRADE_MSG_TYPE_POSTION_INFO 7
#define TRADE_MSG_TYPE_CODE_QUERY_DONE 8


//////////////////
///������������
//////////////////
#define ORDER_TYPE_DONOTHING			-1     //�µ��󲻽��ж�������
#define ORDER_TYPE_TIMEOUT_ABORT		0      //�µ���ʱδ�ɽ����߲��ֳɽ���������Ȼ�����
#define ORDER_TYPE_ZUIJIA_NEWPRICE      1      //�µ���δ�ɽ����������¼۵ĶԼ�׷��
#define ORDER_TYPE_ZUIJIA_JIACHA		2      //�µ��󣬵����鲨����ָ���ļ۲��û�гɽ�������׷��


//////////////////
///CTP�ӿ���������
//////////////////
#define POSTION_DIRECTION_BUY '0'
#define POSTION_DIRECTION_SELL '1'

//////////////////
///UFX�ӿ���������
//////////////////
#define UFX_ORDER_BUY '1'
#define UFX_ORDER_SELL '2'

//////////////////
///�ڻ�UFX�ӿڿ�ƽ����
//////////////////
#define UFX_ORDER_OPEN '1'
#define UFX_ORDER_CLOSE '2'
#define UFX_ORDER_CLOSETODAY '4'

//////////////////
///��Ʊ��ȨUFX�ӿڿ�ƽ����
//////////////////
#define UFX_SP_OPEN 'O'
#define UFX_SP_CLOSE 'C'
#define UFX_SP_EXOPT 'X'
#define UFX_SP_AUTOEXOPT 'A'


//////////////////
///�ر�����
//////////////////
#define RTN_TYPE_ORDER   0
#define RTN_TYPE_TRADE   1



//////////////////
///CTP�ӿڿ�ƽ����
//////////////////
#define HS_ORDER_AllTraded '0'   //�ѳ�
#define HS_ORDER_PartTraded '1'	 //����
#define HS_ORDER_Unknown 'a'	 //����
#define HS_ORDER_HASREPORT '3'	 //�ѱ�
#define HS_ORDER_CANCELED '5'	 //�ѳ�



//////////////////
///CTP�ӿڿ�ƽ����
//////////////////
///����
#define HS_THOST_FTDC_OF_Open '0'
///ƽ��
#define HS_THOST_FTDC_OF_Close '1'
///ǿƽ
#define HS_THOST_FTDC_OF_ForceClose '2'
///ƽ��
#define HS_THOST_FTDC_OF_CloseToday '3'
///ƽ��
#define HS_THOST_FTDC_OF_CloseYesterday '4'
///ǿ��
#define HS_THOST_FTDC_OF_ForceOff '5'
///����ǿƽ
#define HS_THOST_FTDC_OF_LocalForceClose '6'


//////////////////
///���׽ӿ�����
//////////////////
#define TRADE_CTP		0
#define TRADE_UFX_F		1
#define TRADE_UFX_S		2
#define TRADE_UFX_AUTO	3
#define TRADE_UFX_OPT	4



struct CTickData
{
	CTickData()
	{
		memset(szInstrument, 0, 32);
		lastprice = 0.0;
	}
	char szDate[9];
	char szTime[32];
	char szInstrument[32];
	double askprice[5];
	double bidprice[5];
	int   askvol[5];
	int   bidvol[5];	
	double lastprice;				//���¼�
	double UpperLimitPrice;			//��ͣ��
	long  Volume;					//�ɽ���
	double highestprice;			//��߼�
	double preClosePrice;			//������
	double LowerLimitPrice;			//��ͣ��

};


//ָ����Ϣ
struct COrderInfo
{
	char szInstrument[32]; 
	int ref;
	double orderprice;
	double tradeprice;
	int tradecount;
	int ordercount;
	int withdrawcount;
	char bs;
	char kp;
	char selfclose;
	//������Ϣ
	//�����������ͣ�
	int ordertype;
	int timeOut;
	double jiacha;
	char account[20];
};

struct StockInfo
{
	char code[31];				//��Լ����
	char  Name[9];				//֤ȯ����
	long  J_start;				//��������
	double ActiveCapital;		//��ͨ�ɱ�
	int exchid;                 //������id 
};

//�ڻ���Լ
struct FutuCodeInfo
{
	char code[31];				//��Լ����
	char exchid[9];				//����������
	char name[21];				//��Լ����
	int iVolumeMultiple;		//��Լ����
	double LMarginRatio;		//��ͷ��֤��
	double SMarginRatio;		//��ͷ��֤��
	double PriceTick;			//��С�۲�

};


struct ArbitrageCodeInfo
{
	char sCode[31];				//��Լ����
	char sExchid[9];			//����������
	
	int iMinTradeVol;
	int iMaxTradeVol;


};

//��Ʊ��Ȩ
struct StockOpt
{
	char code[31];
	int exchid;
	char name[20];
	int type;					//���ǿ���

};


struct CHoldsinfo
{
	char szInstrument[32]; 
	int PosiDirection;			//���
	int YdPosition;				//���
	int TdPosition;				//���
	int Position;				//�ֲܳ�
	int LongFrozen;				//��ͷ����
	int ShortFrozen;			//��ͷ����
	int TdFrozen;				//�񶳽�
	int YdFrozen;				//�򶳽�
	int TdAvaiable;				//�����
	int YdAvaiable;				//�����
	double dTradePrice;			//�ɱ���

};

//ʵʱ�ֲ֣�����ƽ��ӯ��
struct CHoldsEx
{	
	char szInstrument[32]; 
	int LongPosition;			//���
	int ShortPosition;			//�ղ�
	double dLongTradePrice;		//��ɱ���
	double dShortTradePrice;	//��ɱ���
	double dLongMargin;			//��ͷ��֤��
	double dShortMargin;		//��ͷ��֤��

	CHoldsEx()
	{
		memset(this,0,sizeof(CHoldsEx));
	}

};


//��Լ״̬��Ϣ
struct CCodeStatus
{
	char szInstrument[32];
	char status;
	char entertime[9];

};

//ί�С���ѯ��Ϣ
struct CEntrustInfo
{
	char *account;
	char szInstrument[32];
	int entrustno;
	int status;
	char bs;
	char kp;
	int orderAmount;
	int tradeAmount;
	int cancelAmount;
};

//K������
struct CKStrdata
{
#ifdef _DEBUG

	~CKStrdata()
	{
		int i = 0;
	}
#endif
	int    type; 
//	char   code[31];
	//char   name[15];
	double priceH;
	double priceK;
	double priceL;
	double priceS;
	double vol;
	char    curDate[9];
	char   curTime[5];


};
