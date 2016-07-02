#ifndef _DBF_H_
#define _DBF_H_

#include "public.h"

//DBF�ļ����ݶ�ȡ��
///////////////////////////////////////////////////////////////////////////////////////
//DBF�ļ��������ʽ
//	a.���ݼ�¼����,�����ֶνṹ��������,��ASCII�뷽ʽ˳����,������־Ϊ1AH(���ļ�������־)
//		ÿ����¼�����ֽ�Ϊɾ����־,20H��ʾ��Ч,2AH��ʾ�ѱ�ɾ��.
//		�ֶ�ֵ���޷ָ���,��¼���޽������.

//	b.�ֶνṹ��������,λ���ļ��ײ�,�Զ����Ʒ�ʽ���,������־Ϊ0DH.
//		����Ϊ32*(���ݿ���ֶθ���+1(������������))+1(0DH������־)


//�ļ�ͷ����=�ļ�����ͷ����+�ֶ��������ȣ�32��*�ֶθ���

//DBF �ļ�ͷ�ṹ//��������������,��32���ֽ�
typedef struct _DBFFILEHEADER
{
	unsigned char	dfMark;						//0x03h �� 0x80h(��ͷ�ֽڣ���MEMO�ֶ�)		0
	unsigned char	dfYear,dfMmonth,dfDay;		//����Ϊ������,������						1-3
	unsigned long	dfRecordCount;				//�ܼ�¼����,��λ�ֽ���ǰ					4-7
	unsigned short	dfHeaderLength;				//�ļ�ͷ����=��9�ֽ�ֵ*256+��8�ֽ�ֵ		8-9
	unsigned short	dfRecordLength;				//��¼����=��11�ֽ�ֵ*256+��10�ֽ�ֵ		10-11
	unsigned char	dfReserved[20];				//��������ȥ��ϸ�ڵ�������					12-31
}DBFFILEHEADER;

//DBF �ֶνṹ//���ֶ�������,32���ֽ�/�ֶ�
typedef struct _DBFINFOHEADER
{
	unsigned char	diFieldName[10];			//�ֶ�����,ASCII��			0-9
	unsigned char	diReserved1;				//�����ֽڣ�0��				10
	unsigned char	diFieldType;				//�ֶ�����,CDNL��ASCII��	11
	unsigned long	diOffset;					//���ֶ����׼�¼�е�λ��	12-15
	unsigned char	diFieldLength;				//�ֶγ���<=256				16
	unsigned char	diDecimal;					//С�����λ��				17
	unsigned char	diReserved[14];				//��������ȥ��ϸ�ڵ�������  18-31
}DBFINFOHEADER;

typedef DBFFILEHEADER *		LPDBFFILEHEADER;
typedef DBFINFOHEADER *		LPDBFINFOHEADER;

/*
field identifier table(�ֶα�)
ascii description
c character
d date, format yyyymmdd
f floating point
g general - foxpro addition
l logical, t:t,f:f,y:y,n:n,?-not initialized
m memo (stored as 10 digits representing the dbt block number)
n numeric
p picture - foxpro addition
*/

class CDBF
{
public:
	CDBF();
	~CDBF();
	bool            InitDbf(const char* filename);				//���FILEHEADER��INFOHEADER
	int				GetFieldCount();
	int				GetFieldLength(int FieldNum);
	int				GetDecimal(int FieldNum);
	unsigned char * GetFieldName(int FieldNum);
	unsigned char	GetFieldType(int FieldNum);
	long			GetRecordCount();
	int				GetRecordLength();
	int				GetRecord(int RecordNum,unsigned char *pBuf);
	unsigned long	GetFieldColumSize(int fieldnum);

private:
	FILE*           fp;
	unsigned char * dbfBuffer;		
	DBFFILEHEADER	dbfFileHeader;		//�ļ�����ṹ
	DBFINFOHEADER	dbfInfoHeader;		//���ֶ�����
		
protected:

};


#endif //_DBF_H_
