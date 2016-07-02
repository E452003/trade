#include "HundsunH5sdkApi_impl.h"

//���������Ϣ
inline void DealErrorInfo(IF2UnPacker *lpUnPacker, CHundsunH5sdkRspInfoField *RspInfo)
{
};

CHundsunH5sdkApi *CHundsunH5sdkApi::CreateHundsunH5sdkApi(const char *pszFlowPath, bool bSaveLog)
{
    return new CHundsunH5sdkApi_impl(pszFlowPath, bSaveLog);
}

CHundsunH5sdkApi_impl::CHundsunH5sdkApi_impl(const char *pszFlowPath, bool bSaveLog)
{
    lpH5sdkSpi = NULL;
}

CHundsunH5sdkApi_impl::~CHundsunH5sdkApi_impl()
{
    StopTrd();

    hJoinEvent.Notify();
}

void CHundsunH5sdkApi_impl::ReleaseApi()
{
    delete this;
}

void CHundsunH5sdkApi_impl::Init()
{
    InitH5sdk();
    return;
}

int CHundsunH5sdkApi_impl::Join()
{
    hJoinEvent.Wait(-1);
    return 0;
}

void CHundsunH5sdkApi_impl::RegisterFront(const char *pszFrontAddress)
{
	hs_strncpy(m_szFrontAddress, pszFrontAddress, sizeof(m_szFrontAddress));

    return;
}

void CHundsunH5sdkApi_impl::RegisterSpi(CHundsunH5sdkSpi *pSpi)
{
    lpH5sdkSpi = pSpi;
    if(pSpi != NULL)
    {
        StartTrd();
    }
    else
    {
        StopTrd();
    }
}

