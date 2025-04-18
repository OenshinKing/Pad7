#include <iostream>
#include <fstream>

using namespace std;

/*
 * MUL double the length of input
 * ADDC and SUBB give output that has the same length as their input
 */

int& count_add(int a, int b)
{
	static int count = 0;
	a = a & INT_MAX, b = b & INT_MAX;
	if ((!a) || (!b))
		return count;
	int aBits = 1, bBits = 1;
	while((a = a>>1)&INT_MAX)
		aBits++;
	while((b = b>>1)&INT_MAX)
		bBits++;
	count += aBits < bBits ? aBits : bBits;
	return count;
}

int& count_mul(int a, int b)
{
	static int count = 0;
	a = a & INT_MAX, b = b & INT_MAX;
	if ((!a) || (!b))
		return count;
	int aBits = 1, bBits = 1;
	while ((a = a >> 1)&INT_MAX)
		aBits++;
	while ((b = b >> 1)&INT_MAX)
		bBits++;
	count += aBits * bBits;
	count_add(0,0) += (aBits-1) * (bBits-1);
	return count;
}

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

		count_add(result[i],a[i]%256);
	}

	// overflow is stored in result[0]; 
	// if MUL call DOWN to return value, the overflow should not be accumulated.
	// It should be moved to another short array.
	
	result[0] += a[0];

	count_add(result[0],a[0]);

	return result;
}

short* ADDC(short* a, short* b, const int LEN)
{
	// calculate (w+x) and (y+z)
	
	int* RESULT = new int[LEN];
	for (int i = LEN-1; i >= 0; i--)
	{
		RESULT[i] = a[i] + b[i];
		count_add(a[i],b[i]);
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

		count_add(a[i+SPLIT],b[i]);
		count_add(b[i+SPLIT],c[i]);
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

		count_add(a[i],b[i]);
		count_add(a[i]-b[i],c[i]);

		while (RESULT[i] < 0 && i!=0)
		{
			RESULT[i] += 256;
			RESULT[i-1] -= 1;

			count_add(RESULT[i],256);
			count_add(RESULT[i-1],1);
		}
	}
	RESULT[0] += a[0] - b[0] - c[0];

	count_add(a[0],b[0]);
	count_add(a[0]-b[0],c[0]);

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

		count_mul(a[LEN-1],b[LEN-1]);
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
	ifstream file_L("1IO/HEX1024_L.txt"), file_R("1IO/HEX1024_R.txt");
	ofstream resultFile("1IO/1 result.txt"), countFile("1IO/1 count.txt",ios::app);

	for (int j = 0; j < times; j++)
	{
		// input
		for (int i = 0; i < len; i++)
		{
			file_L >> hex >> a[i];
			file_R >> hex >> b[i];
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
			resultFile << setw(2) << setfill('0') << setiosflags(ios::right) << hex << result[i] << ' ';
		resultFile << endl;
		countFile << bits << ',' << count_add(0,0) << ',' << count_mul(0,0) << endl;

		// restart
		count_add(0,0) = count_mul(0,0) = 0;
		delete[] result;
	}
	return 0;
}
