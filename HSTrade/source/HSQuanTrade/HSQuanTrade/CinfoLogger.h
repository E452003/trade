#ifndef _INFO_LOGGER_H_
#define _INFO_LOGGER_H_

#define		MAX_STR_BUF_LEN		30000	//��־������󳤶ȣ���������־����д�뵽�����ļ�
#define		TIME_TO_WRITE_LOG	5000	//��ʱʱ�䣨���룩����������־����д�뵽�����ļ�

//using namespace std;
#pragma once

#include <stdio.h>
#include <tchar.h>

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include<string>
#include<wchar.h>
#include<thread>
#include<mutex>
#include <fstream>

#define	LOG_LEVEL_FORMAT	-1
#define	LOG_LEVEL_MAIN		0
#define	LOG_LEVEL_SUB_1		1
#define	LOG_LEVEL_SUB_2		2
#define	LOG_LEVEL_SUB_3		3


//-------------------------------------------------------------------------
// Class
//-------------------------------------------------------------------------
class CinfoLogger
{
public:

	static CinfoLogger* instance();

	std::string LogInfo(int level, char *fmt, ...);

	std::string LogInfo(int level, std::string info);
	
	void EnterFunction(std::string funcName)
	{
		LogInfo(1, "Enter function: " + funcName);
	}

	//����1
	void EnterFunction(const char* funcName)
	{
		LogInfo(1, "Enter function: %s", funcName);
	}

	void LeaveFunction(std::string funcName)
	{
		LogInfo(1, "Leave function: " + funcName);
	}

	//����1
	void LeaveFunction(const char* funcName)
	{
		LogInfo(1, "Leave function: %s", funcName);
	}

	enum TIME_STRING_MODE { 
		FOR_FILE_NAME,	// �� �ļ�����ʽ ��ȡʱ���ַ���
		FOR_TEXT,		// �� �ı���ʽ ��ȡʱ���ַ���
		YMD,
		LOG,
		NEWFILE
	};

	std::string GetCurTimeToString(TIME_STRING_MODE mode = FOR_TEXT);


	void SetExit() { m_bRun = false; }

private:

	//-------------------------------------------------------------------------
	// �ڲ�����
	//-------------------------------------------------------------------------
	static CinfoLogger* _instance;	

	std::thread m_hThread;
	std::mutex m_logMutex;
	bool m_bRun;					// �߳�ѭ��ʹ�ܱ�־

	std::string m_strWriteStrInfo;		// ��־�ļ������ַ���
	std::string m_strLogFileName;		// ��־�ļ�����ÿ������ʱ����ϵͳʱ���Զ�����

	//-------------------------------------------------------------------------
	// �ڲ�����
	//-------------------------------------------------------------------------
	

	CinfoLogger(void);

	void WriteLog(std::string strLog);

	void LogProcStart();

	void WriteLogToFile();

	int LogLevel();

	void isNewFile();

};

#endif