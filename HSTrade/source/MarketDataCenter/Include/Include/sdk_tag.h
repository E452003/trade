/**
 *Դ��������:sdk_tag.h
 *�������Ȩ:�������ӹɷ����޹�˾
 *ϵͳ����:H5����Э��
 *ģ������:H5����Э��
 *����˵��:����H5����Э���е��ֶ�TAG
 *��    ��: ��С��
 *��������: 2016-01-27 22:16:31
 *�� �� ��: 1.0.0.1
 *��    ע: �����ĵ���H5�������Э��(�޶���).xls������
 */

#ifndef __H5_SDK_TAG_H__
#define __H5_SDK_TAG_H__

#include "hscomm_message_interface.h"

#define MESSAGE_TEMPLATE_VERSION	"1.0.0.1"


/*****************������Ϣ����*******************/
const int H5PROTO_HEAD_INFO = 0 ; // 
const int H5SDK_MSG_LOGIN = 1001 ; // ��������
const int H5SDK_MSG_LOGOUT = 1002 ; // �ǳ���Ϣ
const int H5SDK_MSG_HEARTBEAT = 1003 ; // ����
const int H5SDK_SERVER_INFO = 1004 ; // �����������Ϣ
const int H5SDK_MSG_BATCH = 1006 ; // ������
const int H5SDK_MSG_FILE = 2001 ; // ����̬�ļ�
const int H5SDK_MSG_MARKET_TYPES = 2002 ; // �г�������Ϣ
const int H5SDK_MSG_MARKET_REFERENCE = 2003 ; // �г������
const int H5SDK_MSG_SNAPSHOT = 3001 ; // �������
const int H5SDK_KEYBOARD_WIZARD = 3002 ; // ���̾�����Ϣ
const int H5SDK_MSG_SUBSCRIBE = 4001 ; // ������ն���
const int H5SDK_MSG_SORT = 5001 ; // ��������
const int H5SDK_MSG_TREND = 6001 ; // ��ָ�������ڻ�ƫ��ȡ��ʱ����
const int H5SDK_MSG_CANDLE_BY_OFFSET = 6002 ; // ��ƫ��ȡK��
const int H5SDK_MSG_CANDLE_BY_RANGE = 6003 ; // ������ʱ�䷶ΧȡK��
const int H5SDK_MSG_TICK_MIN = 6004 ; // ָ�����ӵķֱ�
const int H5SDK_MSG_TICK_DIRECTION = 6005 ; // ��ƫ��ȡ�ֱ�
const int H5SDK_MSG_CANDLE_BY_RANGE_MAINTAIN = 6006 ; // ����ʹ��_����k��
const int H5SDK_MSG_DAY5_TREND = 6007 ; // 5�շ�ʱ
const int H5SDK_MAINTAIN_RETURN = 7001 ; // ���߷���


/*****************ϵͳ����*******************/

/// �������ͣ�uint32(ö��); ������ҵ��prod_code,�����Һ��ʵ�ģ��
const int H5PROTO_TAG_BUSINESS_ID = 1 ;
    const uint32 BIZ_SYSTEM = 0 ; // ϵͳģ��
    const uint32 BIZ_H5HQ = 1 ; // ����ģ��
    const uint32 BIZ_UFX = 2 ; // UFXģ��
    const uint32 BIZ_H5PROTO = 3 ; // H5�������Э��

/// �������ͣ�uint32(ö��); ��������������
const int H5PROTO_TAG_PACKET_TYPE = 3 ;
    const uint32 REQUEST = 0 ; // ����
    const uint32 ANSWER = 1 ; // Ӧ��
    const uint32 PUSH = 2 ; // ���Ʊ���

/// �������ͣ�uint32; ���������ܺ�
const int H5PROTO_TAG_FUNCTION_ID = 5 ;

/// �������ͣ�rawdata; �������ͻ���KEY
const int H5PROTO_TAG_USER_KEY = 7 ;

/// �������ͣ�rawdata; �������Ự��
const int H5PROTO_TAG_SESSION_ID = 8 ;

