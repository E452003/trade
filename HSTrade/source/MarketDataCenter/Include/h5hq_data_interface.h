#ifndef _H5HQ_DATA_INTERFACE_H__
#define _H5HQ_DATA_INTERFACE_H__

#include "h5hq_const_define.h"


///	�г�״̬
enum enumMktStatu
{
	MKT_LEVEL2_LOGIN_OTHER_ERROR		=-4,//	Level2��¼�����������û������뵽�ڵȣ������h5api��־ȷ��
	MKT_LEVEL2_USER_MAX_ONLINE_LIMIT	=-3,//	Level2�����û�������������
	MKT_LEVEL2_USER_INFO_ERROR			=-2,//	Level2��¼ʧ��: �û����������
	MKT_FAIL		=-1,//	�г�������(���г���Ӧ�����ӱ��ж�,������MKT_CREATED)
	MKT_CREATED		=0,	//	�г��ոմ���,��δ����
	MKT_CONNECTING	=1,	//	���ڴ�������
	MKT_CONNECTED	=2,	//	���г���Ӧ�������Ѵ���
	MKT_LOGINED		=3, //	�г��ѵ�¼
	MKT_INITIALIZING=4,	//	�г����ڳ�ʼ����  
	MKT_GETTINGOVERVIEW=5,	//	ȡ�г�ȫ������
	MKT_INITIALIZED	=6,	//	�г��ѳ�ʼ��, (����״̬)
	MKT_MAX_COUNT,		//	�г�״̬����������ҵ���壬һ�����ڶ������鳤��
};

enum enumExpireStatus
{
	ES_FAIL = 0,		// ��Ȩ���󡣱�ʾ����Ȩ�����г����˴���
	ES_OK = 1,			// ��Ȩ�ɹ���δ�������뵽���ջ��нϳ�ʱ��
	ES_TO_EXPIRE = 2,	// ��Ȩ�ɹ�����Ҫ���ڵ���δ����
	ES_EXPIRED = 3,		// ��Ȩʧ�ܡ��Ѿ�����
	ES_MAX_COUNT,		// ���״̬��������ҵ���壬һ�����ڶ������鳤��
};

// SetApiParam�ӿڵĲ���ö������
enum enumApiParamType
{
	APT_LEVEL2_USER_INFO = 0,	// Level2�û���������Ϣ����Ӧ�����ṹ����: LEVEL2_USER_INFO
};

// ��ӦAPT_LEVEL2_USER_INFO����
typedef struct tagLevel2UserInfo
{
	char	szUserName[48];
	char	szUserPassword[48];
}LEVEL2_USER_INFO, *LP_LEVEL2_USER_INFO;


class IStockInfo;
////////////////	���¸�������Ԫ���ݶ���(Ŀǰ�����峣�õļ���,������)	////////////////////////////
//	�г���ʼ����Ϣ,��Ʊ�ľ�̬��Ϣ,��ʼ���󲻻��ٱ仯
class IStockInitInfo : public IHSKnown
{
public:
	virtual const char* FUNCTION_CALL_MODE GetStockCode() = 0;
	virtual const char* FUNCTION_CALL_MODE GetStockName() = 0;
	virtual double		FUNCTION_CALL_MODE GetPrevPrice() = 0;		//	���������
	/*
	*����״̬�ֶο������Ϊһ����̬���ֶΣ���һ�������ȡ���ֶε���ʼ����Ϣ��ȥȡ��
	*�����ֲ���ȫ��һ����̬�ֶΣ����Ƶ�ʱ����ֶ��뵽ISimpleQuote�ӿ���ȥȡ��
	*��������Ƶ����ȥISimpleQuote�ӿ�ȥ���ᴥ��h5api�����������ȥ����ʵʱ���ݡ�
	*���������ƻ�������ʵʱ���ݺ�Ҳ��ͬ���Գ�ʼ��������Ľ���״̬�ֶν��и��£�
	*Ҳ����˵���ۺ�ʲô����£���IStockInitInfo��ISimpleQuote�ӿ�ȡ����״̬����һ�µ�
	*/
	virtual int			FUNCTION_CALL_MODE GetStockStatus() = 0;	//	��ý���״̬: 0:���� 1:ͣ��
	virtual int			FUNCTION_CALL_MODE GetCodeType() = 0;	
	virtual int			FUNCTION_CALL_MODE GetMkt() = 0;	

