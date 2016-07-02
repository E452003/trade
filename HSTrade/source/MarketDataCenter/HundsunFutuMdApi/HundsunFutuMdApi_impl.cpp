#include "HundsunFutuMdApi_impl.h"
#include "HundsunFutuAnalyStrFunction.h"

//���������Ϣ
inline void DealErrorInfo(IF2UnPacker *lpUnPacker, CHundsunFutuRspInfoField *RspInfo)
{
    RspInfo->ErrorNo = lpUnPacker->GetInt("error_no");
    if(RspInfo->ErrorNo == 0)
    {
        hs_strncpy(RspInfo->ErrorInfo, "No Error", sizeof(RspInfo->ErrorInfo));
    }
    else
    {
        hs_strncpy(RspInfo->ErrorInfo, lpUnPacker->GetStr("error_info"), sizeof(RspInfo->ErrorInfo));
    }
};

CHundsunFutuMdApi *CHundsunFutuMdApi::CreateHundsunFutuMdApi(const char *pszFlowPath, bool bSaveLog)
{
    return new CHundsunFutuMdApi_impl(pszFlowPath, bSaveLog);
}

CHundsunFutuMdApi_impl::CHundsunFutuMdApi_impl(const char *pszFlowPath, bool bSaveLog)
{
    lpMdSpi = NULL;
    lpMdApi_log = NULL;

    m_localip = "";
    m_localMac = "";
    m_Logined = false;
    m_SaveLog = bSaveLog;
    m_bHeartKeepStop = false;

    memset(&m_userInfo, 0, sizeof(CHundsunFutuUserInfoField));
}

CHundsunFutuMdApi_impl::~CHundsunFutuMdApi_impl()
{
    m_bHeartKeepStop = true;
    if(m_HeartKeepTrd.IsRunning())
    {
        m_HeartKeepTrd.Join();
    }
    StopTrd();

    m_SubFutuCodeSet.clear();
    m_SubArbitCodeSet.clear();
    hJoinEvent.Notify();
}

void CHundsunFutuMdApi_impl::ReleaseApi()
{
    delete this;
}

void CHundsunFutuMdApi_impl::Init(const char *pszLicFile, const char *pszPwd, const char *pszSslFile, const char *pszSslPwd)
{
    InitT2Connect(pszLicFile, pszPwd, pszSslFile, pszSslPwd);
    return;
}

int CHundsunFutuMdApi_impl::Join()
{
    hJoinEvent.Wait(-1);
    return 0;
}

void CHundsunFutuMdApi_impl::RegisterFront(const char *pszFrontAddress, int nSubSystemNo, int nCompanyID, const char *pszConnName)
{
    //��ֵǰ�õ�ַ
    hs_strncpy(m_szFrontAddress, pszFrontAddress, sizeof(m_szFrontAddress));
    //��ֵ��ϵͳ��
    m_SubSystemNo = nSubSystemNo;
    //��ֵ��˾���
    m_CompanyID = nCompanyID;
    //����T2��Login_name
    hs_strncpy(m_szConnName, pszConnName, sizeof(m_szConnName));

    return;
}

void CHundsunFutuMdApi_impl::RegisterSpi(CHundsunFutuMdSpi *pSpi)
{
    lpMdSpi = pSpi;
    if(pSpi != NULL)
    {
        StartTrd();
    }
    else
    {
        StopTrd();
    }
}

CHundsunFutuUserInfoField* CHundsunFutuMdApi_impl::GetUserInfo()
{
    return &m_userInfo;
}

//331100 �ͻ���¼����
int CHundsunFutuMdApi_impl::ReqFuncClientLogin(CHundsunFutuReqFuncClientLoginField *pReqFuncClientLogin, int nRequestID, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    if(lpMdApi_log == NULL)
    {
        lpMdApi_log = new CHundsunFutuMdApi_log(pReqFuncClientLogin->AccountContent);
    }
    m_userInfo.OpEntrustWay = pReqFuncClientLogin->OpEntrustWay; //����ί�з�ʽ
    hs_strncpy(m_userInfo.Password, pReqFuncClientLogin->Password, sizeof(m_userInfo.Password)); //���潻������

    if(m_SaveLog && lpMdApi_log != NULL)
        lpMdApi_log->AddLog(LOGINFO_Request, FuncClientLogin, 0, pReqFuncClientLogin, sizeof(CHundsunFutuReqFuncClientLoginField));

    //����Pack��
    IF2Packer *lpF2Packer = NewPacker(PackerVersion);
    lpF2Packer->AddRef();
    lpF2Packer->BeginPack();
    lpF2Packer->AddField("op_branch_no",'I');                                   //������֧����
    lpF2Packer->AddField("op_entrust_way",'C');                                 //ί�з�ʽ
    lpF2Packer->AddField("op_station",'S',255);                                 //վ���ַ
    lpF2Packer->AddField("branch_no",'I');                                      //��֧����
    lpF2Packer->AddField("password",'S',50);                                    //����
    lpF2Packer->AddField("password_type",'C');                                  //�������
    lpF2Packer->AddField("input_content",'C');                                  //�ͻ���־���
    lpF2Packer->AddField("account_content",'S',30);                             //��������
    lpF2Packer->AddField("content_type",'S',6);                                 //���кš��г����
    lpF2Packer->AddField("asset_prop",'C');                                     //�ʲ�����

    lpF2Packer->AddInt(m_userInfo.OpBranchNo);
    lpF2Packer->AddChar(m_userInfo.OpEntrustWay);
    lpF2Packer->AddStr(m_userInfo.OpStation);
    lpF2Packer->AddInt(m_userInfo.BranchNo);
    lpF2Packer->AddStr(m_userInfo.Password);
    lpF2Packer->AddChar(m_userInfo.PasswordType);
    lpF2Packer->AddChar(pReqFuncClientLogin->InputContent);
    lpF2Packer->AddStr(pReqFuncClientLogin->AccountContent);
    lpF2Packer->AddStr(pReqFuncClientLogin->ContentType);
    lpF2Packer->AddChar(pReqFuncClientLogin->AssetProp);
    lpF2Packer->EndPack();
    //���������
    int iRet = SendBizMessage(lpF2Packer, FuncClientLogin, 0, nRequestID, ppRetData, pnItems, pRspInfo);
    //����ͷŴ����
    lpF2Packer->FreeMem(lpF2Packer->GetPackBuf());
    lpF2Packer->Release();

    return iRet;
};

