#ifndef CPCSC_H_CAESAR__DEF
#define CPCSC_H_CAESAR__DEF
#include <winscard.h>
#pragma comment(lib,"winscard")
/********************************************************************
Copyright (C) 1999 - 2005, CaesarZou
This software is licensed as described in the file COPYING, which
you should have received as part of this distribution. 
You may opt to use, copy, modify, merge, publish, distribute and/or sell
copies of the Software, and permit persons to whom the Software is
furnished to do so, under the terms of the COPYING file.
This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
KIND, either express or implied.

  file base:	PCSC
  file ext:	h
  author:	邹德强
  email:	zoudeqiang1979@tsinghua.org.cn
			name_caesar@msn.com
  
  purpose:	实现了部分GSM在PCSC下的应用。
  modify his:	
	
*********************************************************************/

class CPCSC  
{
public:
	CPCSC();
	virtual ~CPCSC();

	//CHV1 status
	//UNWAKE				- Unkown status(init)
	//STATUS_DISABLE		- PIN Disable
	//STATUS_ENABLE			- PIN Enable
	//STATUS_VOK			- PIN Enable, but be verified, user should update this value after VERIFY_CHV success.
	//STATUS_BLOCK			- PIN Block, should use PUK unblock
	//STATUS_DEADLOCK		- PUK Block, never unblock.
	enum		PIN_STATUS {STATUS_UNWAKE,STATUS_DISABLE,STATUS_ENABLE,STATUS_VOK,STATUS_BLOCK,STATUS_DEADLOCK};
	//file type
	enum		FILE_TYPE	 {TYPE_RFU=0x00, TYPE_MF=0x01, TYPE_DF=0x02, TYPE_EF=0x04};
	//file struct
	enum		FILE_STRUCT{EF_TRANSFORT=0x00, EF_LINEAR=0x01, EF_CYCLIC=0x03};
	
	/*******************************************************************
	函 数 名 称：	Init(const char* szReader)
	功 能 描 述：	初使化PC/SC环境
	参 数 说 明：	szReader不为NULL则指定初使化读卡器(用以监控卡片状态)，否则不具体读卡器，由连接指定
	返回值 说明：	bool 是否初使化成功
	作       者:	邹德强
	更 新 日 期：	2005.11.29
	*******************************************************************/
	bool Init(const char* szReader=NULL);
	
	
	/*******************************************************************
	函 数 名 称：	Eject()
	功 能 描 述：	释放PC/SC环境
	参 数 说 明：	
	返回值 说明：	
	作       者:	邹德强
	更 新 日 期：	2005.11.29
	*******************************************************************/
	void Eject();
	

	/*******************************************************************
	函 数 名 称：	ListReaders()
	功 能 描 述：	列出全部支持的读卡器名
	参 数 说 明：	IN & OUT, 需要输入足够长度的buffer
	返回值 说明：	0--失败，否则为readers串的长度。readers为多串，A\0B\0C\0\0
					处理方式为：
					char * pReader = pReaders;
					
					while('\0' != *pReader) 
					{
					  //do something to pReader
						pReader = pReader + strlen(pReader) + 1;
					}
	作       者:	邹德强
	更 新 日 期：	2005.11.29
	*******************************************************************/
	int ListReaders(char* readers);


	/*******************************************************************
	函 数 名 称：	GetAtr(char* szATR);
	功 能 描 述：	获得ATR
	参 数 说 明：	unsinged char* usATR--ATR Buffer
	返回值 说明：	0为失败，否则为ATR长度
	作       者:	邹德强
	更 新 日 期：	2005.11.29
	*******************************************************************/
	int GetAtr(unsigned char* usATR);

	
	/*******************************************************************
	函 数 名 称：	ConnectCard(const char*	sReader)
	功 能 描 述：	连接到具体的读卡器上
	参 数 说 明：	sReader 读卡器名字(NULL，使用init()的读卡器名字)
	返回值 说明：	bool 是否连接成功
	作       者:	邹德强
	更 新 日 期：	2005.11.29
	*******************************************************************/
	bool	ConnectCard(const char*	sReader=NULL, int nShareMode=SCARD_SHARE_SHARED);
	bool	ResetCard(const char* sReader=NULL,int nShareMode=SCARD_SHARE_SHARED);

