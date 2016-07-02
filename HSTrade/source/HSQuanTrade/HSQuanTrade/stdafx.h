#pragma once

// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��
#define _ATL_NO_AUTOMATIC_NAMESPACE             // ���������Ƴ�ͻ

#include <afxwin.h> 
#include <afxctl.h>         // ActiveX �ؼ��� MFC ֧��
#include <afxext.h>         // MFC ��չ

#include <afxdisp.h>        // MFC �Զ�����

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

// �����ϣ��ʹ�� MFC ���ݿ��࣬
//  ��ɾ����������������ļ�
#ifndef _WIN64

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT



#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#ifdef _DEBUG
//#include "vld.h"
#endif

#define WM_MESSAGE_TRADE WM_USER + 1000
#define WM_MESSAGE_PRICE WM_USER + 1001

#define WM_MESSAGE_ENTRUST WM_USER + 1002

#define WM_MESSAGE_SOCK WM_USER + 1003

//#define WM_MODEL_RESULT_MSG    WM_USER+1004 /*has been moved to TradeStatistic2.h.*/

#define WM_MESSAGE_BACKTEST_STEP WM_USER + 1005


#define	LOG_LEVEL_FORMAT	-1
#define	LOG_LEVEL_MAIN		0
#define	LOG_LEVEL_SUB_1		1
#define	LOG_LEVEL_SUB_2		2
#define	LOG_LEVEL_SUB_3		3



#endif // _WIN64





