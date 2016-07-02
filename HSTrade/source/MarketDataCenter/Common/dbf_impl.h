/**********************************************************************
* Դ��������: dbf_impl.h
* ������־:
* 2015-10-25 ����
**********************************************************************/

#if !defined(DBF_IMPL_H)
#define DBF_IMPL_H

#include "dbf.h"
#include "HundsunMdCenterApi.h"
#include "public.h"

///H5�ص�����
class CHundsunDBFSpi
{
public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnDBFConnected(){};

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	virtual void OnDBFDisconnected(int nReason){};

	///��������Ӧ��
	virtual void OnDBFRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ȡ����������Ӧ��
	virtual void OnDBFRspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������֪ͨ
	virtual void OnDBFRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData) {};
};

class CDBF_Impl
{
public:
	///����
	CDBF_Impl()
	{
		bStopReadDBF = false;
	}

	///��ʼ��
	int InitDBF(const char *pszDBFFile);

	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	void RegisterDFBSpi(CHundsunDBFSpi *pSpi);

	///�������顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	virtual int SubscribeDBF(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount);

	///�˶����顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	virtual int UnSubscribeDBF(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount);
private:
	//�ص���ָ��
	CHundsunDBFSpi*           lpDBFSpi;

	//�Ϻ�DBF�ļ�·��
	char                      szPath_show2003[1024];
	//����DBF�ļ�·��
	char                      szPath_sjshq[1024];
	//�Ϻ�DBF����
	CDBF                      m_dbf_show2003;
	//����DBF����
	CDBF                      m_dbf_sjshq;
	//���黺��
	map<pair<int, string>, CHundsunDepthMarketDataField*> HQMap;
	//������Ϣ
	set<pair<int, string>>    SubInfoSet;

	//�Ƿ�ֹͣ����DBF
	bool                      bStopReadDBF;
	//���������߳�
	CThread                   m_ReadDBFTrd;
	//��������
	static void               ReadDBF(void* lpvoid);

	//�����Ϻ�DBF�ļ�
	void UpdateSH();
	//��������DBF�ļ�
	void UpdateSZ();

	//��õ�ǰ����
	CHundsunDepthMarketDataField* GetHQ(THundsunExchangeType ExchangeID, const char* pszInstrument);
	//�������鲢���ж��Ƿ���������
	void UpdateHQ(THundsunExchangeType ExchangeID, const char* pszInstrument, CHundsunDepthMarketDataField *pDepthMarketData);
};

#endif


