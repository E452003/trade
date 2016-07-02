#ifndef _H5API_STD_DEF_H__
	#define _H5API_STD_DEF_H__
	#include <stdio.h>
	#ifdef _WIN32
		typedef char				int8;
		typedef short				int16;
		typedef int					int32;
		typedef __int64				int64;
		typedef unsigned char		uint8;
		typedef unsigned short		uint16;
		typedef unsigned int		uint32;
		typedef unsigned __int64	uint64;
	#else
		#include <sys/types.h>
		#if defined(__linux__)
			#include <stdint.h>
		#endif
		typedef int8_t				int8;
		typedef int16_t				int16;
		typedef int32_t				int32;
		typedef int64_t				int64;
		typedef uint8_t				uint8;
		typedef uint16_t			uint16;
		typedef uint32_t			uint32;
		typedef uint64_t			uint64;
	#endif


	#ifdef _WIN32
		#if !defined( FUNCTION_CALL_MODE )
			#define FUNCTION_CALL_MODE	__stdcall
		#endif
	#else
		#define FUNCTION_CALL_MODE
	#endif
		
	#ifndef HS_SID
		#define HS_SID	const char *
	#endif
	#ifndef IHS_KNOWN
		#define IHS_KNOWN
		struct IHSKnown
		{
			///�ӿڲ�ѯ
			/**��ѯ�뵱ǰ�ӿ���ص������ӿڣ�������Բ鵽 IIoC, IManager ��
			 *@param HS_SID  iid  �ӿ�ȫ��Ψһ��ʶ
			 *@param IKnown **ppv ����iid��Ӧ�Ľӿ�ָ��
			 *@return I_OK �ɹ���I_NONE δ�鵽iid ��Ӧ�ӿ�
			 */
			virtual unsigned long  FUNCTION_CALL_MODE QueryInterface( HS_SID iid, IHSKnown **ppv ) = 0;
			///���ýӿڣ����ü�����һ(���߳�����ʱ������ʵ�ִ�����Ҫ�Լ���ֵ�������޸�)
			virtual unsigned long  FUNCTION_CALL_MODE AddRef() = 0;
			///�ͷŽӿڣ����ü�����һ������Ϊ0ʱ�ͷŽӿڵ�ʵ�ֶ���(���߳�����ʱ������ʵ�ִ�����Ҫ�Լ���ֵ�������޸�)
			virtual unsigned long  FUNCTION_CALL_MODE Release() =  0;
		};
	#endif
#endif