//1001 ������������
int CHundsunH5sdkApi_impl::ReqH5sdkLogin(CHundsunH5sdkReqLoginField *pReqLogin, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_LOGIN, REQUEST);
    //������
    //��¼����
    lpRequest->GetItem(H5SDK_TAG_USER_NAME)->SetString(pReqLogin->UserName);
    //ö�١���¼��������
    lpRequest->GetItem(H5SDK_TAG_PASSWORD)->SetString(pReqLogin->Password);
    //��������
    lpRequest->GetItem(H5SDK_TAG_HQ_TOKEN)->SetString(pReqLogin->HqToken);
    //��¼����
    lpRequest->GetItem(H5SDK_TAG_DYNMIC_PASSWORD)->SetString(pReqLogin->DynPassword);
    //Ӧ�����ƣ������汾��Ϣ
    lpRequest->GetItem(H5SDK_TAG_APP_NAME)->SetString(pReqLogin->AppName);
    //�ֻ���
    lpRequest->GetItem(H5SDK_TAG_PHONE_NUMBER)->SetString(pReqLogin->PhoneNumber);
    //sdk�汾�ţ�sdk�Զ����룩
    lpRequest->GetItem(H5SDK_TAG_SDK_VERSION)->SetInt32(pReqLogin->SdkVersion);
    //����ϵͳ�汾��sdk�Զ����룩
    lpRequest->GetItem(H5SDK_TAG_OS_VERSION)->SetString(pReqLogin->OsVersion);
    //�����ַ��sdk�Զ����룩
    lpRequest->GetItem(H5SDK_TAG_MAC_ADDRESS)->SetString(pReqLogin->MacAddress);

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//1002 �ǳ���Ϣ����
int CHundsunH5sdkApi_impl::ReqH5sdkLogout(CHundsunH5sdkReqLogoutField *pReqLogout, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_LOGOUT, REQUEST);
    //������
    //������Ϣ
    lpRequest->GetItem(H5SDK_TAG_QUIT_REASON)->SetString(pReqLogout->QuitReason);

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//1003 ��������
int CHundsunH5sdkApi_impl::ReqH5sdkHeartbeat(CHundsunH5sdkReqHeartbeatField *pReqHeartbeat, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_HEARTBEAT, REQUEST);
    //������

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//1004 �����������Ϣ����
int CHundsunH5sdkApi_impl::ReqH5sdkServerInfo(CHundsunH5sdkReqServerInfoField *pReqServerInfo, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_SERVER_INFO, REQUEST);
    //������

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//1006 ����������
int CHundsunH5sdkApi_impl::ReqH5sdkBatch(CHundsunH5sdkReqBatchField *pReqBatch, int nRequestID)
{
    IGroup* lpGroup = NULL;
    IRecord* lpRecord = NULL;
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_BATCH, REQUEST);
    //������
    //���BatchGrp��Group����
    if(pReqBatch->BatchGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_BATCH_GRP);
        vector<CHundsunH5sdkReqBatchBatchGrpField>::iterator itBatchGrp;
        for(itBatchGrp = pReqBatch->BatchGrp.begin(); itBatchGrp != pReqBatch->BatchGrp.end(); itBatchGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //������ı��Ķ�����
            lpRecord->GetItem(H5SDK_TAG_SUB_PACKET)->SetString(itBatchGrp->SubPacket);
        }
    }

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//2001 ����̬�ļ�����
int CHundsunH5sdkApi_impl::ReqH5sdkFile(CHundsunH5sdkReqFileField *pReqFile, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_FILE, REQUEST);
    //������
    //�ļ�����
    lpRequest->GetItem(H5SDK_TAG_HQ_FILE_TYPE)->SetInt32(pReqFile->HqFileType);
    //�ļ��������
    lpRequest->GetItem(H5SDK_TAG_HQ_FILE_NAME)->SetString(pReqFile->HqFileName);
    //�ļ�CRC
    lpRequest->GetItem(H5SDK_TAG_CRC)->SetInt32(pReqFile->Crc);
    //�ļ�ƫ��
    lpRequest->GetItem(H5SDK_TAG_FILE_OFFSET)->SetInt64(pReqFile->FileOffset);
    //����ĳ��ȣ�����һ����ֵ�󣬸���Ӧ���´�����
    lpRequest->GetItem(H5SDK_TAG_FILE_LENGTH)->SetInt32(pReqFile->FileLength);

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//2002 �г�������Ϣ����
int CHundsunH5sdkApi_impl::ReqH5sdkMarketTypes(CHundsunH5sdkReqMarketTypesField *pReqMarketTypes, int nRequestID)
{
    IGroup* lpGroup = NULL;
    IRecord* lpRecord = NULL;
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_MARKET_TYPES, REQUEST);
    //������
    //���FinanceMicGrp��Group����
    if(pReqMarketTypes->FinanceMicGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_FINANCE_MIC_GRP);
        vector<CHundsunH5sdkReqMarketTypesFinanceMicGrpField>::iterator itFinanceMicGrp;
        for(itFinanceMicGrp = pReqMarketTypes->FinanceMicGrp.begin(); itFinanceMicGrp != pReqMarketTypes->FinanceMicGrp.end(); itFinanceMicGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //����������
            lpRecord->GetItem(H5SDK_TAG_FINANCE_MIC)->SetString(itFinanceMicGrp->FinanceMic);
        }
    }

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//2003 �г����������
int CHundsunH5sdkApi_impl::ReqH5sdkMarketReference(CHundsunH5sdkReqMarketReferenceField *pReqMarketReference, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_MARKET_REFERENCE, REQUEST);
    //������
    //�г����
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqMarketReference->HqTypeCode);

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//3001 �����������
int CHundsunH5sdkApi_impl::ReqH5sdkSnapshot(CHundsunH5sdkReqSnapshotField *pReqSnapshot, int nRequestID)
{
    IGroup* lpGroup = NULL;
    IRecord* lpRecord = NULL;
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_SNAPSHOT, REQUEST);
    //������
    //���鼶��: level1, evel2(��������level1,��ʾ�ӳ�,level2��ʾʵʱ)
    lpRequest->GetItem(H5SDK_TAG_HQ_LEVEL)->SetInt8(pReqSnapshot->HqLevel);
    //���ProdGrp��Group����
    if(pReqSnapshot->ProdGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_PROD_GRP);
        vector<CHundsunH5sdkReqSnapshotProdGrpField>::iterator itProdGrp;
        for(itProdGrp = pReqSnapshot->ProdGrp.begin(); itProdGrp != pReqSnapshot->ProdGrp.end(); itProdGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //��Ʊ����
            lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->SetString(itProdGrp->ProdCode);
            //���ʹ���,[subtype][.cficode].exchage
            lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(itProdGrp->HqTypeCode);
        }
    }
    //���FieldGrp��Group����
    if(pReqSnapshot->FieldGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_FIELD_GRP);
        vector<CHundsunH5sdkReqSnapshotFieldGrpField>::iterator itFieldGrp;
        for(itFieldGrp = pReqSnapshot->FieldGrp.begin(); itFieldGrp != pReqSnapshot->FieldGrp.end(); itFieldGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //�Դ�IDΪ׼����idΪ��ʱ��ȡfilename
            lpRecord->GetItem(H5SDK_TAG_FIELD_ID)->SetInt32(itFieldGrp->FieldId);
            //�ֶ�ID
            lpRecord->GetItem(H5SDK_TAG_FIELD_NAME)->SetString(itFieldGrp->FieldName);
        }
    }

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//3002 ���̾�����Ϣ����
int CHundsunH5sdkApi_impl::ReqH5sdkKeyboardWizard(CHundsunH5sdkReqKeyboardWizardField *pReqKeyboardWizard, int nRequestID)
{
    IGroup* lpGroup = NULL;
    IRecord* lpRecord = NULL;
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_KEYBOARD_WIZARD, REQUEST);
    //������
    //
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqKeyboardWizard->ProdCode);
    //��󷵻ظ���
    lpRequest->GetItem(H5SDK_TAG_MAX_VALUE)->SetInt32(pReqKeyboardWizard->Maxvalue);
    //���TypeGrp��Group����
    if(pReqKeyboardWizard->TypeGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_TYPE_GRP);
        vector<CHundsunH5sdkReqKeyboardWizardTypeGrpField>::iterator itTypeGrp;
        for(itTypeGrp = pReqKeyboardWizard->TypeGrp.begin(); itTypeGrp != pReqKeyboardWizard->TypeGrp.end(); itTypeGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //�������
            lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(itTypeGrp->HqTypeCode);
        }
    }

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//4001 ������ն�������
int CHundsunH5sdkApi_impl::ReqH5sdkSubscribe(CHundsunH5sdkReqSubscribeField *pReqSubscribe, int nRequestID)
{
    IGroup* lpGroup = NULL;
    IRecord* lpRecord = NULL;
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_SUBSCRIBE, REQUEST);
    //������
    //���鼶��: level1, evel2(��������level1,��ʾ�ӳ�,level2��ʾʵʱ)
    lpRequest->GetItem(H5SDK_TAG_HQ_LEVEL)->SetInt8(pReqSubscribe->HqLevel);
    //���ProdGrp��Group����
    if(pReqSubscribe->ProdGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_PROD_GRP);
        vector<CHundsunH5sdkReqSubscribeProdGrpField>::iterator itProdGrp;
        for(itProdGrp = pReqSubscribe->ProdGrp.begin(); itProdGrp != pReqSubscribe->ProdGrp.end(); itProdGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //
            lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(itProdGrp->HqTypeCode);
            //
            lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->SetString(itProdGrp->ProdCode);
        }
    }
    //���Ķ���
    lpRequest->GetItem(H5SDK_TAG_SUB_TYPE)->SetInt32(pReqSubscribe->SubType);

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//5001 ������������
int CHundsunH5sdkApi_impl::ReqH5sdkSort(CHundsunH5sdkReqSortField *pReqSort, int nRequestID)
{
    IGroup* lpGroup = NULL;
    IRecord* lpRecord = NULL;
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_SORT, REQUEST);
    //������
    //���SortProdGrp��Group����
    if(pReqSort->SortProdGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_SORT_PROD_GRP);
        vector<CHundsunH5sdkReqSortSortProdGrpField>::iterator itSortProdGrp;
        for(itSortProdGrp = pReqSort->SortProdGrp.begin(); itSortProdGrp != pReqSort->SortProdGrp.end(); itSortProdGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //��Ʊ����
            lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->SetString(itSortProdGrp->ProdCode);
            //���ʹ���,[subtype][.cficode].exchage
            lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(itSortProdGrp->HqTypeCode);
        }
    }
    //���SortTypeGrp��Group����
    if(pReqSort->SortTypeGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_SORT_TYPE_GRP);
        vector<CHundsunH5sdkReqSortSortTypeGrpField>::iterator itSortTypeGrp;
        for(itSortTypeGrp = pReqSort->SortTypeGrp.begin(); itSortTypeGrp != pReqSort->SortTypeGrp.end(); itSortTypeGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //�������
            lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(itSortTypeGrp->HqTypeCode);
        }
    }
    //��Ҫ������ֶ�ID���Դ�Ϊ׼��������ֶβ�����ʱ��ȡsort_field_name
    lpRequest->GetItem(H5SDK_TAG_SORT_FIELD_ID)->SetInt32(pReqSort->SortFieldId);
    //��Ҫ������ֶ�
    lpRequest->GetItem(H5SDK_TAG_SORT_FIELD_NAME)->SetString(pReqSort->SortFieldName);
    //���FieldGrp��Group����
    if(pReqSort->FieldGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_FIELD_GRP);
        vector<CHundsunH5sdkReqSortFieldGrpField>::iterator itFieldGrp;
        for(itFieldGrp = pReqSort->FieldGrp.begin(); itFieldGrp != pReqSort->FieldGrp.end(); itFieldGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //�Դ�IDΪ׼����idΪ��ʱ��ȡfilename
            lpRecord->GetItem(H5SDK_TAG_FIELD_ID)->SetInt32(itFieldGrp->FieldId);
            //�ֶ�ID
            lpRecord->GetItem(H5SDK_TAG_FIELD_NAME)->SetString(itFieldGrp->FieldName);
        }
    }
    //��ʼλ��,Ĭ��Ϊ0
    lpRequest->GetItem(H5SDK_TAG_START_POS)->SetInt32(pReqSort->StartPos);
    //������Ŀ,Ĭ��100ֻ���룬���������Ŀ10000
    lpRequest->GetItem(H5SDK_TAG_DATA_COUNT)->SetInt32(pReqSort->DataCount);
    //����ʽ/Ĭ�Ͻ���
    lpRequest->GetItem(H5SDK_TAG_SORT_TYPE)->SetInt8(pReqSort->SortType);

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//6001 ��ָ�������ڻ�ƫ��ȡ��ʱ��������
int CHundsunH5sdkApi_impl::ReqH5sdkTrend(CHundsunH5sdkReqTrendField *pReqTrend, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_TREND, REQUEST);
    //������
    //֤ȯ����
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqTrend->ProdCode);
    //���ʹ���
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqTrend->HqTypeCode);
    //����:YYYYMMDD,0:��ʾ����
    lpRequest->GetItem(H5SDK_TAG_DATE)->SetInt32(pReqTrend->Date);
    //���뵱ǰ���ڵ�ƫ������,
    lpRequest->GetItem(H5SDK_TAG_DATE_OFFSET)->SetInt32(pReqTrend->DateOffset);

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//6002 ��ƫ��ȡK������
int CHundsunH5sdkApi_impl::ReqH5sdkCandleByOffset(CHundsunH5sdkReqCandleByOffsetField *pReqCandleByOffset, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_CANDLE_BY_OFFSET, REQUEST);
    //������
    //֤ȯ����
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqCandleByOffset->ProdCode);
    //���ʹ���
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqCandleByOffset->HqTypeCode);
    //k������
    lpRequest->GetItem(H5SDK_TAG_CANDLE_PEROID)->SetInt32(pReqCandleByOffset->CandlePeriod);
    //
    lpRequest->GetItem(H5SDK_TAG_CANDLE_MODE)->SetInt8(pReqCandleByOffset->CandleMode);
    //��ʼ���ڣ�����д��ʾ��ǰ����
    lpRequest->GetItem(H5SDK_TAG_DATE)->SetInt32(pReqCandleByOffset->Date);
    //����K�ߵ�ʱ��HHMM,���ڶ�����K������ʹ��(1min,5min��)������д��ʾ���µ��г�ʱ�䣬����д����ͬʱ��дdate�ֶ�
    lpRequest->GetItem(H5SDK_TAG_MIN_TIME)->SetInt32(pReqCandleByOffset->MinTime);
    //����
    lpRequest->GetItem(H5SDK_TAG_DIRECTION)->SetInt8(pReqCandleByOffset->SearchDirection);
    //����ĸ���
    lpRequest->GetItem(H5SDK_TAG_DATA_COUNT)->SetInt32(pReqCandleByOffset->DataCount);

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//6003 ������ʱ�䷶ΧȡK������
int CHundsunH5sdkApi_impl::ReqH5sdkCandleByRange(CHundsunH5sdkReqCandleByRangeField *pReqCandleByRange, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_CANDLE_BY_RANGE, REQUEST);
    //������
    //֤ȯ����
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqCandleByRange->ProdCode);
    //���ʹ���
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqCandleByRange->HqTypeCode);
    //k������
    lpRequest->GetItem(H5SDK_TAG_CANDLE_PEROID)->SetInt32(pReqCandleByRange->CandlePeriod);
    //
    lpRequest->GetItem(H5SDK_TAG_CANDLE_MODE)->SetInt8(pReqCandleByRange->CandleMode);
    //
    lpRequest->GetItem(H5SDK_TAG_START_DATE)->SetInt32(pReqCandleByRange->StartDate);
    //
    lpRequest->GetItem(H5SDK_TAG_END_DATE)->SetInt32(pReqCandleByRange->EndDate);

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//6004 ָ�����ӵķֱ�����
int CHundsunH5sdkApi_impl::ReqH5sdkTickMin(CHundsunH5sdkReqTickMinField *pReqTickMin, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_TICK_MIN, REQUEST);
    //������
    //֤ȯ����
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqTickMin->ProdCode);
    //���ʹ���
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqTickMin->HqTypeCode);
    //����:YYYYMMDD,0:��ʾ����
    lpRequest->GetItem(H5SDK_TAG_DATE)->SetInt32(pReqTickMin->Date);
    //��ʼ������
    lpRequest->GetItem(H5SDK_TAG_START_MIN)->SetInt32(pReqTickMin->StartMin);
    //����������
    lpRequest->GetItem(H5SDK_TAG_END_MIN)->SetInt32(pReqTickMin->EndMin);
    //�����ٸ���Ĭ��ȫ������
    lpRequest->GetItem(H5SDK_TAG_DATA_COUNT)->SetInt32(pReqTickMin->DataCount);

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//6005 ��ƫ��ȡ�ֱ�����
int CHundsunH5sdkApi_impl::ReqH5sdkTickDirection(CHundsunH5sdkReqTickDirectionField *pReqTickDirection, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_TICK_DIRECTION, REQUEST);
    //������
    //֤ȯ����
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqTickDirection->ProdCode);
    //���ʹ���
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqTickDirection->HqTypeCode);
    //����:YYYYMMDD,0:��ʾ����
    lpRequest->GetItem(H5SDK_TAG_DATE)->SetInt32(pReqTickDirection->Date);
    //��ʼλ��,Ĭ��Ϊ0
    lpRequest->GetItem(H5SDK_TAG_START_POS)->SetInt32(pReqTickDirection->StartPos);
    //����
    lpRequest->GetItem(H5SDK_TAG_DIRECTION)->SetInt8(pReqTickDirection->SearchDirection);
    //�����ٸ���Ĭ��ȫ������
    lpRequest->GetItem(H5SDK_TAG_DATA_COUNT)->SetInt32(pReqTickDirection->DataCount);

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//6006 ����ʹ��_����k������
int CHundsunH5sdkApi_impl::ReqH5sdkCandleByRangeMaintain(CHundsunH5sdkReqCandleByRangeMaintainField *pReqCandleByRangeMaintain, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_CANDLE_BY_RANGE_MAINTAIN, REQUEST);
    //������
    //֤ȯ����
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqCandleByRangeMaintain->ProdCode);
    //���ʹ���
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqCandleByRangeMaintain->HqTypeCode);
    //k������(ֻ��Ҫ����k��)
    lpRequest->GetItem(H5SDK_TAG_CANDLE_PEROID)->SetInt32(pReqCandleByRangeMaintain->CandlePeriod);
    //
    lpRequest->GetItem(H5SDK_TAG_CANDLE_MODE)->SetInt8(pReqCandleByRangeMaintain->CandleMode);
    //
    lpRequest->GetItem(H5SDK_TAG_START_DATE)->SetInt32(pReqCandleByRangeMaintain->StartDate);
    //
    lpRequest->GetItem(H5SDK_TAG_END_DATE)->SetInt32(pReqCandleByRangeMaintain->EndDate);

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//6007 5�շ�ʱ����
int CHundsunH5sdkApi_impl::ReqH5sdkDay5Trend(CHundsunH5sdkReqDay5TrendField *pReqDay5Trend, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_DAY5_TREND, REQUEST);
    //������
    //֤ȯ����
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqDay5Trend->ProdCode);
    //���ʹ���
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqDay5Trend->HqTypeCode);

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