//330851 �ڻ���Լ��֤���ѯ����
int CHundsunFutuMdApi_impl::ReqFuncFutuContractbailQry(CHundsunFutuReqFuncFutuContractbailQryField *pReqFuncFutuContractbailQry, int nRequestID, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    if(m_SaveLog && lpMdApi_log != NULL)
        lpMdApi_log->AddLog(LOGINFO_Request, FuncFutuContractbailQry, 0, pReqFuncFutuContractbailQry, sizeof(CHundsunFutuReqFuncFutuContractbailQryField));

    //��������󱣴棬��ҳ��ѯʹ��
    //�첽�����ʵ�ַ�ҳ��ѯ��ͬ�������ϲ�ʵ��
    if(strcmp(pReqFuncFutuContractbailQry->PositionStr, "") == 0 && ppRetData == NULL)
    {
        CQueryInfo *qryinfo = new CQueryInfo(FuncFutuContractbailQry, nRequestID, pReqFuncFutuContractbailQry, sizeof(CHundsunFutuReqFuncFutuContractbailQryField));
        AddCQueryInfo(qryinfo);
    }

    //����Pack��
    IF2Packer *lpF2Packer = NewPacker(PackerVersion);
    lpF2Packer->AddRef();
    lpF2Packer->BeginPack();
    lpF2Packer->AddField("user_token",'S',512);                                 //�û�����
    lpF2Packer->AddField("op_branch_no",'I');                                   //������֧����
    lpF2Packer->AddField("op_entrust_way",'C');                                 //ί�з�ʽ
    lpF2Packer->AddField("op_station",'S',255);                                 //վ���ַ
    lpF2Packer->AddField("branch_no",'I');                                      //��֧����
    lpF2Packer->AddField("client_id",'S',18);                                   //�ͻ����
    lpF2Packer->AddField("fund_account",'S',18);                                //�ʲ��˻�
    lpF2Packer->AddField("password",'S',50);                                    //����
    lpF2Packer->AddField("futu_exch_type",'S',4);                               //�������
    lpF2Packer->AddField("futu_code",'S',30);                                   //��Լ����
    lpF2Packer->AddField("request_num",'I');                                    //��������
    lpF2Packer->AddField("position_str",'S',100);                               //��λ��

    lpF2Packer->AddStr(pReqFuncFutuContractbailQry->UserToken);
    lpF2Packer->AddInt(m_userInfo.OpBranchNo);
    lpF2Packer->AddChar(m_userInfo.OpEntrustWay);
    lpF2Packer->AddStr(m_userInfo.OpStation);
    lpF2Packer->AddInt(m_userInfo.BranchNo);
    lpF2Packer->AddStr(m_userInfo.ClientId);
    lpF2Packer->AddStr(m_userInfo.FundAccount);
    lpF2Packer->AddStr(m_userInfo.Password);
    lpF2Packer->AddStr(pReqFuncFutuContractbailQry->FutuExchType);
    lpF2Packer->AddStr(pReqFuncFutuContractbailQry->FutuCode);
    lpF2Packer->AddInt(pReqFuncFutuContractbailQry->RequestNum);
    lpF2Packer->AddStr(pReqFuncFutuContractbailQry->PositionStr);
    lpF2Packer->EndPack();
    //���������
    int iRet = SendBizMessage(lpF2Packer, FuncFutuContractbailQry, 0, nRequestID, ppRetData, pnItems, pRspInfo);
    //����ͷŴ����
    lpF2Packer->FreeMem(lpF2Packer->GetPackBuf());
    lpF2Packer->Release();

    return iRet;
};

//620001_33100 ����-�ڻ�������������
int CHundsunFutuMdApi_impl::ReqFuncQhSingleHqSubscribe(char *ppInstrumentID[], int nCount, int nRequestID, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //����Pack��
    IF2Packer *lpF2Packer = NewPacker(PackerVersion);
    lpF2Packer->AddRef();
    lpF2Packer->BeginPack();
    lpF2Packer->AddField("op_branch_no",'I');                                   //������֧����
    lpF2Packer->AddField("op_entrust_way",'C');                                 //ί�з�ʽ
    lpF2Packer->AddField("op_station",'S',255);                                 //վ���ַ
    lpF2Packer->AddField("branch_no",'I');                                      //��֧����
    lpF2Packer->AddField("client_id",'S',18);                                   //�ͻ����
    lpF2Packer->AddField("fund_account",'S',18);                                //�ʽ��˻�
    lpF2Packer->AddField("password",'S',10);                                    //����
    lpF2Packer->AddField("user_token",'S',512);                                 //�û�����
    lpF2Packer->AddField("issue_type",'I');                                     //�������ͣ�����Ϊ33100-�ڻ����ȱ䶯���飩
    lpF2Packer->AddField("futu_code",'S',30);                                   //�ڻ�����

    for(int i = 0; i < nCount; i++)
    {
        if(isArbit(ppInstrumentID[i]))
            continue;
        if( m_SubFutuCodeSet.find(ppInstrumentID[i]) == m_SubFutuCodeSet.end())
        {
            m_SubFutuCodeSet.insert(ppInstrumentID[i]);
        }
        lpF2Packer->AddInt(m_userInfo.OpBranchNo);
        lpF2Packer->AddChar(m_userInfo.OpEntrustWay);
        lpF2Packer->AddStr(m_userInfo.OpStation);
        lpF2Packer->AddInt(m_userInfo.BranchNo);
        lpF2Packer->AddStr(m_userInfo.ClientId);
        lpF2Packer->AddStr(m_userInfo.FundAccount);
        lpF2Packer->AddStr(m_userInfo.Password);
        lpF2Packer->AddStr(m_userInfo.UserToken);
        lpF2Packer->AddInt(33100);
        lpF2Packer->AddStr(ppInstrumentID[i]);
    }
    lpF2Packer->EndPack();
    //���������
    int iRet = SendBizMessage(lpF2Packer, 620001, 33100, nRequestID, ppRetData, pnItems, pRspInfo);
    //����ͷŴ����
    lpF2Packer->FreeMem(lpF2Packer->GetPackBuf());
    lpF2Packer->Release();

    return iRet;
};

//620002_33100 ����ȡ��-�ڻ�������������
int CHundsunFutuMdApi_impl::ReqFuncQhSingleHqCancel(char *ppInstrumentID[], int nCount, int nRequestID, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //����Pack��
    IF2Packer *lpF2Packer = NewPacker(PackerVersion);
    lpF2Packer->AddRef();
    lpF2Packer->BeginPack();
    lpF2Packer->AddField("issue_type",'I');                                     //��������
    lpF2Packer->AddField("futu_code",'S',30);                                   //�ڻ�����

    for(int i = 0; i < nCount; i++)
    {
        if(isArbit(ppInstrumentID[i]))
            continue;
        if( m_SubFutuCodeSet.find(ppInstrumentID[i]) != m_SubFutuCodeSet.end())
        {
            m_SubFutuCodeSet.erase(ppInstrumentID[i]);
        }
        lpF2Packer->AddInt(33100);
        lpF2Packer->AddStr(ppInstrumentID[i]);
    }
    lpF2Packer->EndPack();
    //���������
    int iRet = SendBizMessage(lpF2Packer, 620002, 33100, nRequestID, ppRetData, pnItems, pRspInfo);
    //����ͷŴ����
    lpF2Packer->FreeMem(lpF2Packer->GetPackBuf());
    lpF2Packer->Release();

    return iRet;
};

