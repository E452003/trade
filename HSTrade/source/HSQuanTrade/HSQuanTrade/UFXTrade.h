#pragma once

#include "include/t2sdk_interface.h"

#include "BaseTradeApi.h"

#define SYNCSEND		0	///ͬ������

#define ASYNCSEND		1	///�첽����

#define USERSECTION		"user"

#define T2SDK           "t2sdk"

#define ISSUE_TYPE_REALTIME_SECU 	33099//12			///֤ȯ�ɽ�����

class CUFXTrade;
class CSECUTrade;
//CBusiness
//�������ڻ���ʵ����
class CBusiness
{
public:
	CBusiness();

	virtual ~CBusiness();

	void SetConfig(CConfigInterface *pConfig)
	{
		m_pConfig = pConfig;
	}

	int DealReceive(int funcid,IF2UnPacker *unpacker,int reqid);

	int onRsp331100(IF2UnPacker *unpacker,int reqid);

	int onRsp338202(IF2UnPacker *unpacker,int reqid);

	int onRsp338217(IF2UnPacker *unpacker,int reqid);

	int onRsp338301(IF2UnPacker *unpacker,int reqid);

	//-----------------------�ڻ�----------------------------
	int Login(char * account,char * pwd,int reqid);

	int FutureEntrust(int systemid,char * fundaccount,char * pwd,char * instrument, int count, char direction, char offsetflag, double price,int reqid);

	int WithDraw(char * account,char * pwd,int ref,int reqid);

	int QueryEntrust(char * account,char * pwd,int entrustno,int reqid);

	int FutureAccountInfo(char * account,char * pwd,int reqid);

	int QryFuturePosition(char * account,char * pwd,int reqid);

	int Subscribe(int issue_type,int reqid); ///��������

	int CancelSub(int issueType,int reqid);


	
private:
	CConfigInterface *m_pConfig;

	int SendBizMessage(IF2Packer *lpF2Packer, int nFunction, int nIssueType, int nRequestID, void** ppRetData, int* pnItems, void *pRspInfo);

public:	

	CConnectionInterface * g_pConnection;

	CUFXTrade * m_pTrade;
};




//֤ȯ��ʵ����
class CSECUBusiness
{
public:
	CSECUBusiness();

	virtual ~CSECUBusiness();

	void SetConfig(CConfigInterface *pConfig)
	{
		m_SecuConfig = pConfig;
	}

	int DealReceive(int funcid,IF2UnPacker *unpacker,int reqid);

	int onRsp331100(IF2UnPacker *unpacker,int reqid);
	
	int onRsp333002(IF2UnPacker *unpacker,int reqid);    //�µ�

	int onRsp333017(IF2UnPacker *unpacker,int reqid);    //����
	



	


	//-----------------------֤ȯ----------------------------

	int Login(char * account,char * pwd,int reqid);

	int SecuEntrust(int systemid,char * fundaccount,char * pwd,char * instrument, int count, char direction, double price,int reqid);

	int SearchSecuEntrust(char * fundaccount,char * pwd,char * exchtype,int reqid);

	int SecuEntrustWithdraw(char * fundaccount,char * pwd,int entrustno,int reqid);

	int SecuRealtimeQry(char * fundaccount,char * pwd,int reqid);

    int SecuStockQry(char * fundaccount,char * pwd,int reqid);

	int ClientFundAllQry(char * fundaccount,char * pwd,int reqid);

	int Subscribe(int issue_type,int reqid); ///��������

	int CancelSub(int issueType,int reqid);


private:
	CConfigInterface *m_SecuConfig;

	int SendBizMessage(IF2Packer *lpF2Packer, int nFunction, int nIssueType, int nRequestID, void** ppRetData, int* pnItems, void *pRspInfo);


public:	

	CConnectionInterface * g_SecuConnection;

	CSECUTrade * m_secuTrade;
};



//�ص�

