/**********************************************************************
* Դ��������: h5sdk_impl.h
* ������־:
* 2016-01-27 ��������ʵ���첽���͹���
**********************************************************************/

#if !defined(H5SDK_IMPL_H)
#define H5SDK_IMPL_H

#include "h5sdk.h"
#include "public.h"

class CH5sdkImpl : public H5SdkCallback
{
public:
	///���Ự���ӳɹ�ʱ�ص�
    virtual void OnConnect(Session *session, const char *peerIp, int peerPort, const char *localIp, int localPort);

    ///���Ự��¼�ɹ�ʱ�ص�
    virtual void OnSdkLogin(Session *session);

    ///���Ự�յ��첽��Ϣʱ�ص�
    ///response��Ҫҵ������ͷ�
    virtual void OnReceived(Session *session, IHsCommMessage *response);

    ///���Ự���ӶϿ�ʱ�ص�
    virtual void OnClose(Session *session, int reason);

    ///���Ự����ʱ�ص�
    ///errorNoĿǰ����ΪH5SDK_SDK_LOGIN_TIMEOUT����ʾsdk��¼��ʱ
    virtual void OnError(Session *session, int errorNo);

    ///������ָ����������Ȼ�޷�������ص�
    virtual void OnCore(Session *session);
public:
	///���캯��
	CH5sdkImpl();

	///��������
	~CH5sdkImpl();

	//�߳̿�ʼ
	void StartTrd();

	//�߳�ֹͣ
	void StopTrd();

	///��ʼ��
	int InitH5sdk();

	//���������(֧��ͬ��)
	int SendCommMessage(IHsCommMessage* lpRequest, int nRequestID);

public:
	///���ӻص�
	virtual void OnConnected() = 0;

	///�Ͽ��ص�
	virtual void OnClosed(int nReason) = 0;

	///�����첽HsCommMessage
	virtual void DealAsyCommMessage(IHsCommMessage* lpResponse) = 0;

protected:
	//�Ựѡ��
	SessionOptions*           lpSessionOpt;
	//�Ự
	Session*                  lpSession;
	//ǰ�õ�ַ  
	char                      m_szFrontAddress[64];

private:
	//�첽Ӧ������ն���
	CMessageQueue<IHsCommMessage *> msgList;
	//�����첽Ӧ����¼�
	CEventEx                     hStopEvent;
	//�����첽Ӧ���״̬
	volatile bool                bStopAsy;
	//�����첽Ӧ����߳�
	CThread                      hTrdDealAsyComm;

	///��Ϣ�������
	void AddCommMsg(Session *session, IHsCommMessage *response);

	///ɾ����Ϣ��
	void DelCommMsg();

	///�����첽Ӧ����̺߳���
	static void RunTrdDealAsyComm(void* lpdata);
};

#endif