//620001_33102 ����-�ڻ������������
int CHundsunFutuMdApi_impl::ReqFuncQhCombineSubscribe(char *ppInstrumentID[], int nCount, int nRequestID, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //����Pack��
    IF2Packer *lpF2Packer = NewPacker(PackerVersion);
    lpF2Packer->AddRef();
    lpF2Packer->BeginPack();
    lpF2Packer->AddField("op_branch_no",'I');                                   //������֧����
    lpF2Packer->AddField("op_entrust_way",'C');                                 //ί�з�ʽ
    lpF2Packer->AddField("op_station",'S',255);                                 //վ���ַ
    lpF2Packer->AddField("branch_no",'I');                                      //��֧����
    lpF2Packer->AddField("client_id",'S',18);                                   //�ͻ����
    lpF2Packer->AddField("fund_account",'S',18);                                //�ʽ��˻�
    lpF2Packer->AddField("password",'S',10);                                    //����
    lpF2Packer->AddField("user_token",'S',512);                                 //�û�����
    lpF2Packer->AddField("issue_type",'I');                                     //�������ͣ�����Ϊ33102-�ڻ���ϱ䶯����)
    lpF2Packer->AddField("arbit_code",'S',30);                                  //������Լ��

    for(int i = 0; i < nCount; i++)
    {
        if(!isArbit(ppInstrumentID[i]))
            continue;
        if( m_SubArbitCodeSet.find(ppInstrumentID[i]) == m_SubArbitCodeSet.end())
        {
            m_SubArbitCodeSet.insert(ppInstrumentID[i]);
        }
        lpF2Packer->AddInt(m_userInfo.OpBranchNo);
        lpF2Packer->AddChar(m_userInfo.OpEntrustWay);
        lpF2Packer->AddStr(m_userInfo.OpStation);
        lpF2Packer->AddInt(m_userInfo.BranchNo);
        lpF2Packer->AddStr(m_userInfo.ClientId);
        lpF2Packer->AddStr(m_userInfo.FundAccount);
        lpF2Packer->AddStr(m_userInfo.Password);
        lpF2Packer->AddStr(m_userInfo.UserToken);
        lpF2Packer->AddInt(33102);
        lpF2Packer->AddStr(ppInstrumentID[i]);
    }
    lpF2Packer->EndPack();
    //���������
    int iRet = SendBizMessage(lpF2Packer, 620001, 33102, nRequestID, ppRetData, pnItems, pRspInfo);
    //����ͷŴ����
    lpF2Packer->FreeMem(lpF2Packer->GetPackBuf());
    lpF2Packer->Release();

    return iRet;
};

//620002_33102 ����ȡ��-�ڻ������������
int CHundsunFutuMdApi_impl::ReqFuncQhCombineCancel(char *ppInstrumentID[], int nCount, int nRequestID, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //����Pack��
    IF2Packer *lpF2Packer = NewPacker(PackerVersion);
    lpF2Packer->AddRef();
    lpF2Packer->BeginPack();
    lpF2Packer->AddField("issue_type",'I');                                     //��������
    lpF2Packer->AddField("arbit_code",'S',30);                                  //������Լ��

    for(int i = 0; i < nCount; i++)
    {
        if(!isArbit(ppInstrumentID[i]))
            continue;
        if( m_SubArbitCodeSet.find(ppInstrumentID[i]) != m_SubArbitCodeSet.end())
        {
            m_SubArbitCodeSet.erase(ppInstrumentID[i]);
        }
        lpF2Packer->AddInt(33102);
        lpF2Packer->AddStr(ppInstrumentID[i]);
    }
    lpF2Packer->EndPack();
    //���������
    int iRet = SendBizMessage(lpF2Packer, 620002, 33102, nRequestID, ppRetData, pnItems, pRspInfo);
    //����ͷŴ����
    lpF2Packer->FreeMem(lpF2Packer->GetPackBuf());
    lpF2Packer->Release();

    return iRet;
};

void CHundsunFutuMdApi_impl::OnConnected()
{
    char addr[64];
    char ip[64];
    strcpy(&addr[0], lpConnection->GetRealAddress());
    if(strcmp(addr, "") != 0)
    {
        sscanf(addr, "%[^:]s", ip);
        m_localip = ip;
    }
    strcpy(&addr[0], lpConnection->GetSelfMac());
    if(strcmp(addr, "") != 0)
    {
        m_localMac = addr;
    }
    sprintf(m_userInfo.OpStation, "%s:%s", m_localip.c_str(), m_localMac.c_str());

    if(lpMdSpi)
    {
        lpMdSpi->OnFrontConnected();
    }

    //����֮���Զ�����
    if(true)//m_AutoReConnect
    {
        m_SubSetCs.Lock();
        if(m_SubFutuCodeSet.size() > 0)
        {
            char** pptr = new char*[m_SubFutuCodeSet.size() + 1]; 
            int    i = 0;
            for(set<string>::iterator it = m_SubFutuCodeSet.begin(); it != m_SubFutuCodeSet.end(); it++, i++)
            {
                pptr[i] = (char*)(*it).c_str(); 
            }
            pptr[i] = 0;
            ReqFuncQhSingleHqSubscribe(pptr, m_SubFutuCodeSet.size(), -33100);
        }
        if(m_SubArbitCodeSet.size() > 0)
        {
            char** pptr = new char*[m_SubArbitCodeSet.size() + 1]; 
            int    i = 0;
            for(set<string>::iterator it = m_SubArbitCodeSet.begin(); it != m_SubArbitCodeSet.end(); it++, i++)
            {
                pptr[i] = (char*)(*it).c_str(); 
            }
            pptr[i] = 0;
            ReqFuncQhCombineSubscribe(pptr, m_SubArbitCodeSet.size(), -33102);
        }
        m_SubSetCs.UnLock();
    }
}

void CHundsunFutuMdApi_impl::OnClosed()
{
    if(lpMdSpi)
    {
        lpMdSpi->OnFrontDisconnected(0x1001);
    }
}

void CHundsunFutuMdApi_impl::DealAsyBizMessage(IBizMessage* lpMsg)
{
    if (lpMsg!=NULL)
    {
        //�ɹ�,Ӧ�ó������ͷ�lpBizMessageRecv��Ϣ
        if (lpMsg->GetErrorNo() ==0)
        {
            int iLen = 0;
            const void *lpBuffer = lpMsg->GetContent(iLen);
            IF2UnPacker *lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
            if(lpUnPacker != NULL)
                lpUnPacker->AddRef();
            int iLen_key = 0;
            const void *lpBuffer_key = lpMsg->GetKeyInfo(iLen_key);
            IF2UnPacker *lpUnPacker_key = NewUnPacker((void *)lpBuffer_key,iLen_key);
            if(lpUnPacker_key != NULL)
                lpUnPacker_key->AddRef();

            switch(lpMsg->GetFunction())
            {
            case 620003:
                {
                    if(lpMsg->GetIssueType() == 33100)
                        OnAsyRtn_620003_33100(lpUnPacker, lpMsg->GetSenderId());
                    if(lpMsg->GetIssueType() == 33102)
                        OnAsyRtn_620003_33102(lpUnPacker, lpMsg->GetSenderId());
                }
                break;
            case 620001:
                {
                    if(lpMsg->GetIssueType() == 33100)
                        OnAsyRsp_620001_33100(lpUnPacker_key, lpMsg->GetSenderId());
                    if(lpMsg->GetIssueType() == 33102)
                        OnAsyRsp_620001_33102(lpUnPacker_key, lpMsg->GetSenderId());
                }
                break;
            case 620002:
                {
                    if(lpMsg->GetIssueType() == 33100)
                        OnAsyRsp_620002_33100(lpUnPacker_key, lpMsg->GetSenderId());
                    if(lpMsg->GetIssueType() == 33102)
                        OnAsyRsp_620002_33102(lpUnPacker_key, lpMsg->GetSenderId());
                }
                break;
            case FuncClientLogin:
                OnAsyResponse_331100(lpUnPacker, lpMsg->GetSenderId());
                break;
            case FuncFutuContractbailQry:
                OnAsyResponse_330851(lpUnPacker, lpMsg->GetSenderId());
                break;
            default:
                break;
            }

            if(lpUnPacker != NULL)
                lpUnPacker->Release();
            if(lpUnPacker_key != NULL)
                lpUnPacker_key->Release();

        }
        else
        {
            puts(lpMsg->GetErrorInfo());
        }
    }
};