/// �������ͣ�uint32; �����������
const int H5PROTO_TAG_ERROR_NO = 20016 ;

/// �������ͣ�bytevector; ������������Ϣ
const int H5PROTO_TAG_ERROR_INFO = 20017 ;

/// �������ͣ�array; ������ȱʡ���ͣ�uint32; �����������ߵı�ʶ��Ϣ,��ʾ��Ϣ���ĸ����������ĸ�ͨ�����ĸ����ӻ��,һ��ͨ���������ڽ��յ���Ϣ��,����IDX_CONNECTID��IDX_CHANNEL_INDEX��������Ա��
const int H5PROTO_TAG_SEND_INFO_ARRAY = 100 ;

/// �������ͣ�array; ������ȱʡ���ͣ�uint32; ����������ʱ�䣬�����첽����ʱ��
const int H5PROTO_TAG_TIME_STAMP_ARRAY = 52 ;

/// �������ͣ�uint32; �������������
const int H5PROTO_TAG_BATCH_NO = 21 ;

/// �������ͣ�uint32; ������ͬ�����ñ��
const int H5PROTO_TAG_SYNC_NO = 22 ;

/// �������ͣ�rawdata; ��������ˮ��Ϣ
const int H5PROTO_TAG_SERIAL_INFO = 23 ;


/*****************��Ϣ����*******************/

/// �������ͣ�string; �������û���
const int H5SDK_TAG_USER_NAME = 20001 ;

/// �������ͣ�string; ����������
const int H5SDK_TAG_PASSWORD = 20002 ;

/// �������ͣ�string; ��������̬����
const int H5SDK_TAG_DYNMIC_PASSWORD = 20003 ;

/// �������ͣ�bytevector; ������ԭʼ����
const int H5SDK_TAG_ORGINAL_DATA = 96 ;

/// �������ͣ�uint32; �������������
const int H5SDK_TAG_HEARTBEAT_INTERVAL = 108 ;

/// �������ͣ�uint32; ������SDK�汾
const int H5SDK_TAG_SDK_VERSION = 20005 ;

/// �������ͣ�bytevector; ����������ϵͳ�汾
const int H5SDK_TAG_OS_VERSION = 20006 ;

/// �������ͣ�bytevector; ����������������
const int H5SDK_TAG_SERVER_NAME = 20007 ;

/// �������ͣ�uint64; ������������ʱ�䡣Unixʱ���
const int H5SDK_TAG_SERVER_TIME = 20008 ;

/// �������ͣ�uint32; ��������ǰ����
const int H5SDK_TAG_CURR_ONLINE_COUNT = 20009 ;

/// �������ͣ�uint32; �������������
const int H5SDK_TAG_MAX_ONLINE_COUNT = 20010 ;

/// �������ͣ�uint32(ö��); �������ļ�����
const int H5SDK_TAG_HQ_FILE_TYPE = 20011 ;
    const uint32 H5SDK_ENUM_FT_COMMON_FILE = 0 ; // ��ͨ�ļ�����Ҫʹ���ļ��������·����Ϣ���з���
    const uint32 H5SDK_ENUM_FT_FINDATA_FILE = 1 ; // ���������ļ�
    const uint32 H5SDK_ENUM_FT_EXRIGHT_FILE = 2 ; // ��Ȩ�����ļ�
    const uint32 H5SDK_ENUM_FT_INFO_CONFIG_FILE = 3 ; // ��Ѷ�����ļ�
    const uint32 H5SDK_ENUM_FT_WELCOME_FILE = 4 ; // ��ӭ�ļ�
    const uint32 H5SDK_ENUM_FT_DYNAMIC_NEWS_FILE = 5 ; // ��������Ϣ�ļ�
    const uint32 H5SDK_ENUM_FT_SYS_BLOCK_FILE = 6 ; // ϵͳ����ļ�
    const uint32 H5SDK_ENUM_FT_USR_BLOCK_FILE = 7 ; // �Զ������ļ�
    const uint32 H5SDK_ENUM_FT_BLOCK_CODE_FILE = 8 ; // �����֯��ϵ�ļ�
    const uint32 H5SDK_ENUM_FT_MARKET_MONITOR_CONFIG_FILE = 9 ; // ���߾��������ļ�
    const uint32 H5SDK_ENUM_FT_CALL_AUCTION_TIME_FILE = 10 ; // ���Ͼ���ʱ�������ļ�

