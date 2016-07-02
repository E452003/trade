/** @file
* h5sdk��̬����������
* @author  Ӧ���
* @author  �������ӹɷ����޹�˾
* @version 1.0
* @date    20140428
*/

#ifndef __H5SDK_EXPORT_H__
#define __H5SDK_EXPORT_H__

#ifdef _WIN32
#  define H5SDKAPI __stdcall
#else
#  define H5SDKAPI
#endif

class SessionOptions;
class Session;
class Velocimetry;

extern "C" {
    // ��ȡSDK�汾
    const char * H5SDKAPI GetSdkVersion();

    // �����Ựѡ��SessionOptions
    SessionOptions * H5SDKAPI CreateSessionOptions();

    // �ûỰѡ��SessionOptions�������ỰSession
    Session * H5SDKAPI CreateSession(SessionOptions *sessionOptions);

    // �������ٶ���Velocimetry
    Velocimetry * H5SDKAPI CreateVelocimetry();

    // ��ȡ����Ŷ�Ӧ�Ĵ�����Ϣ
    const char * H5SDKAPI GetErrorStringByErrorNo(int errorNo);

    // ��ʼ�������лỰ�Ļص��ӿ�
    // �ýӿڱ��������лỰ���������е���
    int H5SDKAPI StartSetCallback();

    // �������лỰ
    int H5SDKAPI StartAllSessions();
}

#endif