int CHundsunFutuMdApi_impl::DealSynBizMessage(const void* lpBuffer, int nLen, void** ppRetData, int* pnItems, void *pRspInfo)
{
    IBizMessage* lpMsg = NewBizMessage();
    lpMsg->AddRef();
    lpMsg->SetBuff(lpBuffer, nLen);

    int iRet = lpMsg->GetReturnCode();
    if (iRet == 0 || iRet > 1)
    {
        int iLen = 0;
        const void *lpBuffer = lpMsg->GetContent(iLen);
        IF2UnPacker *lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
        int iLen_key = 0;
        const void *lpBuffer_key = lpMsg->GetKeyInfo(iLen_key);
        IF2UnPacker *lpUnPacker_key = NewUnPacker((void *)lpBuffer_key,iLen_key);

        switch(lpMsg->GetFunction())
        {
        case 620001:
            {
                if(lpMsg->GetIssueType() == 33100)
                    OnSynRsp_620001_33100(lpUnPacker_key, ppRetData, pnItems, (CHundsunFutuRspInfoField *)pRspInfo);
                if(lpMsg->GetIssueType() == 33102)
                    OnSynRsp_620001_33102(lpUnPacker_key, ppRetData, pnItems, (CHundsunFutuRspInfoField *)pRspInfo);
            }
            break;
        case 620002:
            {
                if(lpMsg->GetIssueType() == 33100)
                    OnSynRsp_620002_33100(lpUnPacker_key, ppRetData, pnItems, (CHundsunFutuRspInfoField *)pRspInfo);
                if(lpMsg->GetIssueType() == 33102)
                    OnSynRsp_620002_33102(lpUnPacker_key, ppRetData, pnItems, (CHundsunFutuRspInfoField *)pRspInfo);
            }
            break;
        case FuncClientLogin:
            OnSynResponse_331100(lpUnPacker, ppRetData, pnItems, (CHundsunFutuRspInfoField *)pRspInfo);
            break;
        case FuncFutuContractbailQry:
            OnSynResponse_330851(lpUnPacker, ppRetData, pnItems, (CHundsunFutuRspInfoField *)pRspInfo);
            break;
        default:
            break;
        }
    }
    else
    {
        //ȡ-1��1������Ϣ
        GetMsgErrorInfo(iRet, pRspInfo);
    }

    lpMsg->Release();
    return iRet;
}

void CHundsunFutuMdApi_impl::RecHeartbeet()
{
    m_iHeartKeep = 0;
}

void CHundsunFutuMdApi_impl::GetMsgErrorInfo(int nErrorCode, void *pRspInfo)
{
    if(pRspInfo != NULL)
    {
        ((CHundsunFutuRspInfoField *)pRspInfo)->ErrorNo = nErrorCode;
        hs_strncpy(((CHundsunFutuRspInfoField *)pRspInfo)->ErrorInfo, lpConnection->GetErrorMsg(nErrorCode), sizeof(((CHundsunFutuRspInfoField *)pRspInfo)->ErrorInfo));
    }
}

//331100 �ͻ���¼
void CHundsunFutuMdApi_impl::OnAsyResponse_331100(IF2UnPacker *lpUnPacker, int nRequestID)
{
    //�жϴ�����Ϣ
    CHundsunFutuRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunFutuRspFuncClientLoginField RspFuncClientLogin;
    memset(&RspFuncClientLogin, 0, sizeof(CHundsunFutuRspFuncClientLoginField));

    //��ô�����Ϣ
    DealErrorInfo(lpUnPacker, &RspInfo);
    if( RspInfo.ErrorNo != 0 )
    {
        if(m_SaveLog && lpMdApi_log != NULL)
            lpMdApi_log->AddLog(LOGINFO_Response, FuncClientLogin, 0, &RspFuncClientLogin, sizeof(CHundsunFutuRspFuncClientLoginField), &RspInfo, sizeof(CHundsunFutuRspInfoField));
        lpMdSpi->OnRspFuncClientLogin(&RspFuncClientLogin, &RspInfo, nRequestID, true);
    }
    else
    {
        //��������
        RspFuncClientLogin.InitDate = lpUnPacker->GetInt("init_date");
        //ϵͳ״̬
        RspFuncClientLogin.SysStatus = lpUnPacker->GetChar("sys_status");
        //��˾����
        hs_strncpy(RspFuncClientLogin.CompanyName, lpUnPacker->GetStr("company_name"), sizeof(RspFuncClientLogin.CompanyName));
        //���кš��г����
        hs_strncpy(RspFuncClientLogin.ContentType, lpUnPacker->GetStr("content_type"), sizeof(RspFuncClientLogin.ContentType));
        //��������
        hs_strncpy(RspFuncClientLogin.AccountContent, lpUnPacker->GetStr("account_content"), sizeof(RspFuncClientLogin.AccountContent));
        //��֧����
        RspFuncClientLogin.BranchNo = lpUnPacker->GetInt("branch_no");
        //�ͻ����
        hs_strncpy(RspFuncClientLogin.ClientId, lpUnPacker->GetStr("client_id"), sizeof(RspFuncClientLogin.ClientId));
        //�ͻ�����
        hs_strncpy(RspFuncClientLogin.ClientName, lpUnPacker->GetStr("client_name"), sizeof(RspFuncClientLogin.ClientName));
        //��˾�ͻ�����
        RspFuncClientLogin.CorpClientGroup = lpUnPacker->GetInt("corp_client_group");
        //�ͻ����յȼ�
        RspFuncClientLogin.CorpRiskLevel = lpUnPacker->GetInt("corp_risk_level");
        //�ͻ����ղ�����
        RspFuncClientLogin.CorpBeginDate = lpUnPacker->GetInt("corp_begin_date");
        //�ͻ����յ�����
        RspFuncClientLogin.CorpEndDate = lpUnPacker->GetInt("corp_end_date");
        //��Ч��־
        RspFuncClientLogin.ValidFlag = lpUnPacker->GetChar("valid_flag");
        //�ʲ��˺Ÿ���
        RspFuncClientLogin.FundaccountCount = lpUnPacker->GetInt("fundaccount_count");
        //�ʲ��˻�
        hs_strncpy(RspFuncClientLogin.FundAccount, lpUnPacker->GetStr("fund_account"), sizeof(RspFuncClientLogin.FundAccount));
        //�ͻ�Ȩ��
        hs_strncpy(RspFuncClientLogin.ClientRights, lpUnPacker->GetStr("client_rights"), sizeof(RspFuncClientLogin.ClientRights));
        //�ϴε�½ί�з�ʽ
        RspFuncClientLogin.LastOpEntrustWay = lpUnPacker->GetChar("last_op_entrust_way");
        //�ϴε�½վ��/�绰
        hs_strncpy(RspFuncClientLogin.LastOpStation, lpUnPacker->GetStr("last_op_station"), sizeof(RspFuncClientLogin.LastOpStation));
        //ϵͳ�ڵ���
        RspFuncClientLogin.SysnodeId = lpUnPacker->GetInt("sysnode_id");
        //�û�����
        hs_strncpy(RspFuncClientLogin.UserToken, lpUnPacker->GetStr("user_token"), sizeof(RspFuncClientLogin.UserToken));
        //�������
        RspFuncClientLogin.ErrorNo = lpUnPacker->GetInt("error_no");
        //������ʾ
        hs_strncpy(RspFuncClientLogin.ErrorInfo, lpUnPacker->GetStr("error_info"), sizeof(RspFuncClientLogin.ErrorInfo));
        //�ʲ�����
        RspFuncClientLogin.AssetProp = lpUnPacker->GetChar("asset_prop");
        //��Ʒ��־
        RspFuncClientLogin.ProductFlag = lpUnPacker->GetChar("product_flag");
        //��Ϣ��־
        RspFuncClientLogin.MessageFlag = lpUnPacker->GetChar("message_flag");
        //ǿ���˵�ȷ��
        RspFuncClientLogin.TabconfirmFlag = lpUnPacker->GetChar("tabconfirm_flag");
        //�ϴ��������
        RspFuncClientLogin.LastDate = lpUnPacker->GetInt("last_date");
        //�Ự���
        RspFuncClientLogin.SessionNo = lpUnPacker->GetInt("session_no");

        m_userInfo.BranchNo = lpUnPacker->GetInt("branch_no"); //����Ӫҵ����
        hs_strncpy(m_userInfo.ClientId, lpUnPacker->GetStr("client_id"), sizeof(m_userInfo.ClientId)); //���ÿͻ����
        hs_strncpy(m_userInfo.FundAccount, lpUnPacker->GetStr("fund_account"), sizeof(m_userInfo.FundAccount)); //�����ʽ��˺�
        hs_strncpy(m_userInfo.UserToken, lpUnPacker->GetStr("user_token"), sizeof(m_userInfo.UserToken)); //�����û�����
        m_Logined = true; //�����Ƿ��¼

        //��������ά��
        if(!m_HeartKeepTrd.IsRunning())
            m_HeartKeepTrd.Start(HeartKeep, 0, this);

        //��־���
        if(m_SaveLog && lpMdApi_log != NULL)
            lpMdApi_log->AddLog(LOGINFO_Response, FuncClientLogin, 0, &RspFuncClientLogin, sizeof(CHundsunFutuRspFuncClientLoginField), &RspInfo, sizeof(CHundsunFutuRspInfoField));
        //�ص�
        lpMdSpi->OnRspFuncClientLogin(&RspFuncClientLogin, &RspInfo, nRequestID, true);
    }
}