/// �������ͣ�bytevector; �������ļ�����
const int H5SDK_TAG_HQ_FILE_NAME = 16001 ;

/// �������ͣ�uint64; �������ļ�ƫ��
const int H5SDK_TAG_FILE_OFFSET = 20012 ;

/// �������ͣ�uint32; �������ļ�����
const int H5SDK_TAG_FILE_LENGTH = 20013 ;

/// �������ͣ�uint32; �������ɽ�ʱ��
const int H5SDK_TAG_BUSINESS_TIME = 20014 ;

/// �������ͣ�uint32; ����������CRC
const int H5SDK_TAG_CRC = 20015 ;

/// �������ͣ�uint32; �����������
const int H5SDK_TAG_ERROR_NO = 20016 ;

/// �������ͣ�bytevector; ���������������ַ���
const int H5SDK_TAG_ERROR_INFO = 20017 ;

/// �������ͣ�bytevector; ����������������
const int H5SDK_TAG_FINANCE_MIC = 20018 ;

/// �������ͣ�bytevector; ����������������
const int H5SDK_TAG_FINANCE_NAME = 20019 ;

/// �������ͣ�uint32; �������г�����
const int H5SDK_TAG_MARKET_DATE = 230 ;

/// �������ͣ�uint32; ��������������
const int H5SDK_TAG_INIT_DATE = 75 ;

/// �������ͣ�int32; ������ʱ��
const int H5SDK_TAG_TIMEZONE = 20020 ;

/// �������ͣ�uint8; ����������ʱ��־
const int H5SDK_TAG_DST_FLAG = 20021 ;

/// �������ͣ�sequence; �����������ظ���
const int H5SDK_TAG_TYPE_GRP = 20022 ;

/// �������ͣ�sequence; ���������������ظ���
const int H5SDK_TAG_SORT_TYPE_GRP = 20071 ;

/// �������ͣ�bytevector; ���������ʹ���
const int H5SDK_TAG_HQ_TYPE_CODE = 167 ;

/// �������ͣ�bytevector; ��������������
const int H5SDK_TAG_HQ_TYPE_NAME = 20023 ;

/// �������ͣ�uint32; �������۸�Ŵ���
const int H5SDK_TAG_PRICE_SCALE = 20024 ;

/// �������ͣ�sequence; ����������ʱ���
const int H5SDK_TAG_TRADE_SECTION_GRP = 20025 ;

/// �������ͣ�uint32; ����������ʱ��
const int H5SDK_TAG_OPEN_TIME = 20026 ;

/// �������ͣ�uint32; ����������ʱ��
const int H5SDK_TAG_CLOSE_TIME = 20027 ;

/// �������ͣ�uint32; ���������ռ�
const int H5SDK_TAG_PRECLOSE_PX = 140 ;

/// �������ͣ�uint32; ��������ͣ�۸�
const int H5SDK_TAG_UP_PRICE = 20028 ;

/// �������ͣ�uint32; ��������ͣ�۸�
const int H5SDK_TAG_DOWN_PRICE = 20029 ;

/// �������ͣ�uint8(ö��); ����������ȼ�
const int H5SDK_TAG_HQ_LEVEL = 61 ;
    const uint8 H5SDK_ENUM_LEVEL_1 = 1 ; // ��������
    const uint8 H5SDK_ENUM_LEVEL_2 = 2 ; // 2������

/// �������ͣ�sequence; ��������Ʊ���ظ���
const int H5SDK_TAG_PROD_GRP = 20030 ;