	virtual ~IStockInitInfo();
};

//	���ض��г������ķ�������Ϣ
typedef struct tagSvrAddress
{
	char	szIPAddress[16];
	uint16	wPort;
}SVR_ADDRESS, *LPSVR_ADDRESS;


//	�г���Ҫ��Ϣ,��Ӧ���˽⵱ǰ�г���ҵ����Ϣ,�Լ��ṩ�г���Ϣ�ķ�������ַ��Ϣ
class IMarketOverview : public IHSKnown
{
public:
	virtual enumMktStatu FUNCTION_CALL_MODE GetMktStatus() = 0;	//	����г�״̬	0:	�г���Ϣ��δ���,Ҳ����˵APIĿǰ��û�и��г���Ϣ,���ܲ������Ե�
	virtual int FUNCTION_CALL_MODE GetMarketType() = 0;	//	����г�����
	virtual int FUNCTION_CALL_MODE GetChecksum() = 0;	//	�г���ϢУ���(��ʱδʵ��),Ӧ�ñ����жϵ�ǰУ����Ƿ�һ��
	virtual int FUNCTION_CALL_MODE GetTradeDate() = 0;	//	��ý���������ʱ��֧��
	virtual int FUNCTION_CALL_MODE GetMktTime() = 0;	//	��õ�ǰ����ʱ��

	//	����ϼ���������Ϣ
	virtual int FUNCTION_CALL_MODE GetMktSvrCount() = 0;//	��ø��г���Ӧ���ϼ�����������
	virtual LPSVR_ADDRESS FUNCTION_CALL_MODE GetMktSvrInfo(int iIndex) = 0;	//	����г���Ӧ�ķ�������ַ

	//	��ó�ʼ�������
	virtual int FUNCTION_CALL_MODE GetTotalCount() = 0;	//	��Ʊ�������
	virtual IStockInitInfo* FUNCTION_CALL_MODE GetStockInitInfo(int iIndex) = 0; //	����г��µڼ�����Ʊ�ĳ�ʼ����Ϣ

	virtual ~IMarketOverview() = 0;
};

//	������ʱ
struct TrendUnit	//	��ʱ��Ԫ
{
	double dfLastPrice;		//	���¼�
	double dfAveragePrice;	//	����
	uint64 ddwTotal;		//	�ɽ���
};

class ISimpleTrend : public IHSKnown
{
public:
	//	�������з�ʱ�߷�ʱ��,iCountָ���ж��ٸ�
	/************************************************************************/
	/* ���� iCount����ǰ���շ�ʱ����Ŀ                                                                     */
	/************************************************************************/
	virtual TrendUnit* FUNCTION_CALL_MODE GetTrend(int& iCount) = 0;	//	��÷�ʱ��

	//	�����ȡ��ʱ��
	virtual TrendUnit* FUNCTION_CALL_MODE GetTrendByIndex(int iIndex) = 0;	//	��õڼ�����ʱ��

	virtual ~ISimpleTrend() = 0;
};

