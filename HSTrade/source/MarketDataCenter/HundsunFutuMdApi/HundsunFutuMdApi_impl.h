#if !defined(HUNDSUNFUTUMDAPI_IMPL_H)
#define HUNDSUNFUTUMDAPI_IMPL_H

#define LIB_MD_API_EXPORT
#define ISLIB

#include "t2_connection_impl.h"
#include "public.h"
#include "HundsunFutuMdApi.h"
#include "HundsunFutuMdApi_log.h"

///����
class CHundsunFutuMdApi_impl : public CHundsunFutuMdApi, public CT2ConnectionImpl
{
public:
    ///Ĭ�Ϲ��캯��
    CHundsunFutuMdApi_impl(const char* pszFlowPath, bool bSaveLog);

    ///��������
    ~CHundsunFutuMdApi_impl();

    ///ɾ���ӿڶ�����
    virtual void ReleaseApi();

    ///��ʼ����
    virtual void Init(const char *pszLicFile, const char *pszPwd = "", const char *pszSslFile = "", const char *pszSslPwd = "");

    ///�ȴ��ӿ��߳̽�������
    virtual int Join();

    ///ע��ǰ�û������ַ
    ///@param pszFrontAddress��ǰ�û������ַ��
    ///@param nSubSystemNo����ϵͳ��(���ݾ����ڻ���˾����������һ���ڻ���115)
    ///@param nCompanyID����˾���(�Խ�ITNʱ��Ҫ�����Ծ��������ITNѯ��)
    virtual void RegisterFront(const char *pszFrontAddress, int nSubSystemNo = 0, int nCompanyID = 0, const char *pszConnName = "");

    ///ע��ص��ӿ�
    ///@param pSpi �����Իص��ӿ����ʵ��
    virtual void RegisterSpi(CHundsunFutuMdSpi *pSpi);

    ///�����û���Ϣ
    virtual CHundsunFutuUserInfoField* GetUserInfo();

    ///331100 �ͻ���¼����
    virtual int ReqFuncClientLogin(CHundsunFutuReqFuncClientLoginField *pReqFuncClientLogin, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL);

    ///330851 �ڻ���Լ��֤���ѯ����
    virtual int ReqFuncFutuContractbailQry(CHundsunFutuReqFuncFutuContractbailQryField *pReqFuncFutuContractbailQry, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL);

    ///620001_33100 ����-�ڻ�������������
    virtual int ReqFuncQhSingleHqSubscribe(char *ppInstrumentID[], int nCount, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL);

    ///620002_33100 ����ȡ��-�ڻ�������������
    virtual int ReqFuncQhSingleHqCancel(char *ppInstrumentID[], int nCount, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL);

    ///620001_33102 ����-�ڻ������������
    virtual int ReqFuncQhCombineSubscribe(char *ppInstrumentID[], int nCount, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL);

    ///620002_33102 ����ȡ��-�ڻ������������
    virtual int ReqFuncQhCombineCancel(char *ppInstrumentID[], int nCount, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL);

public:
    //���ӻص�
    void OnConnected();

    //�Ͽ��ص�
    void OnClosed();

    //�����첽BizMessage
    void DealAsyBizMessage(IBizMessage* lpMsg);

    //����ͬ��BizMessage
    int DealSynBizMessage(const void* lpBuffer, int nLen, void** ppRetData, int* pnItems, void *pRspInfo);

    //��������ɹ�
    void RecHeartbeet();

    //���T2ͨѶ������Ϣ
    void GetMsgErrorInfo(int nErrorCode, void *pRspInfo);

protected:
    //331100 �ͻ���¼-�첽
    void OnAsyResponse_331100(IF2UnPacker *lpUnPacker, int nRequestID);

    //330851 �ڻ���Լ��֤���ѯ-�첽
    void OnAsyResponse_330851(IF2UnPacker *lpUnPacker, int nRequestID);

    //620001_33100 ����-�ڻ���������-�첽
    void OnAsyRsp_620001_33100(IF2UnPacker *lpUnPacker, int nRequestID);

    //620002_33100 ����ȡ��-�ڻ���������-�첽
    void OnAsyRsp_620002_33100(IF2UnPacker *lpUnPacker, int nRequestID);

    //620003_33100 ����-�ڻ���������-�첽
    void OnAsyRtn_620003_33100(IF2UnPacker *lpUnPacker, int nRequestID);

    //620001_33102 ����-�ڻ��������-�첽
    void OnAsyRsp_620001_33102(IF2UnPacker *lpUnPacker, int nRequestID);

    //620002_33102 ����ȡ��-�ڻ��������-�첽
    void OnAsyRsp_620002_33102(IF2UnPacker *lpUnPacker, int nRequestID);

    //620003_33102 ����-�ڻ��������-�첽
    void OnAsyRtn_620003_33102(IF2UnPacker *lpUnPacker, int nRequestID);

    ///331100 �ͻ���¼-ͬ��
    void OnSynResponse_331100(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo);

    ///330851 �ڻ���Լ��֤���ѯ-ͬ��
    void OnSynResponse_330851(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo);

    ///620001_33100 ����-�ڻ���������-ͬ��
    void OnSynRsp_620001_33100(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo);

    ///620002_33100 ����ȡ��-�ڻ���������-ͬ��
    void OnSynRsp_620002_33100(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo);

    ///620001_33102 ����-�ڻ��������-ͬ��
    void OnSynRsp_620001_33102(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo);

    ///620002_33102 ����ȡ��-�ڻ��������-ͬ��
    void OnSynRsp_620002_33102(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo);

private:
    //�ص���ָ��
    CHundsunFutuMdSpi *lpMdSpi;
    //��־����ָ��
    CHundsunFutuMdApi_log *lpMdApi_log;
    //�û���Ϣ
    CHundsunFutuUserInfoField     m_userInfo;
    string                    m_localip;        //��t2sdk���ٻ��һ����ʵ������ip
    string                    m_localMac;       //����mac��ַ
    bool                      m_Logined;        //�Ƿ��¼��¼�ɹ�
    bool                      m_SaveLog;        //�Ƿ��¼��־
    CEventEx                  hJoinEvent;       //�¼������¼�Join
    ManualLock                m_SubSetCs;       //������Ϣ��
    set<string>               m_SubFutuCodeSet;
    set<string>               m_SubArbitCodeSet;
    int                       m_iHeartKeep;     //������ʱ������
    CThread                   m_HeartKeepTrd;   //ά�������߳�
    CEventEx                  m_HeartKeepEvent; //�����¼�
    bool volatile             m_bHeartKeepStop; //�Ƿ�ֹͣ�����̣߳�����ѭ��

    static void HeartKeep(void* lpvoid);
};

#endif