/// �������ͣ�sequence; �����������Ʊ���ظ���
const int H5SDK_TAG_SORT_PROD_GRP = 20070 ;

/// �������ͣ�bytevector; ��������Ʊ����
const int H5SDK_TAG_PROD_CODE = 48 ;

/// �������ͣ�string; ��������ҵ����
const int H5SDK_TAG_INDUSTRY_CODE = 20031 ;

/// �������ͣ�string; ����������
const int H5SDK_TAG_MONEY_TYPE = 20032 ;

/// �������ͣ�uint32; ������ʱ���
const int H5SDK_TAG_DATA_TIMESTAMP = 10178 ;

/// �������ͣ�string; ����������״̬
const int H5SDK_TAG_TRADE_STATUS = 10135 ;

/// �������ͣ�uint32; ���������̼�
const int H5SDK_TAG_OPEN_PX = 10018 ;

/// �������ͣ�uint32; ���������¼�
const int H5SDK_TAG_LAST_PX = 31 ;

/// �������ͣ�uint32; ��������߼�
const int H5SDK_TAG_HIGH_PX = 332 ;

/// �������ͣ�uint32; ��������ͼ�
const int H5SDK_TAG_LOW_PX = 333 ;

/// �������ͣ�uint32; ���������̼�
const int H5SDK_TAG_CLOSE_PX = 10204 ;

/// �������ͣ�uint32; ������ƽ����
const int H5SDK_TAG_AVG_PX = 49 ;

/// �������ͣ�uint32; ��������Ȩƽ����
const int H5SDK_TAG_WAVG_PX = 50 ;

/// �������ͣ�uint32; �������ɽ�����
const int H5SDK_TAG_BUSINESS_COUNT = 8503 ;

/// �������ͣ�uint64; �������ɽ���
const int H5SDK_TAG_BUSINESS_AMOUNT = 387 ;

/// �������ͣ�uint64; �������ɽ���
const int H5SDK_TAG_BUSINESS_BALANCE = 8504 ;

/// �������ͣ�uint64; ����������
const int H5SDK_TAG_CURRENT_AMOUNT = 30 ;

/// �������ͣ�uint64; ���������̳ɽ���
const int H5SDK_TAG_BUSINESS_AMOUNT_IN = 20033 ;

/// �������ͣ�uint64; ���������̳ɽ���
const int H5SDK_TAG_BUSINESS_AMOUNT_OUT = 20034 ;

/// �������ͣ�uint64; ��������ί����
const int H5SDK_TAG_TOTAL_BUY_AMOUNT = 10043 ;

/// �������ͣ�uint64; ��������ί����
const int H5SDK_TAG_TOTAL_SELL_AMOUNT = 10044 ;

/// �������ͣ�uint32; ��������Ȩƽ��ί���
const int H5SDK_TAG_WAVG_BID_PX = 10039 ;

/// �������ͣ�uint32; ��������Ȩƽ��ί����
const int H5SDK_TAG_WAVG_OFFER_PX = 10040 ;

/// �������ͣ�sequence; ������ί��λ
const int H5SDK_TAG_BID_GRP = 10068 ;

/// �������ͣ�sequence; ������ί����λ
const int H5SDK_TAG_OFFER_GRP = 10069 ;

/// �������ͣ�uint32; �������۸�
const int H5SDK_TAG_HQ_PRICE = 44 ;

/// �������ͣ�uint64; ������ί����
const int H5SDK_TAG_TOTAL_ENTRUST_AMOUNT = 39 ;

/// �������ͣ�sequence; ������ί�е��ظ���
const int H5SDK_TAG_ENTRUST_GRP = 73 ;

/// �������ͣ�uint32; ������ί�м۸�
const int H5SDK_TAG_ENTRUST_PX = 20067 ;

/// �������ͣ�uint32; ������ί�е���
const int H5SDK_TAG_ENTRUST_AMOUNT = 38 ;

/// �������ͣ�uint32; ������52����ͼ�
const int H5SDK_TAG_WEEK52_LOW_PX = 20035 ;

