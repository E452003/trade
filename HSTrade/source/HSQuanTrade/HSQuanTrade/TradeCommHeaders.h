/**********************************************
 * Filename : TradeCommHeaders.h
 * Author	: Haowc10553
 * Date		: 2015.10.09
 *  
 * Desc		: ����ͨѶЭ��ͷ����
 **********************************************/
#ifndef _TRADE_COMM_HEADER_H_
#define _TRADE_COMM_HEADER_H_

#include <string>
#include <windows.h>

using namespace std;

////////////////////////////////////////////////////////
const char HEARTBEAT_PACKET[]	= "<root Version=\"1.0\" FuncID=\"999\"/>";
const char QUERY_FUND_ACCOUNT[] = "<root Version=\"1.0\" FuncID=\"1000\"/>";
const char ENTRUST_INFO[] = "<root Version=\"1.0\" FuncID=\"1004\" SessionID=\"1\">\
										<item Code=\"600570\" Market=\"Stock\" Account=\"10000013\" BS=\"Buy\" Dir=\"Open\" Amount=\"100\" Price=\"66.55\" Prop=\"0\"/>\
									</root>";
////////////////////////////////////////////////////////

#define MAX_BUF_LEN     (1024 * 100)

/**********************************************************
*				Э�鱨������ṹ��
***********************************************************
*      ����ͷ     +     Э��ͷ      +     Э�����ݰ�
* ��PacketHeader�� ��ProtocolHeader��    ��ProtocolData��
**********************************************************/

#pragma pack(1)
/***********************************************
 * ���ױ���ͷ
 ***********************************************/
struct PackerHeader 
{
	short	HeaderFlag;		// ����2����ͷ��־��ָ��Ϊ7611
	int		DataLength;		// ����4��Э������ȣ�����������ͷ��
};

/***********************************************
 * ����Э��ͷ
 ***********************************************/
struct ProtocolHeader
{
	int		Action;			// ����4�����ܺţ�ָ��Ϊ8888
	int		Data1;			// ����4������
	int		Data2;			// ����4������
	int		Data3;			// ����4������
	int		Data4;			// ����4������
	int		DataLength;		// ����4��Э������ݳ���
	int		Data5;			// ����4������
};
#pragma pack()

#endif
