#pragma once
#include<iostream>
#include<fstream>

using namespace std;

struct Node
{
public:
	unsigned char Data;
	unsigned long Freq;

	static unsigned char alpha;
	static unsigned char beta;

	Node(unsigned char a = alpha):Data(alpha),Freq(0){ if(a == alpha) alpha++; }
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
unsigned char Node::beta = 0x80;
unsigned char Node::alpha = 0;

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
	unsigned char root, *L, *R;
	stack<unsigned char> treeHeader;
	bool check;
	int fileLen;

	virtual unsigned char buildTree(const char*) = 0;
	virtual bool checkParent(unsigned char parent)
	{
		if ((parent <= 0x7F) || (!check)) return false;
		cout << "Parent: " << int(parent);
		cout << " Left: " << int(L[parent-0x80]);
		cout << " Right: " << int(R[parent-0x80]);
		cout << endl;

		return true;
	}
public:
	huffmantree(bool c = false) : L(new unsigned char[128]), R(new unsigned char[128]), check(c) {}
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
	unsigned char* codeLen;

	bool checkParent(unsigned char parent) override
	{
		if (!huffmantree::checkParent(parent)) return false;
		cout << "Parent: " << int(codeTable[parent]);
		cout << " Left: " << int(codeTable[L[parent-0x80]]);
		cout << " Right: " << int(codeTable[R[parent-0x80]]);
		cout << endl << endl;
		return true;
		
	}
	unsigned char buildTree(const char* fileName) override
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

		unsigned char parent = cf.Data, child;
		codeTable[int(parent)] = 1;
		codeLen[int(parent)] = 0;

		stack<unsigned char> traverse;
		traverse.push(parent);
		treeHeader.push(parent).push(parent);
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

			treeHeader.push(R[int(child)]).push(L[int(child)]);
		}

		return cf.Data;
	}
public:
	encodeBlock(bool c = false) : huffmantree(c), codeTable(new short[256]), codeLen(new unsigned char[256]){}
	~encodeBlock()
	{
		delete[] codeTable;
		delete[] codeLen;
	}
	bool writeFile(const char* source, const char* target) override
	{
		unsigned char root = buildTree(source);

		ifstream input(source);
		ofstream output(target, ios::binary);
		if ((!input.is_open()) || (!output.is_open()))
		{
			cout << (!input.is_open() ? source : target) << " was unable to open in encodeBlock::writeFile." << endl;
			return false;
		}

		unsigned char parent;
		while(treeHeader.pop(parent))
			output.write((char*) &parent, sizeof(parent));
		output.write((char*) &fileLen, sizeof(fileLen));

		int read;
		unsigned char cache = 0, scanCache = 0x80, code;

		while((read = input.get()) != -1)
		{
			code = codeTable[read];
			for(short scanCode = 1 << (codeLen[read] -1); scanCode!= 0; scanCode = scanCode >> 1)
			{
				cache = (scanCode & codeTable[read]) > 0 ? cache | scanCache : cache & (~scanCache);
				if (scanCache == 0x01)
				{
					output.write((char*) &cache, sizeof(cache));
					cache = 0, scanCache = 0x80;
					continue;
				}
				scanCache = scanCache >> 1;
			}
		}
		output.write((char*) &cache, sizeof(cache));
		return true;
	}
};

class decodeBlock : public huffmantree
{
private:
	unsigned char buildTree(const char* fileName) override
	{
		ifstream input(fileName, ios::binary);
		if(!input.is_open())
		{
			cout << fileName << " was unable to open in decodeBlock::buildTree." << endl;
			return (root = 0);
		}

		short treeLen = 0;
		unsigned char readerPair[2];
		readerPair[0] = 0, readerPair[1] = 1;
		while(readerPair[0] != readerPair[1])
		{
			input.read((char*) readerPair, 2);
			treeHeader.push(readerPair[0]).push(readerPair[1]);
			treeLen += 2;
			if (treeLen > 256)
				return (root = 0);
		}
		unsigned char &parent = readerPair[0], &child = readerPair[1];
		treeHeader.pop(parent), treeHeader.pop(root);

		stack<unsigned char> traverse;
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

		return root;
	}
public:
	decodeBlock(bool c) : huffmantree(c) {}
	bool writeFile(const char* source, const char* target) override
	{
		root = buildTree(source);
		int treeLen = (root - 0x80 + 1) * 2 + 1 + 1;

		ifstream input(source, ios::binary);
		ofstream output(target);

		if ((!input.is_open()) || (!output.is_open()) || (root == 0))
		{
			cout << ((!input.is_open()) ? source : target) << " was unable to open in decodeBlock::writeFile." << endl;
			return false;
		}

		input.seekg(treeLen, ios::beg);
		input.read((char*) &fileLen, 4);

		unsigned char cache, scanCache = 0x80, parent = root;
		input.read((char*) &cache, 1);
		while(fileLen)
		{
			if (parent <= 0x7F)
			{
				output.write((char*) &parent, 1);
				parent = root;
				fileLen--;
			}

			parent = (scanCache & cache) > 0 ? L[parent-0x80] : R[parent-0x80];
			scanCache = (scanCache >> 1) & 0x7F;
			if (scanCache == 0)
			{
				input.read((char*) &cache, 1);
				scanCache = 0x80;
			}
		}
		return true;
	}
};
