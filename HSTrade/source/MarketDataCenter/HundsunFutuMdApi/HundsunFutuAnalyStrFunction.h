#ifndef HUNDSUNFUTUANALYSTRFUNCTION_H
#define HUNDSUNFUTUANALYSTRFUNCTION_H

#define  UDP_DELIMITE_STR  "\1"

inline void ParseValue(double &val, int size , char *str)
{
    val = atof(str);
}

inline void ParseValue(char& val, int size , char *str)
{
    val = str[0];
}

inline void ParseValue(int& val, int size , char *str)
{
    val = atoi(str);
}

inline void ParseValue(float& val, int size, char *str)
{
    val = atof(str);
}

inline void ParseValue(char *val, int size , char *str)
{
    hs_strncpy(val, str, size);
}

inline void ParseValue(unsigned int& val, int size , char *str)
{
    val = 0;
    for(int i= 0; str[i] != 0; i++)
    {
        if('0' <= str[i] && str[i] <= '9')
        {
            val = val * 10 + str[i] - '0';
        }
        else
        {
            val = 0;
            break;
        }
    }
}

#define BEGINPPARSE(type,outdataPtr,indataStrptr, delim) bool __packValid = true; \
    do{   type *__dataptr = outdataPtr; \
    char *__delim = delim; \
    char *__toksaveptr; \
    char *__token;\
    __token = strtok_t(indataStrptr, __delim, &__toksaveptr);

#define PARSEVALUE(val) if( __token != NULL ) \
{   \
    ParseValue(__dataptr->val,sizeof(__dataptr->val), __token); \
    __token = strtok_t(NULL, __delim, &__toksaveptr); \
}else {  __packValid = false; break; }

#define INGOREVALUE(val) if( __token != NULL ) \
{   \
    __token = strtok_t(NULL, __delim, &__toksaveptr); \
}else {  __packValid = false; break; }

#define ENDPARSE }while(0);

#define ISPACKValid  __packValid

//��������-�ڻ���������
inline bool ParseFuncQhSingleHqPush(CHundsunFutuRtnFuncQhSingleHqPushField *pFuncQhSingleHqPush, char *qhstr)
{
    memset(pFuncQhSingleHqPush, 0, sizeof(CHundsunFutuRtnFuncQhSingleHqPushField));
    BEGINPPARSE(CHundsunFutuRtnFuncQhSingleHqPushField,pFuncQhSingleHqPush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(QuoteType);              //��������(0-������ͨ����1-��������)
    PARSEVALUE(QuoteLength);            //�������ݳ��ȣ��ӵ�3�ֶο�ʼ�����һ���ֶμ��㳤�ȣ�
    PARSEVALUE(UpdateTime);             //ʱ���(eg:144510500,14��45��10��500����)
    PARSEVALUE(QuoteIndex);             //�������
    PARSEVALUE(FutuExchType);           //�������
    PARSEVALUE(CommodityType);          //��Ʒ����
    PARSEVALUE(FutuCode);               //��Լ����
    PARSEVALUE(PreSettlementPrice);     //���ս����
    PARSEVALUE(PreClosePrice);          //�������̼�
    PARSEVALUE(PreOpenInterest);        //���տ�����
    PARSEVALUE(PreDelta);               //������ʵ��
    PARSEVALUE(FutuOpenPrice);          //���̼�
    PARSEVALUE(FutuLastPrice);          //���¼۸�
    PARSEVALUE(BuyHighPrice);           //��������
    PARSEVALUE(BuyHighAmount);          //������������
    PARSEVALUE(LowSellPrice);           //������ۼ۸�
    PARSEVALUE(LowSellAmount);          //�����������
    PARSEVALUE(FutuHighPrice);          //��߼�
    PARSEVALUE(FutuLowPrice);           //��ͼ�
    PARSEVALUE(UplimitedPrice);         //��ͣ��
    PARSEVALUE(DownlimitedPrice);       //��ͣ��
    PARSEVALUE(AveragePrice);           //����
    PARSEVALUE(ChangeDirection);        //����
    PARSEVALUE(BusinessAmount);         //�ɽ���
    PARSEVALUE(OpenInterest);           //�ܳ���
    PARSEVALUE(BusinessBalance);        //�ɽ���
    PARSEVALUE(FutuClosePrice);         //�������̼�
    PARSEVALUE(SquarePrice);            //�����
    PARSEVALUE(CurrDate);               //��ǰ����(yyyymmdd)
    ENDPARSE;
    return ISPACKValid;
}

//��������-�ڻ�ί�лر���Ϣ
inline bool ParseFuncQhEntrustPush(CHundsunFutuRtnFuncQhEntrustPushField *pFuncQhEntrustPush, char *qhstr)
{
    memset(pFuncQhEntrustPush, 0, sizeof(CHundsunFutuRtnFuncQhEntrustPushField));
    BEGINPPARSE(CHundsunFutuRtnFuncQhEntrustPushField,pFuncQhEntrustPush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(EntrustNo);              //ί�к�
    PARSEVALUE(FuturesAccount);         //���ױ���
    PARSEVALUE(FutuExchType);           //���������F1-֣��F2����-F3�Ϻ�-F4�н�
    PARSEVALUE(FutuCode);               //��Լ����
    PARSEVALUE(EntrustBs);              //������ʶ(1-���� 2-����)
    PARSEVALUE(FuturesDirection);       //��ƽ��ʶ(1-����2-ƽ��4-ƽ��֣�����Ϊ��)
    PARSEVALUE(HedgeType);              //�ױ���ʶ(0-Ͷ��1-�ױ�2-����)
    PARSEVALUE(FundAccount);            //�ʽ��˻�
    PARSEVALUE(ReportId);               //���ص���
    PARSEVALUE(FirmId);                 //��Ա��
    PARSEVALUE(OperatorNo);             //����Ա��
    PARSEVALUE(ClientGroup);            //�ͻ����
    PARSEVALUE(EntrustAmount);          //ί������
    PARSEVALUE(TotalBusinessAmount);    //�ɽ�������
    PARSEVALUE(WithdrawAmount);         //��������
    PARSEVALUE(FutuEntrustPrice);       //ί�м۸�
    PARSEVALUE(EntrustStatus);          //ί��״̬
    PARSEVALUE(BranchNo);               //Ӫҵ����
    PARSEVALUE(BatchNo);                //ί������/���۱��
    PARSEVALUE(EntrustType);            //ί������
    PARSEVALUE(AmountPerHand);          //��Լ����
    PARSEVALUE(ForcedropReason);        //ǿƽԭ��
    PARSEVALUE(InitDate);               //��������
    PARSEVALUE(EntrustTime);            //��ǰʱ��
    PARSEVALUE(ConfirmId);              //��������
    PARSEVALUE(EntrustOccasion);        //ί�г���
    PARSEVALUE(FutuEntrustPrice2);      //ί�м۸�
    PARSEVALUE(EntrustProp);            //�ڻ�ί������(0���޼۵���1���м۵� 2��ֹ�𶨵� 3��ֹӯ���� 4���޼�ֹ�𶨵� 5���޼�ֹӯ���� 6��ֹ�� 7����϶��� A����������ȷ�ϣ�B���ֲ��ױ�ȷ�ϣ�C�����󱨼ۣ�D����ȨȨ����ʹ��E����ȨȨ��������F��˫�߱���)
    PARSEVALUE(ArbitCode);              //������Լ����
    PARSEVALUE(EntrustReference);       //ί������
    PARSEVALUE(ErrorMessage);           //����˵��
    PARSEVALUE(PositionStr);            //��λ��
    PARSEVALUE(SessionNo);              //�Ự���
    PARSEVALUE(SecondCode);             //�ڶ��Ⱥ�Լ����
    PARSEVALUE(WeaveType);              //�������
    PARSEVALUE(DeltaEntrustFare);       //ί�з��ñ仯
    PARSEVALUE(DeltaEntrustMargin);     //ί�б�֤��仯
    PARSEVALUE(DeltaEntrustPremium);    //ί��Ȩ����仯
    ENDPARSE;
    return ISPACKValid;
}

//��������-�ڻ��ɽ��ر���Ϣ
inline bool ParseFuncQhRealPush(CHundsunFutuRtnFuncQhRealPushField *pFuncQhRealPush, char *qhstr)
{
    memset(pFuncQhRealPush, 0, sizeof(CHundsunFutuRtnFuncQhRealPushField));
    BEGINPPARSE(CHundsunFutuRtnFuncQhRealPushField,pFuncQhRealPush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(EntrustNo);              //ί�к�
    PARSEVALUE(FuturesAccount);         //���ױ���
    PARSEVALUE(FutuExchType);           //���������
    PARSEVALUE(BusinessNo);             //�ɽ����
    PARSEVALUE(FutuCode);               //��Լ����
    PARSEVALUE(EntrustBs);              //������ʶ(1-���� 2-����)
    PARSEVALUE(FuturesDirection);       //��ƽ��ʶ(1-����2-ƽ��3-ƽ���  ����Ϊ��)
    PARSEVALUE(FutuBusinessPrice);      //�ɽ��۸�
    PARSEVALUE(BusinessAmount);         //�ɽ�����
    PARSEVALUE(HedgeType);              //�ױ���ʶ(0-Ͷ��1-�ױ�2-����)
    PARSEVALUE(FundAccount);            //�ʽ��˻�
    PARSEVALUE(ReportId);               //���ص���
    PARSEVALUE(FirmId);                 //��Ա��
    PARSEVALUE(OperatorNo);             //����Ա��
    PARSEVALUE(ClientGroup);            //�ͻ����
    PARSEVALUE(EntrustAmount);          //ί������
    PARSEVALUE(TotalBusinessAmount);    //�ɽ�������
    PARSEVALUE(WithdrawAmount);         //��������
    PARSEVALUE(FutuEntrustPrice);       //ί�м۸�
    PARSEVALUE(EntrustStatus);          //ί��״̬
    PARSEVALUE(BranchNo);               //Ӫҵ����
    PARSEVALUE(BatchNo);                //ί������/���۱��
    PARSEVALUE(EntrustType);            //ί������
    PARSEVALUE(AmountPerHand);          //��Լ����
    PARSEVALUE(ForcedropReason);        //ǿƽԭ��
    PARSEVALUE(InitDate);               //��������
    PARSEVALUE(BusinessTime);           //�ɽ�ʱ��
    PARSEVALUE(ConfirmId);              //��������
    PARSEVALUE(EntrustOccasion);        //ί�г���
    PARSEVALUE(FutuEntrustPrice2);      //ί������(ί������۸�)
    PARSEVALUE(EntrustProp);            //�ڻ�ί������(0���޼۵���1���м۵� 2��ֹ�𶨵� 3��ֹӯ���� 4���޼�ֹ�𶨵� 5���޼�ֹӯ���� 6��ֹ�� 7����϶��� A����������ȷ�ϣ�B���ֲ��ױ�ȷ�ϣ�C�����󱨼ۣ�D����ȨȨ����ʹ��E����ȨȨ��������F��˫�߱���)
    PARSEVALUE(ArbitCode);              //������Լ����
    PARSEVALUE(EntrustReference);       //ί������
    PARSEVALUE(PositionStr);            //��λ��
    PARSEVALUE(SessionNo);              //�Ự���
    PARSEVALUE(DeltaEntrustFare);       //ί�з��ñ仯
    PARSEVALUE(DeltaBusinessFare);      //�ɽ����ñ仯
    PARSEVALUE(DeltaEntrustMargin);     //ί�б�֤��仯
    PARSEVALUE(DeltaHoldMargin);        //�ֱֲ�֤��仯
    PARSEVALUE(DeltaEntrustPremium);    //ί��Ȩ����仯
    PARSEVALUE(DeltaPremium);           //�ɽ�Ȩ����仯
    PARSEVALUE(DeltaDropIncome);        //ƽ�ֶ���ӯ��
    PARSEVALUE(DeltaDropIncomeFloat);   //ƽ�ָ���ӯ��
    ENDPARSE;
    return ISPACKValid;
}

//��������-�ڻ��������
inline bool ParseFuncQhCombinePush(CHundsunFutuRtnFuncQhCombinePushField *pFuncQhCombinePush, char *qhstr)
{
    memset(pFuncQhCombinePush, 0, sizeof(CHundsunFutuRtnFuncQhCombinePushField));
    BEGINPPARSE(CHundsunFutuRtnFuncQhCombinePushField,pFuncQhCombinePush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(ArbitCode);              //������Լ��
    PARSEVALUE(FutuExchType);           //�������
    PARSEVALUE(FirstCode);              //��һ��
    PARSEVALUE(SecondCode);             //�ڶ���
    PARSEVALUE(WeaveType);              //�������1-SPD, 2 -IPS
    PARSEVALUE(FutuBuyPrice);           //��������
    PARSEVALUE(BuyAmount);              //������������
    PARSEVALUE(TotalBuyAmount);         //ȫ������
    PARSEVALUE(FutuSellPrice);          //������ۼ۸�
    PARSEVALUE(SellAmount);             //�����������
    PARSEVALUE(TotalSaleAmount);        //ȫ������
    PARSEVALUE(FutuHighPrice);          //��߼�
    PARSEVALUE(FutuLowPrice);           //��ͼ�
    PARSEVALUE(UplimitedPrice);         //��ͣ��۸�
    PARSEVALUE(DownlimitedPrice);       //��ͣ��۸�
    ENDPARSE;
    return ISPACKValid;
}

//��������-�ڻ�������״̬��Ϣ
inline bool ParseFuncQhExchStatusPush(CHundsunFutuRtnFuncQhExchStatusPushField *pFuncQhExchStatusPush, char *qhstr)
{
    memset(pFuncQhExchStatusPush, 0, sizeof(CHundsunFutuRtnFuncQhExchStatusPushField));
    BEGINPPARSE(CHundsunFutuRtnFuncQhExchStatusPushField,pFuncQhExchStatusPush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(FutuExchType);           //���������F1-֣��F2����-F3�Ϻ�-F4�н�
    PARSEVALUE(ExchStatus);             //������״̬��0���ӶϿ�,1����ǰ, 2���ϱ��� 3����ƽ�� 4���۴�� 5 �������� 6��ͣ���� 7���� Xδ֪��
    PARSEVALUE(EntradeFlag);            //�Ƿ�ɽ��ף�0���ɽ��ף�1�ɽ��ף�
    ENDPARSE;
    return ISPACKValid;
}

//��������-�ڻ�ѯ��֪ͨ
inline bool ParseFuncQhInquiryPush(CHundsunFutuRtnFuncQhInquiryPushField *pFuncQhInquiryPush, char *qhstr)
{
    memset(pFuncQhInquiryPush, 0, sizeof(CHundsunFutuRtnFuncQhInquiryPushField));
    BEGINPPARSE(CHundsunFutuRtnFuncQhInquiryPushField,pFuncQhInquiryPush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(InquiryNo);              //ѯ�۱��
    PARSEVALUE(FutuCode);               //��Լ����
    PARSEVALUE(InitDate);               //��������
    PARSEVALUE(UpdateTime);             //ѯ��ʱ��(eg:144510500,14��45��10��500����)
    ENDPARSE;
    return ISPACKValid;
}

//��������-�ڻ����ۻر�
inline bool ParseFuncQhQuotePush(CHundsunFutuRtnFuncQhQuotePushField *pFuncQhQuotePush, char *qhstr)
{
    memset(pFuncQhQuotePush, 0, sizeof(CHundsunFutuRtnFuncQhQuotePushField));
    BEGINPPARSE(CHundsunFutuRtnFuncQhQuotePushField,pFuncQhQuotePush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(FutuExchType);           //���������F1-֣��F2����-F3�Ϻ�-F4�н�
    PARSEVALUE(FutuCode);               //��Լ����
    PARSEVALUE(BuyFuturesDirection);    //�򷽿�ƽ��־
    PARSEVALUE(BuyHedgeType);           //���ױ���־
    PARSEVALUE(BuyQuotePrice);          //�򷽼۸�
    PARSEVALUE(BuyQuoteAmount);         //����
    PARSEVALUE(SellFuturesDirection);   //������ƽ��־
    PARSEVALUE(SellHedgeType);          //�����ױ���־
    PARSEVALUE(SellQuotePrice);         //�����۸�
    PARSEVALUE(SellQuoteAmount);        //������
    PARSEVALUE(QuoteNo);                //���۱��
    PARSEVALUE(FundAccount);            //�ʽ��˻�
    PARSEVALUE(ReportId);               //���ص���
    PARSEVALUE(FirmId);                 //��Ա��
    PARSEVALUE(OperatorNo);             //����Ա��
    PARSEVALUE(QuoteStatus);            //ί��״̬
    PARSEVALUE(BranchNo);               //Ӫҵ����
    PARSEVALUE(InitDate);               //��������
    PARSEVALUE(CurrTime);               //��ǰʱ��
    PARSEVALUE(ConfirmId);              //��������
    PARSEVALUE(BuyConfirmId);           //����������
    PARSEVALUE(SellConfirmId);          //������������
    PARSEVALUE(EntrustOccasion);        //ί�г���
    PARSEVALUE(EntrustReference);       //��������
    PARSEVALUE(RequestId);              //������
    PARSEVALUE(PositionStr);            //��λ��
    ENDPARSE;
    return ISPACKValid;
}

//��������-������Ȩ�ɽ��ر�
inline bool ParseFuncOptDealPush(CHundsunFutuRtnFuncOptDealPushField *pFuncOptDealPush, char *qhstr)
{
    memset(pFuncOptDealPush, 0, sizeof(CHundsunFutuRtnFuncOptDealPushField));
    BEGINPPARSE(CHundsunFutuRtnFuncOptDealPushField,pFuncOptDealPush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(BranchNo);               //��֧����
    PARSEVALUE(ClientId);               //�ͻ����
    PARSEVALUE(FundAccount);            //�ʲ��˻�
    PARSEVALUE(RealType);               //�ɽ�����
    PARSEVALUE(RealStatus);             //�����־
    PARSEVALUE(ExchangeType);           //�������
    PARSEVALUE(ReportSeat);             //�걨ϯλ
    PARSEVALUE(ReportNo);               //������
    PARSEVALUE(ReportAccount);          //�걨�˺�
    PARSEVALUE(StockAccount);           //֤ȯ�˺�
    PARSEVALUE(OptholdType);            //��Ȩ�ֲ����
    PARSEVALUE(OptionCode);             //��Ȩ��Լ����
    PARSEVALUE(OptionType);             //��Ȩ����
    PARSEVALUE(RecordNo);               //ί�п��¼��
    PARSEVALUE(ReportBs);               //�걨����
    PARSEVALUE(EntrustBs);              //��������
    PARSEVALUE(EntrustOc);              //��ƽ�ַ���
    PARSEVALUE(CoveredFlag);            //���ұ�־
    PARSEVALUE(EntrustStatus);          //ί��״̬
    PARSEVALUE(EntrustNo);              //ί�б��
    PARSEVALUE(BusinessId);             //�ɽ����
    PARSEVALUE(BusinessAmount);         //�ɽ�����
    PARSEVALUE(OptBusinessPrice);       //�ɽ��۸�
    PARSEVALUE(BusinessTime);           //�ɽ�ʱ��
    PARSEVALUE(ExternCode);             //�ⲿ�������
    PARSEVALUE(ReportTime);             //�걨ʱ��
    PARSEVALUE(EntrustAmount);          //ί������
    PARSEVALUE(BatchNo);                //ί������
    PARSEVALUE(OptEntrustPrice);        //ί�м۸�
    PARSEVALUE(EntrustType);            //ί�����
    ENDPARSE;
    return ISPACKValid;
}

//��������-������Ȩί�л�д
inline bool ParseFuncOptEntrustPush(CHundsunFutuRtnFuncOptEntrustPushField *pFuncOptEntrustPush, char *qhstr)
{
    memset(pFuncOptEntrustPush, 0, sizeof(CHundsunFutuRtnFuncOptEntrustPushField));
    BEGINPPARSE(CHundsunFutuRtnFuncOptEntrustPushField,pFuncOptEntrustPush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(FundAccount);            //�ʲ��˻�
    PARSEVALUE(ReportNo);               //������
    PARSEVALUE(OptionCode);             //��Ȩ��Լ����
    PARSEVALUE(EntrustBs);              //��������
    PARSEVALUE(EntrustOc);              //��ƽ�ַ���
    PARSEVALUE(CoveredFlag);            //���ұ�־
    PARSEVALUE(EntrustStatus);          //ί��״̬
    PARSEVALUE(EntrustNo);              //ί�б��
    PARSEVALUE(EntrustAmount);          //ί������
    PARSEVALUE(BatchNo);                //ί������
    PARSEVALUE(OptEntrustPrice);        //ί�м۸�
    PARSEVALUE(EntrustType);            //ί�����
    PARSEVALUE(ErrorNo);                //�������
    PARSEVALUE(ErrorInfo);              //������ʾ
    ENDPARSE;
    return ISPACKValid;
}

#endif
