#if !defined(HUNDSUNH5SDKUSERAPISTRUCT_H)
#define HUNDSUNH5SDKUSERAPISTRUCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <queue>
using namespace std;

//������Ϣ
struct CHundsunH5sdkRspInfoField
{
    int       ErrorNo;                  //�������
    char      ErrorInfo[255];           //������ʾ
};

//1001 ������������
struct CHundsunH5sdkReqLoginField
{
    char      UserName[255];            //��¼����
    char      Password[255];            //ö�١���¼��������
    char      HqToken[255];             //��������
    char      DynPassword[255];         //��¼����
    char      AppName[255];             //Ӧ�����ƣ������汾��Ϣ
    char      PhoneNumber[255];         //�ֻ���
    int       SdkVersion;               //sdk�汾�ţ�sdk�Զ����룩
    char      OsVersion[255];           //����ϵͳ�汾��sdk�Զ����룩
    char      MacAddress[255];          //�����ַ��sdk�Զ����룩

    CHundsunH5sdkReqLoginField()
    {
        memset(UserName, 0, sizeof(UserName));
        memset(Password, 0, sizeof(Password));
        memset(HqToken, 0, sizeof(HqToken));
        memset(DynPassword, 0, sizeof(DynPassword));
        memset(AppName, 0, sizeof(AppName));
        memset(PhoneNumber, 0, sizeof(PhoneNumber));
        SdkVersion = 0;
        memset(OsVersion, 0, sizeof(OsVersion));
        memset(MacAddress, 0, sizeof(MacAddress));
    };
};

//1001 ��������Ӧ��
struct CHundsunH5sdkRspLoginField
{
    int       ErrorNo;                  //����š�ʹ��H5PROTO_ENUM_EN_CUSTOM��ʹ��CustomErrorNo��LoginData������ʹ��ErrorInfo�ֶ�
    char      ErrorInfo[255];           //������Ϣ
    char      OriginalData[255];        //��¼�������ݡ�����Level2У���̨���ص�����
    int       HeartbeatInterval;        //�������

    CHundsunH5sdkRspLoginField()
    {
        ErrorNo = 0;
        memset(ErrorInfo, 0, sizeof(ErrorInfo));
        memset(OriginalData, 0, sizeof(OriginalData));
        HeartbeatInterval = 0;
    };
};

//1002 �ǳ���Ϣ����
struct CHundsunH5sdkReqLogoutField
{
    char      QuitReason[255];          //������Ϣ

    CHundsunH5sdkReqLogoutField()
    {
        memset(QuitReason, 0, sizeof(QuitReason));
    };
};

//1002 �ǳ���ϢӦ��
struct CHundsunH5sdkRspLogoutField
{

    CHundsunH5sdkRspLogoutField()
    {
    };
};

//1003 ��������
struct CHundsunH5sdkReqHeartbeatField
{

    CHundsunH5sdkReqHeartbeatField()
    {
    };
};

//1003 ����Ӧ��
struct CHundsunH5sdkRspHeartbeatField
{

    CHundsunH5sdkRspHeartbeatField()
    {
    };
};

//1004 �����������Ϣ����
struct CHundsunH5sdkReqServerInfoField
{

    CHundsunH5sdkReqServerInfoField()
    {
    };
};

//1004 �����������ϢӦ�� - ������֧�ֵĽ�����
struct CHundsunH5sdkRspServerInfoFinanceGrpField
{
    char      FinanceMic[255];          //�г�����
    char      FinanceName[255];         //�г���������
    int       InitDate;                 //����������
    int       BusinessTime;             //������ʱ��

    CHundsunH5sdkRspServerInfoFinanceGrpField()
    {
        memset(FinanceMic, 0, sizeof(FinanceMic));
        memset(FinanceName, 0, sizeof(FinanceName));
        InitDate = 0;
        BusinessTime = 0;
    };
};

//1004 �����������ϢӦ��
struct CHundsunH5sdkRspServerInfoField
{
    char      SrvName[255];             //����������
    long      SrvTime;                  //��ǰ������ʱ��
    int       CurrOnlineCount;          //��ǰ���ߣ��ݲ�ʹ�ã�
    int       MaxOnlineCount;           //����������ߣ��ݲ�ʹ�ã�
    vector<CHundsunH5sdkRspServerInfoFinanceGrpField> FinanceGrp; //������֧�ֵĽ�����

    CHundsunH5sdkRspServerInfoField()
    {
        memset(SrvName, 0, sizeof(SrvName));
        SrvTime = 0;
        CurrOnlineCount = 0;
        MaxOnlineCount = 0;
    };
};

//1006 ���������� - ���ڴ����������������ظ���
struct CHundsunH5sdkReqBatchBatchGrpField
{
    char      SubPacket[2000];          //������ı��Ķ�����

    CHundsunH5sdkReqBatchBatchGrpField()
    {
        memset(SubPacket, 0, sizeof(SubPacket));
    };
};

//1006 ����������
struct CHundsunH5sdkReqBatchField
{
    vector<CHundsunH5sdkReqBatchBatchGrpField> BatchGrp; //���ڴ����������������ظ���

    CHundsunH5sdkReqBatchField()
    {
    };
};

//1006 ������Ӧ�� - ���ڴ����������Ӧ����ظ���
struct CHundsunH5sdkRspBatchBatchGrpField
{
    char      SubPacket[2000];          //��Ӧ��ı��Ķ�����

    CHundsunH5sdkRspBatchBatchGrpField()
    {
        memset(SubPacket, 0, sizeof(SubPacket));
    };
};

//1006 ������Ӧ��
struct CHundsunH5sdkRspBatchField
{
    vector<CHundsunH5sdkRspBatchBatchGrpField> BatchGrp; //���ڴ����������Ӧ����ظ���

    CHundsunH5sdkRspBatchField()
    {
    };
};

//2001 ����̬�ļ�����
struct CHundsunH5sdkReqFileField
{
    int       HqFileType;               //�ļ�����
    char      HqFileName[255];          //�ļ��������
    int       Crc;                      //�ļ�CRC
    long      FileOffset;               //�ļ�ƫ��
    int       FileLength;               //����ĳ��ȣ�����һ����ֵ�󣬸���Ӧ���´�����

    CHundsunH5sdkReqFileField()
    {
        HqFileType = 0;
        memset(HqFileName, 0, sizeof(HqFileName));
        Crc = 0;
        FileOffset = 0;
        FileLength = 0;
    };
};

