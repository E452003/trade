#if !defined(T2_CONNECTION_IMPL_H)
#define T2_CONNECTION_IMPL_H

#include <t2sdk_interface.h>
#include <public.h>

void InitGlobalValues();

class CQueryInfo
{
public:
	CQueryInfo(int iFuncid, int iRepid, void* pData, int iLen)
	{
		this->iApiRequestID = NewRequestId();
		this->iFuncionID = iFuncid;
		this->iRequestID = iRepid;
		if(pData)
		{
			this->ReqData = malloc(iLen);
			memcpy( this->ReqData, pData, iLen);
		}
	};
	~CQueryInfo()
	{
		free(this->ReqData);
	};
	int NewRequestId()
	{
		return currRequestId.Increase();
	}
	void* GetReqData()
	{
		return ReqData; 
	}
	const int GetApiRequestID()
	{
		return iApiRequestID;
	};
	const int GetRequestID()  
	{
		return iRequestID; 
	};
private:
	//����Ψһ���
	static CThreadSafeValue currRequestId;
	int iFuncionID;
	int iRequestID;
	int iApiRequestID;
	void* ReqData;
};

class CConnectionCenterItem
{
public:
	CConnectionCenterItem() 
	{ 
		bIsDying = false; 
	}
	virtual void OnConnected() = 0;
	virtual void OnClosed() = 0;
	virtual CConnectionInterface* GetT2Connection() = 0;
	virtual bool AutoReConnect() = 0;
	bool  IsDying(){  return bIsDying; }
protected:
	void ImDying() { bIsDying = true; };
	bool bIsDying;
};

class CConnectionCenter
{
private:
	typedef std::set<CConnectionCenterItem*> CenterItemSet;
	typedef std::list<CConnectionCenterItem*> CenterItemList;
	typedef std::map<CConnectionCenterItem*,CEventEx*> ConnectEvenMap;
	typedef std::queue<CConnectionCenterItem*> CenterItemQueue;
	volatile bool bStop;
	ManualLock logiclock;
	ManualLock dataLock;

	CenterItemSet reconnSet;
	CenterItemList notifyList;
	CenterItemQueue connecions;
	ConnectEvenMap eventMap;

	CEventEx readyEvent;
	CThread trd;
	static void Reconnect(void* lpvoid);
public:
	static CConnectionCenter* GetInstance();
	CConnectionCenter();
	~CConnectionCenter();
	void RemoveMine(CConnectionCenterItem* item);
	void ReConnect(CConnectionCenterItem* item, bool bsync, bool bNotifyClosed);
};

//T2ͨ�Žӿڵ�ʵ��
class CT2ConnectionImpl : public CCallbackInterface, public CConnectionCenterItem
{
public:
	// ��ΪCCallbackInterface�����մ��������IKnown��������Ҫʵ��һ����3������
	unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv) {return 0;};
	unsigned long  FUNCTION_CALL_MODE AddRef() {return 0;};
	unsigned long  FUNCTION_CALL_MODE Release() {return 0;};

	// �����¼�����ʱ�Ļص�������ʵ��ʹ��ʱ���Ը�����Ҫ��ѡ��ʵ�֣����ڲ���Ҫ���¼��ص���������ֱ��return
	// Reserved?Ϊ����������Ϊ�Ժ���չ��׼����ʵ��ʱ��ֱ��return��return 0��
	void FUNCTION_CALL_MODE OnConnect(CConnectionInterface *lpConnection);
	void FUNCTION_CALL_MODE OnSafeConnect(CConnectionInterface *lpConnection);
	void FUNCTION_CALL_MODE OnRegister(CConnectionInterface *lpConnection);
	void FUNCTION_CALL_MODE OnClose(CConnectionInterface *lpConnection);
	void FUNCTION_CALL_MODE OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData){};
	void FUNCTION_CALL_MODE Reserved1(void *a, void *b, void *c, void *d){};
	void FUNCTION_CALL_MODE Reserved2(void *a, void *b, void *c, void *d){};
	int  FUNCTION_CALL_MODE Reserved3(){return 0;};
	void FUNCTION_CALL_MODE Reserved4(){};
	void FUNCTION_CALL_MODE Reserved5(){};
	void FUNCTION_CALL_MODE Reserved6(){};
	void FUNCTION_CALL_MODE Reserved7(){};
	void FUNCTION_CALL_MODE OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult){};
	void FUNCTION_CALL_MODE OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult){};
	void FUNCTION_CALL_MODE OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage *lpMsg);

public:
	//���캯��
	CT2ConnectionImpl();

	//��������
	~CT2ConnectionImpl();

	//��ȡT2���ö���
	CConfigInterface* GetT2Config();

	//��ȡT2ͨ�Ŷ���
	CConnectionInterface* GetT2Connection();

	//�Ƿ��Զ�����
	bool AutoReConnect();

	//���������(֧��ͬ��)
	int SendBizMessage(IF2Packer *lpF2Packer, int nFunction, int nIssueType, int nRequestID, void** ppRetData, int* pnItems, void *pRspInfo);

	//���QueryInfo
	void AddCQueryInfo(CQueryInfo *qryinfo);

	//����QueryInfo
	void DelQuery(CQueryInfo *qryinfo);

	//���CQueryInfo
	CQueryInfo *GetCQueryInfo(int request);

public:
	//�����첽BizMessage
	virtual void DealAsyBizMessage(IBizMessage* lpMsg) = 0;

	//����ͬ��BizMessage
	virtual int DealSynBizMessage(const void* lpBuffer, int nLen, void** ppRetData, int* pnItems, void *pRspInfo) = 0;

	//��������ɹ�
	virtual void RecHeartbeet() = 0;

	//���T2ͨѶ������Ϣ
	virtual void GetMsgErrorInfo(int nErrorCode, void *pRspInfo) = 0;

protected:
	//T2���ö���
	CConfigInterface*      lpConfig;
	//T2���Ӷ���
	CConnectionInterface*  lpConnection;
	//��ϵͳ��
	int                    m_SubSystemNo; 
	//��˾���
	int                    m_CompanyID;
	//ǰ�õ�ַ  
	char                   m_szFrontAddress[64];
	//T2�������ӵ�������  
	char                   m_szConnName[32];

	//�߳̿�ʼ
	void StartTrd();

	//�߳�ֹͣ
	void StopTrd();

	//��ʼ������
	void InitT2Connect(const char *pszLicFile, const char *pszPwd, const char *pszSslFile, const char *pszSslPwd);

private:
	//�첽Ӧ������ն���
	CMessageQueue<IBizMessage *> msgList;
	//�����첽Ӧ����¼�
	CEventEx                     hStopEvent;
	//�����첽Ӧ���״̬
	volatile bool                bStopAsy;
	//�����첽Ӧ����߳�
	CThread                      hTrdDealAsyBiz;
	//���������
	ManualLock                   QueryLock;
	//����Խ�
	map<int, CQueryInfo*>        QueryMap;

	//��Ϣ�������
	void AddBizMsg(CConnectionInterface *lpConnection, IBizMessage* lpData);

	//ɾ����Ϣ��
	void DelBizMsg();

	//�����첽Ӧ����̺߳���
	static void RunTrdDealAsyBiz(void* lpdata);
};

#endif
