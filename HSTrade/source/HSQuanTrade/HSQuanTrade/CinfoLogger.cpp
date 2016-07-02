#include "StdAfx.h"
#include "CinfoLogger.h"

using namespace std;


// ��ʵ����ָ̬�루��ʼΪ�գ�
CinfoLogger* CinfoLogger::_instance = NULL;


CinfoLogger::CinfoLogger(void)
{	
	// ������Ϣ
	m_strWriteStrInfo = "";

	// ��־�ļ�����
	m_strLogFileName ="strlog/"+GetCurTimeToString(YMD) + ".log";

	// ������־�߳�
	m_bRun = true;
	//m_hThread = boost::thread( boost::bind( &CinfoLogger::LogProcStart, this ));
	m_hThread = thread( bind( &CinfoLogger::LogProcStart, this ));
}



void  CinfoLogger::LogProcStart()
{
	int nCount = 1;
	clock_t preSaveTime = clock();
	do 
	{
		Sleep(250);

		if ( (clock() - preSaveTime) > TIME_TO_WRITE_LOG )
		{
			preSaveTime = clock();
			WriteLogToFile();	// ÿ������дһ����־
		}

	} while (m_bRun);

	if (!m_strWriteStrInfo.empty())
		WriteLogToFile();	// �˳�ѭ�����������ǿգ��򽫻���д����־ ���˳��߳�

	std::cout << LogInfo(LOG_LEVEL_SUB_1, "[- Info Logger] �����˳���־�̣߳�") << std::endl;
}



void CinfoLogger::WriteLog(std::string strLog)
{
	if (m_logMutex.try_lock())
	{
		if ( !strLog.empty() )
		{
			m_strWriteStrInfo += GetCurTimeToString(LOG)+"   " + strLog + ("\r\n");
		
			//m_strWriteStrInfo += GetCurTimeToString(LOG)+"   ";
			//m_strWriteStrInfo += strLog;
			//m_strWriteStrInfo += ("\r\n");

			//m_strWriteStrInfo += _T("\r\n");
		
		}
	m_logMutex.unlock();
	}
	if ( m_strWriteStrInfo.length() > MAX_STR_BUF_LEN )
	{			
		WriteLogToFile();	// �ﵽһ����ʱ��д�뵽�ļ���
	}
}



void CinfoLogger::WriteLogToFile()
{
	if (m_logMutex.try_lock())
	//m_logMutex.lock()
	//if (1)
	{
		try
		{
			// �� Append ��ʽ����־�ļ�
			ofstream osWriter( m_strLogFileName.c_str(),  ios::out | ios_base::app );

			if (osWriter.is_open())
			{
				// д����־����
				osWriter << m_strWriteStrInfo.c_str();

				// �����־����
				m_strWriteStrInfo = "";

				// �ر��ļ�д����
				osWriter.close();
			}
		}
		catch (std::exception e)
		{
			cout << LogInfo(LOG_LEVEL_FORMAT, "д���ļ�ʧ�ܣ�%s", e.what()) << endl;
		}

		//�ļ��Ƿ������Ҫ�������ļ�
		isNewFile();

		m_logMutex.unlock();
	}
}



CinfoLogger* CinfoLogger::instance()
{
	// ����ָ̬��Ϊ�գ��򴴽���ʵ��������ȡָ��
	if (_instance == NULL)
	{
		_instance = new CinfoLogger();
	}
	// ���ؾ�ָ̬��
	return _instance;
}



string CinfoLogger::LogInfo(int level,char *fmt, ...)
{
	// ��ʽ��������ַ��� fmt ������ַ��� out
	char out[1024];
	va_list body;
	va_start(body, fmt);
	vsprintf_s(out, fmt, body);    
	va_end(body);    

	// ����־��Ϣת��Ϊ string �ַ��������뻺�������
	string strTemp(out);	              

	//��־������Ҫ�����趨���� ���� С�� 0����ֻ�����ַ�����������¼
	if(level > CinfoLogger::instance()->LogLevel() || level < 0)
	{
		return strTemp;
	}

	// �����������ʾ��־��Ϣ
	OutputDebugStringA(out); 
	OutputDebugStringA("\n");

	// ����Ϣ������־������
	WriteLog( strTemp );

	return strTemp;
}