/// �������ͣ�uint32; ������52����߼�
const int H5SDK_TAG_WEEK52_HIGH_PX = 20036 ;

/// �������ͣ�int32; �������۸��ǵ�
const int H5SDK_TAG_PX_CHANGE = 20037 ;

/// �������ͣ�int32; �������ǵ���
const int H5SDK_TAG_PX_CHANGE_RATE = 20038 ;

/// �������ͣ�uint32; ��������ǰ/�̺�۸�
const int H5SDK_TAG_POPC_PX = 20039 ;

/// �������ͣ�uint64; ��������ǰ�׶εĳɽ���
const int H5SDK_TAG_SESSION_VOLUMUE = 20040 ;

/// �������ͣ�uint32; ����������: YYYYMMDD��0����ʾ��ǰ����
const int H5SDK_TAG_DATE = 20041 ;

/// �������ͣ�int32; ����������ƫ������
const int H5SDK_TAG_DATE_OFFSET = 20042 ;

/// �������ͣ�sequence; ��������ʱ�����ظ���
const int H5SDK_TAG_TREND_GRP = 20043 ;

/// �������ͣ�uint32(ö��); ������K������
const int H5SDK_TAG_CANDLE_PEROID = 20044 ;
    const uint32 H5SDK_ENUM_PEROID_1MIN = 1 ; // һ����
    const uint32 H5SDK_ENUM_PEROID_5MIN = 2 ; // 5����
    const uint32 H5SDK_ENUM_PEROID_15MIN = 3 ; // 15����
    const uint32 H5SDK_ENUM_PEROID_30MIN = 4 ; // 30����
    const uint32 H5SDK_ENUM_PEROID_60MIN = 5 ; // 60����
    const uint32 H5SDK_ENUM_PEROID_DAY = 6 ; // ����
    const uint32 H5SDK_ENUM_PEROID_WEEK = 7 ; // ����
    const uint32 H5SDK_ENUM_PEROID_MONTH = 8 ; // ����
    const uint32 H5SDK_ENUM_PEROID_YEAR = 9 ; // ����

/// �������ͣ�uint8(ö��); ������K����������
const int H5SDK_TAG_DIRECTION = 20046 ;
    const uint8 H5SDK_ENUM_FORWARD = 1 ; // ��ǰ
    const uint8 H5SDK_ENUM_BACKWARD = 2 ; // ���

/// �������ͣ�uint32; ���������ݸ���
const int H5SDK_TAG_DATA_COUNT = 20047 ;

/// �������ͣ�uint32; ��������ʼ����,YYYYMMDD
const int H5SDK_TAG_START_DATE = 20048 ;

/// �������ͣ�uint32; ��������������,YYYYMMDD
const int H5SDK_TAG_END_DATE = 20049 ;

/// �������ͣ�sequence; ������K���ظ���
const int H5SDK_TAG_CANDLE_GRP = 20050 ;

/// �������ͣ�bytevector; ����������TOKEN
const int H5SDK_TAG_HQ_TOKEN = 20051 ;

/// �������ͣ�sequence; �������������ظ���
const int H5SDK_TAG_FINANCE_GRP = 20052 ;

/// �������ͣ�bytevector; �������˳�ԭ��
const int H5SDK_TAG_QUIT_REASON = 20053 ;

/// �������ͣ�sequence; ��������
const int H5SDK_TAG_BATCH_GRP = 20055 ;

/// �������ͣ�rawdata; �������Ӱ�
const int H5SDK_TAG_SUB_PACKET = 20056 ;

/// �������ͣ�sequence; �������ֶμ��ظ���
const int H5SDK_TAG_FIELD_GRP = 20057 ;

/// �������ͣ�bytevector; �������ֶ�����
const int H5SDK_TAG_FIELD_NAME = 20058 ;

/// �������ͣ�bytevector; ������֤ȯ����
const int H5SDK_TAG_PROD_NAME = 55 ;