//7001 ���߷�������
int CHundsunH5sdkApi_impl::ReqH5sdkMaintainReturn(CHundsunH5sdkReqMaintainReturnField *pReqMaintainReturn, int nRequestID)
{
    IGroup* lpGroup = NULL;
    IRecord* lpRecord = NULL;
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MAINTAIN_RETURN, REQUEST);
    //������
    //��������
    lpRequest->GetItem(H5SDK_TAG_SUB_KIND)->SetInt16(pReqMaintainReturn->Subkind);
    //
    lpRequest->GetItem(H5SDK_TAG_MAINTAIN_RETURN_VALUE)->SetInt32(pReqMaintainReturn->Maintainreturnvalue);
    //���ProdGrp��Group����
    if(pReqMaintainReturn->ProdGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_PROD_GRP);
        vector<CHundsunH5sdkReqMaintainReturnProdGrpField>::iterator itProdGrp;
        for(itProdGrp = pReqMaintainReturn->ProdGrp.begin(); itProdGrp != pReqMaintainReturn->ProdGrp.end(); itProdGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //��Ʊ����
            lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->SetString(itProdGrp->ProdCode);
            //���ʹ���,[subtype][.cficode].exchage
            lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(itProdGrp->HqTypeCode);
        }
    }

    //����Message
    SendCommMessage(lpRequest, nRequestID);

    // �ͷ���Ϣ
    lpRequest->Release();

    return 0;
};

void CHundsunH5sdkApi_impl::OnConnected()
{
    if(lpH5sdkSpi)
    {
        lpH5sdkSpi->OnH5sdkFrontConnected();
    }

}

void CHundsunH5sdkApi_impl::OnClosed(int nReason)
{
    if(lpH5sdkSpi)
    {
        lpH5sdkSpi->OnH5sdkFrontDisconnected(nReason);
    }
}

void CHundsunH5sdkApi_impl::DealAsyCommMessage(IHsCommMessage* lpResponse)
{
    if (lpResponse!=NULL)
    {
        // �յ���Ϣʱ, �ȶԸ�Ӧ����Ϣ��������ȷ���ж�
        IRecord *head = lpResponse->GetHead();
        uint32 error_number = head->GetItem(H5PROTO_TAG_ERROR_NO)->GetInt32();
        if (error_number == 0)
        {
            // ��ȡ���ĵĹ��ܺ�
            int function_id = lpResponse->GetFunction();
            // ��ȡ���ĵ���Ϣ���
            IRecord *lpBody = lpResponse->GetBody();

            switch(function_id)
            {
            case H5SDK_MSG_LOGIN:
                OnAsyResponse_1001(lpBody, 0);
                break;
            case H5SDK_MSG_LOGOUT:
                OnAsyResponse_1002(lpBody, 0);
                break;
            case H5SDK_MSG_HEARTBEAT:
                OnAsyResponse_1003(lpBody, 0);
                break;
            case H5SDK_SERVER_INFO:
                OnAsyResponse_1004(lpBody, 0);
                break;
            case H5SDK_MSG_BATCH:
                OnAsyResponse_1006(lpBody, 0);
                break;
            case H5SDK_MSG_FILE:
                OnAsyResponse_2001(lpBody, 0);
                break;
            case H5SDK_MSG_MARKET_TYPES:
                OnAsyResponse_2002(lpBody, 0);
                break;
            case H5SDK_MSG_MARKET_REFERENCE:
                OnAsyResponse_2003(lpBody, 0);
                break;
            case H5SDK_MSG_SNAPSHOT:
                OnAsyResponse_3001(lpBody, 0);
                break;
            case H5SDK_KEYBOARD_WIZARD:
                OnAsyResponse_3002(lpBody, 0);
                break;
            case H5SDK_MSG_SUBSCRIBE:
                OnAsyResponse_4001(lpBody, 0);
                break;
            case H5SDK_MSG_SORT:
                OnAsyResponse_5001(lpBody, 0);
                break;
            case H5SDK_MSG_TREND:
                OnAsyResponse_6001(lpBody, 0);
                break;
            case H5SDK_MSG_CANDLE_BY_OFFSET:
                OnAsyResponse_6002(lpBody, 0);
                break;
            case H5SDK_MSG_CANDLE_BY_RANGE:
                OnAsyResponse_6003(lpBody, 0);
                break;
            case H5SDK_MSG_TICK_MIN:
                OnAsyResponse_6004(lpBody, 0);
                break;
            case H5SDK_MSG_TICK_DIRECTION:
                OnAsyResponse_6005(lpBody, 0);
                break;
            case H5SDK_MSG_CANDLE_BY_RANGE_MAINTAIN:
                OnAsyResponse_6006(lpBody, 0);
                break;
            case H5SDK_MSG_DAY5_TREND:
                OnAsyResponse_6007(lpBody, 0);
                break;
            case H5SDK_MAINTAIN_RETURN:
                OnAsyResponse_7001(lpBody, 0);
                break;
            default:
                break;
            }

        }
        else
        {

        }
    }
};

//1001 ��������
void CHundsunH5sdkApi_impl::OnAsyResponse_1001(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspLoginField RspLogin;

    //����š�ʹ��H5PROTO_ENUM_EN_CUSTOM��ʹ��CustomErrorNo��LoginData������ʹ��ErrorInfo�ֶ�
    RspLogin.ErrorNo = lpBody->GetItem(H5SDK_TAG_ERROR_NO)->GetInt32();
    //������Ϣ
    hs_strncpy(RspLogin.ErrorInfo, U2G(lpBody->GetItem(H5SDK_TAG_ERROR_INFO)->GetString()), sizeof(RspLogin.ErrorInfo));
    //��¼�������ݡ�����Level2У���̨���ص�����
    hs_strncpy(RspLogin.OriginalData, U2G(lpBody->GetItem(H5SDK_TAG_ORGINAL_DATA)->GetString()), sizeof(RspLogin.OriginalData));
    //�������
    RspLogin.HeartbeatInterval = lpBody->GetItem(H5SDK_TAG_HEARTBEAT_INTERVAL)->GetInt32();

    //�ص�
    lpH5sdkSpi->OnRspH5sdkLogin(&RspLogin, &RspInfo, nRequestID, true);
}

//1002 �ǳ���Ϣ
void CHundsunH5sdkApi_impl::OnAsyResponse_1002(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspLogoutField RspLogout;


    //�ص�
    lpH5sdkSpi->OnRspH5sdkLogout(&RspLogout, &RspInfo, nRequestID, true);
}

