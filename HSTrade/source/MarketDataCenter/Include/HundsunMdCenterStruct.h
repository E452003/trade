#if !defined(HUNDSUNMDCENTERSTRUCT_H)
#define HUNDSUNMDCENTERSTRUCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PackerVersion 0x20

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcErrorIDType��һ�������������
/////////////////////////////////////////////////////////////////////////
typedef int THundsunFtdcErrorIDType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcErrorMsgType��һ��������Ϣ����
/////////////////////////////////////////////////////////////////////////
typedef char THundsunFtdcErrorMsgType[81];

/////////////////////////////////////////////////////////////////////////
///THundsunExchangeType��Api��������������
/////////////////////////////////////////////////////////////////////////
///�Ϻ�֤ȯ������
#define HUNDSUN_FTDC_SH        1
///����֤ȯ������
#define HUNDSUN_FTDC_SZ        2
//	�Ϻ�������Ȩ
#define HUNDSUN_FTDC_GQ_SH     3
//	���ڸ�����Ȩ
#define HUNDSUN_FTDC_GQ_SZ	   4
///�Ϻ��ڻ�������
#define HUNDSUN_FTDC_SHFE      11
///֣����Ʒ������
#define HUNDSUN_FTDC_CZCE      12
///������Ʒ������
#define HUNDSUN_FTDC_DCE       13
///�й������ڻ�������
#define HUNDSUN_FTDC_CFFEX     14
///�Ϻ�������Դ�������Ĺɷ����޹�˾
#define HUNDSUN_FTDC_INE       15

typedef int THundsunExchangeType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcInstrument��һ����Լ��������
/////////////////////////////////////////////////////////////////////////
typedef char THundsunFtdcInstrument[31];

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcInstrumentName��һ����Լ��������
/////////////////////////////////////////////////////////////////////////
typedef char THundsunFtdcInstrumentName[21];

/////////////////////////////////////////////////////////////////////////
///THundsunMarkerDataType��Api������������
/////////////////////////////////////////////////////////////////////////
///DBF
#define HUNDSUN_FTDC_DBF '1'
///H5
#define HUNDSUN_FTDC_H5 '2'
///Future
#define HUNDSUN_FTDC_Futu '3'
///H5SDK
#define HUNDSUN_FTDC_H5SDK '4'

typedef char THundsunMarkerDataType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcPriceType��һ���۸�����
/////////////////////////////////////////////////////////////////////////
typedef double THundsunFtdcPriceType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcVolumeType��һ����������
/////////////////////////////////////////////////////////////////////////
typedef int THundsunFtdcVolumeType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcVolumeLongType��һ��������������
/////////////////////////////////////////////////////////////////////////
typedef long THundsunFtdcVolumeLongType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcFundAccountType��һ���ʽ��˺�
/////////////////////////////////////////////////////////////////////////
typedef char THundsunFtdcFundAccountType[19];

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcPasswordType��һ����������
/////////////////////////////////////////////////////////////////////////
typedef char THundsunFtdcPasswordType[41];

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcDateType��һ����������
/////////////////////////////////////////////////////////////////////////
typedef int THundsunFtdcDateType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcSessionIDType��һ���Ự�������
/////////////////////////////////////////////////////////////////////////
typedef int THundsunFtdcSessionIDType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcUserTokenType��һ���û���Կ����
/////////////////////////////////////////////////////////////////////////
typedef char THundsunFtdcUserTokenType[512];


///������Ϣ
struct CHundsunFtdcRspInfoField
{
	///�������
    THundsunFtdcErrorIDType   ErrorID; 
	///������ʾ
    THundsunFtdcErrorMsgType  ErrorMsg;
};

///�û���¼����
struct CHundsunFtdcReqUserLoginField
{
	///��������
	THundsunMarkerDataType    MarkerDataType;
	///�ʽ��˺�
	THundsunFtdcFundAccountType FundAccount;
	///����
	THundsunFtdcPasswordType  Password;
};