/// �������ͣ�uint32; ������ί�е���
const int H5SDK_TAG_ENTRUST_COUNT = 10067 ;

/// �������ͣ�uint32; ���������׷�����
const int H5SDK_TAG_TRADE_MINS = 20059 ;

/// �������ͣ�int32; ���������׽׶�
const int H5SDK_TAG_TRADE_SECTION = 20060 ;

/// �������ͣ�uint8(ö��); ������K��ģʽ
const int H5SDK_TAG_CANDLE_MODE = 20061 ;
    const uint8 H5SDK_ENUM_CANDLE_ORIGINAL = 0 ; // ԭʼK��
    const uint8 H5SDK_ENUM_CANDLE_FORWARD = 1 ; // ǰ��ȨK��
    const uint8 H5SDK_ENUM_CANDLE_BACKWARD = 2 ; // ��ȨK��

/// �������ͣ�uint32; ���������ֵ
const int H5SDK_TAG_MAX_VALUE = 20062 ;

/// �������ͣ�uint64; �������ܹɱ�
const int H5SDK_TAG_TOTAL_SHARES = 20063 ;

/// �������ͣ�int32; ��������ӯ��
const int H5SDK_TAG_PE_RATE = 20064 ;

/// �������ͣ�sequence; �������г��ظ���
const int H5SDK_FINANCE_MIC_GRP = 30065 ;

/// �������ͣ�uint32(ö��); ���������Ļ����˶�, ������˶�,����SubscriberKeyΪ��,��ʾ�����ӵ������Ѷ�����Ϣ��ȡ��
const int H5SDK_TAG_SUB_TYPE = 13 ;
    const uint32 HSUB_ENUM_SUB_OVER = 0 ; // ���Ƕ���(��ǰ�ͻ��˵Ķ���ģʽ)
    const uint32 HSUB_ENUM_SUB = 1 ; // ׷�Ӷ���
    const uint32 HSUB_ENUM_UNSUB = 2 ; // �˶�

/// �������ͣ�uint32; ��������ʼλ��
const int H5SDK_TAG_START_POS = 50002 ;

/// �������ͣ�bytevector; �������ֶ�����
const int H5SDK_TAG_SORT_FIELD_NAME = 20066 ;

/// �������ͣ�int32; ����������
const int H5SDK_TAG_VOL_RATIO = 34 ;

/// �������ͣ�int32; ���������
const int H5SDK_TAG_AMPLITUDE = 46 ;

/// �������ͣ�int64; �������ֲ���
const int H5SDK_TAG_AMOUNT = 95 ;

/// �������ͣ�int32; ������������
const int H5SDK_TAG_TURNOVER_RATIO = 97 ;

/// �������ͣ�int32; ������ί��
const int H5SDK_TAG_ENTRUST_RATE = 99 ;

/// �������ͣ�int64; ������ί��
const int H5SDK_TAG_ENTRUST_DIFF = 101 ;

/// �������ͣ�int8(ö��); ����������ʽ
const int H5SDK_TAG_SORT_TYPE = 102 ;
    const int8 HSUB_ENUM_SORT_ASC = 0 ; // ����
    const int8 HSUB_ENUM_SORT_DESC = 1 ; // ����

/// �������ͣ�bytevector; ������MIC_ABBR
const int H5SDK_TAG_MIC_ABBR = 103 ;

/// �������ͣ�uint32; ��������ʼ������
const int H5SDK_TAG_START_MIN = 104 ;

/// �������ͣ�uint32; ���������ܷ�����
const int H5SDK_TAG_END_MIN = 105 ;

/// �������ͣ�sequence; �������ֱ������ظ���
const int H5SDK_TAG_TICK_GRP = 106 ;

/// �������ͣ�uint32; �������ֱ����
const int H5SDK_TAG_BUSINESS_NO = 107 ;

/// �������ͣ�int32; �������ɽ�����
const int HSSDK_TAG_BUSINESS_DIRECTION = 109 ;

/// �������ͣ�int32; ������ÿ�ֹ���
const int HSSDK_TAG_SHARES_PER_HAND = 72 ;

