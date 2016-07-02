#ifndef __H5SDK_H__
#define __H5SDK_H__

#include "h5sdk_export.h"
#include "velocimetry.h"

#include <Include/hscomm_message_factory_interface.h>
#include <Include/hscomm_message_interface.h>
#include <Include/sdk_tag.h>

// �Ựѡ��
class SessionOptions: public IHSKnown {
public:
    virtual ~SessionOptions() {}

    // ����������ʱʱ�䣬Ĭ��Ϊ8�룬������ʱ�����ӽ����Ͽ�
    virtual int H5SDKAPI SetHeartbeatTimeout(int seconds) = 0;
    // ��ȡ������ʱʱ��
    virtual int H5SDKAPI GetHeartbeatTimeout() = 0;

    // ����ָ���±�ķ����IP��Ĭ��Ϊ"127.0.0.1"
    virtual int H5SDKAPI SetServerIp(const char *ip, size_t index = 0) = 0;
    // ����ָ���±�ķ���˶˿ڣ�Ĭ��Ϊ8888
    virtual int H5SDKAPI SetServerPort(unsigned short port, size_t index = 0) = 0;

    // ��ȡָ���±�ķ����IP
    virtual const char * H5SDKAPI GetServerIp(size_t index = 0) = 0;
    // ��ȡָ���±�ķ���˶˿�
    virtual unsigned short H5SDKAPI GetServerPort(size_t index = 0) = 0;

    // ��ȡ���õķ���˸���
    virtual size_t H5SDKAPI GetServerCount() = 0;

    // ���������������λΪ�룬Ĭ��Ϊ3��
    virtual int H5SDKAPI SetReconnintvl(int reconnintvl) = 0;
    // ��������������Ĭ��Ϊ5��
    virtual int H5SDKAPI SetReconnretries(int reconnretries) = 0;

    // ��ȡ�������
    virtual int H5SDKAPI GetReconnintvl() = 0;
    // ��ȡ��������
    virtual int H5SDKAPI GetReconnretries() = 0;
};

const int H5SDK_DISCONNECT          = -1;
const int H5SDK_TIMEOUT             = -3;
const int H5SDK_SDK_LOGIN_ERROR     = 1010;
const int H5SDK_SERIALIZATION_ERROR = 1002;
const int H5SDK_SET_DECODER_ERROR   = 1004;
const int H5SDK_SDK_LOGIN_TIMEOUT   = 1005;

class Session;

// H5Sdk�ص��ӿ�
class H5SdkCallback
{
public:
    // ���Ự���ӳɹ�ʱ�ص�
    virtual void OnConnect(Session *session, const char *peerIp, int peerPort, const char *localIp, int localPort) = 0;

    // ���Ự��¼�ɹ�ʱ�ص�
    virtual void OnSdkLogin(Session *session) = 0;

    // ���Ự�յ��첽��Ϣʱ�ص�
    // response��Ҫҵ������ͷ�
    virtual void OnReceived(Session *session, IHsCommMessage *response) = 0;

    // ���Ự���ӶϿ�ʱ�ص�
    virtual void OnClose(Session *session, int reason) = 0;

    // ���Ự����ʱ�ص�
    // errorNoĿǰ����ΪH5SDK_SDK_LOGIN_TIMEOUT����ʾsdk��¼��ʱ
    virtual void OnError(Session *session, int errorNo) = 0;

    // ������ָ����������Ȼ�޷�������ص�
    virtual void OnCore(Session *session) = 0;
};

// �Ự
class Session: public IHSKnown {
public:
    virtual ~Session() {}

    // ����ָ��ҵ��š����ܺš������͵���Ϣ
    // �÷��������ڻص��ӿ�H5SdkCallback�ķ���OnSdkLogin�ص���ſ��Ե���
    virtual IHsCommMessage * H5SDKAPI CreateMessage(int businessId, int functionId, int packetType) = 0;

    // ͬ������
    // ͬ�����ò����յ��첽Ӧ��
    // ��ʱӦ��ᱻ�Զ����������ᱻ����SyncCall�յ�
    // @return 0 ��ʾ �ɹ�
    // @return H5SDK_DISCONNECT ��ʾ ����δ�������ѶϿ�
    // @return H5SDK_SERIALIZATION_ERROR ��ʾ request���л�ʧ�ܣ�����ԭ��ɵ���request��GetLastErrInfo�������鿴
    // @return H5SDK_TIMEOUT ��ʾ ���ó�ʱ
    virtual int H5SDKAPI SyncCall(IHsCommMessage *request, IHsCommMessage **response, int milliseconds) = 0;

    // �첽����
    // @return 0 ��ʾ �ɹ�
    // @return H5SDK_DISCONNECT ��ʾ ����δ�������ѶϿ�
    // @return H5SDK_SERIALIZATION_ERROR ��ʾ request���л�ʧ�ܣ�����ԭ��ɵ���request��GetLastErrInfo�������鿴
    virtual int H5SDKAPI AsyncSend(IHsCommMessage *request) = 0;

    // ��ȡ���Ự����������Ϣ����
    virtual IHsComMessageFactory * H5SDKAPI GetMessageFactory() = 0;

    // ���ûص��ӿ�
    // �ýӿڱ�����StartSetCallback֮��StartAllSessions֮ǰ����
    // @return 0 ��ʾ ���óɹ�
    // @return H5SDK_SET_DECODER_ERROR ��ʾ ����SetDecoderʧ��
    virtual int H5SDKAPI SetCallback(H5SdkCallback *h5SdkCallback) = 0;

    // �����첽����
    // ���Ҫʵ�ֶ������������ڻص��ӿ�H5SdkCallback��OnCore��������е���
    // StartAllSessions���ú󣬻��Զ��������лỰ��AsyncConnect
    // �����ӳɹ�ʱ��ͨ���ص��ӿ�H5SdkCallback�ķ���OnConnect����֪ͨ
    // ������һֱ�޷��������ͨ���ص��ӿ�H5SdkCallback�ķ���OnCore����֪ͨ
    // @return 0 ��ʾ ����ɹ�
    virtual int H5SDKAPI AsyncConnect() = 0;

    // �����첽Sdk��¼
    // millseconds��ʾ��ʱʱ�䣬��λ����
    // �÷����ڻص��ӿ�H5SdkCallback��OnConnect��������е���
    // ��Sdk��¼�ɹ�ʱ��ͨ���ص��ӿ�H5SdkCallback�ķ���OnSdkLogin����֪ͨ
    // ��Sdk��¼��ʱʱ��ͨ���ص��ӿ�H5SdkCallback�ķ���OnError����֪ͨ������errorNoΪH5SDK_SDK_LOGIN_TIMEOUT
    // ��OnSdkLogin�ص�֮��ſ��Է���ҵ������
    // @return 0 ��ʾ ����ɹ�
    virtual int H5SDKAPI AsyncSdkLogin(int millseconds) = 0;

    // �����첽����
    // ���ӿں�AsyncConnect����Ҫ�����Ǳ��ӿڿ�������ʱָ��IP�Ͷ˿�
    // @return 0 ��ʾ ����ɹ�
    virtual int H5SDKAPI AsyncConnectEx(const char *ip, unsigned short port) = 0;
};

#endif