	/*******************************************************************
	函 数 名 称：	DisConnectCard()
	功 能 描 述：	断开当前的读卡器连接
	参 数 说 明：	
	返回值 说明：	
	作       者:	邹德强
	更 新 日 期：	2005.11.29
	*******************************************************************/
	void	DisConnectCard();
	
	
	/*******************************************************************
	函 数 名 称：	GetReaderStatusChange(int milseconds)
	功 能 描 述：	获得当前读卡器的状态字(需要在Init的时候指定读卡器)
	参 数 说 明：	milseconds 超时毫秒
	返回值 说明：	long 状态数值
	作       者:	邹德强
	更 新 日 期：	2005.11.29
	*******************************************************************/
	long GetReaderStatusChange(int milseconds);
	void SetReaderStatusChange() {m_rgscState.dwCurrentState ++;}


	/*******************************************************************
	函 数 名 称：	IsConnc()
	功 能 描 述：	判断读卡器是否连接
	参 数 说 明：	
	返回值 说明：	bool 连接状态
	作       者:	邹德强
	更 新 日 期：	2005.11.29
	*******************************************************************/
	bool IsConnc();


	/*******************************************************************
	函 数 名 称：	RunAPDU(IN unsigned char* ucCmd, IN DWORD nCmdLen, OUT unsigned char* ucRes, IN OUT DWORD& nResLen)
	功 能 描 述：	下发APDU
	参 数 说 明：	ucCmd Apdu指令Buffer,nCmdLen Apdu指令长度,ucRes 返回指令缓冲,nResLen 返回指令缓冲大小(IN)/返回指令的长度(OUT)
	返回值 说明：	bool 是否下发成功
	作       者:	邹德强
	更 新 日 期：	2005.11.29
	*******************************************************************/
	bool	RunAPDU(IN unsigned char* ucCmd, IN DWORD nCmdLen, OUT unsigned char* ucRes, IN OUT DWORD& nResLen);



	//////////////////////////////////////////////////////////////////////////
	/*GSM 11.11功能实现*/
	bool	SELECT(IN WORD wFileID);
	bool	STATUS();
	bool	SEEK(IN BYTE* Patten, IN BYTE byPattenLen);
	bool	GET_RESPONSE(OUT BYTE* ucResp, IN BYTE byResLen);

	bool	READ_BINARY(OUT BYTE* ucFile, IN BYTE byLen, IN WORD wOffset);
	bool	UPDATE_BINARY(IN BYTE* ucFile, IN BYTE byLen, IN WORD wOffset);
	bool	READ_RECORD(IN BYTE byRecNo, OUT BYTE* ucData, IN BYTE byRecLen);	//采用0x04绝对模式，RecNo从0开始
	bool	UPDATE_RECORD(IN BYTE byRecNo,IN BYTE* ucData, IN BYTE byRecLen);	//采用0x04绝对模式，RecNo从0开始

	bool	VERIFY_CHV(IN BYTE byChvNo, IN const BYTE* ucChv, IN BYTE byChvLen);
	bool	CHANGE_CHV(IN BYTE byChvNo, IN const BYTE* ucOldChv, IN const BYTE* ucChv, IN BYTE byOldChvLen, IN BYTE byChvLen);
	bool	UNBLOCK_CHV(IN BYTE byChvNo, IN const BYTE* ucPuk, IN const BYTE* ucChv, IN BYTE byPukLen, IN BYTE byChvLen);
	
