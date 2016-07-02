#if !defined(HUNDSUNMDCENTERAPI_IMPL_H)
#define HUNDSUNMDCENTERAPI_IMPL_H

#define LIB_MD_CENTER_API_EXPORT
#define ISLIB

#include "HundsunMdCenterApi.h"
#include "public.h"
#include "dbf_impl.h"
#include "h5_impl.h"
#include "HundsunH5sdkApi.h"
#include "HundsunFutuMdApi.h"

class CMarketDataSpi;

///����
class CHundsunMdCenterApi_impl : public CHundsunMdCenterApi
{
public:
    ///Ĭ�Ϲ��캯��
    CHundsunMdCenterApi_impl(const char* pszFlowPath);

    ///��������
    ~CHundsunMdCenterApi_impl();

    ///ɾ���ӿڶ�����
    virtual void ReleaseApi();

    ///��ʼ����
    virtual void Init(THundsunMarkerDataType MDType);

    ///�ȴ��ӿ��߳̽�������
    virtual int Join();

	///ע��DBF�ļ���ַ(����֤ȯ����)
	///@param pszFilePath��DBF·��
	virtual void RegisterDBFPath(const char *pszFilePath);

	///ע��H5
	///@param pszLicFile��H5֤���ļ�
	///@param pszCfgFile�������ļ�
	///@param bSubLevel2���Ƿ�level2����
	virtual void RegisterH5(const char *pszLicFile, const char *pszCfgFile, bool bSubLevel2 = false);

	///ע��T2ͨѶ����
	///@param pszLicFile��T2ͨѶ֤�顣
	///@param pszPwd��safe_level = pwd������Ҫ����ͨѶ���롣
	///@param pszSslFile��safe_level = ssl��ssl֤�顣
	///@param pszSslPwd��ssl֤�����롣
	virtual void RegisterFutu(const char *pszLicFile, const char *pszPwd = "", const char *pszSslFile = "", const char *pszSslPwd = "");

    ///ע��ǰ�û������ַ
    ///@param pszFrontAddress��ǰ�û������ַ��
    virtual void RegisterFront(const char *pszFrontAddress);

    ///ע��ص��ӿ�
    ///@param pSpi �����Իص��ӿ����ʵ��
    virtual void RegisterSpi(CHundsunMdCenterSpi *pSpi);

	///�û���¼����
	virtual int ReqUserLogin(CHundsunFtdcReqUserLoginField *pReqUserLoginField, int nRequestID);

	///�����ѯ��Լ
	virtual int ReqQryInstrument(CHundsunFtdcQryInstrumentField *pQryInstrument, int nRequestID);

	///�������顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	virtual int SubscribeMarketData(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount);

	///�˶����顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	virtual int UnSubscribeMarketData(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount);

public:
	//����ϲ�ص�����ָ��
	CHundsunMdCenterSpi* GetMdCenterSpi();

	//�����ڻ������Ƿ�ɹ�
	void SetFutuConnected(bool bvl);
	//�����ڻ���¼�Ƿ�ɹ�
	void SetFutuLogined(bool bvl);
private:
    //�ص���ָ��
    CHundsunMdCenterSpi*      lpMdSpi;
	//֤ȯ��������
	THundsunMarkerDataType    m_StockMDType;
	//�ڻ���������
	THundsunMarkerDataType    m_FutuMDType;

	//DBF�ļ�·��
	char                      szDBFPath[1024];
	//DBF����ֻ��Ψһ��
	CDBF_Impl*                lp_DBF_Impl;

	//H5֤���ļ�·��
	char                      szH5Path_LicFile[1024];
	//H5�����ļ�·��
	char                      szH5Path_CfgFile[1024];
	//H5�Ƿ�ʹ��level2����
	bool                      bH5_SubLevel2;
	//H5����ֻ��Ψһ��
	CH5_Impl                  m_H5_Impl;

	//H5sdk����
	CHundsunH5sdkApi*         lp_H5sdk_Impl;

	//FutuͨѶ֤��
	char                      szFutu_LicFile[1024];
	//FutuͨѶ����
	char                      szFutu_Pwd[1024];
	//Futu_ssl֤��
	char                      szFutu_SslFile[1024];
	//Futu_ssl֤������
	char                      szFutu_SslPwd[1024];
	//Futu����
	CHundsunFutuMdApi*        lpFutuMD;
	//�Ƿ����ӳɹ�
	bool                      bFutu_Connected;
	//�Ƿ��¼��¼�ɹ�
	bool                      bFutu_Logined;

	//ǰ�õ�ַ
	char                      szFrontAddress[1024];

	//����ص�����ָ��
	CMarketDataSpi*           lpMarketDataSpi;

	//��Լ��Ϣ
	map<THundsunExchangeType, CHundsunFtdcInstrumentField*> m_InstrumentMap; 

    string                    m_localip;        //��t2sdk���ٻ��һ����ʵ������ip
    string                    m_localMac;       //����mac��ַ
    
    bool                      m_SaveLog;        //�Ƿ��¼��־
    CEventEx                  hJoinEvent;       //�¼������¼�Join
    ManualLock                m_SubSetCs;       //������Ϣ��
    int                       m_iHeartKeep;     //������ʱ������
    CThread                   m_HeartKeepTrd;   //ά�������߳�
    CEventEx                  m_HeartKeepEvent; //�����¼�
    bool volatile             m_bHeartKeepStop; //�Ƿ�ֹͣ�����̣߳�����ѭ��

};

#endif