//330851 �ڻ���Լ��֤���ѯ
void CHundsunFutuMdApi_impl::OnAsyResponse_330851(IF2UnPacker *lpUnPacker, int nRequestID)
{
    //�жϴ�����Ϣ
    CHundsunFutuRspInfoField RspInfo;
    //������Ϣ�ṹ��
    CHundsunFutuRspFuncFutuContractbailQryField RspFuncFutuContractbailQry;
    memset(&RspFuncFutuContractbailQry, 0, sizeof(CHundsunFutuRspFuncFutuContractbailQryField));

    //��ô�����Ϣ
    DealErrorInfo(lpUnPacker, &RspInfo);
    if( RspInfo.ErrorNo != 0 )
    {
        if(m_SaveLog && lpMdApi_log != NULL)
            lpMdApi_log->AddLog(LOGINFO_Response, FuncFutuContractbailQry, 0, &RspFuncFutuContractbailQry, sizeof(CHundsunFutuRspFuncFutuContractbailQryField), &RspInfo, sizeof(CHundsunFutuRspInfoField));
        lpMdSpi->OnRspFuncFutuContractbailQry(&RspFuncFutuContractbailQry, &RspInfo, nRequestID, true);
    }
    else
    {
        //����������
        CQueryInfo *qryinfo = GetCQueryInfo(nRequestID);
        //���������
        int ct = lpUnPacker->GetRowCount();
        if( ct == 0 )
        {
            lpMdSpi->OnRspFuncFutuContractbailQry(NULL, &RspInfo, nRequestID, true);
            if(qryinfo)
                DelQuery(qryinfo);
        }
        else
        {
            string pos = "";
            while (!lpUnPacker->IsEOF())
            {
                ct--;

                //�������
                hs_strncpy(RspFuncFutuContractbailQry.FutuExchType, lpUnPacker->GetStr("futu_exch_type"), sizeof(RspFuncFutuContractbailQry.FutuExchType));
                //��Լ����
                hs_strncpy(RspFuncFutuContractbailQry.FutuCode, lpUnPacker->GetStr("futu_code"), sizeof(RspFuncFutuContractbailQry.FutuCode));
                //Ʒ�ּ��
                hs_strncpy(RspFuncFutuContractbailQry.FutuName, lpUnPacker->GetStr("futu_name"), sizeof(RspFuncFutuContractbailQry.FutuName));
                //��Լ�������
                hs_strncpy(RspFuncFutuContractbailQry.FutucodeType, lpUnPacker->GetStr("futucode_type"), sizeof(RspFuncFutuContractbailQry.FutucodeType));
                //�������
                hs_strncpy(RspFuncFutuContractbailQry.MoneyType, lpUnPacker->GetStr("money_type"), sizeof(RspFuncFutuContractbailQry.MoneyType));
                //ֹͣ���ױ�־
                RspFuncFutuContractbailQry.StopFlag = lpUnPacker->GetChar("stop_flag");
                //��Լ����
                RspFuncFutuContractbailQry.AmountPerHand = lpUnPacker->GetInt("amount_per_hand");
                //�걨��λ
                RspFuncFutuContractbailQry.FutuReportUnit = lpUnPacker->GetChar("futu_report_unit");
                //�۸�λ
                RspFuncFutuContractbailQry.PriceUnit = lpUnPacker->GetChar("price_unit");
                //��С�۲�
                RspFuncFutuContractbailQry.FutuPriceStep = lpUnPacker->GetDouble("futu_price_step");
                //���ֵ�λ
                RspFuncFutuContractbailQry.OpenUnit = lpUnPacker->GetDouble("open_unit");
                //ƽ�ֵ�λ
                RspFuncFutuContractbailQry.DropUnit = lpUnPacker->GetDouble("drop_unit");
                //�м�ί��ÿ������µ�����
                RspFuncFutuContractbailQry.MaxMarketAmount = lpUnPacker->GetInt("max_market_amount");
                //�޼�ί��ÿ������µ�����
                RspFuncFutuContractbailQry.MaxLimitedAmount = lpUnPacker->GetInt("max_limited_amount");
                //���ֳɽ�������
                RspFuncFutuContractbailQry.OpenRatio = lpUnPacker->GetDouble("open_ratio");
                //���ֵ�λ������֤����
                RspFuncFutuContractbailQry.OpenMargin = lpUnPacker->GetDouble("open_margin");
                //��Ȩϵ��
                hs_strncpy(RspFuncFutuContractbailQry.FoptExpiry, lpUnPacker->GetStr("fopt_expiry"), sizeof(RspFuncFutuContractbailQry.FoptExpiry));
                //��Ȩִ�м۸�
                RspFuncFutuContractbailQry.FoptStrikePrice = lpUnPacker->GetDouble("fopt_strike_price");
                //����ʵ��
                RspFuncFutuContractbailQry.PreDelta = lpUnPacker->GetDouble("pre_delta");
                //��Ʒ���
                RspFuncFutuContractbailQry.FutuProductType = lpUnPacker->GetChar("futu_product_type");
                //��Ȩ��Լ����
                RspFuncFutuContractbailQry.FoptType = lpUnPacker->GetChar("fopt_type");
                //��ĺ�Լ����
                hs_strncpy(RspFuncFutuContractbailQry.UnderlyingCode, lpUnPacker->GetStr("underlying_code"), sizeof(RspFuncFutuContractbailQry.UnderlyingCode));
                //������
                RspFuncFutuContractbailQry.ExpireDate = lpUnPacker->GetInt("expire_date");
                //ִ������(�Ϻ���Ȩ���ð�Ľ��ִ�з�ʽ,�ɽ�����ָ��T��)
                RspFuncFutuContractbailQry.StrikeDate = lpUnPacker->GetInt("strike_date");
                //�Ƿ������Ż�
                RspFuncFutuContractbailQry.HighSidePreferFlag = lpUnPacker->GetChar("high_side_prefer_flag");
                //����ʵֵ
                RspFuncFutuContractbailQry.PreDeltaValue = lpUnPacker->GetDouble("pre_delta_value");
                //��λ��
                hs_strncpy(RspFuncFutuContractbailQry.PositionStr, lpUnPacker->GetStr("position_str"), sizeof(RspFuncFutuContractbailQry.PositionStr));

                //��ö�λ�ַ���
                const char *lpStrPos = lpUnPacker->GetStr("position_str");
                if( lpStrPos == 0)
                    pos = "";
                else
                    pos = lpStrPos;
                //��¼��־
                if(m_SaveLog && lpMdApi_log != NULL)
                    lpMdApi_log->AddLog(LOGINFO_Response, FuncFutuContractbailQry, 0, &RspFuncFutuContractbailQry, sizeof(CHundsunFutuRspFuncFutuContractbailQryField), &RspInfo, sizeof(CHundsunFutuRspInfoField));
                //�ص�
                if(qryinfo)
                    lpMdSpi->OnRspFuncFutuContractbailQry(&RspFuncFutuContractbailQry, &RspInfo, nRequestID, false);
                else
                    lpMdSpi->OnRspFuncFutuContractbailQry(&RspFuncFutuContractbailQry, &RspInfo, nRequestID, ct == 0 ? true : false);

                lpUnPacker->Next();
            }
            if(qryinfo)
            {
                if( pos.length() == 0)
                {
                    lpMdSpi->OnRspFuncFutuContractbailQry(NULL, &RspInfo, nRequestID, true);
                    DelQuery(qryinfo);
                }
                else
                {
                    //��ѯ��һҳ
                    CHundsunFutuReqFuncFutuContractbailQryField *pReqFuncFutuContractbailQry = (CHundsunFutuReqFuncFutuContractbailQryField *)qryinfo->GetReqData();
                    hs_strncpy(pReqFuncFutuContractbailQry->PositionStr, pos.c_str(), sizeof(pReqFuncFutuContractbailQry->PositionStr));
                    ReqFuncFutuContractbailQry(pReqFuncFutuContractbailQry, nRequestID);
                }
            }
        }
    }
}