//	��������(���ߵ���)
class ISimpleQuote : public IHSKnown
{
public:
	/*
	*����״̬�ֶο������Ϊһ����̬���ֶΣ���һ�������ȡ���ֶε���ʼ����Ϣ��ȥȡ��
	*�����ֲ���ȫ��һ����̬�ֶΣ����Ƶ�ʱ����ֶ��뵽ISimpleQuote�ӿ���ȥȡ��
	*��������Ƶ����ȥISimpleQuote�ӿ�ȥ���ᴥ��h5api�����������ȥ����ʵʱ���ݡ�
	*���������ƻ�������ʵʱ���ݺ�Ҳ��ͬ���Գ�ʼ��������Ľ���״̬�ֶν��и��£�
	*Ҳ����˵���ۺ�ʲô����£���IStockInitInfo��ISimpleQuote�ӿ�ȡ����״̬����һ�µ�
	*/
	virtual int    FUNCTION_CALL_MODE GetStockStatus() = 0;		//	��ù�Ʊ״̬: 0:���� 1:ͣ��
	virtual double FUNCTION_CALL_MODE GetOpenPrice() = 0;		//	��ÿ��̼�
	virtual double FUNCTION_CALL_MODE GetHighPrice() = 0;		//	�����߼�
	virtual double FUNCTION_CALL_MODE GetLowPrice() = 0;		//	�����ͼ�
	virtual double FUNCTION_CALL_MODE GetLastPrice() = 0;		//	������¼�
	virtual double FUNCTION_CALL_MODE GetPrevPrice() = 0;		//	������ռ�
	virtual double FUNCTION_CALL_MODE GetUpPrice() = 0;			//	�����ͣ��
	virtual double FUNCTION_CALL_MODE GetDownPrice() = 0;		//	��õ�ͣ��
	virtual double FUNCTION_CALL_MODE GetVolum() = 0;			//	��óɽ���
	virtual long   FUNCTION_CALL_MODE GetAmount() = 0;			//	��óɽ���,�ɽ���Ϊ��С��λ
	virtual int   FUNCTION_CALL_MODE GetTimes() = 0;			//	��õ�ǰ������
	virtual int   FUNCTION_CALL_MODE GetSecond() = 0;			//	��õ�ǰ�����ڵڼ���

	virtual double   FUNCTION_CALL_MODE GetPreJieSuanPrice() = 0;			//	���ǰ����ۣ����ڻ��͸�����Ȩʹ�ã�
	virtual long   FUNCTION_CALL_MODE GetPosition() = 0;		// ��ȡ�ֲ��������ڻ��͸�����Ȩʹ�ã�

	virtual int	   FUNCTION_CALL_MODE GetHand() = 0;			// ���ڹ�ƱΪÿ�ֹ����������ڻ�Ϊ��Լ������

	virtual unsigned long FUNCTION_CALL_MODE GetTotalBuy() = 0;	// ��������Ŀǰ��֧����Ϻ�Լ
	virtual unsigned long FUNCTION_CALL_MODE GetTotalSell() = 0;// ��������Ŀǰ��֧����Ϻ�Լ

	//2013��11��27��	caich	��Ӹ�����Ȩ�����ֶ�
	virtual double   FUNCTION_CALL_MODE GetAutionPrice() = 0;			//	��ö�̬�ο��ۣ�����Ȩ��

	virtual unsigned int   FUNCTION_CALL_MODE GetMatchVolume() = 0;			//	�������ƥ������������Ȩ��

	//2014��3��21�� caich ȡ��δƽ�ֺ�Լ���·�
	//virtual unsigned int   FUNCTION_CALL_MODE GetTotalLongPosition() = 0;			//	���δƥ������������Ȩ��
	
	virtual const char * FUNCTION_CALL_MODE GetTradingphasecode()=0;	//��ȡ��Ʒʵʩ��־������Ȩ��4���ֽڣ�

	virtual double   FUNCTION_CALL_MODE GetAvgPrice() = 0;			//	��þ��ۣ����ڻ���

	virtual double   FUNCTION_CALL_MODE GetJieSuanPrice() = 0;			//	��ý���ۣ����ڻ��͸�����Ȩʹ�ã�

	virtual ~ISimpleQuote() = 0;
};

//	������λ��Ϣ
struct LevelInfo
{
	double 	price;		//	��
	long	Amount;		//	��
};

class IBidOffer : public IHSKnown
{
public:	
	virtual int FUNCTION_CALL_MODE GetLevels() = 0;							//	��õ�λ����
	virtual LevelInfo *FUNCTION_CALL_MODE GetBidInfo(int iLevelNo) = 0;		//	��õڼ����򵵵���Ϣ
	virtual LevelInfo *FUNCTION_CALL_MODE GetOfferInfo(int iLevelNo) = 0;	//	��õڼ�����������Ϣ

	virtual ~IBidOffer() = 0;
};


// NEEQͶ���ߵ�λ
struct NEEQZSLevel
{
	double m_lfPrice;		// ί�м۸�
	uint64 m_nSize;			// ί������
};

