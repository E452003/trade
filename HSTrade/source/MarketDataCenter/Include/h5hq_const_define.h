#ifndef _H5API_CONET_DEFINE_H__
#define _H5API_CONET_DEFINE_H__

#include "h5apistddef.h"

//	��Ʊ����״̬
const int TRADE_STOP	=0;		//	ͣ��
const int TRADE_RUNNING =1;		//	��������

/**
	Ӧ����ʹ�������Ķ���ֵʱ,����ñ��ļ�����ĺ�,��Ҫ���в�������
	������������ڼ����Ǹ��ĺ궨��ʱ,Ӧ�ó�����Ҫ��Ӧ�޸�
**/
////////////////////////	��������г������	//////////////////////////////
const int MKT_SH_L1	= 0x101;		//	�Ϻ�level1
const int MKT_SH_L2	= 0x201;		//	�Ϻ�level2
const int MKT_SZ_L1	= 0x102;		//	����level1
const int MKT_SZ_L2	= 0x202;		//	����level2
const int MKT_QH_CFFE =  0x303;		//	�н����ڻ�
const int MKT_QH_SHFE  = 0x304;		//	����
const int MKT_QH_DLCE  = 0x305;		//	������Ʒ������
const int MKT_QH_ZZCE  = 0x306;		//	֣����Ʒ������
const int MKT_QH_ZHHY  = 0x30c;		//	��Ϻ�Լ
const int MKT_NEEQ		= 0x10b;		//	�����壨��ת��
//20131126	caich	��Ӹ�����Ȩ
const int MKT_GGQQ  = 0xd01;		//	�Ϻ�������Ȩ
const int MKT_GGQQ_SZ  = 0xd02;		//	���ڸ�����Ȩ
const int MKT_QQ_CFFE	=0x1103;		//�н�����Ȩ����ָ��Ȩ��
const int MKT_QQ_SHFE	=0x1104	;	//������Ȩ
const int MKT_QQ_DLCE	=0x1105	;	//������Ʒ��������Ȩ
const int MKT_QQ_ZZCE	=0x1106	;	//֣����Ʒ��������Ȩ
//hk
const int MKT_HK_INDEX =0x501;
const int MKT_HK_MAIN =0x502;
const int MKT_HK_GEM =0x503;
const int MKT_HK_NASD =0x504;
const int MKT_HK_ETS =0x505;
//2014820	czh	��ӻ���ͨ
const int MKT_HGT = 0x506;			//����ͨ

////////////////////////	�����������Ԫ���ݱ�Ŷ���(��ʱδ��ȫ����)	Ԫ����������Ͷ��Ӯ��Э���ж�������ݽṹ
//	�������ʵʱ��Ϣ��k��(����Ŀǰֻ��Ҫ��ע����6����Ϣ����)
const int RESULT_MARKET_OVERVIEW=0;	//	�г���Ҫ
const int RESULT_MARKET_INIINFO=1;	//	�г���ʼ����Ϣ
const int RESULT_TREND=2;			//	������ʱ
const int RESULT_SIMPLE_QUOTE=3;	//	��������
const int RESULT_KLINE=4;			//	����K��
const int RESULT_BIDOFF=5;			//	������λ��Ϣ
const int RESULT_NEEQ_ZS_LEVEL=6;	//	��ת����Ͷ���ߵ�λ����
const int RESULT_NEEQ_ZS_ORDER=7;	//	��ת�����������걨����
const int RESULT_NEEQ_XY_ORDER=8;	//	��תЭ��ת���걨����
const int RESULT_MAX_COUNT=9;		//	���RESULT����

#endif