//620001_33100 ����-�ڻ���������Ӧ��
void CHundsunFutuMdApi_impl::OnAsyRsp_620001_33100(IF2UnPacker *lpUnPacker, int nRequestID)
{
    //�жϴ�����Ϣ
    CHundsunFutuRspInfoField RspInfo;
    //��ô�����Ϣ
    DealErrorInfo(lpUnPacker, &RspInfo);
    //�ص�Ӧ��
    if(nRequestID != -33100)
        lpMdSpi->OnRspFuncQhSingleHqSubscribe(&RspInfo, nRequestID, true);
}

//620002_33100 ����ȡ��-�ڻ���������Ӧ��
void CHundsunFutuMdApi_impl::OnAsyRsp_620002_33100(IF2UnPacker *lpUnPacker, int nRequestID)
{
    //�жϴ�����Ϣ
    CHundsunFutuRspInfoField RspInfo;
    //��ô�����Ϣ
    DealErrorInfo(lpUnPacker, &RspInfo);
    //�ص�Ӧ��
    if(nRequestID != -33100)
        lpMdSpi->OnRspFuncQhSingleHqCancel(&RspInfo, nRequestID, true);
}

//620003_33100 ����-�ڻ���������
void CHundsunFutuMdApi_impl::OnAsyRtn_620003_33100(IF2UnPacker *lpUnPacker, int nRequestID)
{
    char buf[1024];
    hs_strncpy(buf, lpUnPacker->GetStr("QH"), strlen(lpUnPacker->GetStr("QH")));
    //������Ϣ�ṹ��
    CHundsunFutuRtnFuncQhSingleHqPushField RtnFuncQhSingleHqPush;
    memset(&RtnFuncQhSingleHqPush, 0, sizeof(CHundsunFutuRtnFuncQhSingleHqPushField));
    //����������Ϣ
    ParseFuncQhSingleHqPush(&RtnFuncQhSingleHqPush, buf);
    //��־���
    if(m_SaveLog && lpMdApi_log != NULL)
        lpMdApi_log->AddLog(LOGINFO_Return, 620003, 33100, &RtnFuncQhSingleHqPush, sizeof(CHundsunFutuRtnFuncQhSingleHqPushField));
    //�ص�����
    lpMdSpi->OnRtnFuncQhSingleHqPush(&RtnFuncQhSingleHqPush);
}

//620001_33102 ����-�ڻ��������Ӧ��
void CHundsunFutuMdApi_impl::OnAsyRsp_620001_33102(IF2UnPacker *lpUnPacker, int nRequestID)
{
    //�жϴ�����Ϣ
    CHundsunFutuRspInfoField RspInfo;
    //��ô�����Ϣ
    DealErrorInfo(lpUnPacker, &RspInfo);
    //�ص�Ӧ��
    if(nRequestID != -33102)
        lpMdSpi->OnRspFuncQhCombineSubscribe(&RspInfo, nRequestID, true);
}

//620002_33102 ����ȡ��-�ڻ��������Ӧ��
void CHundsunFutuMdApi_impl::OnAsyRsp_620002_33102(IF2UnPacker *lpUnPacker, int nRequestID)
{
    //�жϴ�����Ϣ
    CHundsunFutuRspInfoField RspInfo;
    //��ô�����Ϣ
    DealErrorInfo(lpUnPacker, &RspInfo);
    //�ص�Ӧ��
    if(nRequestID != -33102)
        lpMdSpi->OnRspFuncQhCombineCancel(&RspInfo, nRequestID, true);
}

//620003_33102 ����-�ڻ��������
void CHundsunFutuMdApi_impl::OnAsyRtn_620003_33102(IF2UnPacker *lpUnPacker, int nRequestID)
{
    char buf[1024];
    hs_strncpy(buf, lpUnPacker->GetStr("QH"), strlen(lpUnPacker->GetStr("QH")));
    //������Ϣ�ṹ��
    CHundsunFutuRtnFuncQhCombinePushField RtnFuncQhCombinePush;
    memset(&RtnFuncQhCombinePush, 0, sizeof(CHundsunFutuRtnFuncQhCombinePushField));
    //����������Ϣ
    ParseFuncQhCombinePush(&RtnFuncQhCombinePush, buf);
    //��־���
    if(m_SaveLog && lpMdApi_log != NULL)
        lpMdApi_log->AddLog(LOGINFO_Return, 620003, 33102, &RtnFuncQhCombinePush, sizeof(CHundsunFutuRtnFuncQhCombinePushField));
    //�ص�����
    lpMdSpi->OnRtnFuncQhCombinePush(&RtnFuncQhCombinePush);
}