//	��ת����Ͷ���ߵ�λ����
class INEEQZSLevelQuote : public IHSKnown
{
public:
	virtual uint32 FUNCTION_CALL_MODE GetTimestamp() = 0;						//	��ȡ����ʱ���
	virtual int   FUNCTION_CALL_MODE GetTimes() = 0;							//	��õ�ǰ������
	virtual int   FUNCTION_CALL_MODE GetSecond() = 0;							//	��õ�ǰ�����ڵڼ���
	virtual int FUNCTION_CALL_MODE GetLevels() = 0;								//	��õ�λ����
	virtual NEEQZSLevel *FUNCTION_CALL_MODE GetBidLevelInfo(int iLevelNo) = 0;	//	��õڼ����򵵵���Ϣ
	virtual NEEQZSLevel *FUNCTION_CALL_MODE GetOfferLevelInfo(int iLevelNo) = 0;//	��õڼ�����������Ϣ
	
	virtual ~INEEQZSLevelQuote() = 0;
};

// NEEQ�������걨
struct NEEQZSOrder
{
	double m_lfPrice;		// �걨�۸�
	uint64 m_nSize;			// �걨����
	uint32 m_nTimestamp;	// �걨ʱ��
};

//	��ת�����������걨����
class INEEQZSOrderQuote : public IHSKnown
{
public:
	virtual uint32 FUNCTION_CALL_MODE GetTimestamp() = 0;						//	��ȡ����ʱ���
	virtual int   FUNCTION_CALL_MODE GetTimes() = 0;							//	��õ�ǰ������
	virtual int   FUNCTION_CALL_MODE GetSecond() = 0;							//	��õ�ǰ�����ڵڼ���
	virtual int FUNCTION_CALL_MODE GetBidOrderCount() = 0;						//	����������
	virtual NEEQZSOrder *FUNCTION_CALL_MODE GetBidOrderInfo(int iIndex) = 0;	//	��õڼ����򵵵���Ϣ
	virtual int FUNCTION_CALL_MODE GetOfferOrderCount() = 0;					//	�����������
	virtual NEEQZSOrder *FUNCTION_CALL_MODE GetOfferOrderInfo(int iIndex) = 0;	//	��õڼ�����������Ϣ
	
	virtual ~INEEQZSOrderQuote() = 0;
};

struct NEEQXYOrder
{
	double			m_fOrderPrice;			//�걨�۸�
	uint64			m_nOrderVolume;			//�걨����
	uint32			m_nDealNO;				//�ɽ�Լ����
	uint32			m_nOrderTime;			//�걨ʱ��
	uint32			m_nStatus;				//��¼״̬
	uint32			m_nSellSide;			//��������
	char			m_cJYDY[6];				//ҵ��Ԫ
	char			m_cReserved[1];			//���ñ�־λ
};

//��תЭ��ת���걨����
class INEEQXYOrderQuote : public IHSKnown
{
public:
	virtual uint32 FUNCTION_CALL_MODE GetTimestamp() = 0;						//��ȡ����ʱ���
	virtual int	FUNCTION_CALL_MODE GetAnsType() = 0;							//��ȡ��ǰ�����Ӧ�����������0:��ʾ����δ����;1:��ʾ������;2:��ʾ������
	virtual int	FUNCTION_CALL_MODE GetOrderCount() = 0;							//��ȡ��ǰ�걨������
	virtual NEEQXYOrder *FUNCTION_CALL_MODE GetXYOrderInfo(int iIndex) = 0;		//��õڼ����걨����

	virtual ~INEEQXYOrderQuote() = 0;
};


enum enumKType
{
	KTYPE_UNKNOWN=0,	//	δ֪
	KTYPE_MINUTE1,		//	1������
	KTYPE_MINUTE5,		//	5������
	KTYPE_MINUTE15,		//	15������
	KTYPE_MINUTE30,		//	30������
	KTYPE_MINUTE60,		//	60������
	KTYPE_MINUTE120,	//	120������
	KTYPE_DAY,			//	����
	KTYPE_WEEK,			//	����
	KTYPE_MONTH,		//	����
	KTYPE_MAX			//	�������
};

