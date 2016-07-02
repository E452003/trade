#if !defined(MARKDATATRADERAPI_H)
#define MARKDATATRADERAPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include "MarkDataUserApiStruct.h"

#if defined(ISLIB)

#ifdef WIN32
#ifdef LIB_TRADER_API_EXPORT
#define TRADER_API_EXPORT __declspec(dllexport)
#else
#define TRADER_API_EXPORT __declspec(dllimport)
#endif
#endif

#ifdef LINUX
#ifdef LIB_TRADER_API_EXPORT
#define TRADER_API_EXPORT __attribute__((visibility("default")))
#else
#define TRADER_API_EXPORT
#endif
#endif

#else
#define TRADER_API_EXPORT
#endif

//�ص�����
class CMarkDataTraderSpi
{
public:
    ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
    virtual void OnFrontConnected(){};

    ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
    virtual void OnFrontDisconnected(int nReason){};

    ///338499 ����ؿ�Ӧ��
    virtual void OnRspFuncMarkdatainsert(CMarkDataRspFuncMarkdatainsertField *pRspFuncMarkdatainsert, CMarkDataRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

};

///����
class TRADER_API_EXPORT CMarkDataTraderApi
{
public:
    ///����TraderApi
    static CMarkDataTraderApi *CreateMarkDataTraderApi(const char *pszFlowPath = "", bool bSaveLog = false);

    ///ɾ���ӿڶ�����
    virtual void ReleaseApi() = 0;

    ///��ʼ����
    virtual void Init(const char *pszLicFile, const char *pszPwd = "", const char *pszSslFile = "", const char *pszSslPwd = "") = 0;

    ///�ȴ��ӿ��߳̽�������
    virtual int Join() = 0;

    ///ע��ǰ�û������ַ
    ///@param pszFrontAddress��ǰ�û������ַ��
    ///@param nSubSystemNo����ϵͳ��(���ݾ����ڻ���˾����������һ���ڻ���115)
    ///@param nCompanyID����˾���(�Խ�ITNʱ��Ҫ�����Ծ��������ITNѯ��)
    virtual void RegisterFront(const char *pszFrontAddress, int nSubSystemNo = 0, int nCompanyID = 0, const char *pszConnName = "") = 0;

    ///ע��ص��ӿ�
    ///@param pSpi �����Իص��ӿ����ʵ��
    virtual void RegisterSpi(CMarkDataTraderSpi *pSpi) = 0;

    ///�����û���Ϣ
    virtual CMarkDataUserInfoField* GetUserInfo() = 0;

    ///338499 ����ؿ�����
    virtual int ReqFuncMarkdatainsert(CMarkDataReqFuncMarkdatainsertField *pReqFuncMarkdatainsert, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CMarkDataRspInfoField *pRspInfo = NULL) = 0;

protected:
    ~CMarkDataTraderApi(){};
};

#endif