	//Only for CHV1
	bool	DISABLE_CHV(IN const BYTE* ucChv,IN BYTE byChvLen);	
	bool	ENABLE_CHV(IN const BYTE* ucChv,IN BYTE byChvLen);

	//////////////////////////////////////////////////////////////////////////
	/*PIM 规范指令扩展*/
	bool	RUN_PIM_ALGORITHM(const BYTE* sRand);


	//////////////////////////////////////////////////////////////////////////
	/*SMS协议解析函数*/
	/*******************************************************************
	函 数 名 称：	ReadSMS,WriteSMS
	功 能 描 述：	从SIM卡文件协议中解析短消息信息/从短消息信息组织SIM卡文件
	参 数 说 明：	ucADN SIM卡文件缓冲区,szPhone 电话,szMsg消息, time 时间
					当读操作的时候szPhone,szMsg的buffer应该由调用方给出。
	返回值 说明：	状态值：0为空记录/1已读，3未读，5已发，7待发
	作       者:	邹德强
	更 新 日 期：	2005.11.29
	*******************************************************************/
	static int  ReadSMS_SIM(BYTE *ucADN,char* szPhone,char* szMsg,SYSTEMTIME& time);
	static int  ReadSMS_PIM(BYTE *ucADN,char* szPhone,char* szMsg,SYSTEMTIME& time);	
	static int  ReadSMS_UIM(BYTE *ucADN,char* szPhone,char* szMsg,SYSTEMTIME& time);
	static bool WriteSMS_PIM(BYTE* ucADN,int reclen, int type, char* szPhone,char* szMsg,SYSTEMTIME& time);

	
	//////////////////////////////////////////////////////////////////////////
	/*ADN协议解析函数*/
	/*******************************************************************
	函 数 名 称：	ReadADN,WriteADN
	功 能 描 述：	从SIM卡文件协议中解析电话本信息/从电话本信息组织SIM卡文件
	参 数 说 明：	ucADN SIM卡文件缓冲区,ADNFileLen = nXLen+14(见GSM 11.11),szMobile 电话,szName消息
			当读操作的时候szMobile,szName的buffer应该由调用方给出。
	返回值 说明：	是否成功
	作       者:	邹德强
	更 新 日 期：	2005.11.29
	*******************************************************************/	
	static bool ReadADN(BYTE* ucADN,int nXLen,char* szMobile,char* szName);
	static bool WriteADN(BYTE* ucADN,int nXLen,char* szMobile,char* szName);


	/*Response数据解析函数*/
	static FILE_TYPE RS_GetFileType(const BYTE *usResponse);
	//Only for response of EF
	static WORD RS_GetFileSize(const BYTE *usResponse);
	static BYTE RS_GetRecordLen(const BYTE *usResponse);
	static BYTE RS_GetRecordNum(const BYTE *usResponse);
	static FILE_STRUCT RS_GetFileStruct(const BYTE *usResponse);
	//Only for response of DF
	static PIN_STATUS RS_GetPinStatus(const BYTE*usResponse);

public:
	BYTE			m_bySW1;				//SW1
	BYTE			m_bySW2;				//SW2
	PIN_STATUS			m_nPINStatus;		//卡片PIN1状态，用户自己控制状态，初使化为UNWAKE

protected:
	SCARDCONTEXT		m_hContext;			//卡设备上下文
	SCARDHANDLE			m_hCard;			//读卡器句柄
	//
	SCARD_READERSTATE	m_rgscState;		//读卡器状态值
	SCARD_IO_REQUEST	m_ioRequest;		//活动协议
	
	CHAR				m_szReader[1024];	//读卡器名字
	CRITICAL_SECTION	m_csReader;			//操作互斥体

	bool			SW_GSM();				
	
	void UNLOCK()
	{
		::LeaveCriticalSection(&m_csReader);
	}
	void LOCK()
	{
		::EnterCriticalSection(&m_csReader);
	}
	
public:
	bool RUN_GSM_ALGORITHM(const BYTE * sRand);
};

#endif



