///�û���¼Ӧ��
struct CHundsunFtdcRspUserLoginField
{
	///������
	THundsunFtdcDateType      TradingDay;
	///��������
	THundsunMarkerDataType    MarkerDataType;
	///�ʽ��˺�
	THundsunFtdcFundAccountType FundAccount;
	///�û���Կ
	THundsunFtdcUserTokenType UserToken;
	///�Ự���
	THundsunFtdcSessionIDType SessionID;
};

///��Լ��ѯ����
struct CHundsunFtdcQryInstrumentField
{
	///��Լ����
	THundsunFtdcInstrument    Instrument;
	///����������
	THundsunExchangeType      ExchangeID;
};

///��Լ
struct CHundsunFtdcInstrumentField
{
	///��Լ����
	THundsunFtdcInstrument    Instrument;
	///����������
	THundsunExchangeType      ExchangeID;
	///��Լ����
	THundsunFtdcInstrumentName InstrumentName;
};

struct CHundsunSpecificInstrumentField
{
	///����������
	THundsunExchangeType      ExchangeID;
	///��Լ����
	THundsunFtdcInstrument    Instrument;
};

struct CHundsunDepthMarketDataField
{
	//���չ�ϵ                                                   �Ϻ�        ����         �ڻ�
	THundsunFtdcVolumeType    UpdateTime;          //����޸�ʱ��                         UpdateTime
	THundsunFtdcVolumeType    UpdateMillisec;      //����޸ĺ���                         UpdateMillisec
	THundsunExchangeType      ExchangeID;          //����������                           ExchangeID
	THundsunFtdcInstrument    Instrument;          //��Լ����    S1          HQZQDM       InstrumentID
	THundsunFtdcInstrumentName InstrumentName;     //��Լ����    S2          HQZQJC       InstrumentName
	THundsunFtdcPriceType     LastPrice;           //���¼�      S8          HQZJCJ       LastPrice
	THundsunFtdcPriceType     PreSettlementPrice;  //�ϴν����                           PreSettlementPrice
	THundsunFtdcPriceType     PreClosePrice;       //�����̼�    S3          HQZRSP       PreClosePrice
	THundsunFtdcPriceType     PreOpenInterest;     //��ֲ���                             PreOpenInterest
	THundsunFtdcPriceType     OpenPrice;           //���̼�    S4          HQJRKP       OpenPrice
	THundsunFtdcPriceType     HighestPrice;        //����߼�    S6          HQZGCJ       HighestPrice
	THundsunFtdcPriceType     LowestPrice;         //����ͼ�    S7          HQZDCJ       LowestPrice
	THundsunFtdcPriceType     Turnover;            //��ɽ����  S5          HQCJJE       Turnover
	THundsunFtdcVolumeLongType Volume;             //�ɽ�����    S11         HQCJSL       Volume
	THundsunFtdcVolumeType    Count;               //�ɽ�����                HQCJBS
	THundsunFtdcPriceType     OpenInterest;        //�ֲ���                               OpenInterest
	THundsunFtdcPriceType     ClosePrice;          //�����̼�                             ClosePrice
	THundsunFtdcPriceType     SettlementPrice;     //���ν����                           SettlementPrice
	THundsunFtdcPriceType     UpperLimitPrice;     //��ͣ���                             UpperLimitPrice
	THundsunFtdcPriceType     LowerLimitPrice;     //��ͣ���                             LowerLimitPrice
	THundsunFtdcPriceType     BuyPrice1;           //��һ��      S9          HQBJW1       BidPrice1
	THundsunFtdcVolumeType    BuyAmount1;          //��һ��      S15         HQBSL1       BidVolume1
	THundsunFtdcPriceType     SalePrice1;          //��һ��      S10         HQSJW1       AskPrice1
	THundsunFtdcVolumeType    SaleAmount1;         //��һ��      S21         HQSSL1       AskVolume1
	THundsunFtdcPriceType     BuyPrice2;           //�����      S16         HQBJW2       BidPrice2
	THundsunFtdcVolumeType    BuyAmount2;          //�����      S17         HQBSL2       BidVolume2
	THundsunFtdcPriceType     SalePrice2;          //������      S22         HQSJW2       AskPrice2
	THundsunFtdcVolumeType    SaleAmount2;         //������      S23         HQSSL2       AskVolume2
	THundsunFtdcPriceType     BuyPrice3;           //������      S18         HQBJW3       BidPrice3
	THundsunFtdcVolumeType    BuyAmount3;          //������      S19         HQBSL3       BidVolume3
	THundsunFtdcPriceType     SalePrice3;          //������      S24         HQSJW3       AskPrice3
	THundsunFtdcVolumeType    SaleAmount3;         //������      S25         HQSSL3       AskVolume3
	THundsunFtdcPriceType     BuyPrice4;           //���ļ�      S26         HQBJW4       BidPrice4
	THundsunFtdcVolumeType    BuyAmount4;          //������      S27         HQBSL4       BidVolume4
	THundsunFtdcPriceType     SalePrice4;          //���ļ�      S30         HQSJW4       AskPrice4
	THundsunFtdcVolumeType    SaleAmount4;         //������      S31         HQSSL4       AskVolume4
	THundsunFtdcPriceType     BuyPrice5;           //�����      S28         HQBJW5       BidPrice5
	THundsunFtdcVolumeType    BuyAmount5;          //������      S29         HQBSL5       BidVolume5
	THundsunFtdcPriceType     SalePrice5;          //�����      S32         HQSJW5       AskPrice5
	THundsunFtdcVolumeType    SaleAmount5;         //������      S33         HQSSL5       AskVolume5
	THundsunFtdcPriceType     BuyPrice6;           //������
	THundsunFtdcVolumeType    BuyAmount6;          //������
	THundsunFtdcPriceType     SalePrice6;          //������
	THundsunFtdcVolumeType    SaleAmount6;         //������
	THundsunFtdcPriceType     BuyPrice7;           //���߼�
	THundsunFtdcVolumeType    BuyAmount7;          //������
	THundsunFtdcPriceType     SalePrice7;          //���߼�
	THundsunFtdcVolumeType    SaleAmount7;         //������
	THundsunFtdcPriceType     BuyPrice8;           //��˼�
	THundsunFtdcVolumeType    BuyAmount8;          //�����
	THundsunFtdcPriceType     SalePrice8;          //���˼�
	THundsunFtdcVolumeType    SaleAmount8;         //������
	THundsunFtdcPriceType     BuyPrice9;           //��ż�
	THundsunFtdcVolumeType    BuyAmount9;          //�����
	THundsunFtdcPriceType     SalePrice9;          //���ż�
	THundsunFtdcVolumeType    SaleAmount9;         //������
	THundsunFtdcPriceType     BuyPrice10;          //��ʮ��
	THundsunFtdcVolumeType    BuyAmount10;         //��ʮ��
	THundsunFtdcPriceType     SalePrice10;         //��ʮ��
	THundsunFtdcVolumeType    SaleAmount10;        //��ʮ��
};

struct CHundsunKLineDataField
{
	THundsunFtdcVolumeType    KLineDate;           //K������
	THundsunFtdcVolumeType    KLineTime;           //K��ʱ���
	THundsunFtdcPriceType     LastPrice;           //���¼�
	THundsunFtdcVolumeType    KLineVolume;         //K�߳ɽ�����
	THundsunFtdcVolumeType    KLineOpenInterest;   //K�ֲ߳�����
	THundsunFtdcPriceType     KLineOpenPrice;      //K�߿���
	THundsunFtdcPriceType     KLineHighestPrice;   //K����߼�
	THundsunFtdcPriceType     KLineLowestPrice;    //K����ͼ�
	THundsunFtdcPriceType     KLineClosePrice;     //K������
	THundsunFtdcPriceType     OpenPrice;           //����
	THundsunFtdcPriceType     HighestPrice;        //��߼�
	THundsunFtdcPriceType     LowestPrice;         //��ͼ�
	THundsunFtdcPriceType     ClosePrice;          //������
};

#endif