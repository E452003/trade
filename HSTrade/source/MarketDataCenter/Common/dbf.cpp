
/******************************************************************************/
/*������CDBF
/*���ã��ṩ����dbf���ݿ��ļ���һЩ�򵥺��� 
/******************************************************************************/

#include "dbf.h"

//���캯��
CDBF::CDBF()
{
	dbfBuffer = new unsigned char[4 * 1024 * 1024];
}

CDBF::~CDBF()
{
	if(dbfBuffer) delete[] dbfBuffer;
	dbfBuffer = NULL;
}

//��ʼ�����ݿ�
bool CDBF::InitDbf(const char* filename)
{
	fp = fopen(filename, "rb");
	char errMsg[1024];
	if(!fp)
	{
		sprintf(errMsg, "���ļ�[%s]ʧ��!", filename);
		return false;
	}

	int len = fread(dbfBuffer, 1, 4 * 1024 * 1024, fp);  
	dbfBuffer[len] = '\0';
	memcpy((void*)&dbfFileHeader, dbfBuffer, sizeof(DBFFILEHEADER));
	fclose(fp);
	return true;
}
//��ȡ�ֶθ���
int CDBF::GetFieldCount()
{
	return(dbfFileHeader.dfHeaderLength / 32 - 1);
}
//��ȡ��һ�ֶ���
unsigned char * CDBF::GetFieldName(int FieldNum)
{
	if (FieldNum < 1) return NULL;
	unsigned char * p = &(dbfBuffer[FieldNum * 32]);
	//		fp.Seek(FieldNum * 32,SEEK_SET);
	
	memcpy(&dbfInfoHeader, p, sizeof(dbfInfoHeader));
	
	//		fp.Read((void*)&dbfInfoHeader,sizeof(DBFINFOHEADER));
	return(dbfInfoHeader.diFieldName);
}

//��ȡ��һ�ֶγ���
int CDBF::GetFieldLength(int FieldNum)
{
	if (FieldNum < 1) return -1;
	unsigned char * p = &(dbfBuffer[FieldNum * 32]);
	//		fp.Seek(FieldNum * 32,SEEK_SET);
	memcpy(&dbfInfoHeader, p, sizeof(dbfInfoHeader));
	//		fp.Read((void*)&dbfInfoHeader,sizeof(DBFINFOHEADER));
	return(dbfInfoHeader.diFieldLength);
}

//��ȡ��һ�ֶ�����
unsigned char CDBF::GetFieldType(int FieldNum)
{
	if (FieldNum < 1) return '#';
	unsigned char * p = &(dbfBuffer[FieldNum * 32]);
	//		fp.Seek(FieldNum * 32,SEEK_SET);
	memcpy(&dbfInfoHeader, p, sizeof(dbfInfoHeader));
	//		fp.Read((void*)&dbfInfoHeader,sizeof(DBFINFOHEADER));
	return(dbfInfoHeader.diFieldType);
}
//��ȡ��һ��ֵ���ֶ�С����λ��
int CDBF::GetDecimal(int FieldNum)
{
	if (FieldNum < 1) return -1;
	unsigned char * p = &(dbfBuffer[FieldNum * 32]);
	//		fp.Seek(FieldNum * 32,SEEK_SET);
	memcpy(&dbfInfoHeader, p, sizeof(dbfInfoHeader));
	//		fp.Read((void*)&dbfInfoHeader,sizeof(DBFINFOHEADER));
	return(dbfInfoHeader.diDecimal);
}
//��ȡ��¼����
long CDBF::GetRecordCount()
{
	return(dbfFileHeader.dfRecordCount);
}
//��ȡ��¼����
int CDBF::GetRecordLength()
{
	return(dbfFileHeader.dfRecordLength);
}

//��ȡ��һ��¼
int CDBF::GetRecord(int RecordNum,unsigned char *pBuf)
{
	if (RecordNum < 1) return 0;
	
	//ͷ����+ͷ������־����+(RecordNum-1) *(��¼����+��¼ɾ�����ű�־����[1])
	long lPos=dbfFileHeader.dfHeaderLength + 1 + (RecordNum-1) * (dbfFileHeader.dfRecordLength);
	unsigned char * p = &(dbfBuffer[lPos]);
	//		fp.Seek(lPos,SEEK_SET);
	memcpy(pBuf, p, dbfFileHeader.dfRecordLength);
	//		fp.Read((void*)pBuf,GetRecordLength());
	
	return 1;
}

unsigned long	CDBF::GetFieldColumSize(int fieldnum)
{
	if (fieldnum < 1) return 0;
	return GetFieldLength(fieldnum) * GetRecordCount();
}

