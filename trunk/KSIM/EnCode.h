#include <WinSCard.h>
#pragma once

class CEnCode
{
public:
	CEnCode(void);
	~CEnCode(void);
	static int DecodeUCS2( BYTE *P1, char* P2,int p3);
	static int Bytes2String( BYTE *P1,char* P2, int p3);
	static int StrReverse(char* p1);
	static int EncodeUCS2(char* p1, BYTE* p2, int p3);
	static int String2Bytes(char* p1, BYTE* P2, int p3);
	static int Decode7bit(BYTE* p1, char * p2,int p3);
};