//	K��
struct KInfo
{
	/************************************************************************/
	/* K��������ʱ��˵����
	K�����ڣ����ڷ������ݰ�����������
	�������ڴ��ڵ����յ�k��ʱ���ʽΪYYYYMMDD,��iTime=20140507����ʾ2014��5��7��
	���ڷ���K�߸�ʽΪYYMMDDHHmm����2405071300��������λ�ꡰ24���ǻ���1990�꿪ʼ����ģ����ԡ�24�������⴦��1990+24=2014�꣬��ʾ�Ľ��Ϊ2014��5��7��13��00��                                                                     */
	/************************************************************************/
	int    iTime;			//	ʱ��
	double dfOpenPrice;		//	���̼�
	double dfHighPrice;		//	��߼�
	double dfLowPrice;		//	��ͼ�
	double dfClosePrice;	//	���̼�
	uint64 ddwTotalVolum;	//	�ܳɽ���
	double dfAvgePrice;		//	�ɽ����
};
class IKInfo : public IHSKnown
{
public:
	// 
	/**
	���:
	iType:K������  �� 1������ 5�����߸�ֵ��Ϊö�����Ͷ���(Ϊ�˱��ֽӿڵ�һ���Ա������˲�������)
	iDeriction:	��ѯ����	0:��ǰ��	1:����飨Ŀǰ���������ң��˲������ã�
	iOffset:	��ʼ��ѯ��ƫ��,0��Ϊһ�������ֵ,����ǰ��ʱ,��ʾ�����һ������ǰ��,�����ʱ,��ʾ�ӵ�һ��������飨Ϊ�˱��ֽӿڵ�һ���Ա������˲������ã�

	����:	iNumber,��ǰ����k�߸���
	**/
	virtual KInfo* FUNCTION_CALL_MODE GetKInfo(enumKType KType, int iDeriction, int iOffset, int& iNumber) = 0;

	virtual KInfo* FUNCTION_CALL_MODE GetKInfoByIndex(int iIndex) = 0;

	virtual ~IKInfo() = 0;
};

////////////////////////	��Ʊ���еĽ����	//////////////////////////////////
typedef int (FUNCTION_CALL_MODE * H5_RECV_FUNCTION)(int iRegID, IStockInfo* lpStockInfo, int iResults, int* lpResultid, int iConnectID);
typedef int (FUNCTION_CALL_MODE * H5_RECV_FUNCTION_EX)(int iRegID, int iStockCount, IStockInfo** lpStockInfos, int iResults, int* lpResultid, int iConnectID);
struct  IResultSetInterface;

class IStockInfo : public IHSKnown
{
public:
	/**
	����Ļص�Ӧ������ʱ��,����һ����ʱ�Ķ���,��Ӧ�������,֪ͨӦ��,ͬʱɾ���ö�����
	**/

	//	ȡ������ʱ
	/************************************************************************/
	/* ��Σ�RecvFunc��
	*		RecvFunc=NULL��ͬ�����ã�api���������ã�һֱ��ȡ�����ݻ��߳�ʱ����
	*		RecvFunc��ΪNULL���첽���ã�api���첽�������������ȡ������ʱ���ݣ�
	*		�ȵ�������������غ��첽�ص��˻ص�����
	*	iConnectID�����Ӻţ��û�����Ҫ����
	/************************************************************************/
	virtual ISimpleTrend* 	FUNCTION_CALL_MODE GetSimpleTrendInfo(H5_RECV_FUNCTION RecvFunc=NULL, int iConnectID=-1) = 0;	//	��ü򵥷�ʱ
	
