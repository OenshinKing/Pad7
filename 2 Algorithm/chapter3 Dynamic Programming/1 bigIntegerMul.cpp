#include <iostream>
#include <fstream>

using namespace std;

/*
 * MUL double the length of input
 * ADDC and SUBB give output that has the same length as their input
 */

static int count_add = 0, count_mul = 0;
const int INT_BITS = sizeof(int) * 8, SHORT_BITS = sizeof(short) * 8;

short* DOWN(int* a, const int LEN)
{
	// MUL is finished within int array to avoid overflow,
	// but the result needs to be transformed into short array to call function MUL
	// and avoid overflow caused by bit accumulation.
	
	short* result = new short[LEN];
	result[LEN-1] = 0;
	for (int i = LEN-1; i > 0; i--)
	{
		result[i] += a[i] % 256;
		result[i-1] = a[i] / 256;

		count_add += INT_BITS; 
	}

	// overflow is stored in result[0]; 
	// if MUL call DOWN to return value, the overflow should not be accumulated.
	// It should be moved to another short array.
	
	result[0] += a[0];

	count_add += INT_BITS;

	return result;
}

short* ADDC(short* a, short* b, const int LEN)
{
	// calculate (w+x) and (y+z)
	
	int* RESULT = new int[LEN];
	for (int i = LEN-1; i >= 0; i--)
	{
		RESULT[i] = a[i] + b[i];
		count_add += SHORT_BITS;
	}

	short* result = DOWN(RESULT, LEN);
	delete[] RESULT;
	return result;
}

int* ADDC(short* a, short* b, short* c, const int LEN)
{
	// calculate (a<<n) + [b<<(n/2)] + c
	
	const int SPLIT = LEN/2;
	int* result = new int[SPLIT*4];
	for (int i = 0; i < SPLIT; i++)
	{
		result[i] = a[i];
		result[i+SPLIT] = a[i+SPLIT] + b[i];
		result[i+SPLIT*2] = b[i+SPLIT] + c[i];
		result[i+SPLIT*3] = c[i+SPLIT];

		count_add += SHORT_BITS * 3;
	}
	return result;
}

short* SUBB(short* a, short* b, short* c, const int LEN)
{
	// calculate (w+x)(y+z) - wy - xz
	
	int* RESULT = new int[LEN];
	RESULT[LEN-1] = 0;
	for (int i = LEN-1; i > 0; i--)
	{
		RESULT[i] += a[i] - b[i] - c[i];
		RESULT[i-1] = 0;

		count_add += SHORT_BITS * 2;

		while (RESULT[i] < 0 && i!=0)
		{
			RESULT[i] += 256;
			RESULT[i-1] -= 1;

			count_add += SHORT_BITS * 2;
		}
	}
	RESULT[0] += a[0] - b[0] - c[0];

	count_add += SHORT_BITS * 2;

	short* result = DOWN(RESULT,LEN);
	delete[] RESULT;

	return result;
}

short* MUL(short* a, short* b, const int LEN)
{
	const int size = LEN*2;
	int* RESULT;

	if (LEN == 1)
	{
		// the end of recursion
		
		RESULT = new int[size];
		RESULT[size-1] = a[LEN-1] * b[LEN-1];
		RESULT[size-2] = 0;

		count_mul += SHORT_BITS * SHORT_BITS;
	}

	else
	{
		// uv = (w*2^(n/2)+x) + (y*2^(n/2)+z)
		//    = wy*2^n + [(w+x)(y+z)-wy-xz]*2^(n/2) + xz
		const int SPLIT = LEN/2;
		short *x, *y1, *y2, *z;
		x = MUL(a, b, SPLIT);			// wy
		z = MUL(a+(SPLIT),b+(SPLIT), SPLIT);	// xz

		short *sum1, *sum2;
		sum1 = ADDC(a, a+(SPLIT), SPLIT);	// w+x
		sum2 = ADDC(b, b+(SPLIT), SPLIT);	// y+z
		y1 = MUL(sum1, sum2, SPLIT);		// (w+x)(y+z)
		y2 = SUBB(y1, x, z, LEN);

		RESULT = ADDC(x, y2, z, LEN);

		delete[] x;
		delete[] y1;
		delete[] y2;
		delete[] z;
		delete[] sum1;
		delete[] sum2;
	}

	short* result = DOWN(RESULT, size);
	delete[] RESULT;
	return result;
}


int main()
{
	int bits = 1024;
	cout << "Enter the length of a binary number:\t" ;
	cin >> bits;
	int len = bits/8;

	cout << "Enter the times of test:\t";
	int times;
	cin >> times;

	short *a = new short[len], *b = new short[len];
	ifstream file_L("1IO/DEC1024_L.txt"), file_R("1IO/DEC1024_R.txt");
	ofstream resultFile("1IO/1 result.txt"), countFile("1IO/1 count.txt",ios::app);

	for (int j = 0; j < times; j++)
	{
		// input
		for (int i = 0; i < len; i++)
		{
			file_L >> a[i];
			file_R >> b[i];
			file_L.get(), file_R.get();
		}

		// calculation process
		short* result = MUL(a, b, len);
		for (int i = len; i > 0; i--)
		{
			while (result[i] < 0)
			{
				result[i] += 256;
				result[i-1] -= 1;
			}
			result[i-1] += result[i] / 256;
			result[i] = result[i] % 256;
		}

		// output
		for (int i = 0; i < len * 2; i++)
			resultFile << hex << result[i] << ' ';
		resultFile << endl;
		countFile << bits << ',' <<count_add << ',' << count_mul << endl;

		// restart
		count_add = count_mul = 0;
		delete[] result;
	}
	return 0;
}