class CCallback : public CCallbackInterface
{
public:
    // ��ΪCCallbackInterface�����մ��������IKnown��������Ҫʵ��һ����3������
    unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);

    unsigned long  FUNCTION_CALL_MODE AddRef();

    unsigned long  FUNCTION_CALL_MODE Release();
	
    // �����¼�����ʱ�Ļص�������ʵ��ʹ��ʱ���Ը�����Ҫ��ѡ��ʵ�֣����ڲ���Ҫ���¼��ص���������ֱ��return
    // Reserved?Ϊ����������Ϊ�Ժ���չ��׼����ʵ��ʱ��ֱ��return��return 0��
    void FUNCTION_CALL_MODE OnConnect(CConnectionInterface *lpConnection);

    void FUNCTION_CALL_MODE OnSafeConnect(CConnectionInterface *lpConnection);

    void FUNCTION_CALL_MODE OnRegister(CConnectionInterface *lpConnection);

    void FUNCTION_CALL_MODE OnClose(CConnectionInterface *lpConnection);

    void FUNCTION_CALL_MODE OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData);

    void FUNCTION_CALL_MODE Reserved1(void *a, void *b, void *c, void *d);

    void FUNCTION_CALL_MODE Reserved2(void *a, void *b, void *c, void *d);

    int  FUNCTION_CALL_MODE Reserved3();

    void FUNCTION_CALL_MODE Reserved4();

    void FUNCTION_CALL_MODE Reserved5();

    void FUNCTION_CALL_MODE Reserved6();

    void FUNCTION_CALL_MODE Reserved7();

    void FUNCTION_CALL_MODE OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult);

	void FUNCTION_CALL_MODE OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult);
	
	void FUNCTION_CALL_MODE OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg);

public:

	CBusiness * m_pBusiness;     //�ڻ�

	CSECUBusiness * m_secuBusiness;   //֤ȯ

	void CallbackLog(char *msg);
};


//�ڻ�UFX
class CUFXTrade :public  CBaseTradeApi
{
public:
	CUFXTrade(void);

	~CUFXTrade(void);

	virtual void init(CString serverAddress);

	CConfigInterface * m_lpConfig; 

	CBusiness g_szBusiness;

	virtual int InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int orderref);

	virtual int WithDraw(int entrustno,char *sCode);

	virtual int QueryEntrust(int entrustno,int reqid);

	virtual void RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount);

	virtual void RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins);

	virtual void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid); 

	CCallback  m_szCallback;


	//����״̬��Ϣ
	virtual bool IsWeiBao(char status){return false;};    //δ��

	virtual bool IsDaiBao(char status){return false;};    //����

	virtual bool IsYiBao(char status){return false;};     //�ѱ�

	virtual bool IsBuChe(char status){return false;};     //����

	virtual bool IsYiChe(char status){return false;};     //�ѳ�

	virtual bool IsBuCheng(char status){return false;};   //����

	virtual bool IsYiCheng(char status){return false;};   //�ѳ�

	virtual bool IsFeiDan(char status){return false;};    //�ϵ�

	virtual std::string TransBS(char bs){return "";};

	virtual std::string TransKP(char kp){return "";};

	virtual std::string TransStatus(char st){return "";};
};


//֤ȯUFX
class CSECUTrade:public CBaseTradeApi
{
public:
	CSECUTrade(void);

	~CSECUTrade(void);

	virtual void init(CString serverAddress);

	CConfigInterface * m_secuConfig;

	CSECUBusiness g_secuBusiness;

	virtual int InsertOrder(char* instrument,int count,char direction,char offsetflag,double price,int reqid);

	virtual int WithDraw(int entrustno,char *sCode);

	virtual int QueryEntrust(int entrustno,int reqid);

	virtual void RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount);

	virtual void RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins);

	virtual void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid);

	CCallback  m_secuCallback;

	//����״̬��Ϣ
	virtual bool IsWeiBao(char status){return false;};    //δ��

	virtual bool IsDaiBao(char status){return false;};    //����

	virtual bool IsYiBao(char status){return false;};     //�ѱ�

	virtual bool IsBuChe(char status){return false;};     //����

	virtual bool IsYiChe(char status){return false;};     //�ѳ�

	virtual bool IsBuCheng(char status){return false;};   //����

	virtual bool IsYiCheng(char status){return false;};   //�ѳ�

	virtual bool IsFeiDan(char status){return false;};    //�ϵ�

	virtual std::string TransBS(char bs){return "";};

	virtual std::string TransKP(char kp){return "";};

	virtual std::string TransStatus(char st){return "";};

	virtual int GetInterfaceType(){return TRADE_UFX_S;};

};