//1003 ����
void CHundsunH5sdkApi_impl::OnAsyResponse_1003(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspHeartbeatField RspHeartbeat;


    //�ص�
    lpH5sdkSpi->OnRspH5sdkHeartbeat(&RspHeartbeat, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_ServerInfoFinanceGrp(CHundsunH5sdkRspServerInfoField* lpRspServerInfo, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspServerInfoFinanceGrpField m_FinanceGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //�г�����
        hs_strncpy(m_FinanceGrp.FinanceMic, U2G(lpRecord->GetItem(H5SDK_TAG_FINANCE_MIC)->GetString()), sizeof(m_FinanceGrp.FinanceMic));
        //�г���������
        hs_strncpy(m_FinanceGrp.FinanceName, U2G(lpRecord->GetItem(H5SDK_TAG_FINANCE_NAME)->GetString()), sizeof(m_FinanceGrp.FinanceName));
        //����������
        m_FinanceGrp.InitDate = lpRecord->GetItem(H5SDK_TAG_INIT_DATE)->GetInt32();
        //������ʱ��
        m_FinanceGrp.BusinessTime = lpRecord->GetItem(H5SDK_TAG_BUSINESS_TIME)->GetInt32();

        //����CHundsunH5sdkRspServerInfoFinanceGrpField����
        lpRspServerInfo->FinanceGrp.push_back(m_FinanceGrp);
    }
}

//1004 �����������Ϣ
void CHundsunH5sdkApi_impl::OnAsyResponse_1004(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspServerInfoField RspServerInfo;

    //����������
    hs_strncpy(RspServerInfo.SrvName, U2G(lpBody->GetItem(H5SDK_TAG_SERVER_NAME)->GetString()), sizeof(RspServerInfo.SrvName));
    //��ǰ������ʱ��
    RspServerInfo.SrvTime = lpBody->GetItem(H5SDK_TAG_SERVER_TIME)->GetInt64();
    //��ǰ���ߣ��ݲ�ʹ�ã�
    RspServerInfo.CurrOnlineCount = lpBody->GetItem(H5SDK_TAG_CURR_ONLINE_COUNT)->GetInt32();
    //����������ߣ��ݲ�ʹ�ã�
    RspServerInfo.MaxOnlineCount = lpBody->GetItem(H5SDK_TAG_MAX_ONLINE_COUNT)->GetInt32();
    //������֧�ֵĽ�����
    DealGroupAsyResponse_ServerInfoFinanceGrp(&RspServerInfo, lpBody->GetGroup(H5SDK_TAG_FINANCE_GRP));

    //�ص�
    lpH5sdkSpi->OnRspH5sdkServerInfo(&RspServerInfo, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_BatchBatchGrp(CHundsunH5sdkRspBatchField* lpRspBatch, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspBatchBatchGrpField m_BatchGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //��Ӧ��ı��Ķ�����
        hs_strncpy(m_BatchGrp.SubPacket, U2G(lpRecord->GetItem(H5SDK_TAG_SUB_PACKET)->GetString()), sizeof(m_BatchGrp.SubPacket));

        //����CHundsunH5sdkRspBatchBatchGrpField����
        lpRspBatch->BatchGrp.push_back(m_BatchGrp);
    }
}

//1006 ������
void CHundsunH5sdkApi_impl::OnAsyResponse_1006(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspBatchField RspBatch;

    //���ڴ����������Ӧ����ظ���
    DealGroupAsyResponse_BatchBatchGrp(&RspBatch, lpBody->GetGroup(H5SDK_TAG_BATCH_GRP));

    //�ص�
    lpH5sdkSpi->OnRspH5sdkBatch(&RspBatch, &RspInfo, nRequestID, true);
}

//2001 ����̬�ļ�
void CHundsunH5sdkApi_impl::OnAsyResponse_2001(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspFileField RspFile;

    //�����
    RspFile.ErrorNo = lpBody->GetItem(H5SDK_TAG_ERROR_NO)->GetInt32();
    //�ļ�����
    RspFile.HqFileType = lpBody->GetItem(H5SDK_TAG_HQ_FILE_TYPE)->GetInt32();
    //�ļ�����
    hs_strncpy(RspFile.HqFileName, U2G(lpBody->GetItem(H5SDK_TAG_HQ_FILE_NAME)->GetString()), sizeof(RspFile.HqFileName));
    //ʣ�೤��
    RspFile.FileLength = lpBody->GetItem(H5SDK_TAG_FILE_LENGTH)->GetInt32();
    //�´������offset
    RspFile.FileOffset = lpBody->GetItem(H5SDK_TAG_FILE_OFFSET)->GetInt64();
    //�ļ�CRC
    RspFile.Crc = lpBody->GetItem(H5SDK_TAG_CRC)->GetInt32();
    //�ļ�����
    hs_strncpy(RspFile.OriginalData, U2G(lpBody->GetItem(H5SDK_TAG_ORGINAL_DATA)->GetString()), sizeof(RspFile.OriginalData));

    //�ص�
    lpH5sdkSpi->OnRspH5sdkFile(&RspFile, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_MarketTypesTypeGrpTradeSectionGrp(CHundsunH5sdkRspMarketTypesTypeGrpField* lpRspMarketTypesTypeGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspMarketTypesTradeSectionGrpField m_TradeSectionGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //����ʱ��
        m_TradeSectionGrp.Opentime = lpRecord->GetItem(H5SDK_TAG_OPEN_TIME)->GetInt32();
        //����ʱ��
        m_TradeSectionGrp.Closetime = lpRecord->GetItem(H5SDK_TAG_CLOSE_TIME)->GetInt32();

        //����CHundsunH5sdkRspMarketTypesTradeSectionGrpField����
        lpRspMarketTypesTypeGrp->TradeSectionGrp.push_back(m_TradeSectionGrp);
    }
}

inline void DealGroupAsyResponse_MarketTypesFinanceMicGrpTypeGrp(CHundsunH5sdkRspMarketTypesFinanceMicGrpField* lpRspMarketTypesFinanceMicGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspMarketTypesTypeGrpField m_TypeGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //�������
        hs_strncpy(m_TypeGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_TypeGrp.HqTypeCode));
        //��������
        hs_strncpy(m_TypeGrp.HqTypeName, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_NAME)->GetString()), sizeof(m_TypeGrp.HqTypeName));
        //����Ŵ���
        m_TypeGrp.PxScale = lpRecord->GetItem(H5SDK_TAG_PRICE_SCALE)->GetInt32();
        //�۸񾫶�
        m_TypeGrp.PxPrecision = lpRecord->GetItem(HSSDK_TAG_PX_PRECISION)->GetInt32();
        //��������
        m_TypeGrp.InitDate = lpRecord->GetItem(H5SDK_TAG_INIT_DATE)->GetInt32();
        //����ʱ���
        DealGroupAsyResponse_MarketTypesTypeGrpTradeSectionGrp(&m_TypeGrp, lpRecord->GetGroup(H5SDK_TAG_TRADE_SECTION_GRP));

        //����CHundsunH5sdkRspMarketTypesTypeGrpField����
        lpRspMarketTypesFinanceMicGrp->TypeGrp.push_back(m_TypeGrp);
    }
}

inline void DealGroupAsyResponse_MarketTypesFinanceMicGrp(CHundsunH5sdkRspMarketTypesField* lpRspMarketTypes, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspMarketTypesFinanceMicGrpField m_FinanceMicGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //����������
        hs_strncpy(m_FinanceMicGrp.FinanceMic, U2G(lpRecord->GetItem(H5SDK_TAG_FINANCE_MIC)->GetString()), sizeof(m_FinanceMicGrp.FinanceMic));
        //����������
        hs_strncpy(m_FinanceMicGrp.FinanceName, U2G(lpRecord->GetItem(H5SDK_TAG_FINANCE_NAME)->GetString()), sizeof(m_FinanceMicGrp.FinanceName));
        //�г�����
        m_FinanceMicGrp.MarketDate = lpRecord->GetItem(H5SDK_TAG_MARKET_DATE)->GetInt32();
        //��������
        m_FinanceMicGrp.InitDate = lpRecord->GetItem(H5SDK_TAG_INIT_DATE)->GetInt32();
        //�г�����ʱ��
        m_FinanceMicGrp.Timezone = lpRecord->GetItem(H5SDK_TAG_TIMEZONE)->GetInt32();
        //ʱ����
        hs_strncpy(m_FinanceMicGrp.TimezoneCode, U2G(lpRecord->GetItem(H5SDK_TAG_TIMEZONE_CODE)->GetString()), sizeof(m_FinanceMicGrp.TimezoneCode));
        //����ʱ��־
        m_FinanceMicGrp.Dst = lpRecord->GetItem(H5SDK_TAG_DST_FLAG)->GetInt8();
        //֤ȯ����
        DealGroupAsyResponse_MarketTypesFinanceMicGrpTypeGrp(&m_FinanceMicGrp, lpRecord->GetGroup(H5SDK_TAG_TYPE_GRP));

        //����CHundsunH5sdkRspMarketTypesFinanceMicGrpField����
        lpRspMarketTypes->FinanceMicGrp.push_back(m_FinanceMicGrp);
    }
}

//2002 �г�������Ϣ
void CHundsunH5sdkApi_impl::OnAsyResponse_2002(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspMarketTypesField RspMarketTypes;

    //����������
    DealGroupAsyResponse_MarketTypesFinanceMicGrp(&RspMarketTypes, lpBody->GetGroup(H5SDK_FINANCE_MIC_GRP));

    //�ص�
    lpH5sdkSpi->OnRspH5sdkMarketTypes(&RspMarketTypes, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_MarketReferenceProdGrp(CHundsunH5sdkRspMarketReferenceField* lpRspMarketReference, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspMarketReferenceProdGrpField m_ProdGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //֤ȯ����
        hs_strncpy(m_ProdGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_ProdGrp.ProdCode));
        //���ʹ���
        hs_strncpy(m_ProdGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_ProdGrp.HqTypeCode));
        //֤ȯ����
        hs_strncpy(m_ProdGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_ProdGrp.ProdName));
        //ǰ���̼�
        m_ProdGrp.PreclosePx = lpRecord->GetItem(H5SDK_TAG_PRECLOSE_PX)->GetInt32();
        //��ͣ�۸�
        m_ProdGrp.UpPx = lpRecord->GetItem(H5SDK_TAG_UP_PRICE)->GetInt32();
        //��ͣ�۸�
        m_ProdGrp.DownPx = lpRecord->GetItem(H5SDK_TAG_DOWN_PRICE)->GetInt32();

        //����CHundsunH5sdkRspMarketReferenceProdGrpField����
        lpRspMarketReference->ProdGrp.push_back(m_ProdGrp);
    }
}

//2003 �г������
void CHundsunH5sdkApi_impl::OnAsyResponse_2003(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspMarketReferenceField RspMarketReference;

    //����������
    hs_strncpy(RspMarketReference.FinanceMic, U2G(lpBody->GetItem(H5SDK_TAG_FINANCE_MIC)->GetString()), sizeof(RspMarketReference.FinanceMic));
    //������MIC��д
    hs_strncpy(RspMarketReference.MicAbbr, U2G(lpBody->GetItem(H5SDK_TAG_MIC_ABBR)->GetString()), sizeof(RspMarketReference.MicAbbr));
    //����������
    hs_strncpy(RspMarketReference.FinanceName, U2G(lpBody->GetItem(H5SDK_TAG_FINANCE_NAME)->GetString()), sizeof(RspMarketReference.FinanceName));
    //�г�����
    RspMarketReference.MarketDate = lpBody->GetItem(H5SDK_TAG_MARKET_DATE)->GetInt32();
    //��������
    RspMarketReference.InitDate = lpBody->GetItem(H5SDK_TAG_INIT_DATE)->GetInt32();
    //�г�����ʱ��
    RspMarketReference.Timezone = lpBody->GetItem(H5SDK_TAG_TIMEZONE)->GetInt32();
    //ʱ����
    hs_strncpy(RspMarketReference.TimezoneCode, U2G(lpBody->GetItem(H5SDK_TAG_TIMEZONE_CODE)->GetString()), sizeof(RspMarketReference.TimezoneCode));
    //����ʱ��־
    RspMarketReference.Dst = lpBody->GetItem(H5SDK_TAG_DST_FLAG)->GetInt8();
    //����
    DealGroupAsyResponse_MarketReferenceProdGrp(&RspMarketReference, lpBody->GetGroup(H5SDK_TAG_PROD_GRP));

    //�ص�
    lpH5sdkSpi->OnRspH5sdkMarketReference(&RspMarketReference, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_SnapshotBidGrpEntrustGrp(CHundsunH5sdkRspSnapshotBidGrpField* lpRspSnapshotBidGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSnapshotEntrustGrpField m_EntrustGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //ί����
        m_EntrustGrp.EntrustAmount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_AMOUNT)->GetInt32();

        //����CHundsunH5sdkRspSnapshotEntrustGrpField����
        lpRspSnapshotBidGrp->EntrustGrp.push_back(m_EntrustGrp);
    }
}

inline void DealGroupAsyResponse_SnapshotOfferGrpEntrustGrp(CHundsunH5sdkRspSnapshotOfferGrpField* lpRspSnapshotOfferGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSnapshotEntrustGrpField m_EntrustGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //ί����
        m_EntrustGrp.EntrustAmount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_AMOUNT)->GetInt32();

        //����CHundsunH5sdkRspSnapshotEntrustGrpField����
        lpRspSnapshotOfferGrp->EntrustGrp.push_back(m_EntrustGrp);
    }
}

