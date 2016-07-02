/**********************************************************************
* Դ��������: MarketDataSpi.h
* ������־:
* 2015-10-25 ����
**********************************************************************/

#if !defined(MARKETDATASPI_H)
#define MARKETDATASPI_H

#include "MarketDataCenter_impl.h"

class CMarketDataSpi : public CHundsunDBFSpi, public CHundsunH5Spi, public CHundsunH5sdkSpi, public CHundsunFutuMdSpi
{
public:
	CMarketDataSpi(CHundsunMdCenterApi_impl* pApi)
	{
		lpApi = pApi;
	}
private:
	CHundsunMdCenterApi_impl* lpApi;
public:
	//////////////////////////////////////////////DBF///////////////////////////////////////////////
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnDBFConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	virtual void OnDBFDisconnected(int nReason){};

	///��������Ӧ��
	virtual void OnDBFRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnDBFRspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������֪ͨ
	virtual void OnDBFRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData);

	//////////////////////////////////////////////H5///////////////////////////////////////////////
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnH5Connected(int iMktType);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	virtual void OnH5Disconnected(int nReason){};

	///�����ѯ��Լ��Ӧ
	virtual void OnH5RspQryInstrument(CHundsunFtdcInstrumentField *pInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnH5RspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnH5RspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnH5RtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData);

	//////////////////////////////////////////////H5SDK///////////////////////////////////////////////
	    ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
    virtual void OnH5sdkFrontConnected();

    ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
    virtual void OnH5sdkFrontDisconnected(int nReason){};

	//////////////////////////////////////////////Futu///////////////////////////////////////////////
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	virtual void OnFrontDisconnected(int nReason){};

	///331100 �ͻ���¼Ӧ��
	virtual void OnRspFuncClientLogin(CHundsunFutuRspFuncClientLoginField *pRspFuncClientLogin, CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///330851 �ڻ���Լ��֤���ѯӦ��
    virtual void OnRspFuncFutuContractbailQry(CHundsunFutuRspFuncFutuContractbailQryField *pRspFuncFutuContractbailQry, CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///620001_33100 ����-�ڻ���������Ӧ��
	virtual void OnRspFuncQhSingleHqSubscribe(CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///620002_33100 ����ȡ��-�ڻ���������Ӧ��
	virtual void OnRspFuncQhSingleHqCancel(CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///620003_33100 ����-�ڻ���������Ӧ��
	virtual void OnRtnFuncQhSingleHqPush(CHundsunFutuRtnFuncQhSingleHqPushField *pFuncQhSingleHqPush);

	///620001_33102 ����-�ڻ��������Ӧ��
	virtual void OnRspFuncQhCombineSubscribe(CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///620002_33102 ����ȡ��-�ڻ��������Ӧ��
	virtual void OnRspFuncQhCombineCancel(CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///620003_33102 ����-�ڻ��������Ӧ��
	virtual void OnRtnFuncQhCombinePush(CHundsunFutuRtnFuncQhCombinePushField *pFuncQhCombinePush) {};
};

#endif

