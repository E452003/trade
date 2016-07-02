#if !defined(HUNDSUNH5SDKAPI_IMPL_H)
#define HUNDSUNH5SDKAPI_IMPL_H

#define LIB_H5SDK_API_EXPORT
#define ISLIB

#include "h5sdk_impl.h"
#include "public.h"
#include "HundsunH5sdkApi.h"

///����
class CHundsunH5sdkApi_impl : public CHundsunH5sdkApi, public CH5sdkImpl
{
public:
    ///Ĭ�Ϲ��캯��
    CHundsunH5sdkApi_impl(const char* pszFlowPath, bool bSaveLog);

    ///��������
    ~CHundsunH5sdkApi_impl();

    ///ɾ���ӿڶ�����
    virtual void ReleaseApi();

    ///��ʼ����
    virtual void Init();

    ///�ȴ��ӿ��߳̽�������
    virtual int Join();

    ///ע��ǰ�û������ַ
    ///@param pszFrontAddress��ǰ�û������ַ��
    virtual void RegisterFront(const char *pszFrontAddress);

    ///ע��ص��ӿ�
    ///@param pSpi �����Իص��ӿ����ʵ��
    virtual void RegisterSpi(CHundsunH5sdkSpi *pSpi);

    ///1001 ������������
    virtual int ReqH5sdkLogin(CHundsunH5sdkReqLoginField *pReqLogin, int nRequestID = 0);

    ///1002 �ǳ���Ϣ����
    virtual int ReqH5sdkLogout(CHundsunH5sdkReqLogoutField *pReqLogout, int nRequestID = 0);

    ///1003 ��������
    virtual int ReqH5sdkHeartbeat(CHundsunH5sdkReqHeartbeatField *pReqHeartbeat, int nRequestID = 0);

    ///1004 �����������Ϣ����
    virtual int ReqH5sdkServerInfo(CHundsunH5sdkReqServerInfoField *pReqServerInfo, int nRequestID = 0);

    ///1006 ����������
    virtual int ReqH5sdkBatch(CHundsunH5sdkReqBatchField *pReqBatch, int nRequestID = 0);

    ///2001 ����̬�ļ�����
    virtual int ReqH5sdkFile(CHundsunH5sdkReqFileField *pReqFile, int nRequestID = 0);

    ///2002 �г�������Ϣ����
    virtual int ReqH5sdkMarketTypes(CHundsunH5sdkReqMarketTypesField *pReqMarketTypes, int nRequestID = 0);

    ///2003 �г����������
    virtual int ReqH5sdkMarketReference(CHundsunH5sdkReqMarketReferenceField *pReqMarketReference, int nRequestID = 0);

    ///3001 �����������
    virtual int ReqH5sdkSnapshot(CHundsunH5sdkReqSnapshotField *pReqSnapshot, int nRequestID = 0);

    ///3002 ���̾�����Ϣ����
    virtual int ReqH5sdkKeyboardWizard(CHundsunH5sdkReqKeyboardWizardField *pReqKeyboardWizard, int nRequestID = 0);

    ///4001 ������ն�������
    virtual int ReqH5sdkSubscribe(CHundsunH5sdkReqSubscribeField *pReqSubscribe, int nRequestID = 0);

    ///5001 ������������
    virtual int ReqH5sdkSort(CHundsunH5sdkReqSortField *pReqSort, int nRequestID = 0);

    ///6001 ��ָ�������ڻ�ƫ��ȡ��ʱ��������
    virtual int ReqH5sdkTrend(CHundsunH5sdkReqTrendField *pReqTrend, int nRequestID = 0);

    ///6002 ��ƫ��ȡK������
    virtual int ReqH5sdkCandleByOffset(CHundsunH5sdkReqCandleByOffsetField *pReqCandleByOffset, int nRequestID = 0);

    ///6003 ������ʱ�䷶ΧȡK������
    virtual int ReqH5sdkCandleByRange(CHundsunH5sdkReqCandleByRangeField *pReqCandleByRange, int nRequestID = 0);

