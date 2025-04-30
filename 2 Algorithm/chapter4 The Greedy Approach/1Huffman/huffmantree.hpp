#pragma once
#include<climits>
#include<iostream>
#include<fstream>

using namespace std;

struct Node
{
public:
	char Data;
	unsigned long Freq;

	static char alpha;
	static char beta;

	Node(char a = alpha):Data(alpha),Freq(0){ if(a == alpha) alpha++; }
	Node(const Node& a):Data(a.Data),Freq(a.Freq){}
	Node(const Node& a, const Node& b):Data(beta),Freq(1+a.Freq+b.Freq){ beta++; }
	friend bool operator<(const Node& a, const Node& b)
	{
		return a.Freq < b.Freq;
	}
	friend bool operator>(const Node& a, const Node& b)
	{
		return a.Freq > b.Freq;
	}
	friend bool operator!=(const Node& a, const Node& b)
	{
		return a.Freq != b.Freq;
	}
	Node& operator=(const Node& a)
	{
		this->Data = a.Data;
		this->Freq = a.Freq;
		return *this;
	}
	
};
char Node::beta = 0x80;
char Node::alpha = 0;

class heap : public heapBase<Node>
{
private:
	std::ifstream textFile;
	int fileLen;
public:

	heap(const char* file, int len=128, bool t = MIN_HEAP)
		: heapBase<Node>(len,t), textFile(file), fileLen(0)
	{
		init();
		buildHeap();
	}
	void init() override
	{
		int input;
		while((input = textFile.get()) != -1)
		{
			(Array[input].Freq)++;
			fileLen++;
		}
		textFile.clear();
		textFile.close();

		Node temp(0);
		top(temp);
		while(temp.Freq == 0 && num() != 0)
			removeTop(temp), top(temp);
	}

	void swap(Node& a, Node& b) override
	{
		Node temp(a);
		a = b;
		b = temp;
	}

	int count() const { return fileLen; }
};

class huffmantree
{
protected:
	char root, *L, *R;
	stack<char> treeHeader;
	bool check;
	int fileLen;

	virtual char buildTree(const char*) = 0;
	virtual bool checkParent(char parent)
	{
		if ((parent <= 0x7F) || (!check)) return false;
		cout << "Parent: " << int(parent);
		cout << " Left: " << int(L[parent-0x80]);
		cout << " Right: " << int(R[parent-0x80]);
		cout << endl;

		return true;
	}
public:
	huffmantree(bool c = false) : L(new char[128]), R(new char[128]), check(c) {}
	virtual bool writeFile(const char*, const char*) = 0;
	~huffmantree()
	{
		delete[] L;
		delete[] R;
	}
};

class encodeBlock : public huffmantree
{
protected:
	short* codeTable;
	char* codeLen;

	bool checkParent(char parent) override
	{
		if (!huffmantree::checkParent(parent)) return false;
		cout << "Parent: " << int(codeTable[parent]);
		cout << " Left: " << int(codeTable[L[parent-0x80]]);
		cout << " Right: " << int(codeTable[R[parent-0x80]]);
		cout << endl << endl;
		return true;
		
	}
	char buildTree(const char* fileName) override
	{
		heap CF(fileName, 128, MIN_HEAP);

		fileLen = CF.count();

		Node cf1(-1), cf2(-1), cf(-1);
		while(CF.num() > 1)
		{
			CF.removeTop(cf1);
			CF.removeTop(cf2);
			cf = Node(cf1, cf2);
			CF.insert(cf);
			
			L[int(cf.Data - 0x80)] = cf1.Data;
			R[int(cf.Data - 0x80)] = cf2.Data;
		}

		char parent = cf.Data, child;
		codeTable[int(parent)] = 1;
		codeLen[int(parent)] = 0;

		stack<char> traverse;
		traverse.push(parent);
		
		while(!traverse.isempty())
		{
			traverse.pop(parent);
			if (parent <= 0x7F) continue;

			child = parent - 0x80;
			codeTable[L[int(child)]] = (codeTable[int(parent)] << 1) | 0x0001;
			codeTable[R[int(child)]] = (codeTable[int(parent)] << 1) & 0xFFFE;
			codeLen[L[int(child)]] = codeLen[R[int(child)]] = codeLen[int(parent)] + 1;
			checkParent(parent);
			traverse.push(R[int(child)]);
			traverse.push(L[int(child)]);
		}

		return cf.Data;
	}
public:
	encodeBlock(bool c = false) : huffmantree(c), codeTable(new short[256]), codeLen(new char[256]){}
	~encodeBlock()
	{
		delete[] codeTable;
		delete[] codeLen;
	}
	bool writeFile(const char* source, const char* target) override
	{
		char root = buildTree(source);

		ifstream input(source);
		ofstream output(target, ios::binary);
		if ((!input.is_open()) || (!output.is_open()))
		{
			printf("%s is not open\n", (input.is_open() ? target : source));
			return false;
		}


		stack<char> traverse;
		traverse.push(root);
		treeHeader.push(root).push(root);

		char parent, child;
		while(!traverse.isempty())
		{
			traverse.pop(parent);
			if (parent <= 0x7F) continue;

			child = parent - 0x80;
			treeHeader.push(R[child]).push(L[child]);
			traverse.push(R[child]).push(L[child]);
		}
		while(treeHeader.pop(parent))
			output.write(&parent, sizeof(parent));
		output.write((char*) &fileLen, sizeof(fileLen));

		int read;
		char cache = 0, scanCache = 0x80, code;

		while((read = input.get()) != -1)
		{
			code = codeTable[read];
			for(short scanCode = 1 << (codeLen[read] -1); scanCode!= 0; scanCode = scanCode >> 1)
			{
				cache = (scanCode & codeTable[read]) > 0 ? cache | scanCache : cache & (~scanCache);
				if (scanCache == 0x01)
				{
					output.write(&cache, sizeof(cache));
					cache = 0, scanCache = 0x80;
					continue;
				}
				scanCache = scanCache >> 1;
			}
		}
		output.write(&cache, sizeof(cache));
		return true;
	}
};