	/************************************************************************/
	/* ��Σ�
	* lpDate����ʷ��ʱ���ڣ���ʽΪ��YYYYMMDD��
	* RecvFunc��
	*		RecvFunc=NULL��ͬ�����ã�api���������ã�һֱ��ȡ�����ݻ��߳�ʱ����
	*		RecvFunc��ΪNULL���첽���ã�api���첽�������������ȡ��ʷ��ʱ���ݣ�
	*		�ȵ�������������غ��첽�ص��˻ص�����
	*	iConnectID�����Ӻţ��û�����Ҫ����
	/************************************************************************/
	virtual ISimpleTrend* 	FUNCTION_CALL_MODE GetHisSimpleTrendInfo(const char* lpDate, H5_RECV_FUNCTION RecvFunc=NULL, int iConnectID=-1) = 0;	//	�����ʷ�򵥷�ʱ
	
	
	/* ʹ��˵��������֮ǰʹ�ã����ĺ󣬽�ֱ�ӷ��ص�ǰ�Ŀ�����Ϣ
	* ��Σ�
	* RecvFunc��
	*		RecvFunc=NULL��ͬ�����ã�api���������ã�һֱ��ȡ�����ݻ��߳�ʱ����
	*		RecvFunc��ΪNULL���첽���ã������ǰ�Ŀ�����Ϣ�Ƚ��»��߸Ĵ��뱻�����ˣ���ֱ�ӷ��ص�ǰ�Ŀ��գ�
	*		���û�б����Ļ��������Ѿ����ڣ�api���첽�������������ȡ�������ۣ��ȵ�������������غ��첽�ص��˻ص�����
	*iConnectID�����Ӻţ��û�����Ҫ����
	/************************************************************************/
	virtual ISimpleQuote*	FUNCTION_CALL_MODE GetSimpleQuoteInfo(H5_RECV_FUNCTION RecvFunc=NULL, int iConnectID=-1) = 0;	//	��ü򵥱�����Ϣ
	
	
	/* ʹ��˵��������֮ǰʹ�ã����ĺ󣬽�ֱ�ӷ��ص�ǰ�Ŀ�����Ϣ
	* ��Σ�
	* RecvFunc��
	*		RecvFunc=NULL��ͬ�����ã�api���������ã�һֱ��ȡ�����ݻ��߳�ʱ����
	*		RecvFunc��ΪNULL���첽���ã������ǰ�Ŀ�����Ϣ�Ƚ��»��߸Ĵ��뱻�����ˣ���ֱ�ӷ��ص�ǰ�Ŀ��գ�
	*		���û�б����Ļ��������Ѿ����ڣ�api���첽�������������ȡ������λ��Ϣ���ȵ�������������غ��첽�ص��˻ص�����
	*iConnectID�����Ӻţ��û�����Ҫ����
	/************************************************************************/
	virtual IBidOffer*  	FUNCTION_CALL_MODE GetBidOffer(H5_RECV_FUNCTION RecvFunc=NULL, int iConnectID=-1) = 0;			//	���������λ��Ϣ


	//	ȡ��Ʊ��ʼ����Ϣ,ͬ������
	virtual IStockInitInfo*	FUNCTION_CALL_MODE GetInItInfo()=0;
	
	// 
	/**
	���:
	iType:K������  �� 1������ 5�����߸�ֵ��Ϊö�����Ͷ���,ֻ������/Ӧ��,û�ж���
	iDeriction:	��ѯ����	0:��ǰ��	1:����飨Ŀǰ���ã�
	iOffset:	��ʼ��ѯ��ƫ��,0��Ϊһ�������ֵ,����ǰ��ʱ,��ʾ�����һ������ǰ��,�����ʱ,��ʾ�ӵ�һ���������
	iNumber:	��ѯ���ٸ���
	**/
	virtual IKInfo*	FUNCTION_CALL_MODE GetKInfo(enumKType KType, int iDeriction, int iOffset, int& iNumber, H5_RECV_FUNCTION RecvFunc=NULL, int iConnectID=-1) = 0;	//	���K��


	//	�������Զ�������,��ʱ�Ǹ���ʵ��
	virtual IResultSetInterface* FUNCTION_CALL_MODE GetResultSet(int iResultID, H5_RECV_FUNCTION RecvFunc=NULL, int iConnectID=-1) = 0;
	
	//caich 2014��4��15�� ����½ӿ�
	/**
	��ǰ���������ݣ�����������������������������첽�ص��������Ļص�����֮��ʹ�ô˽ӿ�
	**/
	virtual ISimpleTrend* 	FUNCTION_CALL_MODE GetSimpleTrendCache() = 0;	//	��÷�ʱ���ա� ��Ӧ�¼� RESULT_TREND=2;������ʱ
	virtual IKInfo*	FUNCTION_CALL_MODE GetKLineCache()=0;//��ȡk�߿��ա���Ӧ�¼� RESULT_KLINE=4;����K��
	virtual IBidOffer*  	FUNCTION_CALL_MODE GetBidOfferCache()=0;//���������λ��Ϣ���ա���Ӧ�¼�RESULT_BIDOFF=5;������λ��Ϣ
	virtual ISimpleQuote*	FUNCTION_CALL_MODE GetSimpleQuoteInfoCache()=0;//��ü򵥱�����Ϣ���ա���Ӧ�¼�RESULT_SIMPLE_QUOTE=3;��������

