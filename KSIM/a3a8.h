#ifndef __A3A8_H__
#define __A3A8_H__

typedef unsigned char Byte;
//void A3A8(/* in */ const unsigned char rand[16], /* in */ const unsigned char key[16],
//		  /* out */ unsigned char simoutput[12]);

void A3A8(/* in */ Byte rand[16], /* in */ Byte key[16],
		  /* out */ Byte simoutput[12]);

#endif