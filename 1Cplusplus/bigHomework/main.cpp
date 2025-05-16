#include<iostream>
#include<fstream>
#include<climits>
#include"tree.hpp"
#include"heap.hpp"
#include"stack.hpp"
#include"huffmantree.hpp"
using namespace std;

int main(int argc, char* argv[])
{
	if (argv[1][0] == 'e')
	{
		encodeBlock a(false);
		a.writeFile(argv[2], argv[3]);
	}
	if (argv[1][0] == 'd')
	{
		decodeBlock b(false);
		b.writeFile(argv[2], argv[3]);
	}

	return 0;
}