inline void DealGroupAsyResponse_SnapshotProdGrpBidGrp(CHundsunH5sdkRspSnapshotProdGrpField* lpRspSnapshotProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSnapshotBidGrpField m_BidGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //ί�м۸�
        m_BidGrp.EntrustPx = lpRecord->GetItem(H5SDK_TAG_ENTRUST_PX)->GetInt32();
        //ί����
        m_BidGrp.TotalEntrustAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_ENTRUST_AMOUNT)->GetInt64();
        //ί����
        m_BidGrp.EntrustCount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_COUNT)->GetInt32();
        //ί�ж����б�
        DealGroupAsyResponse_SnapshotBidGrpEntrustGrp(&m_BidGrp, lpRecord->GetGroup(H5SDK_TAG_ENTRUST_GRP));

        //����CHundsunH5sdkRspSnapshotBidGrpField����
        lpRspSnapshotProdGrp->BidGrp.push_back(m_BidGrp);
    }
}

inline void DealGroupAsyResponse_SnapshotProdGrpOfferGrp(CHundsunH5sdkRspSnapshotProdGrpField* lpRspSnapshotProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSnapshotOfferGrpField m_OfferGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //ί�м۸�
        m_OfferGrp.EntrustPx = lpRecord->GetItem(H5SDK_TAG_ENTRUST_PX)->GetInt32();
        //ί����
        m_OfferGrp.TotalEntrustAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_ENTRUST_AMOUNT)->GetInt64();
        //ί����
        m_OfferGrp.EntrustCount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_COUNT)->GetInt32();
        //ί�ж����б�
        DealGroupAsyResponse_SnapshotOfferGrpEntrustGrp(&m_OfferGrp, lpRecord->GetGroup(H5SDK_TAG_ENTRUST_GRP));

        //����CHundsunH5sdkRspSnapshotOfferGrpField����
        lpRspSnapshotProdGrp->OfferGrp.push_back(m_OfferGrp);
    }
}

inline void DealGroupAsyResponse_SnapshotProdGrpRiseFirstGrp(CHundsunH5sdkRspSnapshotProdGrpField* lpRspSnapshotProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSnapshotRiseFirstGrpField m_RiseFirstGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //֤ȯ����
        hs_strncpy(m_RiseFirstGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_RiseFirstGrp.ProdCode));
        //֤ȯ����
        hs_strncpy(m_RiseFirstGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_RiseFirstGrp.ProdName));
        //֤ȯ����
        hs_strncpy(m_RiseFirstGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_RiseFirstGrp.HqTypeCode));
        //���¼�
        m_RiseFirstGrp.LastPx = lpRecord->GetItem(H5SDK_TAG_LAST_PX)->GetInt32();
        //�ǵ���
        m_RiseFirstGrp.PxChangeRate = lpRecord->GetItem(H5SDK_TAG_PX_CHANGE_RATE)->GetInt32();

        //����CHundsunH5sdkRspSnapshotRiseFirstGrpField����
        lpRspSnapshotProdGrp->RiseFirstGrp.push_back(m_RiseFirstGrp);
    }
}

inline void DealGroupAsyResponse_SnapshotProdGrpFallFirstGrp(CHundsunH5sdkRspSnapshotProdGrpField* lpRspSnapshotProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSnapshotFallFirstGrpField m_FallFirstGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //֤ȯ����
        hs_strncpy(m_FallFirstGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_FallFirstGrp.ProdCode));
        //֤ȯ����
        hs_strncpy(m_FallFirstGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_FallFirstGrp.ProdName));
        //֤ȯ����
        hs_strncpy(m_FallFirstGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_FallFirstGrp.HqTypeCode));
        //���¼�
        m_FallFirstGrp.LastPx = lpRecord->GetItem(H5SDK_TAG_LAST_PX)->GetInt32();
        //�ǵ���
        m_FallFirstGrp.PxChangeRate = lpRecord->GetItem(H5SDK_TAG_PX_CHANGE_RATE)->GetInt32();

        //����CHundsunH5sdkRspSnapshotFallFirstGrpField����
        lpRspSnapshotProdGrp->FallFirstGrp.push_back(m_FallFirstGrp);
    }
}

inline void DealGroupAsyResponse_SnapshotProdGrp(CHundsunH5sdkRspSnapshotField* lpRspSnapshot, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSnapshotProdGrpField m_ProdGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //֤ȯ����
        hs_strncpy(m_ProdGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_ProdGrp.ProdCode));
        //֤ȯ����
        hs_strncpy(m_ProdGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_ProdGrp.ProdName));
        //֤ȯ����
        hs_strncpy(m_ProdGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_ProdGrp.HqTypeCode));
        //������루δ���壩
        hs_strncpy(m_ProdGrp.IndustryCode, U2G(lpRecord->GetItem(H5SDK_TAG_INDUSTRY_CODE)->GetString()), sizeof(m_ProdGrp.IndustryCode));
        //���Ҵ��루δ���壩
        hs_strncpy(m_ProdGrp.MoneyType, U2G(lpRecord->GetItem(H5SDK_TAG_MONEY_TYPE)->GetString()), sizeof(m_ProdGrp.MoneyType));
        //ʱ���
        m_ProdGrp.DataTimestamp = lpRecord->GetItem(H5SDK_TAG_DATA_TIMESTAMP)->GetInt32();
        //���׷�����
        m_ProdGrp.TradeMins = lpRecord->GetItem(H5SDK_TAG_TRADE_MINS)->GetInt32();
        //����״̬
        hs_strncpy(m_ProdGrp.TradeStatus, U2G(lpRecord->GetItem(H5SDK_TAG_TRADE_STATUS)->GetString()), sizeof(m_ProdGrp.TradeStatus));
        //���ռ�
        m_ProdGrp.PreclosePx = lpRecord->GetItem(H5SDK_TAG_PRECLOSE_PX)->GetInt32();
        //����
        m_ProdGrp.OpenPx = lpRecord->GetItem(H5SDK_TAG_OPEN_PX)->GetInt32();
        //���³ɽ���
        m_ProdGrp.LastPx = lpRecord->GetItem(H5SDK_TAG_LAST_PX)->GetInt32();
        //��߼�
        m_ProdGrp.HighPx = lpRecord->GetItem(H5SDK_TAG_HIGH_PX)->GetInt32();
        //��ͼ�
        m_ProdGrp.LowPx = lpRecord->GetItem(H5SDK_TAG_LOW_PX)->GetInt32();
        //��������
        m_ProdGrp.ClosePx = lpRecord->GetItem(H5SDK_TAG_CLOSE_PX)->GetInt32();
        //����
        m_ProdGrp.AvgPx = lpRecord->GetItem(H5SDK_TAG_AVG_PX)->GetInt32();
        //��Ȩƽ����
        m_ProdGrp.WavgPx = lpRecord->GetItem(H5SDK_TAG_WAVG_PX)->GetInt32();
        //�ɽ�����
        m_ProdGrp.BusinessCount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_COUNT)->GetInt32();
        //�ܳɽ���
        m_ProdGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //�ܳɽ���
        m_ProdGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();
        //��ͣ�۸�
        m_ProdGrp.UpPx = lpRecord->GetItem(H5SDK_TAG_UP_PRICE)->GetInt32();
        //��ͣ�۸�
        m_ProdGrp.DownPx = lpRecord->GetItem(H5SDK_TAG_DOWN_PRICE)->GetInt32();
        //����ɽ���(����)
        m_ProdGrp.CurrentAmount = lpRecord->GetItem(H5SDK_TAG_CURRENT_AMOUNT)->GetInt64();
        //���̳ɽ���
        m_ProdGrp.BusinessAmountIn = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT_IN)->GetInt64();
        //���̳ɽ���
        m_ProdGrp.BusinessAmountOut = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT_OUT)->GetInt64();
        //��ί������δ���壩
        m_ProdGrp.TotBuyAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_BUY_AMOUNT)->GetInt64();
        //��ί������δ���壩
        m_ProdGrp.TotSellAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_SELL_AMOUNT)->GetInt64();
        //��Ȩƽ��ί��۸�δ���壩
        m_ProdGrp.WavgBidPx = lpRecord->GetItem(H5SDK_TAG_WAVG_BID_PX)->GetInt32();
        //��Ȩƽ��ί���۸�δ���壩
        m_ProdGrp.WavgOfferPx = lpRecord->GetItem(H5SDK_TAG_WAVG_OFFER_PX)->GetInt32();
        //ί��λ
        DealGroupAsyResponse_SnapshotProdGrpBidGrp(&m_ProdGrp, lpRecord->GetGroup(H5SDK_TAG_BID_GRP));

        //����CHundsunH5sdkRspSnapshotProdGrpField����
        lpRspSnapshot->ProdGrp.push_back(m_ProdGrp);
    }
}

//3001 �������
void CHundsunH5sdkApi_impl::OnAsyResponse_3001(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspSnapshotField RspSnapshot;

    //���鼶��: level1, evel2(��������level1,��ʾ�ӳ�,level2��ʾʵʱ)
    RspSnapshot.HqLevel = lpBody->GetItem(H5SDK_TAG_HQ_LEVEL)->GetInt8();
    //�����б�
    DealGroupAsyResponse_SnapshotProdGrp(&RspSnapshot, lpBody->GetGroup(H5SDK_TAG_PROD_GRP));

    //�ص�
    lpH5sdkSpi->OnRspH5sdkSnapshot(&RspSnapshot, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_KeyboardWizardProdGrp(CHundsunH5sdkRspKeyboardWizardField* lpRspKeyboardWizard, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspKeyboardWizardProdGrpField m_ProdGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //֤ȯ����
        hs_strncpy(m_ProdGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_ProdGrp.ProdCode));
        //֤ȯ����
        hs_strncpy(m_ProdGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_ProdGrp.ProdName));
        //֤ȯ���
        hs_strncpy(m_ProdGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_ProdGrp.HqTypeCode));

        //����CHundsunH5sdkRspKeyboardWizardProdGrpField����
        lpRspKeyboardWizard->ProdGrp.push_back(m_ProdGrp);
    }
}

