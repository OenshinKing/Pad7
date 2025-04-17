#include <iostream>
#include <climits>

using namespace std;

int count_add(int a, int b)
{
	static int count = 0;
	if ((!a) || (!b))
		return 0;
	int aBits = 1, bBits = 1;
	a = a & INT_MAX, b = b & INT_MAX;
	while((a = a>>1)&INT_MIN)
		aBits++;
	while((b = b>>1)&INT_MIN)
		bBits++;
	return (count += a > b ? a : b);
}

int count_mul(int a, int b)
{
	
}

void split(unsigned int num, unsigned int &low, unsigned int &high)
{
	static const int EX_L = UINT_MAX >> (sizeof(int) * 4);
	static const int EX_H = UINT_MAX << (sizeof(int) * 4);

	low = num & EX_L;
	high = num & EX_H;
	// Move data from the high bits to the low bits.
	high = high >> (sizeof(high) * 4);
}

void MUL_BASE(unsigned int a, unsigned int b, unsigned int &low, unsigned int &high)
{
	// 0000 0000 FFFE 0001
	// 0000 FFFE 0001 0000
	// 0000 FFFE 0001 0000
	// FFFE 0001 0000 0000

	// FFFF FFFE 0000 0001

	unsigned int al, ah, bl, bh;
	split(a, al, ah), split(b, bl, bh);

	unsigned int r21, r22, r2l, r2h;
	low = al * bl;
	r21 = al * bh;
	r22 = ah * bl;
	high = ah * bh;

	cout << hex << "\t\t" << low << "\n\t" << r21 << "\n\t" << r22 << "\n"
		 << high << endl;

	split(r21, r2l, r2h);
	high += r2h;
	// Move data from the low bits to the high bits to align two numbers.
	// Circulation utilized so >> is applied here instead of <<.
	low += r2l >> (sizeof(r2l) * 4);
	if (low < r2l)
		high += 1;

	split(r22, r2l, r2h);
	high += r2h;
	low += r2l >> (sizeof(r2l) * 4);
	if (low < r2l)
		high += 1;

	cout << hex << high << low << endl;
}

void MUL(unsigned int *a, int len_a, unsigned int *b, int len_b)
{
	int result_size = len_a > len_b ? (len_a * 2) : (len_b * 2);
	unsigned int result[result_size];
}

int main()
{
	unsigned int a[2] = {UINT_MAX, UINT_MAX};
	unsigned int b[2] = {UINT_MAX, UINT_MAX};
	MUL(a, 2, b, 2);
	char c = 0xff;
	char d = 0xff;
	int e = int(c) * int(d);
	cout << hex << e << endl;
}
