#if !defined(MARKDATATRADERAPI_IMPL_H)
#define MARKDATATRADERAPI_IMPL_H

#define LIB_TRADER_API_EXPORT
#define ISLIB

#include "t2_connection_impl.h"
#include "public.h"
#include "MarkDataTraderApi.h"
#include "MarkDataTraderApi_log.h"

///����
class CMarkDataTraderApi_impl : public CMarkDataTraderApi, public CT2ConnectionImpl
{
public:
    ///Ĭ�Ϲ��캯��
    CMarkDataTraderApi_impl(const char* pszFlowPath, bool bSaveLog);

    ///��������
    ~CMarkDataTraderApi_impl();

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
    virtual void RegisterSpi(CMarkDataTraderSpi *pSpi);

    ///�����û���Ϣ
    virtual CMarkDataUserInfoField* GetUserInfo();

    ///338499 ����ؿ�����
    virtual int ReqFuncMarkdatainsert(CMarkDataReqFuncMarkdatainsertField *pReqFuncMarkdatainsert, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CMarkDataRspInfoField *pRspInfo = NULL);

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
    //338499 ����ؿ�-�첽
    void OnAsyResponse_338499(IF2UnPacker *lpUnPacker, int nRequestID);

    ///338499 ����ؿ�-ͬ��
    void OnSynResponse_338499(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CMarkDataRspInfoField *pRspInfo);

private:
    //�ص���ָ��
    CMarkDataTraderSpi *lpTraderSpi;
    //��־����ָ��
    CMarkDataTraderApi_log *lpTraderApi_log;
    //�û���Ϣ
    CMarkDataUserInfoField     m_userInfo;
    string                    m_localip;        //��t2sdk���ٻ��һ����ʵ������ip
    string                    m_localMac;       //����mac��ַ
    bool                      m_Logined;        //�Ƿ��¼��¼�ɹ�
    bool                      m_SaveLog;        //�Ƿ��¼��־
    CEventEx                  hJoinEvent;       //�¼������¼�Join
    ManualLock                QueryLock;        //��ҳ��ѯ��
    map<int, CQueryInfo*>     QueryMap;         //��ҳ��ѯ���������Map
    int                       m_iHeartKeep;     //������ʱ������
    CThread                   m_HeartKeepTrd;   //ά�������߳�
    CEventEx                  m_HeartKeepEvent; //�����¼�
    bool volatile             m_bHeartKeepStop; //�Ƿ�ֹͣ�����̣߳�����ѭ��

    static void HeartKeep(void* lpvoid);
};

#endif
