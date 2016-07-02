#if !defined(MARKDATAUSERAPISTRUCT_H)
#define MARKDATAUSERAPISTRUCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PackerVersion 0x20
#define PANICHEARTBEATTIME (20*1000)

#define FuncMarkdatainsert              338499  ///����ؿ�

//������Ϣ
struct CMarkDataRspInfoField
{
    int       ErrorNo;                  //�������
    char      ErrorInfo[255];           //������ʾ
};

//�û���Ϣ
struct CMarkDataUserInfoField
{
    char      UserToken[512];           //�û�����
    int       OpBranchNo;               //������֧����
    char      OpEntrustWay;             //ί�з�ʽ
    char      OpStation[255];           //վ���ַ
    int       BranchNo;                 //��֧����
    char      ClientId[18];             //�ͻ����
    char      FundAccount[18];          //�ʲ��˻�
    char      Password[50];             //����
};

//338499 ����ؿ�����
struct CMarkDataReqFuncMarkdatainsertField
{
    int       UpdateTime;               //��󶳽����ʱ��
    char      FutuExchType[4];          //�������
    char      FutuCode[30];             //��Լ����
    char      FutuName[30];             //Ʒ�ּ��
    double    LastPrice;                //���¼�
    double    PreSettlementPrice;       //���ս����
    double    PreClosePrice;            //������
    double    PreOpenInterest;          //���տ�����
    double    OpenPrice;                //���̼�
    double    HighPrice;                //��߼�
    double    LowPrice;                 //��ͼ�
    double    BusinessBalance;          //�ɽ����
    int       BusinessAmount;           //�ɽ�����
    int       BusinCount;               //��������
    double    OpenInterest;             //�ܳ���
    double    ClosePrice;               //������
    double    SquarePrice;              //�����
    double    UplimitedPrice;           //��ͣ��۸�
    double    DownlimitedPrice;         //��ͣ��۸�
    double    BuyPrice1;                //�����һ
    int       BuyAmount1;               //������һ
    double    SalePrice1;               //������һ
    int       SaleAmount1;              //������һ
    double    BuyPrice2;                //����۶�
    int       BuyAmount2;               //��������
    double    SalePrice2;               //�����۶�
    int       SaleAmount2;              //��������
    double    BuyPrice3;                //�������
    int       BuyAmount3;               //��������
    double    SalePrice3;               //��������
    int       SaleAmount3;              //��������
    double    BuyPrice4;                //�������
    int       BuyAmount4;               //��������
    double    SalePrice4;               //��������
    int       SaleAmount4;              //��������
    double    BuyPrice5;                //�������
    int       BuyAmount5;               //��������
    double    SalePrice5;               //��������
    int       SaleAmount5;              //��������
};

//338499 ����ؿ�Ӧ��
struct CMarkDataRspFuncMarkdatainsertField
{
};

#endif