//2001 ����̬�ļ�Ӧ��
struct CHundsunH5sdkRspFileField
{
    int       ErrorNo;                  //�����
    int       HqFileType;               //�ļ�����
    char      HqFileName[255];          //�ļ�����
    int       FileLength;               //ʣ�೤��
    long      FileOffset;               //�´������offset
    int       Crc;                      //�ļ�CRC
    char      OriginalData[255];        //�ļ�����

    CHundsunH5sdkRspFileField()
    {
        ErrorNo = 0;
        HqFileType = 0;
        memset(HqFileName, 0, sizeof(HqFileName));
        FileLength = 0;
        FileOffset = 0;
        Crc = 0;
        memset(OriginalData, 0, sizeof(OriginalData));
    };
};

//2002 �г�������Ϣ���� - ����������
struct CHundsunH5sdkReqMarketTypesFinanceMicGrpField
{
    char      FinanceMic[255];          //����������

    CHundsunH5sdkReqMarketTypesFinanceMicGrpField()
    {
        memset(FinanceMic, 0, sizeof(FinanceMic));
    };
};

//2002 �г�������Ϣ����
struct CHundsunH5sdkReqMarketTypesField
{
    vector<CHundsunH5sdkReqMarketTypesFinanceMicGrpField> FinanceMicGrp; //����������

    CHundsunH5sdkReqMarketTypesField()
    {
    };
};

//2002 �г�������ϢӦ�� - ����ʱ���
struct CHundsunH5sdkRspMarketTypesTradeSectionGrpField
{
    int       Opentime;                 //����ʱ��
    int       Closetime;                //����ʱ��

    CHundsunH5sdkRspMarketTypesTradeSectionGrpField()
    {
        Opentime = 0;
        Closetime = 0;
    };
};

//2002 �г�������ϢӦ�� - ֤ȯ����
struct CHundsunH5sdkRspMarketTypesTypeGrpField
{
    char      HqTypeCode[255];          //�������
    char      HqTypeName[255];          //��������
    int       PxScale;                  //����Ŵ���
    int       PxPrecision;              //�۸񾫶�
    int       InitDate;                 //��������
    vector<CHundsunH5sdkRspMarketTypesTradeSectionGrpField> TradeSectionGrp; //����ʱ���

    CHundsunH5sdkRspMarketTypesTypeGrpField()
    {
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        memset(HqTypeName, 0, sizeof(HqTypeName));
        PxScale = 0;
        PxPrecision = 0;
        InitDate = 0;
    };
};

//2002 �г�������ϢӦ�� - ����������
struct CHundsunH5sdkRspMarketTypesFinanceMicGrpField
{
    char      FinanceMic[255];          //����������
    char      FinanceName[255];         //����������
    int       MarketDate;               //�г�����
    int       InitDate;                 //��������
    int       Timezone;                 //�г�����ʱ��
    char      TimezoneCode[255];        //ʱ����
    int       Dst;                      //����ʱ��־
    vector<CHundsunH5sdkRspMarketTypesTypeGrpField> TypeGrp; //֤ȯ����

    CHundsunH5sdkRspMarketTypesFinanceMicGrpField()
    {
        memset(FinanceMic, 0, sizeof(FinanceMic));
        memset(FinanceName, 0, sizeof(FinanceName));
        MarketDate = 0;
        InitDate = 0;
        Timezone = 0;
        memset(TimezoneCode, 0, sizeof(TimezoneCode));
        Dst = 0;
    };
};

//2002 �г�������ϢӦ��
struct CHundsunH5sdkRspMarketTypesField
{
    vector<CHundsunH5sdkRspMarketTypesFinanceMicGrpField> FinanceMicGrp; //����������

    CHundsunH5sdkRspMarketTypesField()
    {
    };
};

//2003 �г����������
struct CHundsunH5sdkReqMarketReferenceField
{
    char      HqTypeCode[255];          //�г����

    CHundsunH5sdkReqMarketReferenceField()
    {
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//2003 �г������Ӧ�� - ����
struct CHundsunH5sdkRspMarketReferenceProdGrpField
{
    char      ProdCode[255];            //֤ȯ����
    char      HqTypeCode[255];          //���ʹ���
    char      ProdName[255];            //֤ȯ����
    int       PreclosePx;               //ǰ���̼�
    int       UpPx;                     //��ͣ�۸�
    int       DownPx;                   //��ͣ�۸�

    CHundsunH5sdkRspMarketReferenceProdGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        memset(ProdName, 0, sizeof(ProdName));
        PreclosePx = 0;
        UpPx = 0;
        DownPx = 0;
    };
};

//2003 �г������Ӧ��
struct CHundsunH5sdkRspMarketReferenceField
{
    char      FinanceMic[255];          //����������
    char      MicAbbr[255];             //������MIC��д
    char      FinanceName[255];         //����������
    int       MarketDate;               //�г�����
    int       InitDate;                 //��������
    int       Timezone;                 //�г�����ʱ��
    char      TimezoneCode[255];        //ʱ����
    int       Dst;                      //����ʱ��־
    vector<CHundsunH5sdkRspMarketReferenceProdGrpField> ProdGrp; //����

    CHundsunH5sdkRspMarketReferenceField()
    {
        memset(FinanceMic, 0, sizeof(FinanceMic));
        memset(MicAbbr, 0, sizeof(MicAbbr));
        memset(FinanceName, 0, sizeof(FinanceName));
        MarketDate = 0;
        InitDate = 0;
        Timezone = 0;
        memset(TimezoneCode, 0, sizeof(TimezoneCode));
        Dst = 0;
    };
};

//3001 ����������� - �����б�
struct CHundsunH5sdkReqSnapshotProdGrpField
{
    char      ProdCode[255];            //��Ʊ����
    char      HqTypeCode[255];          //���ʹ���,[subtype][.cficode].exchage

    CHundsunH5sdkReqSnapshotProdGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//3001 ����������� - ��Ҫ���ֶ��б�
struct CHundsunH5sdkReqSnapshotFieldGrpField
{
    int       FieldId;                  //�Դ�IDΪ׼����idΪ��ʱ��ȡfilename
    char      FieldName[255];           //�ֶ�ID

