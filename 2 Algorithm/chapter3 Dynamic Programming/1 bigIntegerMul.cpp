#include <iostream>
#include <climits>
using namespace std;

/*
 * MUL double the length of input
 * ADDC and SUBB give output that has the same length as their input
 */

short* DOWN(int* a, int len)
{
	// MUL is finished within int array to avoid overflow,
	// but the result needs to be transformed into short array to call function MUL
	// and avoid overflow caused by bit accumulation.
	
	short* result = new short[len];
	result[len-1] = 0;
	for (int i = len-1; i > 0; i--)
	{
		result[i] += a[i] % 256;
		result[i-1] = a[i] / 256;
	}

	// overflow is stored in result[0]; 
	// if MUL call DOWN to return value, the overflow should not be accumulated.
	// It should be moved to another short array.
	
	result[0] += a[0];
	return result;
}

short* ADDC(short* a, short* b, int len)
{
	// calculate (w+x) and (y+z)
	
	int* RESULT = new int[len];
	for (int i = len-1; i >= 0; i--)
	{
		RESULT[i] = a[i] + b[i];
	}

	short* result = DOWN(RESULT, len);
	delete[] RESULT;
	return result;
}

int* ADDC(short* a, short* b, short* c, int len)
{
	// calculate (a<<n) + [b<<(n/2)] + c
	
	const int SPLIT = len/2;
	int* result = new int[SPLIT*4];
	for (int i = 0; i < SPLIT; i++)
	{
		result[i] = a[i];
		result[i+SPLIT] = a[i+SPLIT] + b[i];
		result[i+SPLIT*2] = b[i+SPLIT] + c[i];
		result[i+SPLIT*3] = c[i+SPLIT];
	}
	return result;
}

short* SUBB(short* a, short* b, short* c, int len)
{
	// calculate (w+x)(y+z) - wy - xz
	
	short* result = new short[len];
	result[len-1] = 0;
	for (int i = len-1; i > 0; i--)
	{
		result[i] += a[i] - b[i] - c[i];
		result[i-1] = 0;
		if (result[i] < 0 && i!=0)
		{
			result[i] += 256;
			result[i-1] -= 1;
		}
	}
	result[0] += a[0] - b[0] - c[0];
	return result;
}

short* MUL(short* a, short* b, int len)
{
	const int size = len*2;
	int* RESULT;

	if (len == 1)
	{
		// the end of recursion
		
		RESULT = new int[size];
		RESULT[size-1] = 0;
		RESULT[size-1] += a[len-1] * b[len-1];
		RESULT[size-2] = 0;
	}

	else
	{
		// uv = (w*2^(n/2)+x) + (y*2^(n/2)+z)
		//    = wy*2^n + [(w+x)(y+z)-wy-xz]*2^(n/2) + xz
		
		short *x, *y1, *y2, *z;
		x = MUL(a, b, len/2);			// wy
		z = MUL(a+(len/2),b+(len/2), len/2);	// xz

		short *sum1, *sum2;
		sum1 = ADDC(a, a+(len/2), len/2);	// w+x
		sum2 = ADDC(b, b+(len/2), len/2);	// y+z
		y1 = MUL(sum1, sum2, len/2);		// (w+x)(y+z)
		y2 = SUBB(y1, x, z, len);

		RESULT = ADDC(x, y2, z, len);

		delete[] x;
		delete[] y1;
		delete[] y2;
		delete[] z;
		delete[] sum1;
		delete[] sum2;
	}

	short* result = DOWN(RESULT, len*2);
	delete[] RESULT;
	return result;
}


int main()
{
	const int LEN = 16;
	short a[LEN] = {0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
	short b[LEN] = {0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0};
	short* result = MUL(a, b, LEN);
	for (int i = LEN; i > 0; i--)
	{
		if (result[i] < 0)
		{
			result[i] += 256;
			result[i-1] -= 1;
		}
		result[i-1] += result[i] / 256;
		result[i] = result[i] % 256;
	}
	for (int i = 0; i < LEN * 2; i++)
		cout << result[i] << ' ';
	cout << endl;
	return 0;
}
