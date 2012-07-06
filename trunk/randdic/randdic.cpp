// randdic.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "a3a8.h"

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <set>
#include <algorithm>

using namespace std;
struct node
{
	unsigned char rand1;
	unsigned char rand2;
	unsigned char simout[12];

	bool operator==(const node &m) const
	{
		if (!memcmp(this->simout,m.simout,12))
			return true;
		return false;
	}

	bool operator<(const node &m) const
	{
		if (memcmp(this->simout,m.simout,12) < 0)
			return true;
		return false;
	}
};

void fun(node& n)
{
	delete &n;
}

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned char rand[16] = {0};
	unsigned char ki[16] = {0
	};


	cout << "format: Filename StartK1 EndK1" << endl;
	string filename;
	int start ,end;
	cin >> filename >> hex >> start >> end;

	ofstream fout(filename.c_str() ,ios_base::out|ios_base::app);
	//for (int m=1;m<8;++m)
	//{

	cout << hex;
	fout << hex;
	for (int k1=start ;k1<=end;++k1)
	{
		ki[0]=k1;
		for (int k2=0;k2<256;++k2)
		{
			ki[8]=k2;

			/*ki[0]=0x36;
			ki[8]=0x64;*/

			cout << setfill('0') << setw(2) << (int)ki[0] << setw(2) << (int)ki[8] <<endl;
			fout << setfill('0') << setw(2) << (int)ki[0] << setw(2) << (int)ki[8] <<endl;

			set<node> hashset;
			node n ;
			for (int r1=0;r1<256;++r1)
			{
				rand[0]=r1;
				for (int r2=0;r2<256;++r2)
				{
					rand[8]=r2;

					A3A8(rand,ki,n.simout);

					set<node>::iterator it = hashset.find(n);
					if (it!=hashset.end())
					{
						cout << setfill('0') << setw(2) << (int)it->rand1 << setw(2) << (int)it->rand2 << " ";
						cout << setfill('0') << setw(2) << r1			  << setw(2) << r2			   << endl;

						fout << setfill('0') << setw(2) << (int)it->rand1 << setw(2) << (int)it->rand2 << " ";
						fout << setfill('0') << setw(2) << r1			  << setw(2) << r2			   << endl;

						continue;
					}
					n.rand1 = r1;
					n.rand2 = r2;
					hashset.insert(n);
				}
			}
		}
		fout << flush;
	}

	fout.close();

	return 0;
}