class decodeBlock : public huffmantree
{
private:
	char buildTree(const char* fileName) override
	{
		ifstream input(fileName, ios::binary);
		if(!input.is_open())
		{
			cout << "File " << fileName << " is not open." << endl;
			return 0;
		}

		char treeLen = 0;
		char readerPair[2];
		readerPair[0] = 0, readerPair[1] = 1;
		while(readerPair[0] != readerPair[1])
		{
			input.read(readerPair, 2);
			treeHeader.push(readerPair[0]).push(readerPair[1]);
			treeLen += 2;
		}
		char &parent = readerPair[0], &child = readerPair[1];
		treeHeader.pop(parent), treeHeader.pop(root);

		stack<char> traverse;
		traverse.push(parent);

		while(!treeHeader.isempty())
		{
			traverse.pop(parent);
			if (parent <= 0x7F) continue;

			treeHeader.pop(child);
			R[parent-0x80] = child;
			traverse.push(child);

			treeHeader.pop(child);
			L[parent-0x80] = child;
			traverse.push(child);

			checkParent(parent);
		}

		return treeLen;
	}
public:
	decodeBlock(bool c) : huffmantree(c) {}
	bool writeFile(const char* source, const char* target) override
	{
		char treeLen = buildTree(source);

		ifstream input(source, ios::binary);
		ofstream output(target);

		if ((!input.is_open()) || (!output.is_open()) || (treeLen == 0))
		{
			cout << ((!input.is_open()) ? source : target) << " was unable to open." << endl;
			return false;
		}

		input.seekg(treeLen, ios::beg);
		input.read((char*) &fileLen, 4);

		char cache, scanCache = 0x80, parent = root;
		input.read(&cache, 1);
		while(fileLen)
		{
			if (parent <= 0x7F)
			{
				output.write(&parent, 1);
				parent = root;
				fileLen--;
			}

			parent = (scanCache & cache) > 0 ? L[parent-0x80] : R[parent-0x80];
			scanCache = (scanCache >> 1) & 0x7F;
			if (scanCache == 0)
			{
				input.read(&cache, 1);
				scanCache = 0x80;
			}
		}
		return true;
	}
};
/*
class decodeBlock
{
private:
	char *L, *R;
	char root;

	char buildTree(const char*);
	void writeFile(const char*, const char*);
	inline void check(char);
public:
	decodeBlock(const char* source, const char* target, int len = 128)
		:L(new char[len]), R(new char[len])
	{
		root = buildTree(source);
		writeFile(source,target);
	}
	~decodeBlock()
	{
		delete[] L;
		delete[] R;
	}
};

char decodeBlock::buildTree(const char* filename)
{
	char readerPair[2];
	readerPair[0] = 0, readerPair[1] = 1;
	stack<char> treeHead, traverse;

	ifstream source(filename, ios::binary);
	while (readerPair[0] != readerPair[1])
	{
		source.read(readerPair, 2);
		treeHead.push(readerPair[0]).push(readerPair[1]);
	}

	char& parent = readerPair[0];
	char& child = readerPair[1];
	char par;
	treeHead.pop(parent);
	treeHead.pop(par);
	
	while (!treeHead.isempty())
	{
		if (parent <= 0x7f)
		{
			traverse.pop(parent);
			continue;
		}

		treeHead.pop(child);
		R[parent-0x80] = child;
		traverse.push(child);

		treeHead.pop(child);
		L[parent-0x80] = child;
		
		check(parent);
		
		parent = child;
	}

	return par;
}

inline void decodeBlock::check(char read)
{
		printf("Parent %d Left %d Right %d\n", read, L[read-0x80],R[read-0x80]);
}

void decodeBlock::writeFile(const char* source, const char* target)
{
	char reader[2];
	reader[0] = 0, reader[1] = 1;
	
	int count;

	ifstream input(source, ios::binary);
	ofstream output(target);

	while (reader[0] != reader[1])
		input.read(reader,2);
	input.read((char*) &count, sizeof(count));
	
	char& scan = reader[0] = 0;
	char& cache = reader[1];

	char parent = root, child;
	while (count)
	{
		if(scan == 0)
		{
			input.read(&cache, 1);
			scan = 0x80;
		}

		child = ( (scan & cache) > 0 )? L[parent-0x80] : R[parent-0x80];
		scan = scan >> 1;

		if (child <= 0x7f)
		{
			output.write(&child, 1);
			count--;
			parent = root;
		}
		else
			parent = child;
	}
}
*/