//331100 �ͻ���¼
void CHundsunFutuMdApi_impl::OnSynResponse_331100(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //��ô�����Ϣ
    DealErrorInfo(lpUnPacker, pRspInfo);
    if( pRspInfo->ErrorNo != 0 )
        return;

    *pnItems = lpUnPacker->GetRowCount();
    if(*pnItems > 0)
    {
        *ppRetData = new CHundsunFutuRspFuncClientLoginField[*pnItems];
        CHundsunFutuRspFuncClientLoginField* ppRspFuncClientLogin = (CHundsunFutuRspFuncClientLoginField*) (*ppRetData);

        int ct = 0;
        while (!lpUnPacker->IsEOF())
        {
            //��������
            ppRspFuncClientLogin[ct].InitDate = lpUnPacker->GetInt("init_date");
            //ϵͳ״̬
            ppRspFuncClientLogin[ct].SysStatus = lpUnPacker->GetChar("sys_status");
            //��˾����
            hs_strncpy(ppRspFuncClientLogin[ct].CompanyName, lpUnPacker->GetStr("company_name"), sizeof(ppRspFuncClientLogin[ct].CompanyName));
            //���кš��г����
            hs_strncpy(ppRspFuncClientLogin[ct].ContentType, lpUnPacker->GetStr("content_type"), sizeof(ppRspFuncClientLogin[ct].ContentType));
            //��������
            hs_strncpy(ppRspFuncClientLogin[ct].AccountContent, lpUnPacker->GetStr("account_content"), sizeof(ppRspFuncClientLogin[ct].AccountContent));
            //��֧����
            ppRspFuncClientLogin[ct].BranchNo = lpUnPacker->GetInt("branch_no");
            //�ͻ����
            hs_strncpy(ppRspFuncClientLogin[ct].ClientId, lpUnPacker->GetStr("client_id"), sizeof(ppRspFuncClientLogin[ct].ClientId));
            //�ͻ�����
            hs_strncpy(ppRspFuncClientLogin[ct].ClientName, lpUnPacker->GetStr("client_name"), sizeof(ppRspFuncClientLogin[ct].ClientName));
            //��˾�ͻ�����
            ppRspFuncClientLogin[ct].CorpClientGroup = lpUnPacker->GetInt("corp_client_group");
            //�ͻ����յȼ�
            ppRspFuncClientLogin[ct].CorpRiskLevel = lpUnPacker->GetInt("corp_risk_level");
            //�ͻ����ղ�����
            ppRspFuncClientLogin[ct].CorpBeginDate = lpUnPacker->GetInt("corp_begin_date");
            //�ͻ����յ�����
            ppRspFuncClientLogin[ct].CorpEndDate = lpUnPacker->GetInt("corp_end_date");
            //��Ч��־
            ppRspFuncClientLogin[ct].ValidFlag = lpUnPacker->GetChar("valid_flag");
            //�ʲ��˺Ÿ���
            ppRspFuncClientLogin[ct].FundaccountCount = lpUnPacker->GetInt("fundaccount_count");
            //�ʲ��˻�
            hs_strncpy(ppRspFuncClientLogin[ct].FundAccount, lpUnPacker->GetStr("fund_account"), sizeof(ppRspFuncClientLogin[ct].FundAccount));
            //�ͻ�Ȩ��
            hs_strncpy(ppRspFuncClientLogin[ct].ClientRights, lpUnPacker->GetStr("client_rights"), sizeof(ppRspFuncClientLogin[ct].ClientRights));
            //�ϴε�½ί�з�ʽ
            ppRspFuncClientLogin[ct].LastOpEntrustWay = lpUnPacker->GetChar("last_op_entrust_way");
            //�ϴε�½վ��/�绰
            hs_strncpy(ppRspFuncClientLogin[ct].LastOpStation, lpUnPacker->GetStr("last_op_station"), sizeof(ppRspFuncClientLogin[ct].LastOpStation));
            //ϵͳ�ڵ���
            ppRspFuncClientLogin[ct].SysnodeId = lpUnPacker->GetInt("sysnode_id");
            //�û�����
            hs_strncpy(ppRspFuncClientLogin[ct].UserToken, lpUnPacker->GetStr("user_token"), sizeof(ppRspFuncClientLogin[ct].UserToken));
            //�������
            ppRspFuncClientLogin[ct].ErrorNo = lpUnPacker->GetInt("error_no");
            //������ʾ
            hs_strncpy(ppRspFuncClientLogin[ct].ErrorInfo, lpUnPacker->GetStr("error_info"), sizeof(ppRspFuncClientLogin[ct].ErrorInfo));
            //�ʲ�����
            ppRspFuncClientLogin[ct].AssetProp = lpUnPacker->GetChar("asset_prop");
            //��Ʒ��־
            ppRspFuncClientLogin[ct].ProductFlag = lpUnPacker->GetChar("product_flag");
            //��Ϣ��־
            ppRspFuncClientLogin[ct].MessageFlag = lpUnPacker->GetChar("message_flag");
            //ǿ���˵�ȷ��
            ppRspFuncClientLogin[ct].TabconfirmFlag = lpUnPacker->GetChar("tabconfirm_flag");
            //�ϴ��������
            ppRspFuncClientLogin[ct].LastDate = lpUnPacker->GetInt("last_date");
            //�Ự���
            ppRspFuncClientLogin[ct].SessionNo = lpUnPacker->GetInt("session_no");

            m_userInfo.BranchNo = lpUnPacker->GetInt("branch_no"); //����Ӫҵ����
            hs_strncpy(m_userInfo.ClientId, lpUnPacker->GetStr("client_id"), sizeof(m_userInfo.ClientId)); //���ÿͻ����
            hs_strncpy(m_userInfo.FundAccount, lpUnPacker->GetStr("fund_account"), sizeof(m_userInfo.FundAccount)); //�����ʽ��˺�
            hs_strncpy(m_userInfo.UserToken, lpUnPacker->GetStr("user_token"), sizeof(m_userInfo.UserToken)); //�����û�����
            m_Logined = true; //�����Ƿ��¼

            //��������ά��
            if(!m_HeartKeepTrd.IsRunning())
                m_HeartKeepTrd.Start(HeartKeep, 0, this);

            ct++;
            lpUnPacker->Next();
        }
    }
    return;
}