    CHundsunH5sdkReqSnapshotFieldGrpField()
    {
        FieldId = 0;
        memset(FieldName, 0, sizeof(FieldName));
    };
};

//3001 �����������
struct CHundsunH5sdkReqSnapshotField
{
    int       HqLevel;                  //���鼶��: level1, evel2(��������level1,��ʾ�ӳ�,level2��ʾʵʱ)
    vector<CHundsunH5sdkReqSnapshotProdGrpField> ProdGrp; //�����б�
    vector<CHundsunH5sdkReqSnapshotFieldGrpField> FieldGrp; //��Ҫ���ֶ��б�

    CHundsunH5sdkReqSnapshotField()
    {
        HqLevel = 0;
    };
};

//3001 �������Ӧ�� - ί�ж����б�
struct CHundsunH5sdkRspSnapshotEntrustGrpField
{
    int       EntrustAmount;            //ί����

    CHundsunH5sdkRspSnapshotEntrustGrpField()
    {
        EntrustAmount = 0;
    };
};

//3001 �������Ӧ�� - ί��λ
struct CHundsunH5sdkRspSnapshotBidGrpField
{
    int       EntrustPx;                //ί�м۸�
    long      TotalEntrustAmount;       //ί����
    int       EntrustCount;             //ί����
    vector<CHundsunH5sdkRspSnapshotEntrustGrpField> EntrustGrp; //ί�ж����б�

    CHundsunH5sdkRspSnapshotBidGrpField()
    {
        EntrustPx = 0;
        TotalEntrustAmount = 0;
        EntrustCount = 0;
    };
};

//3001 �������Ӧ�� - ί����λ
struct CHundsunH5sdkRspSnapshotOfferGrpField
{
    int       EntrustPx;                //ί�м۸�
    long      TotalEntrustAmount;       //ί����
    int       EntrustCount;             //ί����
    vector<CHundsunH5sdkRspSnapshotEntrustGrpField> EntrustGrp; //ί�ж����б�

    CHundsunH5sdkRspSnapshotOfferGrpField()
    {
        EntrustPx = 0;
        TotalEntrustAmount = 0;
        EntrustCount = 0;
    };
};

//3001 �������Ӧ�� - ���ǹ�Ʊ
struct CHundsunH5sdkRspSnapshotRiseFirstGrpField
{
    char      ProdCode[255];            //֤ȯ����
    char      ProdName[255];            //֤ȯ����
    char      HqTypeCode[255];          //֤ȯ����
    int       LastPx;                   //���¼�
    int       PxChangeRate;             //�ǵ���

    CHundsunH5sdkRspSnapshotRiseFirstGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(ProdName, 0, sizeof(ProdName));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        LastPx = 0;
        PxChangeRate = 0;
    };
};

//3001 �������Ӧ�� - ���
struct CHundsunH5sdkRspSnapshotFallFirstGrpField
{
    char      ProdCode[255];            //֤ȯ����
    char      ProdName[255];            //֤ȯ����
    char      HqTypeCode[255];          //֤ȯ����
    int       LastPx;                   //���¼�
    int       PxChangeRate;             //�ǵ���

    CHundsunH5sdkRspSnapshotFallFirstGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(ProdName, 0, sizeof(ProdName));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        LastPx = 0;
        PxChangeRate = 0;
    };
};

//3001 �������Ӧ�� - �����б�
struct CHundsunH5sdkRspSnapshotProdGrpField
{
    char      ProdCode[255];            //֤ȯ����
    char      ProdName[255];            //֤ȯ����
    char      HqTypeCode[255];          //֤ȯ����
    char      IndustryCode[255];        //������루δ���壩
    char      MoneyType[255];           //���Ҵ��루δ���壩
    int       DataTimestamp;            //ʱ���
    int       TradeMins;                //���׷�����
    char      TradeStatus[255];         //����״̬
    int       PreclosePx;               //���ռ�
    int       OpenPx;                   //����
    int       LastPx;                   //���³ɽ���
    int       HighPx;                   //��߼�
    int       LowPx;                    //��ͼ�
    int       ClosePx;                  //��������
    int       AvgPx;                    //����
    int       WavgPx;                   //��Ȩƽ����
    int       BusinessCount;            //�ɽ�����
    long      BusinessAmount;           //�ܳɽ���
    long      BusinessBalance;          //�ܳɽ���
    int       UpPx;                     //��ͣ�۸�
    int       DownPx;                   //��ͣ�۸�
    long      CurrentAmount;            //����ɽ���(����)
    long      BusinessAmountIn;         //���̳ɽ���
    long      BusinessAmountOut;        //���̳ɽ���
    long      TotBuyAmount;             //��ί������δ���壩
    long      TotSellAmount;            //��ί������δ���壩
    int       WavgBidPx;                //��Ȩƽ��ί��۸�δ���壩
    int       WavgOfferPx;              //��Ȩƽ��ί���۸�δ���壩
    vector<CHundsunH5sdkRspSnapshotBidGrpField> BidGrp; //ί��λ
    vector<CHundsunH5sdkRspSnapshotOfferGrpField> OfferGrp; //ί����λ
    int       EntrustRate;              //ί��
    long      EntrustDiff;              //ί��
    int       W52LowPx;                 //52����ͼۣ�δ���壩
    int       W52HighPx;                //52����߼ۣ�δ���壩
    int       PxChange;                 //�ǵ���
    int       Amplitude;                //���
    int       PxChangeRate;             //�ǵ���
    int       PopcPx;                   //��ǰ�̺��
    int       TradeSection;             //��ǰ���׽׶�:0:����(û���ӳ�����) 1:��ǰ 2:���� 3:�̺� 4:ʵʱ�̺�(û���ӳ�����)
    long      Svol;                     //��ǰ���׽ڳɽ�����δ���壩
    long      CirculationAmount;        //��ͨ�ɱ�
    long      TotalShares;              //�ܹɱ�
    long      MarketValue;              //��ֵ
    long      CirculationValue;         //��ͨ��ֵ
    int       VolRatio;                 //����
    int       TurnoverRatio;            //������
    long      Amount;                   //�ֲ���
    long      PrevAmount;               //��ֲ�
    long      AmountDelta;              //������
    int       PrevSettlement;           //�����
    int       Settlement;               //�����
    int       Iopv;                     //iopv
    int       DebtFundValue;            //��ծ����ֵ
    int       Eps;                      //ÿ������
    int       Bps;                      //ÿ�ɾ��ʲ�
    int       DynPbRate;                //�о���
    int       PeRate;                   //��ӯ�ʣ���̬��
    int       FinQuarter;               //���񼾶�
    int       FinEndDate;               //�����������
    int       SharesPerHand;            //ÿ�ֹ���
    int       RiseCount;                //���Ǽ���
    int       FallCount;                //�µ�����
    int       MemberCount;              //��Ա����
    vector<CHundsunH5sdkRspSnapshotRiseFirstGrpField> RiseFirstGrp; //���ǹ�Ʊ
    vector<CHundsunH5sdkRspSnapshotFallFirstGrpField> FallFirstGrp; //���

    CHundsunH5sdkRspSnapshotProdGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(ProdName, 0, sizeof(ProdName));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        memset(IndustryCode, 0, sizeof(IndustryCode));
        memset(MoneyType, 0, sizeof(MoneyType));
        DataTimestamp = 0;
        TradeMins = 0;
        memset(TradeStatus, 0, sizeof(TradeStatus));
        PreclosePx = 0;
        OpenPx = 0;
        LastPx = 0;
        HighPx = 0;
        LowPx = 0;
        ClosePx = 0;
        AvgPx = 0;
        WavgPx = 0;
        BusinessCount = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
        UpPx = 0;
        DownPx = 0;
        CurrentAmount = 0;
        BusinessAmountIn = 0;
        BusinessAmountOut = 0;
        TotBuyAmount = 0;
        TotSellAmount = 0;
        WavgBidPx = 0;
        WavgOfferPx = 0;
        EntrustRate = 0;
        EntrustDiff = 0;
        W52LowPx = 0;
        W52HighPx = 0;
        PxChange = 0;
        Amplitude = 0;
        PxChangeRate = 0;
        PopcPx = 0;
        TradeSection = 0;
        Svol = 0;
        CirculationAmount = 0;
        TotalShares = 0;
        MarketValue = 0;
        CirculationValue = 0;
        VolRatio = 0;
        TurnoverRatio = 0;
        Amount = 0;
        PrevAmount = 0;
        AmountDelta = 0;
        PrevSettlement = 0;
        Settlement = 0;
        Iopv = 0;
        DebtFundValue = 0;
        Eps = 0;
        Bps = 0;
        DynPbRate = 0;
        PeRate = 0;
        FinQuarter = 0;
        FinEndDate = 0;
        SharesPerHand = 0;
        RiseCount = 0;
        FallCount = 0;
        MemberCount = 0;
    };
};

//3001 �������Ӧ��
struct CHundsunH5sdkRspSnapshotField
{
    int       HqLevel;                  //���鼶��: level1, evel2(��������level1,��ʾ�ӳ�,level2��ʾʵʱ)
    vector<CHundsunH5sdkRspSnapshotProdGrpField> ProdGrp; //�����б�

    CHundsunH5sdkRspSnapshotField()
    {
        HqLevel = 0;
    };
};

//3002 ���̾�����Ϣ���� - ��Ҫɸѡ��֤ȯ����
struct CHundsunH5sdkReqKeyboardWizardTypeGrpField
{
    char      HqTypeCode[255];          //�������

    CHundsunH5sdkReqKeyboardWizardTypeGrpField()
    {
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//3002 ���̾�����Ϣ����
struct CHundsunH5sdkReqKeyboardWizardField
{
    char      ProdCode[255];            //
    int       Maxvalue;                 //��󷵻ظ���
    vector<CHundsunH5sdkReqKeyboardWizardTypeGrpField> TypeGrp; //��Ҫɸѡ��֤ȯ����

    CHundsunH5sdkReqKeyboardWizardField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        Maxvalue = 0;
    };
};

//3002 ���̾�����ϢӦ�� - �����б�
struct CHundsunH5sdkRspKeyboardWizardProdGrpField
{
    char      ProdCode[255];            //֤ȯ����
    char      ProdName[255];            //֤ȯ����
    char      HqTypeCode[255];          //֤ȯ���

    CHundsunH5sdkRspKeyboardWizardProdGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(ProdName, 0, sizeof(ProdName));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//3002 ���̾�����ϢӦ��
struct CHundsunH5sdkRspKeyboardWizardField
{
    vector<CHundsunH5sdkRspKeyboardWizardProdGrpField> ProdGrp; //�����б�

    CHundsunH5sdkRspKeyboardWizardField()
    {
    };
};

//4001 ������ն������� - 
struct CHundsunH5sdkReqSubscribeProdGrpField
{
    char      HqTypeCode[255];          //
    char      ProdCode[255];            //

    CHundsunH5sdkReqSubscribeProdGrpField()
    {
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        memset(ProdCode, 0, sizeof(ProdCode));
    };
};

//4001 ������ն�������
struct CHundsunH5sdkReqSubscribeField
{
    int       HqLevel;                  //���鼶��: level1, evel2(��������level1,��ʾ�ӳ�,level2��ʾʵʱ)
    vector<CHundsunH5sdkReqSubscribeProdGrpField> ProdGrp; //
    int       SubType;                  //���Ķ���

    CHundsunH5sdkReqSubscribeField()
    {
        HqLevel = 0;
        SubType = 0;
    };
};

//4001 ������ն���Ӧ�� - 
struct CHundsunH5sdkRspSubscribeProdGrpField
{
    char      HqTypeCode[255];          //
    char      ProdCode[255];            //����
    int       ErrorNo;                  //0�ɹ�,����:ʧ��

    CHundsunH5sdkRspSubscribeProdGrpField()
    {
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        memset(ProdCode, 0, sizeof(ProdCode));
        ErrorNo = 0;
    };
};

//4001 ������ն���Ӧ��
struct CHundsunH5sdkRspSubscribeField
{
    int       HqLevel;                  //���鼶��: level1, evel2(��������level1,��ʾ�ӳ�,level2��ʾʵʱ)
    vector<CHundsunH5sdkRspSubscribeProdGrpField> ProdGrp; //
    int       SubType;                  //ԭ�����ض��Ķ���

    CHundsunH5sdkRspSubscribeField()
    {
        HqLevel = 0;
        SubType = 0;
    };
};

//5001 ������������ - �����б�
struct CHundsunH5sdkReqSortSortProdGrpField
{
    char      ProdCode[255];            //��Ʊ����
    char      HqTypeCode[255];          //���ʹ���,[subtype][.cficode].exchage

    CHundsunH5sdkReqSortSortProdGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//5001 ������������ - ֤ȯ����
struct CHundsunH5sdkReqSortSortTypeGrpField
{
    char      HqTypeCode[255];          //�������

    CHundsunH5sdkReqSortSortTypeGrpField()
    {
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//5001 ������������ - ��Ҫ���ֶ��б�
struct CHundsunH5sdkReqSortFieldGrpField
{
    int       FieldId;                  //�Դ�IDΪ׼����idΪ��ʱ��ȡfilename
    char      FieldName[255];           //�ֶ�ID

    CHundsunH5sdkReqSortFieldGrpField()
    {
        FieldId = 0;
        memset(FieldName, 0, sizeof(FieldName));
    };
};

//5001 ������������
struct CHundsunH5sdkReqSortField
{
    vector<CHundsunH5sdkReqSortSortProdGrpField> SortProdGrp; //�����б�
    vector<CHundsunH5sdkReqSortSortTypeGrpField> SortTypeGrp; //֤ȯ����
    int       SortFieldId;              //��Ҫ������ֶ�ID���Դ�Ϊ׼��������ֶβ�����ʱ��ȡsort_field_name
    char      SortFieldName[255];       //��Ҫ������ֶ�
    vector<CHundsunH5sdkReqSortFieldGrpField> FieldGrp; //��Ҫ���ֶ��б�
    int       StartPos;                 //��ʼλ��,Ĭ��Ϊ0
    int       DataCount;                //������Ŀ,Ĭ��100ֻ���룬���������Ŀ10000
    int       SortType;                 //����ʽ/Ĭ�Ͻ���

    CHundsunH5sdkReqSortField()
    {
        SortFieldId = 0;
        memset(SortFieldName, 0, sizeof(SortFieldName));
        StartPos = 0;
        DataCount = 0;
        SortType = 0;
    };
};

//5001 ��������Ӧ�� - ί�ж����б�
struct CHundsunH5sdkRspSortEntrustGrpField
{
    int       EntrustAmount;            //ί����

    CHundsunH5sdkRspSortEntrustGrpField()
    {
        EntrustAmount = 0;
    };
};

//5001 ��������Ӧ�� - ί��λ
struct CHundsunH5sdkRspSortBidGrpField
{
    int       EntrustPx;                //ί�м۸�
    long      TotalEntrustAmount;       //ί����
    int       EntrustCount;             //ί����
    vector<CHundsunH5sdkRspSortEntrustGrpField> EntrustGrp; //ί�ж����б�

    CHundsunH5sdkRspSortBidGrpField()
    {
        EntrustPx = 0;
        TotalEntrustAmount = 0;
        EntrustCount = 0;
    };
};

//5001 ��������Ӧ�� - ί����λ
struct CHundsunH5sdkRspSortOfferGrpField
{
    int       EntrustPx;                //ί�м۸�
    long      TotalEntrustAmount;       //ί����
    int       EntrustCount;             //ί����
    vector<CHundsunH5sdkRspSortEntrustGrpField> EntrustGrp; //ί�ж����б�

    CHundsunH5sdkRspSortOfferGrpField()
    {
        EntrustPx = 0;
        TotalEntrustAmount = 0;
        EntrustCount = 0;
    };
};

//5001 ��������Ӧ�� - ���ǹ�Ʊ
struct CHundsunH5sdkRspSortRiseFirstGrpField
{
    char      ProdCode[255];            //֤ȯ����
    char      ProdName[255];            //֤ȯ����
    char      HqTypeCode[255];          //֤ȯ����
    int       LastPx;                   //���¼�
    int       PxChangeRate;             //�ǵ���

    CHundsunH5sdkRspSortRiseFirstGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(ProdName, 0, sizeof(ProdName));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        LastPx = 0;
        PxChangeRate = 0;
    };
};

//5001 ��������Ӧ�� - ���
struct CHundsunH5sdkRspSortFallFirstGrpField
{
    char      ProdCode[255];            //֤ȯ����
    char      ProdName[255];            //֤ȯ����
    char      HqTypeCode[255];          //֤ȯ����
    int       LastPx;                   //���¼�
    int       PxChangeRate;             //�ǵ���

    CHundsunH5sdkRspSortFallFirstGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(ProdName, 0, sizeof(ProdName));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        LastPx = 0;
        PxChangeRate = 0;
    };
};

//5001 ��������Ӧ�� - ֤ȯ����
struct CHundsunH5sdkRspSortSortTypeGrpField
{
    char      HqTypeCode[255];          //�������

    CHundsunH5sdkRspSortSortTypeGrpField()
    {
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//5001 ��������Ӧ�� - �����б�
struct CHundsunH5sdkRspSortSortProdGrpField
{
    char      ProdCode[255];            //֤ȯ����
    char      ProdName[255];            //֤ȯ����
    char      HqTypeCode[255];          //֤ȯ����
    char      IndustryCode[255];        //������루δ���壩
    char      MoneyType[255];           //���Ҵ��루δ���壩
    int       DataTimestamp;            //ʱ���
    int       TradeMins;                //���׷�����
    char      TradeStatus[255];         //����״̬
    int       PreclosePx;               //���ռ�
    int       OpenPx;                   //����
    int       LastPx;                   //���³ɽ���
    int       HighPx;                   //��߼�
    int       LowPx;                    //��ͼ�
    int       ClosePx;                  //��������
    int       AvgPx;                    //����
    int       WavgPx;                   //��Ȩƽ����
    int       BusinessCount;            //�ɽ�����
    long      BusinessAmount;           //�ܳɽ���
    long      BusinessBalance;          //�ܳɽ���
    int       UpPx;                     //��ͣ�۸�
    int       DownPx;                   //��ͣ�۸�
    long      CurrentAmount;            //����ɽ���(����)
    long      BusinessAmountIn;         //���̳ɽ���
    long      BusinessAmountOut;        //���̳ɽ���
    long      TotBuyAmount;             //��ί������δ���壩
    long      TotSellAmount;            //��ί������δ���壩
    int       WavgBidPx;                //��Ȩƽ��ί��۸�δ���壩
    int       WavgOfferPx;              //��Ȩƽ��ί���۸�δ���壩
    vector<CHundsunH5sdkRspSortBidGrpField> BidGrp; //ί��λ
    vector<CHundsunH5sdkRspSortOfferGrpField> OfferGrp; //ί����λ
    int       EntrustRate;              //ί��
    long      EntrustDiff;              //ί��
    int       W52LowPx;                 //52����ͼۣ�δ���壩
    int       W52HighPx;                //52����߼ۣ�δ���壩
    int       PxChange;                 //�ǵ���
    int       Amplitude;                //���
    int       PxChangeRate;             //�ǵ���
    int       PopcPx;                   //��ǰ�̺��
    int       TradeSection;             //��ǰ���׽׶�:0:����(û���ӳ�����) 1:��ǰ 2:���� 3:�̺� 4:ʵʱ�̺�(û���ӳ�����)
    long      Svol;                     //��ǰ���׽ڳɽ�����δ���壩
    long      CirculationAmount;        //��ͨ�ɱ�
    long      TotalShares;              //�ܹɱ�
    long      MarketValue;              //��ֵ
    long      CirculationValue;         //��ͨ��ֵ
    int       VolRatio;                 //����
    int       TurnoverRatio;            //������
    long      Amount;                   //�ֲ���
    long      PrevAmount;               //��ֲ�
    long      AmountDelta;              //������
    int       PrevSettlement;           //�����
    int       Settlement;               //�����
    int       Iopv;                     //iopv
    int       DebtFundValue;            //��ծ����ֵ
    int       Eps;                      //ÿ������
    int       Bps;                      //ÿ�ɾ��ʲ�
    int       DynPbRate;                //�о���
    int       PeRate;                   //��ӯ�ʣ���̬��
    int       FinQuarter;               //���񼾶�
    int       FinEndDate;               //�����������
    int       SharesPerHand;            //ÿ�ֹ���
    int       RiseCount;                //���Ǽ���
    int       FallCount;                //�µ�����
    int       MemberCount;              //��Ա����
    vector<CHundsunH5sdkRspSortRiseFirstGrpField> RiseFirstGrp; //���ǹ�Ʊ
    vector<CHundsunH5sdkRspSortFallFirstGrpField> FallFirstGrp; //���

    CHundsunH5sdkRspSortSortProdGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(ProdName, 0, sizeof(ProdName));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        memset(IndustryCode, 0, sizeof(IndustryCode));
        memset(MoneyType, 0, sizeof(MoneyType));
        DataTimestamp = 0;
        TradeMins = 0;
        memset(TradeStatus, 0, sizeof(TradeStatus));
        PreclosePx = 0;
        OpenPx = 0;
        LastPx = 0;
        HighPx = 0;
        LowPx = 0;
        ClosePx = 0;
        AvgPx = 0;
        WavgPx = 0;
        BusinessCount = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
        UpPx = 0;
        DownPx = 0;
        CurrentAmount = 0;
        BusinessAmountIn = 0;
        BusinessAmountOut = 0;
        TotBuyAmount = 0;
        TotSellAmount = 0;
        WavgBidPx = 0;
        WavgOfferPx = 0;
        EntrustRate = 0;
        EntrustDiff = 0;
        W52LowPx = 0;
        W52HighPx = 0;
        PxChange = 0;
        Amplitude = 0;
        PxChangeRate = 0;
        PopcPx = 0;
        TradeSection = 0;
        Svol = 0;
        CirculationAmount = 0;
        TotalShares = 0;
        MarketValue = 0;
        CirculationValue = 0;
        VolRatio = 0;
        TurnoverRatio = 0;
        Amount = 0;
        PrevAmount = 0;
        AmountDelta = 0;
        PrevSettlement = 0;
        Settlement = 0;
        Iopv = 0;
        DebtFundValue = 0;
        Eps = 0;
        Bps = 0;
        DynPbRate = 0;
        PeRate = 0;
        FinQuarter = 0;
        FinEndDate = 0;
        SharesPerHand = 0;
        RiseCount = 0;
        FallCount = 0;
        MemberCount = 0;
    };
};

//5001 ��������Ӧ��
struct CHundsunH5sdkRspSortField
{
    int       SortType;                 //����ʽ/Ĭ�Ͻ���
    vector<CHundsunH5sdkRspSortSortTypeGrpField> SortTypeGrp; //֤ȯ����
    int       SortFieldId;              //��Ҫ������ֶ�ID
    char      SortFieldName[255];       //��Ҫ������ֶ�
    int       StartPos;                 //��ʼλ��,Ĭ��Ϊ0
    vector<CHundsunH5sdkRspSortSortProdGrpField> SortProdGrp; //�����б�

    CHundsunH5sdkRspSortField()
    {
        SortType = 0;
        SortFieldId = 0;
        memset(SortFieldName, 0, sizeof(SortFieldName));
        StartPos = 0;
    };
};

//6001 ��ָ�������ڻ�ƫ��ȡ��ʱ��������
struct CHundsunH5sdkReqTrendField
{
    char      ProdCode[255];            //֤ȯ����
    char      HqTypeCode[255];          //���ʹ���
    int       Date;                     //����:YYYYMMDD,0:��ʾ����
    int       DateOffset;               //���뵱ǰ���ڵ�ƫ������,

    CHundsunH5sdkReqTrendField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        Date = 0;
        DateOffset = 0;
    };
};

//6001 ��ָ�������ڻ�ƫ��ȡ��ʱ����Ӧ�� - ��ʱ����
struct CHundsunH5sdkRspTrendTrendGrpField
{
    int       MinTime;                  //��ʱʱ��hhmm
    int       HqPx;                     //�۸�
    int       AvgPx;                    //����
    int       WavgPx;                   //��Ȩ����
    long      BusinessAmount;           //�ܳɽ���
    long      BusinessBalance;          //�ܳɽ���

    CHundsunH5sdkRspTrendTrendGrpField()
    {
        MinTime = 0;
        HqPx = 0;
        AvgPx = 0;
        WavgPx = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
    };
};

//6001 ��ָ�������ڻ�ƫ��ȡ��ʱ����Ӧ��
struct CHundsunH5sdkRspTrendField
{
    char      ProdCode[255];            //֤ȯ����
    char      HqTypeCode[255];          //���ʹ���
    int       Date;                     //���ڡ���ʽYYYYMMDD
    vector<CHundsunH5sdkRspTrendTrendGrpField> TrendGrp; //��ʱ����

    CHundsunH5sdkRspTrendField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        Date = 0;
    };
};

//6002 ��ƫ��ȡK������
struct CHundsunH5sdkReqCandleByOffsetField
{
    char      ProdCode[255];            //֤ȯ����
    char      HqTypeCode[255];          //���ʹ���
    int       CandlePeriod;             //k������
    int       CandleMode;               //
    int       Date;                     //��ʼ���ڣ�����д��ʾ��ǰ����
    int       MinTime;                  //����K�ߵ�ʱ��HHMM,���ڶ�����K������ʹ��(1min,5min��)������д��ʾ���µ��г�ʱ�䣬����д����ͬʱ��дdate�ֶ�
    int       SearchDirection;          //����
    int       DataCount;                //����ĸ���

    CHundsunH5sdkReqCandleByOffsetField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        CandlePeriod = 0;
        CandleMode = 0;
        Date = 0;
        MinTime = 0;
        SearchDirection = 0;
        DataCount = 0;
    };
};

//6002 ��ƫ��ȡK��Ӧ�� - ��ʱ����
struct CHundsunH5sdkRspCandleByOffsetCandleGrpField
{
    int       Date;                     //��ʼ���ڣ�0����ʾ��ǰʱ��
    int       MinTime;                  //����K�ߵ�ʱ�䣬HHMM
    int       OpenPx;                   //���̼�
    int       HighPx;                   //��߼�
    int       LowPx;                    //��ͼ�
    int       ClosePx;                  //���̼�
    long      BusinessAmount;           //�ɽ���
    long      BusinessBalance;          //�ɽ���

    CHundsunH5sdkRspCandleByOffsetCandleGrpField()
    {
        Date = 0;
        MinTime = 0;
        OpenPx = 0;
        HighPx = 0;
        LowPx = 0;
        ClosePx = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
    };
};

//6002 ��ƫ��ȡK��Ӧ��
struct CHundsunH5sdkRspCandleByOffsetField
{
    char      ProdCode[255];            //֤ȯȫ����
    char      HqTypeCode[255];          //���ʹ���
    int       CandlePeriod;             //k������
    int       CandleMode;               //
    int       AllDataFlag;              //����������Ƿ������� 1��ʾ�������ݣ���ʾ�û���������������Ŀǰ֧�����ĸ�������Ҫ�û����ݷ��ؽ���ٴ�����
    vector<CHundsunH5sdkRspCandleByOffsetCandleGrpField> CandleGrp; //��ʱ����

    CHundsunH5sdkRspCandleByOffsetField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        CandlePeriod = 0;
        CandleMode = 0;
        AllDataFlag = 0;
    };
};

//6003 ������ʱ�䷶ΧȡK������
struct CHundsunH5sdkReqCandleByRangeField
{
    char      ProdCode[255];            //֤ȯ����
    char      HqTypeCode[255];          //���ʹ���
    int       CandlePeriod;             //k������
    int       CandleMode;               //
    int       StartDate;                //
    int       EndDate;                  //

    CHundsunH5sdkReqCandleByRangeField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        CandlePeriod = 0;
        CandleMode = 0;
        StartDate = 0;
        EndDate = 0;
    };
};

//6003 ������ʱ�䷶ΧȡK��Ӧ�� - ��ʱ����
struct CHundsunH5sdkRspCandleByRangeCandleGrpField
{
    int       Date;                     //��ʼ���ڣ�0����ʾ��ǰʱ��
    int       MinTime;                  //
    int       OpenPx;                   //���̼�
    int       HighPx;                   //��߼�
    int       LowPx;                    //��ͼ�
    int       ClosePx;                  //���̼�
    long      BusinessAmount;           //�ɽ���
    long      BusinessBalance;          //�ɽ���

    CHundsunH5sdkRspCandleByRangeCandleGrpField()
    {
        Date = 0;
        MinTime = 0;
        OpenPx = 0;
        HighPx = 0;
        LowPx = 0;
        ClosePx = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
    };
};

//6003 ������ʱ�䷶ΧȡK��Ӧ��
struct CHundsunH5sdkRspCandleByRangeField
{
    char      ProdCode[255];            //֤ȯ����
    char      HqTypeCode[255];          //���ʹ���
    int       CandlePeriod;             //k������
    int       CandleMode;               //
    int       AllDataFlag;              //����������Ƿ������� 1��ʾ��������
    vector<CHundsunH5sdkRspCandleByRangeCandleGrpField> CandleGrp; //��ʱ����

    CHundsunH5sdkRspCandleByRangeField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        CandlePeriod = 0;
        CandleMode = 0;
        AllDataFlag = 0;
    };
};

//6004 ָ�����ӵķֱ�����
struct CHundsunH5sdkReqTickMinField
{
    char      ProdCode[255];            //֤ȯ����
    char      HqTypeCode[255];          //���ʹ���
    int       Date;                     //����:YYYYMMDD,0:��ʾ����
    int       StartMin;                 //��ʼ������
    int       EndMin;                   //����������
    int       DataCount;                //�����ٸ���Ĭ��ȫ������

    CHundsunH5sdkReqTickMinField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        Date = 0;
        StartMin = 0;
        EndMin = 0;
        DataCount = 0;
    };
};

//6004 ָ�����ӵķֱ�Ӧ�� - ��ʱ����
struct CHundsunH5sdkRspTickMinTickGrpField
{
    int       BusinessNo;               //�ֱ����
    int       BusinessTime;             //ʱ���
    int       HqPx;                     //�۸�
    long      BusinessAmount;           //�ɽ���
    long      BusinessBalance;          //�ɽ���
    int       BusinessCount;            //�ɽ�����
    int       BusinessDirection;        //�ɽ�����0������1����)

    CHundsunH5sdkRspTickMinTickGrpField()
    {
        BusinessNo = 0;
        BusinessTime = 0;
        HqPx = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
        BusinessCount = 0;
        BusinessDirection = 0;
    };
};

//6004 ָ�����ӵķֱ�Ӧ��
struct CHundsunH5sdkRspTickMinField
{
    char      ProdCode[255];            //֤ȯ����
    char      HqTypeCode[255];          //���ʹ���
    int       Date;                     //���ڡ���ʽYYYYMMDD
    int       StartMin;                 //��ʼ������
    int       EndMin;                   //����������
    int       AllDataFlag;              //����������Ƿ������� 1��ʾ��������
    vector<CHundsunH5sdkRspTickMinTickGrpField> TickGrp; //��ʱ����

    CHundsunH5sdkRspTickMinField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        Date = 0;
        StartMin = 0;
        EndMin = 0;
        AllDataFlag = 0;
    };
};

