#if !defined(HUNDSUNFUTUMDAPI_H)
#define HUNDSUNFUTUMDAPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include "HundsunFutuUserApiStruct.h"

#if defined(ISLIB)

#ifdef WIN32
#ifdef LIB_MD_API_EXPORT
#define MD_API_EXPORT __declspec(dllexport)
#else
#define MD_API_EXPORT __declspec(dllimport)
#endif
#endif

#ifdef LINUX
#ifdef LIB_MD_API_EXPORT
#define MD_API_EXPORT __attribute__((visibility("default")))
#else
#define MD_API_EXPORT
#endif
#endif

#else
#define MD_API_EXPORT
#endif

///�ص�����
class CHundsunFutuMdSpi
{
public:
    ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
    virtual void OnFrontConnected(){};

    ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
    virtual void OnFrontDisconnected(int nReason){};

    ///331100 �ͻ���¼Ӧ��
    virtual void OnRspFuncClientLogin(CHundsunFutuRspFuncClientLoginField *pRspFuncClientLogin, CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///330851 �ڻ���Լ��֤���ѯӦ��
    virtual void OnRspFuncFutuContractbailQry(CHundsunFutuRspFuncFutuContractbailQryField *pRspFuncFutuContractbailQry, CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///620001_33100 ����-�ڻ���������Ӧ��
    virtual void OnRspFuncQhSingleHqSubscribe(CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///620002_33100 ����ȡ��-�ڻ���������Ӧ��
    virtual void OnRspFuncQhSingleHqCancel(CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///620003_33100 ����-�ڻ���������Ӧ��
    virtual void OnRtnFuncQhSingleHqPush(CHundsunFutuRtnFuncQhSingleHqPushField *pFuncQhSingleHqPush) {};

    ///620001_33102 ����-�ڻ��������Ӧ��
    virtual void OnRspFuncQhCombineSubscribe(CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///620002_33102 ����ȡ��-�ڻ��������Ӧ��
    virtual void OnRspFuncQhCombineCancel(CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///620003_33102 ����-�ڻ��������Ӧ��
    virtual void OnRtnFuncQhCombinePush(CHundsunFutuRtnFuncQhCombinePushField *pFuncQhCombinePush) {};

};

///����
class MD_API_EXPORT CHundsunFutuMdApi
{
public:
    ///����MdApi
    static CHundsunFutuMdApi *CreateHundsunFutuMdApi(const char *pszFlowPath = "", bool bSaveLog = false);

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
    virtual void RegisterSpi(CHundsunFutuMdSpi *pSpi) = 0;

    ///�����û���Ϣ
    virtual CHundsunFutuUserInfoField* GetUserInfo() = 0;

    ///331100 �ͻ���¼����
    virtual int ReqFuncClientLogin(CHundsunFutuReqFuncClientLoginField *pReqFuncClientLogin, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL) = 0;

    ///330851 �ڻ���Լ��֤���ѯ����
    virtual int ReqFuncFutuContractbailQry(CHundsunFutuReqFuncFutuContractbailQryField *pReqFuncFutuContractbailQry, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL) = 0;

    ///620001_33100 ����-�ڻ�������������
    virtual int ReqFuncQhSingleHqSubscribe(char *ppInstrumentID[], int nCount, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL) = 0;

    ///620002_33100 ����ȡ��-�ڻ�������������
    virtual int ReqFuncQhSingleHqCancel(char *ppInstrumentID[], int nCount, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL) = 0;

    ///620001_33102 ����-�ڻ������������
    virtual int ReqFuncQhCombineSubscribe(char *ppInstrumentID[], int nCount, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL) = 0;

    ///620002_33102 ����ȡ��-�ڻ������������
    virtual int ReqFuncQhCombineCancel(char *ppInstrumentID[], int nCount, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL) = 0;

protected:
    ~CHundsunFutuMdApi(){};
 };

#endif