    ///6004 ָ�����ӵķֱ�����
    virtual int ReqH5sdkTickMin(CHundsunH5sdkReqTickMinField *pReqTickMin, int nRequestID = 0);

    ///6005 ��ƫ��ȡ�ֱ�����
    virtual int ReqH5sdkTickDirection(CHundsunH5sdkReqTickDirectionField *pReqTickDirection, int nRequestID = 0);

    ///6006 ����ʹ��_����k������
    virtual int ReqH5sdkCandleByRangeMaintain(CHundsunH5sdkReqCandleByRangeMaintainField *pReqCandleByRangeMaintain, int nRequestID = 0);

    ///6007 5�շ�ʱ����
    virtual int ReqH5sdkDay5Trend(CHundsunH5sdkReqDay5TrendField *pReqDay5Trend, int nRequestID = 0);

    ///7001 ���߷�������
    virtual int ReqH5sdkMaintainReturn(CHundsunH5sdkReqMaintainReturnField *pReqMaintainReturn, int nRequestID = 0);

public:
    ///���ӻص�
    void OnConnected();

    ///�Ͽ��ص�
    void OnClosed(int nReason);

    ///HsCommMessage
    void DealAsyCommMessage(IHsCommMessage* lpResponse);

protected:
    //1001 ��������-�첽
    void OnAsyResponse_1001(IRecord *lpBody, int nRequestID);

    //1002 �ǳ���Ϣ-�첽
    void OnAsyResponse_1002(IRecord *lpBody, int nRequestID);

    //1003 ����-�첽
    void OnAsyResponse_1003(IRecord *lpBody, int nRequestID);

    //1004 �����������Ϣ-�첽
    void OnAsyResponse_1004(IRecord *lpBody, int nRequestID);

    //1006 ������-�첽
    void OnAsyResponse_1006(IRecord *lpBody, int nRequestID);

    //2001 ����̬�ļ�-�첽
    void OnAsyResponse_2001(IRecord *lpBody, int nRequestID);

    //2002 �г�������Ϣ-�첽
    void OnAsyResponse_2002(IRecord *lpBody, int nRequestID);

    //2003 �г������-�첽
    void OnAsyResponse_2003(IRecord *lpBody, int nRequestID);

    //3001 �������-�첽
    void OnAsyResponse_3001(IRecord *lpBody, int nRequestID);

    //3002 ���̾�����Ϣ-�첽
    void OnAsyResponse_3002(IRecord *lpBody, int nRequestID);

    //4001 ������ն���-�첽
    void OnAsyResponse_4001(IRecord *lpBody, int nRequestID);

    //5001 ��������-�첽
    void OnAsyResponse_5001(IRecord *lpBody, int nRequestID);

    //6001 ��ָ�������ڻ�ƫ��ȡ��ʱ����-�첽
    void OnAsyResponse_6001(IRecord *lpBody, int nRequestID);

    //6002 ��ƫ��ȡK��-�첽
    void OnAsyResponse_6002(IRecord *lpBody, int nRequestID);

    //6003 ������ʱ�䷶ΧȡK��-�첽
    void OnAsyResponse_6003(IRecord *lpBody, int nRequestID);

    //6004 ָ�����ӵķֱ�-�첽
    void OnAsyResponse_6004(IRecord *lpBody, int nRequestID);

    //6005 ��ƫ��ȡ�ֱ�-�첽
    void OnAsyResponse_6005(IRecord *lpBody, int nRequestID);

    //6006 ����ʹ��_����k��-�첽
    void OnAsyResponse_6006(IRecord *lpBody, int nRequestID);

    //6007 5�շ�ʱ-�첽
    void OnAsyResponse_6007(IRecord *lpBody, int nRequestID);

    //7001 ���߷���-�첽
    void OnAsyResponse_7001(IRecord *lpBody, int nRequestID);

private:
    //�ص���ָ��
    CHundsunH5sdkSpi *lpH5sdkSpi;

    CEventEx                  hJoinEvent;       //�¼������¼�Join
};

#endif
