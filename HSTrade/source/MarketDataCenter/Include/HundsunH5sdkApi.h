#if !defined(HUNDSUNH5SDKAPI_H)
#define HUNDSUNH5SDKAPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include "HundsunH5sdkUserApiStruct.h"

#if defined(ISLIB)

#ifdef WIN32
#ifdef LIB_H5SDK_API_EXPORT
#define H5SDK_API_EXPORT __declspec(dllexport)
#else
#define H5SDK_API_EXPORT __declspec(dllimport)
#endif
#endif

#ifdef LINUX
#ifdef LIB_H5SDK_API_EXPORT
#define H5SDK_API_EXPORT __attribute__((visibility("default")))
#else
#define H5SDK_API_EXPORT
#endif
#endif

#else
#define H5SDK_API_EXPORT
#endif

//�ص�����
class CHundsunH5sdkSpi
{
public:
    ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
    virtual void OnH5sdkFrontConnected(){};

    ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
    virtual void OnH5sdkFrontDisconnected(int nReason){};

    ///1001 ��������Ӧ��
    virtual void OnRspH5sdkLogin(CHundsunH5sdkRspLoginField *pRspLogin, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///1002 �ǳ���ϢӦ��
    virtual void OnRspH5sdkLogout(CHundsunH5sdkRspLogoutField *pRspLogout, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///1003 ����Ӧ��
    virtual void OnRspH5sdkHeartbeat(CHundsunH5sdkRspHeartbeatField *pRspHeartbeat, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///1004 �����������ϢӦ��
    virtual void OnRspH5sdkServerInfo(CHundsunH5sdkRspServerInfoField *pRspServerInfo, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///1006 ������Ӧ��
    virtual void OnRspH5sdkBatch(CHundsunH5sdkRspBatchField *pRspBatch, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///2001 ����̬�ļ�Ӧ��
    virtual void OnRspH5sdkFile(CHundsunH5sdkRspFileField *pRspFile, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///2002 �г�������ϢӦ��
    virtual void OnRspH5sdkMarketTypes(CHundsunH5sdkRspMarketTypesField *pRspMarketTypes, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///2003 �г������Ӧ��
    virtual void OnRspH5sdkMarketReference(CHundsunH5sdkRspMarketReferenceField *pRspMarketReference, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///3001 �������Ӧ��
    virtual void OnRspH5sdkSnapshot(CHundsunH5sdkRspSnapshotField *pRspSnapshot, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///3002 ���̾�����ϢӦ��
    virtual void OnRspH5sdkKeyboardWizard(CHundsunH5sdkRspKeyboardWizardField *pRspKeyboardWizard, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///4001 ������ն���Ӧ��
    virtual void OnRspH5sdkSubscribe(CHundsunH5sdkRspSubscribeField *pRspSubscribe, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///5001 ��������Ӧ��
    virtual void OnRspH5sdkSort(CHundsunH5sdkRspSortField *pRspSort, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///6001 ��ָ�������ڻ�ƫ��ȡ��ʱ����Ӧ��
    virtual void OnRspH5sdkTrend(CHundsunH5sdkRspTrendField *pRspTrend, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///6002 ��ƫ��ȡK��Ӧ��
    virtual void OnRspH5sdkCandleByOffset(CHundsunH5sdkRspCandleByOffsetField *pRspCandleByOffset, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///6003 ������ʱ�䷶ΧȡK��Ӧ��
    virtual void OnRspH5sdkCandleByRange(CHundsunH5sdkRspCandleByRangeField *pRspCandleByRange, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///6004 ָ�����ӵķֱ�Ӧ��
    virtual void OnRspH5sdkTickMin(CHundsunH5sdkRspTickMinField *pRspTickMin, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///6005 ��ƫ��ȡ�ֱ�Ӧ��
    virtual void OnRspH5sdkTickDirection(CHundsunH5sdkRspTickDirectionField *pRspTickDirection, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///6006 ����ʹ��_����k��Ӧ��
    virtual void OnRspH5sdkCandleByRangeMaintain(CHundsunH5sdkRspCandleByRangeMaintainField *pRspCandleByRangeMaintain, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///6007 5�շ�ʱӦ��
    virtual void OnRspH5sdkDay5Trend(CHundsunH5sdkRspDay5TrendField *pRspDay5Trend, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///7001 ���߷���Ӧ��
    virtual void OnRspH5sdkMaintainReturn(CHundsunH5sdkRspMaintainReturnField *pRspMaintainReturn, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

};

///����
class H5SDK_API_EXPORT CHundsunH5sdkApi
{
public:
    ///����H5sdkApi
    static CHundsunH5sdkApi *CreateHundsunH5sdkApi(const char *pszFlowPath = "", bool bSaveLog = false);

    ///ɾ���ӿڶ�����
    virtual void ReleaseApi() = 0;

    ///��ʼ����
    virtual void Init() = 0;

    ///�ȴ��ӿ��߳̽�������
    virtual int Join() = 0;

    ///ע��ǰ�û������ַ
    ///@param pszFrontAddress��ǰ�û������ַ��
    virtual void RegisterFront(const char *pszFrontAddress) = 0;

    ///ע��ص��ӿ�
    ///@param pSpi �����Իص��ӿ����ʵ��
    virtual void RegisterSpi(CHundsunH5sdkSpi *pSpi) = 0;

    ///1001 ������������
    virtual int ReqH5sdkLogin(CHundsunH5sdkReqLoginField *pReqLogin, int nRequestID = 0) = 0;

    ///1002 �ǳ���Ϣ����
    virtual int ReqH5sdkLogout(CHundsunH5sdkReqLogoutField *pReqLogout, int nRequestID = 0) = 0;

    ///1003 ��������
    virtual int ReqH5sdkHeartbeat(CHundsunH5sdkReqHeartbeatField *pReqHeartbeat, int nRequestID = 0) = 0;

    ///1004 �����������Ϣ����
    virtual int ReqH5sdkServerInfo(CHundsunH5sdkReqServerInfoField *pReqServerInfo, int nRequestID = 0) = 0;

    ///1006 ����������
    virtual int ReqH5sdkBatch(CHundsunH5sdkReqBatchField *pReqBatch, int nRequestID = 0) = 0;

    ///2001 ����̬�ļ�����
    virtual int ReqH5sdkFile(CHundsunH5sdkReqFileField *pReqFile, int nRequestID = 0) = 0;

    ///2002 �г�������Ϣ����
    virtual int ReqH5sdkMarketTypes(CHundsunH5sdkReqMarketTypesField *pReqMarketTypes, int nRequestID = 0) = 0;

    ///2003 �г����������
    virtual int ReqH5sdkMarketReference(CHundsunH5sdkReqMarketReferenceField *pReqMarketReference, int nRequestID = 0) = 0;

    ///3001 �����������
    virtual int ReqH5sdkSnapshot(CHundsunH5sdkReqSnapshotField *pReqSnapshot, int nRequestID = 0) = 0;

    ///3002 ���̾�����Ϣ����
    virtual int ReqH5sdkKeyboardWizard(CHundsunH5sdkReqKeyboardWizardField *pReqKeyboardWizard, int nRequestID = 0) = 0;

    ///4001 ������ն�������
    virtual int ReqH5sdkSubscribe(CHundsunH5sdkReqSubscribeField *pReqSubscribe, int nRequestID = 0) = 0;

    ///5001 ������������
    virtual int ReqH5sdkSort(CHundsunH5sdkReqSortField *pReqSort, int nRequestID = 0) = 0;

    ///6001 ��ָ�������ڻ�ƫ��ȡ��ʱ��������
    virtual int ReqH5sdkTrend(CHundsunH5sdkReqTrendField *pReqTrend, int nRequestID = 0) = 0;

    ///6002 ��ƫ��ȡK������
    virtual int ReqH5sdkCandleByOffset(CHundsunH5sdkReqCandleByOffsetField *pReqCandleByOffset, int nRequestID = 0) = 0;

    ///6003 ������ʱ�䷶ΧȡK������
    virtual int ReqH5sdkCandleByRange(CHundsunH5sdkReqCandleByRangeField *pReqCandleByRange, int nRequestID = 0) = 0;

    ///6004 ָ�����ӵķֱ�����
    virtual int ReqH5sdkTickMin(CHundsunH5sdkReqTickMinField *pReqTickMin, int nRequestID = 0) = 0;

    ///6005 ��ƫ��ȡ�ֱ�����
    virtual int ReqH5sdkTickDirection(CHundsunH5sdkReqTickDirectionField *pReqTickDirection, int nRequestID = 0) = 0;

    ///6006 ����ʹ��_����k������
    virtual int ReqH5sdkCandleByRangeMaintain(CHundsunH5sdkReqCandleByRangeMaintainField *pReqCandleByRangeMaintain, int nRequestID = 0) = 0;

    ///6007 5�շ�ʱ����
    virtual int ReqH5sdkDay5Trend(CHundsunH5sdkReqDay5TrendField *pReqDay5Trend, int nRequestID = 0) = 0;

    ///7001 ���߷�������
    virtual int ReqH5sdkMaintainReturn(CHundsunH5sdkReqMaintainReturnField *pReqMaintainReturn, int nRequestID = 0) = 0;

protected:
    ~CHundsunH5sdkApi(){};
};

#endif