//330851 �ڻ���Լ��֤���ѯ
void CHundsunFutuMdApi_impl::OnSynResponse_330851(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //��ô�����Ϣ
    DealErrorInfo(lpUnPacker, pRspInfo);
    if( pRspInfo->ErrorNo != 0 )
        return;

    *pnItems = lpUnPacker->GetRowCount();
    if(*pnItems > 0)
    {
        *ppRetData = new CHundsunFutuRspFuncFutuContractbailQryField[*pnItems];
        CHundsunFutuRspFuncFutuContractbailQryField* ppRspFuncFutuContractbailQry = (CHundsunFutuRspFuncFutuContractbailQryField*) (*ppRetData);

        int ct = 0;
        while (!lpUnPacker->IsEOF())
        {
            //�������
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].FutuExchType, lpUnPacker->GetStr("futu_exch_type"), sizeof(ppRspFuncFutuContractbailQry[ct].FutuExchType));
            //��Լ����
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].FutuCode, lpUnPacker->GetStr("futu_code"), sizeof(ppRspFuncFutuContractbailQry[ct].FutuCode));
            //Ʒ�ּ��
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].FutuName, lpUnPacker->GetStr("futu_name"), sizeof(ppRspFuncFutuContractbailQry[ct].FutuName));
            //��Լ�������
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].FutucodeType, lpUnPacker->GetStr("futucode_type"), sizeof(ppRspFuncFutuContractbailQry[ct].FutucodeType));
            //�������
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].MoneyType, lpUnPacker->GetStr("money_type"), sizeof(ppRspFuncFutuContractbailQry[ct].MoneyType));
            //ֹͣ���ױ�־
            ppRspFuncFutuContractbailQry[ct].StopFlag = lpUnPacker->GetChar("stop_flag");
            //��Լ����
            ppRspFuncFutuContractbailQry[ct].AmountPerHand = lpUnPacker->GetInt("amount_per_hand");
            //�걨��λ
            ppRspFuncFutuContractbailQry[ct].FutuReportUnit = lpUnPacker->GetChar("futu_report_unit");
            //�۸�λ
            ppRspFuncFutuContractbailQry[ct].PriceUnit = lpUnPacker->GetChar("price_unit");
            //��С�۲�
            ppRspFuncFutuContractbailQry[ct].FutuPriceStep = lpUnPacker->GetDouble("futu_price_step");
            //���ֵ�λ
            ppRspFuncFutuContractbailQry[ct].OpenUnit = lpUnPacker->GetDouble("open_unit");
            //ƽ�ֵ�λ
            ppRspFuncFutuContractbailQry[ct].DropUnit = lpUnPacker->GetDouble("drop_unit");
            //�м�ί��ÿ������µ�����
            ppRspFuncFutuContractbailQry[ct].MaxMarketAmount = lpUnPacker->GetInt("max_market_amount");
            //�޼�ί��ÿ������µ�����
            ppRspFuncFutuContractbailQry[ct].MaxLimitedAmount = lpUnPacker->GetInt("max_limited_amount");
            //���ֳɽ�������
            ppRspFuncFutuContractbailQry[ct].OpenRatio = lpUnPacker->GetDouble("open_ratio");
            //���ֵ�λ������֤����
            ppRspFuncFutuContractbailQry[ct].OpenMargin = lpUnPacker->GetDouble("open_margin");
            //��Ȩϵ��
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].FoptExpiry, lpUnPacker->GetStr("fopt_expiry"), sizeof(ppRspFuncFutuContractbailQry[ct].FoptExpiry));
            //��Ȩִ�м۸�
            ppRspFuncFutuContractbailQry[ct].FoptStrikePrice = lpUnPacker->GetDouble("fopt_strike_price");
            //����ʵ��
            ppRspFuncFutuContractbailQry[ct].PreDelta = lpUnPacker->GetDouble("pre_delta");
            //��Ʒ���
            ppRspFuncFutuContractbailQry[ct].FutuProductType = lpUnPacker->GetChar("futu_product_type");
            //��Ȩ��Լ����
            ppRspFuncFutuContractbailQry[ct].FoptType = lpUnPacker->GetChar("fopt_type");
            //��ĺ�Լ����
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].UnderlyingCode, lpUnPacker->GetStr("underlying_code"), sizeof(ppRspFuncFutuContractbailQry[ct].UnderlyingCode));
            //������
            ppRspFuncFutuContractbailQry[ct].ExpireDate = lpUnPacker->GetInt("expire_date");
            //ִ������(�Ϻ���Ȩ���ð�Ľ��ִ�з�ʽ,�ɽ�����ָ��T��)
            ppRspFuncFutuContractbailQry[ct].StrikeDate = lpUnPacker->GetInt("strike_date");
            //�Ƿ������Ż�
            ppRspFuncFutuContractbailQry[ct].HighSidePreferFlag = lpUnPacker->GetChar("high_side_prefer_flag");
            //����ʵֵ
            ppRspFuncFutuContractbailQry[ct].PreDeltaValue = lpUnPacker->GetDouble("pre_delta_value");
            //��λ��
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].PositionStr, lpUnPacker->GetStr("position_str"), sizeof(ppRspFuncFutuContractbailQry[ct].PositionStr));

            ct++;
            lpUnPacker->Next();
        }
    }
    return;
}

//620001_33100 ����-�ڻ���������Ӧ��
void CHundsunFutuMdApi_impl::OnSynRsp_620001_33100(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //��ô�����Ϣ
    DealErrorInfo(lpUnPacker, pRspInfo);
}

//620002_33100 ����ȡ��-�ڻ���������Ӧ��
void CHundsunFutuMdApi_impl::OnSynRsp_620002_33100(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //��ô�����Ϣ
    DealErrorInfo(lpUnPacker, pRspInfo);
}

//620001_33102 ����-�ڻ��������Ӧ��
void CHundsunFutuMdApi_impl::OnSynRsp_620001_33102(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //��ô�����Ϣ
    DealErrorInfo(lpUnPacker, pRspInfo);
}

//620002_33102 ����ȡ��-�ڻ��������Ӧ��
void CHundsunFutuMdApi_impl::OnSynRsp_620002_33102(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //��ô�����Ϣ
    DealErrorInfo(lpUnPacker, pRspInfo);
}

void CHundsunFutuMdApi_impl::HeartKeep(void* lpvoid)
{
    CHundsunFutuMdApi_impl *helper = (CHundsunFutuMdApi_impl*)lpvoid;
    while( helper->m_bHeartKeepStop == false )
    {
        helper->m_HeartKeepEvent.Wait(PANICHEARTBEATTIME);
        helper->m_iHeartKeep++;
        if (helper->m_iHeartKeep >= 3)
        {
            if(helper->m_Logined)
            {
                //Ĭ�϶���������Ϣ
                helper->m_SubSetCs.Lock();
                if(helper->m_SubFutuCodeSet.size() > 0)
                {
                    char** pptr = new char*[helper->m_SubFutuCodeSet.size() + 1]; 
                    int    i = 0;
                    for(set<string>::iterator it = helper->m_SubFutuCodeSet.begin(); it != helper->m_SubFutuCodeSet.end(); it++, i++)
                    {
                        pptr[i] = (char*)(*it).c_str(); 
                    }
                    pptr[i] = 0;
                    helper->ReqFuncQhSingleHqSubscribe(pptr, helper->m_SubFutuCodeSet.size(), -33100);
                }
                if(helper->m_SubArbitCodeSet.size() > 0)
                {
                    char** pptr = new char*[helper->m_SubArbitCodeSet.size() + 1]; 
                    int    i = 0;
                    for(set<string>::iterator it = helper->m_SubArbitCodeSet.begin(); it != helper->m_SubArbitCodeSet.end(); it++, i++)
                    {
                        pptr[i] = (char*)(*it).c_str(); 
                    }
                    pptr[i] = 0;
                    helper->ReqFuncQhCombineSubscribe(pptr, helper->m_SubArbitCodeSet.size(), -33102);
                }
                helper->m_SubSetCs.UnLock();
            }
        }
    }
}
