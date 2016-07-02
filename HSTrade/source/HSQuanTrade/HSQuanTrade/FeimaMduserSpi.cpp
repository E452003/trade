#include "stdafx.h"
#include "FeimaMduserSpi.h"
#include <string.h>
#include <iostream>
using namespace std;

void CFeimaMduserSpi::OnFrontConnected()
{
	CUstpFtdcReqUserLoginField req;
	memset(&req,0,sizeof(CUstpFtdcReqUserLoginField));
	strcpy(req.TradingDay,pUserApiHQ_FM->GetTradingDay());
	strcpy(req.BrokerID,m_szBroker);
	strcpy(req.UserID,m_szInvertID);
	strcpy(req.Password,m_szPwd);
	strcpy(req.UserProductInfo, "fuhaoming hft1.0");
	pUserApiHQ_FM->ReqUserLogin(&req,0);
}
	
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
void CFeimaMduserSpi::OnFrontDisconnected(int nReason)
{
}
		
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
void CFeimaMduserSpi::OnHeartBeatWarning(int nTimeLapse)
{
}
	
	///���Ļص���ʼ֪ͨ����API�յ�һ�����ĺ����ȵ��ñ�������Ȼ���Ǹ�������Ļص�������Ǳ��Ļص�����֪ͨ��
	///@param nTopicID ������루��˽���������������������ȣ�
	///@param nSequenceNo �������
void CFeimaMduserSpi::OnPackageStart(int nTopicID, int nSequenceNo)
{
}
	
	///���Ļص�����֪ͨ����API�յ�һ�����ĺ����ȵ��ñ��Ļص���ʼ֪ͨ��Ȼ���Ǹ�������Ļص��������ñ�������
	///@param nTopicID ������루��˽���������������������ȣ�
	///@param nSequenceNo �������
void CFeimaMduserSpi::OnPackageEnd(int nTopicID, int nSequenceNo)
{
}


	///����Ӧ��
void CFeimaMduserSpi::OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}

	///���ǰ��ϵͳ�û���¼Ӧ��
void CFeimaMduserSpi::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if( bIsLast && !IsErrorRspInfo(pRspInfo) )
	{
		SubscribeMarketData();
	}
}

void CFeimaMduserSpi::SubscribeMarketData()
{
	pUserApiHQ_FM->SubMarketData(m_ppInstrumentID,m_nInsNum);
}

	///�û��˳�Ӧ��
void CFeimaMduserSpi::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}

	///��������Ӧ��
void CFeimaMduserSpi::OnRspSubscribeTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}

	///�����ѯӦ��
void CFeimaMduserSpi::OnRspQryTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}

	///�������֪ͨ
void CFeimaMduserSpi::OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *pDepthMarketData) 
{


	
}

	///���ĺ�Լ�������Ϣ
void CFeimaMduserSpi::OnRspSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if( bIsLast && !IsErrorRspInfo(pRspInfo) )
	{
		
	}
}

	///�˶���Լ�������Ϣ
void CFeimaMduserSpi::OnRspUnSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}

bool CFeimaMduserSpi::IsErrorRspInfo(CUstpFtdcRspInfoField *pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
	{
		char temp[128];

		sprintf(temp," ErrorID= %d ErrorMsg=%s\n",pRspInfo->ErrorID,pRspInfo->ErrorMsg);

	}
	return bResult;
}
