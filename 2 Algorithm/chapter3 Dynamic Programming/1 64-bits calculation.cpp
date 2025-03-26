#include <iostream>
#include <climits>

using namespace std;

void split(unsigned int num, unsigned int& low, unsigned int& high)
{
	static const int EX_L = UINT_MAX >> (sizeof(int) * 4);
	static const int EX_H = UINT_MAX << (sizeof(int) * 4);

	low = num & EX_L;
	high = num & EX_H;
	// Move data from the high bits to the low bits.
	high = high >> (sizeof(high) * 4);
}

void MUL(unsigned int a, unsigned int b)
{
	// 0000 0000 FFFE 0001
	// 0000 FFFE 0001 0000
	// 0000 FFFE 0001 0000
	// FFFE 0001 0000 0000

	// FFFF FFFE 0000 0001 

	unsigned int al, ah, bl, bh;
	split(a, al, ah), split(b, bl, bh);

	unsigned int r1, r21,r22, r2l, r2h, r3;
	r1 = al * bl;
	r21 = al * bh;
	r22 = ah * bl;
	r3 = ah * bh;

	cout << hex << "\t\t" << r1 << "\n\t" << r21 << "\n\t" << r22 << "\n" << r3 << endl;

	split(r21, r2l, r2h);
	r3 += r2h;
	// Move data from the low bits to the high bits to align two numbers.
	// Circulation utilized so >> is applied here instead of <<.
	r1 += r2l >> (sizeof(r2l)*4);
	if (r1 < r2l)
		r3 += 1;

	split(r22, r2l, r2h);
	r3 += r2h;
	r1 += r2l >> (sizeof(r2l)*4);
	if (r1 < r2l)
		r3 += 1;

	cout << hex << r3 << r1 << endl;
}

int main()
{
	MUL(UINT_MAX, UINT_MAX);
}