//3002 ���̾�����Ϣ
void CHundsunH5sdkApi_impl::OnAsyResponse_3002(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspKeyboardWizardField RspKeyboardWizard;

    //�����б�
    DealGroupAsyResponse_KeyboardWizardProdGrp(&RspKeyboardWizard, lpBody->GetGroup(H5SDK_TAG_PROD_GRP));

    //�ص�
    lpH5sdkSpi->OnRspH5sdkKeyboardWizard(&RspKeyboardWizard, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_SubscribeProdGrp(CHundsunH5sdkRspSubscribeField* lpRspSubscribe, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSubscribeProdGrpField m_ProdGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //
        hs_strncpy(m_ProdGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_ProdGrp.HqTypeCode));
        //����
        hs_strncpy(m_ProdGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_ProdGrp.ProdCode));
        //0�ɹ�,����:ʧ��
        m_ProdGrp.ErrorNo = lpRecord->GetItem(H5SDK_TAG_ERROR_NO)->GetInt32();

        //����CHundsunH5sdkRspSubscribeProdGrpField����
        lpRspSubscribe->ProdGrp.push_back(m_ProdGrp);
    }
}

//4001 ������ն���
void CHundsunH5sdkApi_impl::OnAsyResponse_4001(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspSubscribeField RspSubscribe;

    //���鼶��: level1, evel2(��������level1,��ʾ�ӳ�,level2��ʾʵʱ)
    RspSubscribe.HqLevel = lpBody->GetItem(H5SDK_TAG_HQ_LEVEL)->GetInt8();
    //
    DealGroupAsyResponse_SubscribeProdGrp(&RspSubscribe, lpBody->GetGroup(H5SDK_TAG_PROD_GRP));
    //ԭ�����ض��Ķ���
    RspSubscribe.SubType = lpBody->GetItem(H5SDK_TAG_SUB_TYPE)->GetInt32();

    //�ص�
    lpH5sdkSpi->OnRspH5sdkSubscribe(&RspSubscribe, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_SortBidGrpEntrustGrp(CHundsunH5sdkRspSortBidGrpField* lpRspSortBidGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortEntrustGrpField m_EntrustGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //ί����
        m_EntrustGrp.EntrustAmount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_AMOUNT)->GetInt32();

        //����CHundsunH5sdkRspSortEntrustGrpField����
        lpRspSortBidGrp->EntrustGrp.push_back(m_EntrustGrp);
    }
}

inline void DealGroupAsyResponse_SortOfferGrpEntrustGrp(CHundsunH5sdkRspSortOfferGrpField* lpRspSortOfferGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortEntrustGrpField m_EntrustGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //ί����
        m_EntrustGrp.EntrustAmount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_AMOUNT)->GetInt32();

        //����CHundsunH5sdkRspSortEntrustGrpField����
        lpRspSortOfferGrp->EntrustGrp.push_back(m_EntrustGrp);
    }
}

inline void DealGroupAsyResponse_SortSortProdGrpBidGrp(CHundsunH5sdkRspSortSortProdGrpField* lpRspSortSortProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortBidGrpField m_BidGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //ί�м۸�
        m_BidGrp.EntrustPx = lpRecord->GetItem(H5SDK_TAG_ENTRUST_PX)->GetInt32();
        //ί����
        m_BidGrp.TotalEntrustAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_ENTRUST_AMOUNT)->GetInt64();
        //ί����
        m_BidGrp.EntrustCount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_COUNT)->GetInt32();
        //ί�ж����б�
        DealGroupAsyResponse_SortBidGrpEntrustGrp(&m_BidGrp, lpRecord->GetGroup(H5SDK_TAG_ENTRUST_GRP));

        //����CHundsunH5sdkRspSortBidGrpField����
        lpRspSortSortProdGrp->BidGrp.push_back(m_BidGrp);
    }
}

inline void DealGroupAsyResponse_SortSortProdGrpOfferGrp(CHundsunH5sdkRspSortSortProdGrpField* lpRspSortSortProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortOfferGrpField m_OfferGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //ί�м۸�
        m_OfferGrp.EntrustPx = lpRecord->GetItem(H5SDK_TAG_ENTRUST_PX)->GetInt32();
        //ί����
        m_OfferGrp.TotalEntrustAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_ENTRUST_AMOUNT)->GetInt64();
        //ί����
        m_OfferGrp.EntrustCount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_COUNT)->GetInt32();
        //ί�ж����б�
        DealGroupAsyResponse_SortOfferGrpEntrustGrp(&m_OfferGrp, lpRecord->GetGroup(H5SDK_TAG_ENTRUST_GRP));

        //����CHundsunH5sdkRspSortOfferGrpField����
        lpRspSortSortProdGrp->OfferGrp.push_back(m_OfferGrp);
    }
}

inline void DealGroupAsyResponse_SortSortProdGrpRiseFirstGrp(CHundsunH5sdkRspSortSortProdGrpField* lpRspSortSortProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortRiseFirstGrpField m_RiseFirstGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //֤ȯ����
        hs_strncpy(m_RiseFirstGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_RiseFirstGrp.ProdCode));
        //֤ȯ����
        hs_strncpy(m_RiseFirstGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_RiseFirstGrp.ProdName));
        //֤ȯ����
        hs_strncpy(m_RiseFirstGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_RiseFirstGrp.HqTypeCode));
        //���¼�
        m_RiseFirstGrp.LastPx = lpRecord->GetItem(H5SDK_TAG_LAST_PX)->GetInt32();
        //�ǵ���
        m_RiseFirstGrp.PxChangeRate = lpRecord->GetItem(H5SDK_TAG_PX_CHANGE_RATE)->GetInt32();

        //����CHundsunH5sdkRspSortRiseFirstGrpField����
        lpRspSortSortProdGrp->RiseFirstGrp.push_back(m_RiseFirstGrp);
    }
}

inline void DealGroupAsyResponse_SortSortProdGrpFallFirstGrp(CHundsunH5sdkRspSortSortProdGrpField* lpRspSortSortProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortFallFirstGrpField m_FallFirstGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //֤ȯ����
        hs_strncpy(m_FallFirstGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_FallFirstGrp.ProdCode));
        //֤ȯ����
        hs_strncpy(m_FallFirstGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_FallFirstGrp.ProdName));
        //֤ȯ����
        hs_strncpy(m_FallFirstGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_FallFirstGrp.HqTypeCode));
        //���¼�
        m_FallFirstGrp.LastPx = lpRecord->GetItem(H5SDK_TAG_LAST_PX)->GetInt32();
        //�ǵ���
        m_FallFirstGrp.PxChangeRate = lpRecord->GetItem(H5SDK_TAG_PX_CHANGE_RATE)->GetInt32();

        //����CHundsunH5sdkRspSortFallFirstGrpField����
        lpRspSortSortProdGrp->FallFirstGrp.push_back(m_FallFirstGrp);
    }
}

inline void DealGroupAsyResponse_SortSortTypeGrp(CHundsunH5sdkRspSortField* lpRspSort, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortSortTypeGrpField m_SortTypeGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //�������
        hs_strncpy(m_SortTypeGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_SortTypeGrp.HqTypeCode));

        //����CHundsunH5sdkRspSortSortTypeGrpField����
        lpRspSort->SortTypeGrp.push_back(m_SortTypeGrp);
    }
}

inline void DealGroupAsyResponse_SortSortProdGrp(CHundsunH5sdkRspSortField* lpRspSort, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortSortProdGrpField m_SortProdGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //֤ȯ����
        hs_strncpy(m_SortProdGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_SortProdGrp.ProdCode));
        //֤ȯ����
        hs_strncpy(m_SortProdGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_SortProdGrp.ProdName));
        //֤ȯ����
        hs_strncpy(m_SortProdGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_SortProdGrp.HqTypeCode));
        //������루δ���壩
        hs_strncpy(m_SortProdGrp.IndustryCode, U2G(lpRecord->GetItem(H5SDK_TAG_INDUSTRY_CODE)->GetString()), sizeof(m_SortProdGrp.IndustryCode));
        //���Ҵ��루δ���壩
        hs_strncpy(m_SortProdGrp.MoneyType, U2G(lpRecord->GetItem(H5SDK_TAG_MONEY_TYPE)->GetString()), sizeof(m_SortProdGrp.MoneyType));
        //ʱ���
        m_SortProdGrp.DataTimestamp = lpRecord->GetItem(H5SDK_TAG_DATA_TIMESTAMP)->GetInt32();
        //���׷�����
        m_SortProdGrp.TradeMins = lpRecord->GetItem(H5SDK_TAG_TRADE_MINS)->GetInt32();
        //����״̬
        hs_strncpy(m_SortProdGrp.TradeStatus, U2G(lpRecord->GetItem(H5SDK_TAG_TRADE_STATUS)->GetString()), sizeof(m_SortProdGrp.TradeStatus));
        //���ռ�
        m_SortProdGrp.PreclosePx = lpRecord->GetItem(H5SDK_TAG_PRECLOSE_PX)->GetInt32();
        //����
        m_SortProdGrp.OpenPx = lpRecord->GetItem(H5SDK_TAG_OPEN_PX)->GetInt32();
        //���³ɽ���
        m_SortProdGrp.LastPx = lpRecord->GetItem(H5SDK_TAG_LAST_PX)->GetInt32();
        //��߼�
        m_SortProdGrp.HighPx = lpRecord->GetItem(H5SDK_TAG_HIGH_PX)->GetInt32();
        //��ͼ�
        m_SortProdGrp.LowPx = lpRecord->GetItem(H5SDK_TAG_LOW_PX)->GetInt32();
        //��������
        m_SortProdGrp.ClosePx = lpRecord->GetItem(H5SDK_TAG_CLOSE_PX)->GetInt32();
        //����
        m_SortProdGrp.AvgPx = lpRecord->GetItem(H5SDK_TAG_AVG_PX)->GetInt32();
        //��Ȩƽ����
        m_SortProdGrp.WavgPx = lpRecord->GetItem(H5SDK_TAG_WAVG_PX)->GetInt32();
        //�ɽ�����
        m_SortProdGrp.BusinessCount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_COUNT)->GetInt32();
        //�ܳɽ���
        m_SortProdGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //�ܳɽ���
        m_SortProdGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();
        //��ͣ�۸�
        m_SortProdGrp.UpPx = lpRecord->GetItem(H5SDK_TAG_UP_PRICE)->GetInt32();
        //��ͣ�۸�
        m_SortProdGrp.DownPx = lpRecord->GetItem(H5SDK_TAG_DOWN_PRICE)->GetInt32();
        //����ɽ���(����)
        m_SortProdGrp.CurrentAmount = lpRecord->GetItem(H5SDK_TAG_CURRENT_AMOUNT)->GetInt64();
        //���̳ɽ���
        m_SortProdGrp.BusinessAmountIn = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT_IN)->GetInt64();
        //���̳ɽ���
        m_SortProdGrp.BusinessAmountOut = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT_OUT)->GetInt64();
        //��ί������δ���壩
        m_SortProdGrp.TotBuyAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_BUY_AMOUNT)->GetInt64();
        //��ί������δ���壩
        m_SortProdGrp.TotSellAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_SELL_AMOUNT)->GetInt64();
        //��Ȩƽ��ί��۸�δ���壩
        m_SortProdGrp.WavgBidPx = lpRecord->GetItem(H5SDK_TAG_WAVG_BID_PX)->GetInt32();
        //��Ȩƽ��ί���۸�δ���壩
        m_SortProdGrp.WavgOfferPx = lpRecord->GetItem(H5SDK_TAG_WAVG_OFFER_PX)->GetInt32();
        //ί��λ
        DealGroupAsyResponse_SortSortProdGrpBidGrp(&m_SortProdGrp, lpRecord->GetGroup(H5SDK_TAG_BID_GRP));

        //����CHundsunH5sdkRspSortSortProdGrpField����
        lpRspSort->SortProdGrp.push_back(m_SortProdGrp);
    }
}