string CinfoLogger::LogInfo(int level, string info)
{
	//��־������ҪС�ڻ�����趨���𣬷��򲻼�¼
	if(level > CinfoLogger::instance()->LogLevel())
	{
		return info;
	}

	if (level >= 0)
	{
		// �����������ʾ��־��Ϣ
		OutputDebugStringA(info.c_str()); 
		// ����Ϣ������־������
		WriteLog( info );
	}

	return info;
}



 
string CinfoLogger::GetCurTimeToString(TIME_STRING_MODE mode /* = FOR_TEXT */)
{
	int strLen = 20;
	string retStr = "0";

	if (mode == FOR_TEXT)
	{
		char *pszCurrTime1 = (char*)malloc(sizeof(char)*strLen);
		if (pszCurrTime1)
		{
			memset(pszCurrTime1, 0, sizeof(char)*strLen);
			time_t now;
			time(&now);
			strftime(pszCurrTime1, strLen , "%Y-%m-%d %H:%M:%S", localtime(&now));
			//strftime(pszCurrTime1, strLen , "%Y-%m-%d %H:%M:%S", localtime_s(&now));
			retStr = pszCurrTime1;
			free(pszCurrTime1);
			pszCurrTime1 = NULL;
		}
		
		

		

		
	} 	
	else if (mode == FOR_FILE_NAME)
	{
		char *pszCurrTime2 = (char*)malloc(sizeof(char)*strLen);
		if (pszCurrTime2)
		{
			memset(pszCurrTime2, 0, sizeof(char)*strLen);
			time_t now;
			time(&now);
			strftime(pszCurrTime2, strLen , "%Y_%m_%d_%H_%M_%S", localtime(&now));

			retStr = pszCurrTime2;

			free(pszCurrTime2);
		}
		
	}
	else if(mode == YMD)
	{
		char *pszCurrTime3 = (char*)malloc(sizeof(char)*strLen);
		

		if (pszCurrTime3)
		{
			memset(pszCurrTime3, 0, sizeof(char)*strLen);
			time_t now;
			time(&now);
			strftime(pszCurrTime3, strLen , "%Y%m%d", localtime(&now));
			retStr = pszCurrTime3;

			free(pszCurrTime3);
			pszCurrTime3 = NULL;
		}

	}
	else if(mode == LOG)
	{
		//char *pszCurrTime4 = (char*)malloc(sizeof(char)*strLen);
		//memset(pszCurrTime4, 0, sizeof(char)*strLen);
		//time_t now;
		//time(&now);
		//strftime(pszCurrTime4, strLen , "%H:%M:%S", localtime(&now));

		//retStr = pszCurrTime4;

		SYSTEMTIME  time;
		GetLocalTime(&time);
		char* strTime =(char*)malloc(sizeof(char)*strLen);;
		if (strTime)
		{
			sprintf(strTime, "%02d:%02d:%02d:%03d", time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
			retStr =  strTime;
			free(strTime);
			strTime = NULL;
		}
		

	}
	else if(mode == NEWFILE)
	{
		char *pszCurrTime4 = (char*)malloc(sizeof(char)*strLen);
		if (pszCurrTime4)
		{
			memset(pszCurrTime4, 0, sizeof(char)*strLen);
			time_t now;
			time(&now);
			strftime(pszCurrTime4, strLen , "%Y%m%d%H%M%S", localtime(&now));

			retStr = pszCurrTime4;
			free(pszCurrTime4);
			pszCurrTime4 = NULL;
		}
	}

 	return retStr;	
}


int CinfoLogger:: LogLevel()
{
	return 3;

}

void CinfoLogger::isNewFile()
{

	  string sFileName= m_strLogFileName ;
	  ifstream in(sFileName.c_str());   
	  in.seekg(0,ios::end);   
	  long size = in.tellg();   
	  in.close();   
	  cout<< "�ļ���СΪ��"<<size<<" �ֽ�"<<endl; 

	  if(size > 10 * 1024 * 1024)
	  //if(size > 10 * 1024 )
	  {
		m_strLogFileName = "strlog/"+GetCurTimeToString(NEWFILE) + ".log";
	  }
  
}
