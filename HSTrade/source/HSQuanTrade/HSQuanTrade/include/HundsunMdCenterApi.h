/**********************************************************************
* Դ��������: HundsunMdCenterApi.h
* ʹ�ý��飺
*   -->ʹ��DBF����Դ��
*      1.����RegisterDBFPath��ע�᱾��DBF·��
*      2.����Init���Ҳ�������HUNDSUN_FTDC_DBF
*   -->ʹ��H5����Դ��
*      1.����RegisterH5��ע��H5��Ҫ��֤��+����
*      2.����Init���Ҳ�������HUNDSUN_FTDC_H5
*   -->ʹ��Futu����Դ��
*      1.����RegisterFutu��ע��T2ͨѶ����
*      2.����RegisterFront��ע��ǰ�õ�ַ
*      3.����Init���Ҳ�������HUNDSUN_FTDC_Futu
*      4.����ReqUserLogin֮�󣬲��ܵ���ReqQryInstrument
* ������־:
* 2015-10-09 ����
* 2015-10-25 �Ż�ʵ�ַ�ʽ��DBF��H5ʵ�ִ���ɵ���ʹ��
* 2015-11-07 �����ڻ�UFX�����ȡ
* 2015-11-20 ����֤ȯ�ڻ�����ͬʱ���ĵĹ���
* 2016-01-11 ����֤ȯ��ѯ��Լ��Ϣ���ܣ�IsLast=Trueʱ������=NULL��
* 2016-01-13 �����ڻ���ѯ��Լ��Ϣ���ܣ�ǰ����Ҫ�ʽ��˺ŵ�¼��
* 2016-01-22 ����֧��H5����Level2���顣
**********************************************************************/

#if !defined(HUNDSUNMDCENTERAPI_H)
#define HUNDSUNMDCENTERAPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HundsunMdCenterStruct.h"

#if defined(ISLIB)

#ifdef WIN32
#ifdef LIB_MD_CENTER_API_EXPORT
#define MD_CENTER_API_EXPORT __declspec(dllexport)
#else
#define MD_CENTER_API_EXPORT __declspec(dllimport)
#endif
#endif

#ifdef LINUX
#ifdef LIB_MD_CENTER_API_EXPORT
#define MD_CENTER_API_EXPORT __attribute__((visibility("default")))
#else
#define MD_CENTER_API_EXPORT
#endif
#endif

#else
#define MD_CENTER_API_EXPORT
#endif

///�ص�����
class CHundsunMdCenterSpi
{
public:
    ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
    virtual void OnFrontConnected(){};

    ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
    virtual void OnFrontDisconnected(int nReason){};

	///��¼������Ӧ
	virtual void OnRspUserLogin(CHundsunFtdcRspUserLoginField *pRspUserLogin, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CHundsunFtdcInstrumentField *pInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��������Ӧ��
	virtual void OnRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData) {};

	///K������֪ͨ
	virtual void OnRtnKLineData(CHundsunKLineDataField *pKLineData) {};

};

///����
class MD_CENTER_API_EXPORT CHundsunMdCenterApi
{
public:
    ///����MdApi
    static CHundsunMdCenterApi *CreateHundsunMdCenterApi(const char *pszFlowPath = "");

    ///ɾ���ӿڶ�����
    virtual void ReleaseApi() = 0;

    ///��ʼ����
    virtual void Init(THundsunMarkerDataType MDType) = 0;

    ///�ȴ��ӿ��߳̽�������
    virtual int Join() = 0;

	///ע��DBF�ļ���ַ(����֤ȯ����)
	///@param pszFilePath��DBF·��
	virtual void RegisterDBFPath(const char *pszFilePath) = 0;

	///ע��H5(������������)
	///@param pszLicFile��H5֤���ļ�
	///@param pszCfgFile�������ļ�
	///@param bSubLevel2���Ƿ�level2����
	virtual void RegisterH5(const char *pszLicFile, const char *pszCfgFile, bool bSubLevel2 = false) = 0;

	///ע��T2ͨѶ����
	///@param pszLicFile��T2ͨѶ֤�顣
	///@param pszPwd��safe_level = pwd������Ҫ����ͨѶ���롣
	///@param pszSslFile��safe_level = ssl��ssl֤�顣
	///@param pszSslPwd��ssl֤�����롣
	virtual void RegisterFutu(const char *pszLicFile, const char *pszPwd = "", const char *pszSslFile = "", const char *pszSslPwd = "") = 0;

    ///ע��ǰ�û������ַ
    ///@param pszFrontAddress��ǰ�û������ַ��
    virtual void RegisterFront(const char *pszFrontAddress) = 0;

    ///ע��ص��ӿ�
    ///@param pSpi �����Իص��ӿ����ʵ��
    virtual void RegisterSpi(CHundsunMdCenterSpi *pSpi) = 0;

	///�û���¼����
	virtual int ReqUserLogin(CHundsunFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) = 0;

	///�����ѯ��Լ
	virtual int ReqQryInstrument(CHundsunFtdcQryInstrumentField *pQryInstrument, int nRequestID) = 0;

	///�������顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	virtual int SubscribeMarketData(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount) = 0;

	///�˶����顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	virtual int UnSubscribeMarketData(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount) = 0;

protected:
    ~CHundsunMdCenterApi(){};
 };

#endif