//5001 ��������
void CHundsunH5sdkApi_impl::OnAsyResponse_5001(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspSortField RspSort;

    //����ʽ/Ĭ�Ͻ���
    RspSort.SortType = lpBody->GetItem(H5SDK_TAG_SORT_TYPE)->GetInt8();
    //֤ȯ����
    DealGroupAsyResponse_SortSortTypeGrp(&RspSort, lpBody->GetGroup(H5SDK_TAG_SORT_TYPE_GRP));
    //��Ҫ������ֶ�ID
    RspSort.SortFieldId = lpBody->GetItem(H5SDK_TAG_SORT_FIELD_ID)->GetInt32();
    //��Ҫ������ֶ�
    hs_strncpy(RspSort.SortFieldName, U2G(lpBody->GetItem(H5SDK_TAG_SORT_FIELD_NAME)->GetString()), sizeof(RspSort.SortFieldName));
    //��ʼλ��,Ĭ��Ϊ0
    RspSort.StartPos = lpBody->GetItem(H5SDK_TAG_START_POS)->GetInt32();
    //�����б�
    DealGroupAsyResponse_SortSortProdGrp(&RspSort, lpBody->GetGroup(H5SDK_TAG_SORT_PROD_GRP));

    //�ص�
    lpH5sdkSpi->OnRspH5sdkSort(&RspSort, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_TrendTrendGrp(CHundsunH5sdkRspTrendField* lpRspTrend, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspTrendTrendGrpField m_TrendGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //��ʱʱ��hhmm
        m_TrendGrp.MinTime = lpRecord->GetItem(H5SDK_TAG_MIN_TIME)->GetInt32();
        //�۸�
        m_TrendGrp.HqPx = lpRecord->GetItem(H5SDK_TAG_HQ_PRICE)->GetInt32();
        //����
        m_TrendGrp.AvgPx = lpRecord->GetItem(H5SDK_TAG_AVG_PX)->GetInt32();
        //��Ȩ����
        m_TrendGrp.WavgPx = lpRecord->GetItem(H5SDK_TAG_WAVG_PX)->GetInt32();
        //�ܳɽ���
        m_TrendGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //�ܳɽ���
        m_TrendGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();

        //����CHundsunH5sdkRspTrendTrendGrpField����
        lpRspTrend->TrendGrp.push_back(m_TrendGrp);
    }
}

//6001 ��ָ�������ڻ�ƫ��ȡ��ʱ����
void CHundsunH5sdkApi_impl::OnAsyResponse_6001(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspTrendField RspTrend;

    //֤ȯ����
    hs_strncpy(RspTrend.ProdCode, U2G(lpBody->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(RspTrend.ProdCode));
    //���ʹ���
    hs_strncpy(RspTrend.HqTypeCode, U2G(lpBody->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(RspTrend.HqTypeCode));
    //���ڡ���ʽYYYYMMDD
    RspTrend.Date = lpBody->GetItem(H5SDK_TAG_DATE)->GetInt32();
    //��ʱ����
    DealGroupAsyResponse_TrendTrendGrp(&RspTrend, lpBody->GetGroup(H5SDK_TAG_TREND_GRP));

    //�ص�
    lpH5sdkSpi->OnRspH5sdkTrend(&RspTrend, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_CandleByOffsetCandleGrp(CHundsunH5sdkRspCandleByOffsetField* lpRspCandleByOffset, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspCandleByOffsetCandleGrpField m_CandleGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //��ʼ���ڣ�0����ʾ��ǰʱ��
        m_CandleGrp.Date = lpRecord->GetItem(H5SDK_TAG_DATE)->GetInt32();
        //����K�ߵ�ʱ�䣬HHMM
        m_CandleGrp.MinTime = lpRecord->GetItem(H5SDK_TAG_MIN_TIME)->GetInt32();
        //���̼�
        m_CandleGrp.OpenPx = lpRecord->GetItem(H5SDK_TAG_OPEN_PX)->GetInt32();
        //��߼�
        m_CandleGrp.HighPx = lpRecord->GetItem(H5SDK_TAG_HIGH_PX)->GetInt32();
        //��ͼ�
        m_CandleGrp.LowPx = lpRecord->GetItem(H5SDK_TAG_LOW_PX)->GetInt32();
        //���̼�
        m_CandleGrp.ClosePx = lpRecord->GetItem(H5SDK_TAG_CLOSE_PX)->GetInt32();
        //�ɽ���
        m_CandleGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //�ɽ���
        m_CandleGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();

        //����CHundsunH5sdkRspCandleByOffsetCandleGrpField����
        lpRspCandleByOffset->CandleGrp.push_back(m_CandleGrp);
    }
}

//6002 ��ƫ��ȡK��
void CHundsunH5sdkApi_impl::OnAsyResponse_6002(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspCandleByOffsetField RspCandleByOffset;

    //֤ȯȫ����
    hs_strncpy(RspCandleByOffset.ProdCode, U2G(lpBody->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(RspCandleByOffset.ProdCode));
    //���ʹ���
    hs_strncpy(RspCandleByOffset.HqTypeCode, U2G(lpBody->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(RspCandleByOffset.HqTypeCode));
    //k������
    RspCandleByOffset.CandlePeriod = lpBody->GetItem(H5SDK_TAG_CANDLE_PEROID)->GetInt32();
    //
    RspCandleByOffset.CandleMode = lpBody->GetItem(H5SDK_TAG_CANDLE_MODE)->GetInt8();
    //����������Ƿ������� 1��ʾ�������ݣ���ʾ�û���������������Ŀǰ֧�����ĸ�������Ҫ�û����ݷ��ؽ���ٴ�����
    RspCandleByOffset.AllDataFlag = lpBody->GetItem(HSSDK_TAG_ALL_DATA_FLAG)->GetInt8();
    //��ʱ����
    DealGroupAsyResponse_CandleByOffsetCandleGrp(&RspCandleByOffset, lpBody->GetGroup(H5SDK_TAG_CANDLE_GRP));

    //�ص�
    lpH5sdkSpi->OnRspH5sdkCandleByOffset(&RspCandleByOffset, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_CandleByRangeCandleGrp(CHundsunH5sdkRspCandleByRangeField* lpRspCandleByRange, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspCandleByRangeCandleGrpField m_CandleGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //��ʼ���ڣ�0����ʾ��ǰʱ��
        m_CandleGrp.Date = lpRecord->GetItem(H5SDK_TAG_DATE)->GetInt32();
        //
        m_CandleGrp.MinTime = lpRecord->GetItem(H5SDK_TAG_MIN_TIME)->GetInt32();
        //���̼�
        m_CandleGrp.OpenPx = lpRecord->GetItem(H5SDK_TAG_OPEN_PX)->GetInt32();
        //��߼�
        m_CandleGrp.HighPx = lpRecord->GetItem(H5SDK_TAG_HIGH_PX)->GetInt32();
        //��ͼ�
        m_CandleGrp.LowPx = lpRecord->GetItem(H5SDK_TAG_LOW_PX)->GetInt32();
        //���̼�
        m_CandleGrp.ClosePx = lpRecord->GetItem(H5SDK_TAG_CLOSE_PX)->GetInt32();
        //�ɽ���
        m_CandleGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //�ɽ���
        m_CandleGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();

        //����CHundsunH5sdkRspCandleByRangeCandleGrpField����
        lpRspCandleByRange->CandleGrp.push_back(m_CandleGrp);
    }
}

//6003 ������ʱ�䷶ΧȡK��
void CHundsunH5sdkApi_impl::OnAsyResponse_6003(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspCandleByRangeField RspCandleByRange;

    //֤ȯ����
    hs_strncpy(RspCandleByRange.ProdCode, U2G(lpBody->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(RspCandleByRange.ProdCode));
    //���ʹ���
    hs_strncpy(RspCandleByRange.HqTypeCode, U2G(lpBody->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(RspCandleByRange.HqTypeCode));
    //k������
    RspCandleByRange.CandlePeriod = lpBody->GetItem(H5SDK_TAG_CANDLE_PEROID)->GetInt32();
    //
    RspCandleByRange.CandleMode = lpBody->GetItem(H5SDK_TAG_CANDLE_MODE)->GetInt8();
    //����������Ƿ������� 1��ʾ��������
    RspCandleByRange.AllDataFlag = lpBody->GetItem(HSSDK_TAG_ALL_DATA_FLAG)->GetInt8();
    //��ʱ����
    DealGroupAsyResponse_CandleByRangeCandleGrp(&RspCandleByRange, lpBody->GetGroup(H5SDK_TAG_CANDLE_GRP));

    //�ص�
    lpH5sdkSpi->OnRspH5sdkCandleByRange(&RspCandleByRange, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_TickMinTickGrp(CHundsunH5sdkRspTickMinField* lpRspTickMin, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspTickMinTickGrpField m_TickGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //�ֱ����
        m_TickGrp.BusinessNo = lpRecord->GetItem(H5SDK_TAG_BUSINESS_NO)->GetInt32();
        //ʱ���
        m_TickGrp.BusinessTime = lpRecord->GetItem(H5SDK_TAG_BUSINESS_TIME)->GetInt32();
        //�۸�
        m_TickGrp.HqPx = lpRecord->GetItem(H5SDK_TAG_HQ_PRICE)->GetInt32();
        //�ɽ���
        m_TickGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //�ɽ���
        m_TickGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();
        //�ɽ�����
        m_TickGrp.BusinessCount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_COUNT)->GetInt32();
        //�ɽ�����0������1����)
        m_TickGrp.BusinessDirection = lpRecord->GetItem(HSSDK_TAG_BUSINESS_DIRECTION)->GetInt32();

        //����CHundsunH5sdkRspTickMinTickGrpField����
        lpRspTickMin->TickGrp.push_back(m_TickGrp);
    }
}

//6004 ָ�����ӵķֱ�
void CHundsunH5sdkApi_impl::OnAsyResponse_6004(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspTickMinField RspTickMin;

    //֤ȯ����
    hs_strncpy(RspTickMin.ProdCode, U2G(lpBody->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(RspTickMin.ProdCode));
    //���ʹ���
    hs_strncpy(RspTickMin.HqTypeCode, U2G(lpBody->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(RspTickMin.HqTypeCode));
    //���ڡ���ʽYYYYMMDD
    RspTickMin.Date = lpBody->GetItem(H5SDK_TAG_DATE)->GetInt32();
    //��ʼ������
    RspTickMin.StartMin = lpBody->GetItem(H5SDK_TAG_START_MIN)->GetInt32();
    //����������
    RspTickMin.EndMin = lpBody->GetItem(H5SDK_TAG_END_MIN)->GetInt32();
    //����������Ƿ������� 1��ʾ��������
    RspTickMin.AllDataFlag = lpBody->GetItem(HSSDK_TAG_ALL_DATA_FLAG)->GetInt8();
    //��ʱ����
    DealGroupAsyResponse_TickMinTickGrp(&RspTickMin, lpBody->GetGroup(H5SDK_TAG_TICK_GRP));

    //�ص�
    lpH5sdkSpi->OnRspH5sdkTickMin(&RspTickMin, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_TickDirectionTickGrp(CHundsunH5sdkRspTickDirectionField* lpRspTickDirection, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspTickDirectionTickGrpField m_TickGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //�ֱ����
        m_TickGrp.BusinessNo = lpRecord->GetItem(H5SDK_TAG_BUSINESS_NO)->GetInt32();
        //ʱ���
        m_TickGrp.BusinessTime = lpRecord->GetItem(H5SDK_TAG_BUSINESS_TIME)->GetInt32();
        //�۸�
        m_TickGrp.HqPx = lpRecord->GetItem(H5SDK_TAG_HQ_PRICE)->GetInt32();
        //�ɽ���
        m_TickGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //�ɽ���
        m_TickGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();
        //�ɽ�����
        m_TickGrp.BusinessCount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_COUNT)->GetInt32();
        //�ɽ�����0������1����)
        m_TickGrp.BusinessDirection = lpRecord->GetItem(HSSDK_TAG_BUSINESS_DIRECTION)->GetInt32();

        //����CHundsunH5sdkRspTickDirectionTickGrpField����
        lpRspTickDirection->TickGrp.push_back(m_TickGrp);
    }
}

//6005 ��ƫ��ȡ�ֱ�
void CHundsunH5sdkApi_impl::OnAsyResponse_6005(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspTickDirectionField RspTickDirection;

    //֤ȯ����
    hs_strncpy(RspTickDirection.ProdCode, U2G(lpBody->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(RspTickDirection.ProdCode));
    //���ʹ���
    hs_strncpy(RspTickDirection.HqTypeCode, U2G(lpBody->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(RspTickDirection.HqTypeCode));
    //����:YYYYMMDD,0:��ʾ����
    RspTickDirection.Date = lpBody->GetItem(H5SDK_TAG_DATE)->GetInt32();
    //��ʼλ��,Ĭ��Ϊ0����ǰȡʱ��0��ʾ�����һ�ʣ����ȡʱ0��ʾ�����һ���ֱ�
    RspTickDirection.StartPos = lpBody->GetItem(H5SDK_TAG_START_POS)->GetInt32();
    //����
    RspTickDirection.SearchDirection = lpBody->GetItem(H5SDK_TAG_DIRECTION)->GetInt8();
    //����������Ƿ������� 1��ʾ��������
    RspTickDirection.AllDataFlag = lpBody->GetItem(HSSDK_TAG_ALL_DATA_FLAG)->GetInt8();
    //��ʱ����
    DealGroupAsyResponse_TickDirectionTickGrp(&RspTickDirection, lpBody->GetGroup(H5SDK_TAG_TICK_GRP));

    //�ص�
    lpH5sdkSpi->OnRspH5sdkTickDirection(&RspTickDirection, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_CandleByRangeMaintainCandleGrp(CHundsunH5sdkRspCandleByRangeMaintainField* lpRspCandleByRangeMaintain, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspCandleByRangeMaintainCandleGrpField m_CandleGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //��ʼ���ڣ�0����ʾ��ǰʱ��
        m_CandleGrp.Date = lpRecord->GetItem(H5SDK_TAG_DATE)->GetInt32();
        //ʱ��
        m_CandleGrp.MinTime = lpRecord->GetItem(H5SDK_TAG_MIN_TIME)->GetInt32();
        //����
        m_CandleGrp.WavgPx = lpRecord->GetItem(H5SDK_TAG_WAVG_PX)->GetInt32();
        //���̼�
        m_CandleGrp.OpenPx = lpRecord->GetItem(H5SDK_TAG_OPEN_PX)->GetInt32();
        //��߼�
        m_CandleGrp.HighPx = lpRecord->GetItem(H5SDK_TAG_HIGH_PX)->GetInt32();
        //��ͼ�
        m_CandleGrp.LowPx = lpRecord->GetItem(H5SDK_TAG_LOW_PX)->GetInt32();
        //���̼�
        m_CandleGrp.ClosePx = lpRecord->GetItem(H5SDK_TAG_CLOSE_PX)->GetInt32();
        //�ɽ���
        m_CandleGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //�ɽ���
        m_CandleGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();
        //
        m_CandleGrp.Amount = lpRecord->GetItem(H5SDK_TAG_AMOUNT)->GetInt64();

        //����CHundsunH5sdkRspCandleByRangeMaintainCandleGrpField����
        lpRspCandleByRangeMaintain->CandleGrp.push_back(m_CandleGrp);
    }
}

//6006 ����ʹ��_����k��
void CHundsunH5sdkApi_impl::OnAsyResponse_6006(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspCandleByRangeMaintainField RspCandleByRangeMaintain;

    //֤ȯ����
    hs_strncpy(RspCandleByRangeMaintain.ProdCode, U2G(lpBody->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(RspCandleByRangeMaintain.ProdCode));
    //���ʹ���
    hs_strncpy(RspCandleByRangeMaintain.HqTypeCode, U2G(lpBody->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(RspCandleByRangeMaintain.HqTypeCode));
    //k������
    RspCandleByRangeMaintain.CandlePeriod = lpBody->GetItem(H5SDK_TAG_CANDLE_PEROID)->GetInt32();
    //
    RspCandleByRangeMaintain.CandleMode = lpBody->GetItem(H5SDK_TAG_CANDLE_MODE)->GetInt8();
    //����������Ƿ������� 1��ʾ��������
    RspCandleByRangeMaintain.AllDataFlag = lpBody->GetItem(HSSDK_TAG_ALL_DATA_FLAG)->GetInt8();
    //��ʱ����
    DealGroupAsyResponse_CandleByRangeMaintainCandleGrp(&RspCandleByRangeMaintain, lpBody->GetGroup(H5SDK_TAG_CANDLE_GRP));

    //�ص�
    lpH5sdkSpi->OnRspH5sdkCandleByRangeMaintain(&RspCandleByRangeMaintain, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_Day5TrendTrendGrp(CHundsunH5sdkRspDay5TrendField* lpRspDay5Trend, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // ��ȡ��¼����
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspDay5TrendTrendGrpField m_TrendGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //����:YYYYMMDD
        m_TrendGrp.Date = lpRecord->GetItem(H5SDK_TAG_DATE)->GetInt32();
        //��ʱʱ��hhmm
        m_TrendGrp.MinTime = lpRecord->GetItem(H5SDK_TAG_MIN_TIME)->GetInt32();
        //�۸�
        m_TrendGrp.HqPx = lpRecord->GetItem(H5SDK_TAG_HQ_PRICE)->GetInt32();
        //��Ȩ����
        m_TrendGrp.WavgPx = lpRecord->GetItem(H5SDK_TAG_WAVG_PX)->GetInt32();
        //�ܳɽ���
        m_TrendGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //�ܳɽ���
        m_TrendGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();

        //����CHundsunH5sdkRspDay5TrendTrendGrpField����
        lpRspDay5Trend->TrendGrp.push_back(m_TrendGrp);
    }
}

//6007 5�շ�ʱ
void CHundsunH5sdkApi_impl::OnAsyResponse_6007(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspDay5TrendField RspDay5Trend;

    //֤ȯ����
    hs_strncpy(RspDay5Trend.ProdCode, U2G(lpBody->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(RspDay5Trend.ProdCode));
    //���ʹ���
    hs_strncpy(RspDay5Trend.HqTypeCode, U2G(lpBody->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(RspDay5Trend.HqTypeCode));
    //��ʱ����
    DealGroupAsyResponse_Day5TrendTrendGrp(&RspDay5Trend, lpBody->GetGroup(H5SDK_TAG_TREND_GRP));

    //�ص�
    lpH5sdkSpi->OnRspH5sdkDay5Trend(&RspDay5Trend, &RspInfo, nRequestID, true);
}

//7001 ���߷���
void CHundsunH5sdkApi_impl::OnAsyResponse_7001(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //�жϴ�����Ϣ
    CHundsunH5sdkRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunH5sdkRspMaintainReturnField RspMaintainReturn;


    //�ص�
    lpH5sdkSpi->OnRspH5sdkMaintainReturn(&RspMaintainReturn, &RspInfo, nRequestID, true);
}