/// �������ͣ�int32; �������۸񾫶�
const int HSSDK_TAG_PX_PRECISION = 71 ;

/// �������ͣ�int64; ��������ͨ�ɱ�
const int HSSDK_TAG_CIRCULATION_AMOUNT = 110 ;

/// �������ͣ�int64; ��������ֵ
const int HSSDK_TAG_MARKET_VALUE = 91 ;

/// �������ͣ�int64; ��������ͨ��ֵ
const int HSSDK_TAG_CIRCULATION_VALUE = 111 ;

/// �������ͣ�int32; ������ÿ�ɹ�����
const int HSSDK_TAG_EPS = 112 ;

/// �������ͣ�int32; ������ÿ�ɾ��ʲ�
const int HSSDK_TAG_BPS = 94 ;

/// �������ͣ�int32; �������о���
const int HSSDK_TAG_DYN_PB_RATE = 45 ;

/// �������ͣ�int32; ���������񼾶�
const int HSSDK_TAG_FIN_QUARTER = 113 ;

/// �������ͣ�int32; �����������������
const int HSSDK_TAG_FIN_END_DATE = 114 ;

/// �������ͣ�int8; �����������Ƿ��ȡ��ȫ
const int HSSDK_TAG_ALL_DATA_FLAG = 115 ;

/// �������ͣ�int32; ���������Ǽ���
const int HSSDK_TAG_RISE_COUNT = 124 ;

/// �������ͣ�int32; �������µ�����
const int HSSDK_TAG_FALL_COUNT = 125 ;

/// �������ͣ�int32; ��������Ա����
const int HSSDK_TAG_MEMBER_COUNT = 126 ;

/// �������ͣ�sequence; ���������ǹ�
const int H5SDK_TAG_RISE_FIRST_GRP = 1009 ;

/// �������ͣ�sequence; �����������
const int H5SDK_TAG_FALL_FIRST_GRP = 1010 ;

/// �������ͣ�string; ������ʱ����
const int H5SDK_TAG_TIMEZONE_CODE = 128 ;

/// �������ͣ�uint32; ����������ʱ��
const int H5SDK_TAG_MIN_TIME = 10014 ;

/// �������ͣ�uint32; �����������ֶ�ID
const int H5SDK_TAG_SORT_FIELD_ID = 20068 ;

/// �������ͣ�uint32; �������ֶ�id
const int H5SDK_TAG_FIELD_ID = 20069 ;

/// �������ͣ�int64; ��������ֲ֣��ڻ�����Ȩ��
const int HSSDK_TAG_PREV_AMOUNT = 122 ;

/// �������ͣ�int64; �����������֣��ڻ�����Ȩ��
const int HSSDK_TAG_AMOUNT_DELTA = 123 ;

/// �������ͣ�int32; ������IOPV
const int HSSDK_TAG_IOPV = 29 ;

/// �������ͣ�int32; ��������ծ����ֵ
const int HSSDK_TAG_DEBT_FUND_VALUE = 90 ;

/// �������ͣ�int32; �����������
const int HSSDK_TAG_PREV_SETTLEMENT = 28 ;

/// �������ͣ�int32; ������������
const int HSSDK_TAG_SETTLEMENT = 27 ;

/// �������ͣ�uint16; ��������������
const int H5SDK_TAG_SUB_KIND = 676 ;

/// �������ͣ�int32; ����������ֵ
const int H5SDK_TAG_MAINTAIN_RETURN_VALUE = 767 ;

/// �������ͣ�bytevector; ������Ӧ������
const int H5SDK_TAG_APP_NAME = 20072 ;

/// �������ͣ�bytevector; �������ֻ���
const int H5SDK_TAG_PHONE_NUMBER = 20073 ;

/// �������ͣ�bytevector; ������MAC��ַ����Ϊ�û�ʶ���־
const int H5SDK_TAG_MAC_ADDRESS = 20074 ;


#endif /* __H5_SDK_TAG_H__ */