	virtual INEEQZSLevelQuote*	FUNCTION_CALL_MODE GetNEEQZSLevelQuote(H5_RECV_FUNCTION RecvFunc = NULL, int iConnectID = -1) = 0;//��ù�ת����Ͷ���ߵ�λ���顣��Ӧ�¼�RESULT_NEEQ_ZS_LEVEL=6;��ת����Ͷ���ߵ�λ����
	virtual INEEQZSLevelQuote*	FUNCTION_CALL_MODE GetNEEQZSLevelQuoteCache() = 0;//��ù�ת����Ͷ���ߵ�λ������ա���Ӧ�¼�RESULT_NEEQ_ZS_LEVEL=6;��ת����Ͷ���ߵ�λ����
	virtual INEEQZSOrderQuote*	FUNCTION_CALL_MODE GetNEEQZSOrderQuote(H5_RECV_FUNCTION RecvFunc = NULL, int iConnectID = -1) = 0;//��ù�ת�����������걨���顣��Ӧ�¼�RESULT_NEEQ_ZS_ORDER=7;��ת�����������걨����
	virtual INEEQZSOrderQuote*	FUNCTION_CALL_MODE GetNEEQZSOrderQuoteCache() = 0;//��ù�ת�����������걨������ա���Ӧ�¼�RESULT_NEEQ_ZS_ORDER=7;��ת�����������걨����

	virtual INEEQXYOrderQuote*  FUNCTION_CALL_MODE GetNEEQXYOrderQuote(H5_RECV_FUNCTION RecvFunc = NULL, int iConnectID = -1) = 0;//��ù�תЭ��ת���걨���顣��Ӧ�¼�RESULT_NEEQ_XY_ORDER=8;��תЭ��ת���걨����
	virtual INEEQXYOrderQuote*  FUNCTION_CALL_MODE GetNEEQXYOrderQuoteCache() = 0;//��ù�תЭ��ת���걨���顣��Ӧ�¼�RESULT_NEEQ_XY_ORDER=8;��תЭ��ת���걨����

	virtual ~IStockInfo() = 0;
};


class IStockRealTimeInfo :public IHSKnown
{
public:
	virtual int FUNCTION_CALL_MODE GetMarketType()= 0;
	virtual const char * FUNCTION_CALL_MODE GetCode()= 0;
	virtual ISimpleQuote*	FUNCTION_CALL_MODE GetSimpleQuoteInfo()= 0;
	virtual IBidOffer*  	FUNCTION_CALL_MODE GetBidOffer()= 0;
	virtual ~IStockRealTimeInfo() = 0;
};

class IRealTimeEX :public IHSKnown
{
public:
	virtual int FUNCTION_CALL_MODE GetCount() = 0;
	virtual IStockRealTimeInfo *FUNCTION_CALL_MODE GetStockRealTimeInfo(int index)= 0;
	//�ͷ��ڴ档
	//������ڴ������Ҫʹ�ô˽ӿڣ���c++ʹ��ʱ����ָ��ʹ�ã�������ô˽ӿ�,delphi�ӿ��в�����ʾ���á�
	virtual unsigned long  FUNCTION_CALL_MODE Release() =  0;
	virtual ~IRealTimeEX()=0;
};

//////////////////////////////	�Զ���������ʱ��ʵ��	////////////////////////////////
//	��Ϊ�Զ�����������ʹ��,�ӿ����걸,����,���ս����ȡֵ,������Ը���
struct  IResultSetInterface :public IHSKnown
{
	/// �����ظ����ڼ�¼����
	virtual int FUNCTION_CALL_MODE GetRecordCount() const = 0;
	virtual int FUNCTION_CALL_MODE GetValurByField(int iFieldID) const = 0;

	virtual ~IResultSetInterface() = 0;
};
#endif