//6005 ��ƫ��ȡ�ֱ�����
struct CHundsunH5sdkReqTickDirectionField
{
    char      ProdCode[255];            //֤ȯ����
    char      HqTypeCode[255];          //���ʹ���
    int       Date;                     //����:YYYYMMDD,0:��ʾ����
    int       StartPos;                 //��ʼλ��,Ĭ��Ϊ0
    int       SearchDirection;          //����
    int       DataCount;                //�����ٸ���Ĭ��ȫ������

    CHundsunH5sdkReqTickDirectionField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        Date = 0;
        StartPos = 0;
        SearchDirection = 0;
        DataCount = 0;
    };
};

//6005 ��ƫ��ȡ�ֱ�Ӧ�� - ��ʱ����
struct CHundsunH5sdkRspTickDirectionTickGrpField
{
    int       BusinessNo;               //�ֱ����
    int       BusinessTime;             //ʱ���
    int       HqPx;                     //�۸�
    long      BusinessAmount;           //�ɽ���
    long      BusinessBalance;          //�ɽ���
    int       BusinessCount;            //�ɽ�����
    int       BusinessDirection;        //�ɽ�����0������1����)

    CHundsunH5sdkRspTickDirectionTickGrpField()
    {
        BusinessNo = 0;
        BusinessTime = 0;
        HqPx = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
        BusinessCount = 0;
        BusinessDirection = 0;
    };
};

//6005 ��ƫ��ȡ�ֱ�Ӧ��
struct CHundsunH5sdkRspTickDirectionField
{
    char      ProdCode[255];            //֤ȯ����
    char      HqTypeCode[255];          //���ʹ���
    int       Date;                     //����:YYYYMMDD,0:��ʾ����
    int       StartPos;                 //��ʼλ��,Ĭ��Ϊ0����ǰȡʱ��0��ʾ�����һ�ʣ����ȡʱ0��ʾ�����һ���ֱ�
    int       SearchDirection;          //����
    int       AllDataFlag;              //����������Ƿ������� 1��ʾ��������
    vector<CHundsunH5sdkRspTickDirectionTickGrpField> TickGrp; //��ʱ����

    CHundsunH5sdkRspTickDirectionField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        Date = 0;
        StartPos = 0;
        SearchDirection = 0;
        AllDataFlag = 0;
    };
};

//6006 ����ʹ��_����k������
struct CHundsunH5sdkReqCandleByRangeMaintainField
{
    char      ProdCode[255];            //֤ȯ����
    char      HqTypeCode[255];          //���ʹ���
    int       CandlePeriod;             //k������(ֻ��Ҫ����k��)
    int       CandleMode;               //
    int       StartDate;                //
    int       EndDate;                  //

    CHundsunH5sdkReqCandleByRangeMaintainField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        CandlePeriod = 0;
        CandleMode = 0;
        StartDate = 0;
        EndDate = 0;
    };
};

//6006 ����ʹ��_����k��Ӧ�� - ��ʱ����
struct CHundsunH5sdkRspCandleByRangeMaintainCandleGrpField
{
    int       Date;                     //��ʼ���ڣ�0����ʾ��ǰʱ��
    int       MinTime;                  //ʱ��
    int       WavgPx;                   //����
    int       OpenPx;                   //���̼�
    int       HighPx;                   //��߼�
    int       LowPx;                    //��ͼ�
    int       ClosePx;                  //���̼�
    long      BusinessAmount;           //�ɽ���
    long      BusinessBalance;          //�ɽ���
    long      Amount;                   //

    CHundsunH5sdkRspCandleByRangeMaintainCandleGrpField()
    {
        Date = 0;
        MinTime = 0;
        WavgPx = 0;
        OpenPx = 0;
        HighPx = 0;
        LowPx = 0;
        ClosePx = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
        Amount = 0;
    };
};

//6006 ����ʹ��_����k��Ӧ��
struct CHundsunH5sdkRspCandleByRangeMaintainField
{
    char      ProdCode[255];            //֤ȯ����
    char      HqTypeCode[255];          //���ʹ���
    int       CandlePeriod;             //k������
    int       CandleMode;               //
    int       AllDataFlag;              //����������Ƿ������� 1��ʾ��������
    vector<CHundsunH5sdkRspCandleByRangeMaintainCandleGrpField> CandleGrp; //��ʱ����

    CHundsunH5sdkRspCandleByRangeMaintainField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        CandlePeriod = 0;
        CandleMode = 0;
        AllDataFlag = 0;
    };
};

//6007 5�շ�ʱ����
struct CHundsunH5sdkReqDay5TrendField
{
    char      ProdCode[255];            //֤ȯ����
    char      HqTypeCode[255];          //���ʹ���

    CHundsunH5sdkReqDay5TrendField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//6007 5�շ�ʱӦ�� - ��ʱ����
struct CHundsunH5sdkRspDay5TrendTrendGrpField
{
    int       Date;                     //����:YYYYMMDD
    int       MinTime;                  //��ʱʱ��hhmm
    int       HqPx;                     //�۸�
    int       WavgPx;                   //��Ȩ����
    long      BusinessAmount;           //�ܳɽ���
    long      BusinessBalance;          //�ܳɽ���

    CHundsunH5sdkRspDay5TrendTrendGrpField()
    {
        Date = 0;
        MinTime = 0;
        HqPx = 0;
        WavgPx = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
    };
};

//6007 5�շ�ʱӦ��
struct CHundsunH5sdkRspDay5TrendField
{
    char      ProdCode[255];            //֤ȯ����
    char      HqTypeCode[255];          //���ʹ���
    vector<CHundsunH5sdkRspDay5TrendTrendGrpField> TrendGrp; //��ʱ����

    CHundsunH5sdkRspDay5TrendField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//7001 ���߷������� - �����б�
struct CHundsunH5sdkReqMaintainReturnProdGrpField
{
    char      ProdCode[255];            //��Ʊ����
    char      HqTypeCode[255];          //���ʹ���,[subtype][.cficode].exchage

    CHundsunH5sdkReqMaintainReturnProdGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//7001 ���߷�������
struct CHundsunH5sdkReqMaintainReturnField
{
    int       Subkind;                  //��������
    int       Maintainreturnvalue;      //
    vector<CHundsunH5sdkReqMaintainReturnProdGrpField> ProdGrp; //�����б�

    CHundsunH5sdkReqMaintainReturnField()
    {
        Subkind = 0;
        Maintainreturnvalue = 0;
    };
};

//7001 ���߷���Ӧ��
struct CHundsunH5sdkRspMaintainReturnField
{

    CHundsunH5sdkRspMaintainReturnField()
    {
    };
};

#endif
