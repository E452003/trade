#ifndef _H5HQ_API_INTERFACE_H__
#define _H5HQ_API_INTERFACE_H__

#include "h5hq_data_interface.h"		//	���Ⱪ¶�Ĺ�Ʊ�ṹ

#ifdef __cplusplus
extern "C" {
#endif

/// ����״̬֪ͨ�ص���������
/**
	enumExpireStatus eExpireStatus:	����״̬��
	unsigned int nExpireDate:		�������ڡ���ʽYYYYMMDD
	const char *szExpireTip:		������ʾ�ַ�����
 **/
typedef int (FUNCTION_CALL_MODE * FP_ON_EXPIRE)(enumExpireStatus eExpireStatus, unsigned int nExpireDate, const char *szExpireTip);


/// ��H5API������Ȩ����ȷ���ڵ���InitApiǰ���ô˺�����ֻ������ȷ���ô˺�������Ȩ�ɹ����������ʹ��h5api��
/**
	��Σ�
		const char *lpLicenseFile:	�����Ȩxml�ļ�������"h5apilicense.xml"
		��Ȩ��ʽ���£�

			<?xml  version="1.0" encoding="GBK"?>
			<root>
				<licensekey note="�����Կ">jx8k16XitJM=</licensekey>
				<toexpiretip note="��Ҫ������ʾ����ǰһ���½�����ʾ��ÿ10���ӻص���ʾһ�¡�%u�����10����������ʽ�Ķ��������ڣ�%s�������ʽ��2014��2��1�յĵ����ա�����ʱ�뱣֤%u��%s֮ǰ��">H5API����%u����ڣ�������Ϊ%s��</toexpiretip>
				<expiredtip note="������ʾ����ǰ�������ڴ��ڹ����պ�ÿ10���ӻص���ʾһ�¡�%s�������ʽ��2014��2��1�յĹ�����">H5API�Ѿ������ˣ�������Ϊ%s��</expiredtip>
			</root>

		FP_ON_EXPIRE fpOnExpire:	����״̬�ص��������˺���������ȷ�����Ҳ���Ϊ�գ�
			
			������ʲô����£�AuthorizeApi�л�����ͬ���ص�fpOnExpire������
			����AuthorizeApi�������غ󣬻�����Ȩ����첽�ص�fpOnExpire������

			��Ȩ����:
				eExpireStatusΪES_FAIL��nExpireDateΪ0��szExpireTipΪ����������AuthorizeApi����-1��
				AuthorizeApi���غ��ÿ��10�����첽�ص�һ��fpOnExpire����������h5apiʹ�÷��ܼ�ʱ���ִ���
			��Ȩ��ɳɹ����뵽���ճ���һ����:
				eExpireStatusΪES_OK��nExpireDateΪʵ����ɵ����գ�szExpireTipΪ������Ȩ�ɹ����ַ�����AuthorizeApi����0��
				AuthorizeApi���غ󲻻����첽�ص�fpOnExpire��
			��Ȩ��ɳɹ����뵽����ֻ��һ����:
				eExpireStatusΪES_TO_EXPIRE��nExpireDateΪʵ����ɵ����գ�szExpireTipΪ�����Ȩxml�ļ������õ�toexpiretip�ַ�����AuthorizeApi����0��
				AuthorizeApi���غ��ÿ��10�����첽�ص�һ��fpOnExpire����������h5apiʹ�÷��ٴ���Ȩ��ʹ��Ӱ������ʹ�á�
			��Ȩ��ɵ���:
				eExpireStatusΪES_EXPIRED��nExpireDateΪʵ����ɵ����գ�szExpireTipΪ�����Ȩxml�ļ������õ�expiredtip�ַ�����AuthorizeApi����1��
				AuthorizeApi���غ��ÿ��10�����첽�ص�һ��fpOnExpire����������h5apiʹ�÷��ٴ���Ȩ�Ӷ����ٴ�����ʹ�á�


	����ֵ��
		-1:		��Ȩ��ɴ��󡣱�ʾ����Ȩ�����г����˴��󣬴��������ڻص��������ṩ��
		0:		��Ȩ��ɳɹ���
		1:		��Ȩ��ɵ��ڡ�

 **/
int FUNCTION_CALL_MODE AuthorizeApi(const char *lpLicenseFile, FP_ON_EXPIRE fpOnExpire);


///	�ض��г���״̬�仯֪ͨ	���� iMktType �� iConnectID һһ��Ӧ,����API����������������,�ö�Ӧ��ϵ���ᷢ���仯
/**	
	int iMktType:	�г����	��h5hq_const_define.h�Ķ���
	int iConnectID:	���г���Ӧ�����Ӻ�
	��ע:ֻ�е�MktStatu= MKT_INITIALIZEDʱ���г��ſ��ã������ڸ��г�ִ�������Ĳ�����
		��MKTstatu= MKT_FAIL�г����ӱ��жϣ��г�ȡ����Ϣ���ǲ����õģ����������ϢҲӦ�����û����ĵġ�
		MktStatu= MKT_INITIALIZINGʱ���û�Ӧ��ֹͣʹ�ø��г��Լ��г���������з�������ֱ����ʼ��ɣ�MKT_INITIALIZED����ͬʱAPI����ն����б��û���Ҫ���յ�MKT_INITIALIZED״̬�����¶��ġ�
		�����һЩ״̬���û����Բ����ġ�
**/
typedef int (FUNCTION_CALL_MODE * ON_MKT_STATUS)(int iMktType, int iConnectID, enumMktStatu MktStatu);

///	���������ļ�,��ʼ��API,�����ļ�������:	
/**	
	<?xml  version="1.0" encoding="GBK"?>
	<supportmkt>
		<market mkt="" user="" password="">
			<svraddr svrip=""  svrport=""/>
			<svraddr svrip=""  svrport=""/>
		</market>
		<market mkt="" user="" password="">
			<svraddr svrip=""  svrport=""/>
			<svraddr svrip=""  svrport=""/>
		</market>
		<market mkt="" user="" password="">
			<svraddr svrip=""  svrport=""/>
			<svraddr svrip=""  svrport=""/>
		</market>
	</supportmkt>
	�����ļ��г�API֧�ֵ������г�,�Լ����г���Ӧ�ķ�������ַ,�������ö��ַ,API��������ĳ��������ʧЧʱ,��������һ����������

	���:	
		lpCfgFile:		�����ļ���
		MktStatusFunc:	�г�״̬�仯ʱ�Ļص�����(һ����˵,Ӧ�ù��ĵ����г���ʼ���¼�)
		iTimeOut:		ͬ����������µĳ�ʱʱ��,���ĳЩ����,����ͬ������ģʽ,��ôAPI�ڵȴ�����ʱ���,
						�����û��Ӧ�𷵻�,��ʱ����,
						��λ����
	����:
		0:		�ɹ�
		����:	ʧ��

**/	
int FUNCTION_CALL_MODE InitApi(const char* lpCfgFile, ON_MKT_STATUS MktStatusFunc=NULL, int iTimeOut=1000);



////////////////////////////	����2����,���Թ����г���ʼ����Ӧ��	////////////////////////////
///	����г��ſ�,�����ڳ�ʼ����ɡ��� nbvcxczsAqaZr6�����ִ��
/**
	���:
		int iMktType:	�г����
		int iTimeOut:	��ʱʱ��
	����:
		iErrno:	�������	0:�ɹ�	�������󣨱�������ʱ���ã�
	����:
		�г���Ҫ��Ϣָ��
**/
IMarketOverview* FUNCTION_CALL_MODE GetSupportMarketInfo(int iMktType, int iTimeOut, int& iErrno);


///	����ض���Ʊ����Ϣ,��Ʊ��Ϣ�����ù�Ʊ���еı�׼Ԫ����,�Լ��Զ�������
/**	
	���:	
		iMktType	�г�����
		lpStockCode	��Ʊ����
**/
IStockInfo* FUNCTION_CALL_MODE GetStockInfo(int iMktType, const char* lpStockCode);




//	���Ĺ�Ʊ��Ϣ�����Ķ���
enum enumSubAction
{
	SUB_CANCEL	= 0,		// ȡ������
	SUB_ADD		= 1,		// ׷��ʽ
	SUB_OVER	= 2,		// ����ʽ
};


/**	�������Զ����������Ǳ�׼�����õ�Ԫ����,API����Ϊ�����һ��ID,��Ŷ����h5hq_const_define.h
	����Ҫ���������ĳ���Զ�����������ĳ��Ԫ����ʱ,���Զ���һ���������ŵ����
	ֻҪ�ù�Ʊ���⼸��������е�����һ�������仯,API�ͻ����ע��Ļص�,��֪ͨӦ��

	cSubType:	���Ķ�������enumSubAction
	iRegID:	Ӧ�����ж����京��,������仯ʱ,API�ص�Ӧ��ע��ĺ���ʱ,ȷ��ԭ������
	int iMktType, const char* lpStockCode:	���ĵĹ�Ʊ
	int iResults, int* lpResultid:	�����¼��б�Ŀǰ�ܶ��ĵ���:������λ,�������ۣ�
	RecvFunc:	�ص�����
	iConnectID:	Ӧ��ָ����ĳ������ȥ��������,���ڲ�����������ά�������г����ӵ�Ӧ����˵,���봫���ֵ,
				��API���������Զ�Ϊ��ѡ����ʵ����ӣ�����ʹ������˵�ǲ��ɼ��ģ�ʹ���߿��Ժ��Դ˲���
**/
int FUNCTION_CALL_MODE SubStock(unsigned char cSubType, int iRegID, int iMktType, const char* lpStockCode, int iResults, int* lpResultid, H5_RECV_FUNCTION RecvFunc, int iConnectID=-1);


/**
	�������붩��
	��SubStock�Ļ������ܹ��ṩ��֧����ͬʱ���ĵĽӿ�
	int iStockCount: ��������
	int* iMktTypes, const char** lpStockCodes: ��iStockCountΪ���ȴ����б����顣 ����iMktTypes[0]��lpStockCodes[0]��Ӧ
	H5_RECV_FUNCTION_EX RecvFunc: ֧����������Ļص�����
	��������������SubStock����ͬ

	����ֵ��0Ϊ��������0Ϊ�쳣

	˵����
	1. ����ʽ���ĻὫ��ǰiRegID����������(�������е�iResultID)���и��ǣ�����Ӱ������iRegID���������ġ�׷�Ӻ�ɾ������Ҳ��ͬ���������
	2. ��֧�ִ�����Ϊ0�ĸ���ʽ���ģ�����Ϊ��յ�ǰiRegID������iResultID���ĵ����д�����������ġ���֧�ִ�����Ϊ0��׷�ӻ�ɾ�����ġ�
 **/
int FUNCTION_CALL_MODE SubStockEx(unsigned char cSubType, int iRegID, int iStockCount, int* iMktTypes, const char** lpStockCodes, int iResults, int* lpResultid, H5_RECV_FUNCTION_EX RecvFunc, int iConnectID=-1);

//	Ӧ�����˳�ǰ,�ͷ�APIռ�õ���Դ
int FUNCTION_CALL_MODE UnInitApi();

/*
 *����һ��RealTimeEX�ڴ�
 *
 **/
IRealTimeEX *FUNCTION_CALL_MODE CreatRealTimeEX();
/**
	������������
	��SubStock�Ļ������ܹ��ṩ��֧����ͬʱ��ȡ�Ľӿ�
	int iMktTypes:�г�
	int iStockCount: ��������
	const char** lpStockCodes������
	RealTimeEX *lpRealTimeEX���������
 **/
int FUNCTION_CALL_MODE GetRealTimeEx(IRealTimeEX *lpRealTimeEX, int iMktType,int iStockCount, const char** lpStockCodes,int iResultid);


/**
	˵����
	�ڵ���InitApi֮ǰʹ�ã�Ԥ��h5api����

	����˵����
	enumApiParamType eApiParamType: ��Ҫ���õĲ�������
	void *lpParam: ��Ҫ���õĲ�����ÿ��enumApiParamType�������ͻ��Ӧһ���ṹ��

	����ֵ��0Ϊ��������0�쳣

**/
int FUNCTION_CALL_MODE SetApiParam(enumApiParamType eApiParamType, void *lpParam);

#ifdef __cplusplus
}
#endif	//	__cplusplus

#endif	//	_H5HQ_API_INTERFACE_H__
