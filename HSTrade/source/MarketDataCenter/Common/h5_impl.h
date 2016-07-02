/**********************************************************************
* Դ��������: h5_impl.h
* ������־:
* 2015-10-19 ����(�ӽӿڶ���������һ������ֻ������һ������)
* 2015-10-25 �Ż�Ϊ�ɵ���ʹ��
* 2016-01-11 ���Ӳ�ѯ��Լ��Ϣ
**********************************************************************/

#if !defined(H5_IMPL_H)
#define H5_IMPL_H

#include "h5hq_api_interface.h"
#include "HundsunMdCenterApi.h"
#include "public.h"

///H5�ص�����
class CHundsunH5Spi
{
public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnH5Connected(int iMktType){};

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	virtual void OnH5Disconnected(int nReason){};

	///�����ѯ��Լ��Ӧ
	virtual void OnH5RspQryInstrument(CHundsunFtdcInstrumentField *pInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��������Ӧ��
	virtual void OnH5RspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ȡ����������Ӧ��
	virtual void OnH5RspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������֪ͨ
	virtual void OnH5RtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData) {};
};

class CH5_Impl
{
public:
	//��ȡ����ָ��
	CH5_Impl* GetInstance();

	///ɾ���ӿڶ�����
	void ReleaseH5();

	///��ʼ��
	int InitH5(const char *pszLicFile, const char *pszCfgFile, bool bSubLevel2 = false);

	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	void RegisterH5Spi(CHundsunH5Spi *pSpi);

	///�����ѯ��Լ
	int ReqQryInstrumentH5(CHundsunFtdcQryInstrumentField *pQryInstrument, int nRequestID);

	///�������顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	int SubscribeH5(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount);

	///ȡ������
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	int UnSubscribeH5(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount);

public:
	//��ú�Լ��Ϣ
	void GetH5MarketInfo(int iMktType);

	//������������
	void DealRecv(IStockInfo* lpStockInfo, int iResults, int* lpResultid);
private:
	//�ص���ָ��
	CHundsunH5Spi*            lpH5Spi;
	//�Ƿ���level2����
	bool                      m_SubLevel2;

	//��Լ��Ϣ
	map<pair<int, string>, CHundsunFtdcInstrumentField*> m_InstrumentMap; 
	//��ǰ���黺��
	map<pair<int, string>, CHundsunDepthMarketDataField*> m_DepthMDMap;

	//�ж��Ƿ�Ϊ��H5��Ϊ��Լ����
	bool IsSpecialInstrument(THundsunFtdcInstrument pszInstrument);

	//��IStockInfo��ȡ��������
	void GetH5StockInfo(IStockInfo* pStockInfo, CHundsunDepthMarketDataField *pDepthMarketData, int iSetID = -1);
	//���»�������
	void UpdateSimpleQuote(IStockInfo* pStockInfo, CHundsunDepthMarketDataField *pDepthMarketData);
	//����������λ��Ϣ
	void UpdateBidoff(IStockInfo* pStockInfo, CHundsunDepthMarketDataField *pDepthMarketData);

	//��ú�Լ��Ϣ
	CHundsunFtdcInstrumentField* GetInstrument(THundsunExchangeType ExchangeID, THundsunFtdcInstrument pszInstrument);
	//��õ�ǰ����
	CHundsunDepthMarketDataField* GetDepthMD(THundsunExchangeType ExchangeID, const char* pszInstrument);

	//���º�Լ��Ϣ
	void UpdateInstrument(CHundsunFtdcInstrumentField *pInstrument);
	//���µ�ǰ����
	void UpdateDepthMD(CHundsunDepthMarketDataField *pDepthMarketData);
};

#endif
