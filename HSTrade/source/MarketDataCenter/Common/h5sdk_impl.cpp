#include "h5sdk_impl.h"

void CH5sdkImpl::OnConnect(Session *session, const char *peerIp, int peerPort, const char *localIp, int localPort)
{
	printf("�����ѽ���: ʹ�ñ�����IP��ַ[%s]�˿�[%d]���ӵ�Զ��IP��ַ[%s]�˿�[%d]\n", localIp, localPort, peerIp, peerPort);

	// H5SDK��¼
	lpSession->AsyncSdkLogin(1000);

	OnConnected();
}

void CH5sdkImpl::OnSdkLogin(Session *session)
{
	printf("H5SDK�ײ��¼�ɹ�\n");

	// ����ҵ���¼��ֻ����������ã�
	IHsCommMessage *loginRequest = session->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_LOGIN, REQUEST);
	loginRequest->GetItem(H5SDK_TAG_USER_NAME)->SetString("any name is ok");
	loginRequest->GetItem(H5SDK_TAG_HQ_TOKEN)->SetString("any token is ok");
	int ret = session->AsyncSend(loginRequest);
	if (ret != 0)
	{
		cout << "session->AsyncSend() = " << ret << endl;
	    return;
	}
	loginRequest->Release();
}

void CH5sdkImpl::OnReceived(Session *session, IHsCommMessage *response)
{
	AddCommMsg(session, response);
}

void CH5sdkImpl::OnClose(Session *session, int reason)
{
	printf("���ӶϿ�: ԭ��[%d]\n", reason);
	OnClosed(reason);
}

void CH5sdkImpl::OnError(Session *session, int errorNo)
{
	printf("��������: �����[%d]\n", errorNo);
}

void CH5sdkImpl::OnCore(Session *session)
{
	printf("���Զ�����Ӻ���Ȼ�޷����ӳɹ�\n");
}

CH5sdkImpl::CH5sdkImpl()
{
	lpSessionOpt = NULL;
	lpSession = NULL;
	memset(m_szFrontAddress, 0, sizeof(m_szFrontAddress));

	bStopAsy = true;
}

CH5sdkImpl::~CH5sdkImpl()
{

}

void CH5sdkImpl::StartTrd()
{
	if(bStopAsy)
	{
		bStopAsy = false;
		hStopEvent.Reset();
		hTrdDealAsyComm.Start(RunTrdDealAsyComm, 0, this);
	}
}

void CH5sdkImpl::StopTrd()
{
	if(!bStopAsy)
	{
		bStopAsy = true;
		hStopEvent.Wait(-1);
		DelCommMsg();
	}
}

int CH5sdkImpl::InitH5sdk()
{
	if(strcmp(m_szFrontAddress, "") == 0)
		return -1;

	char IP[64];
	char Port[16];

	char* toksaveptr;
	char* token;
	token = strtok_t(m_szFrontAddress, ":", &toksaveptr);
	hs_strncpy(IP, token, sizeof(IP));
	hs_strncpy(Port, toksaveptr, sizeof(Port));

	// �����Ựѡ��
	lpSessionOpt = CreateSessionOptions();
	lpSessionOpt->AddRef();
	// ���ûỰѡ��
	lpSessionOpt->SetHeartbeatTimeout(3);
	lpSessionOpt->SetServerIp(IP);
	lpSessionOpt->SetServerPort(atoi(Port));

	// ͨ���Ựѡ����Ự
	lpSession = CreateSession(lpSessionOpt);
	lpSession->AddRef();
	// ��ʼ���ûص�
	StartSetCallback();
	// ���ûỰ�Ļص�
	lpSession->SetCallback(this);
	// ��ʼ���лỰ
	StartAllSessions();

	return 0;
}

int CH5sdkImpl::SendCommMessage(IHsCommMessage* lpRequest, int nRequestID)
{
	lpRequest->GetItem(H5PROTO_TAG_USER_KEY)->SetInt32(nRequestID);
	return lpSession->AsyncSend(lpRequest);
}

void CH5sdkImpl::AddCommMsg(Session *session, IHsCommMessage *response)
{
	session->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_MARKET_TYPES, REQUEST);

	if(!bStopAsy)
	{
		IHsCommMessage* lpMsg = response->Clone();

		msgList.Push(lpMsg);
	}
}

void CH5sdkImpl::DelCommMsg()
{
	IHsCommMessage* lpMsg;
	while((lpMsg = msgList.Pop(0)) != NULL)
	{
		lpMsg->Release();
	}
}

void CH5sdkImpl::RunTrdDealAsyComm(void* lpdata)
{
	CH5sdkImpl *pros = (CH5sdkImpl *)lpdata;
	while(pros->bStopAsy == false)
	{
	    IHsCommMessage* msg = pros->msgList.Pop(1000);
	    if(msg != NULL)
	    {
	        pros->DealAsyCommMessage(msg); 
	        msg->Release();
	    }
	}
	pros->hStopEvent.Notify();
}
