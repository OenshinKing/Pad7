#include<iostream>
#include<fstream>
#include<climits>
#include"heap.hpp"
#include"stack.hpp"
#include"huffmantree.hpp"
using namespace std;

int main(int argc, char* argv[])
{
	HuffmanTree Test;
	if (argv[1][0] == 'e')
		Test.encode(argv[2], argv[3]);
	if (argv[1][0] == 'd')
		Test.decode(argv[2], argv[3]);
	// Test.encode("text.txt", "encode.txt");
	// Test.decode("encode.txt", "decode.txt");
	return 0;
}
