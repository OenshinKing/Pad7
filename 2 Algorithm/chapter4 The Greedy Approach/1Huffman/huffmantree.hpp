#pragma once
#include<climits>

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
			std::cout<<char(input);
			fileLen++;
		}
		textFile.clear();
		textFile.close();

		Node temp(0);
		top(temp);
		while(temp.Freq == 0)
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

class encodeBlock
{
private:
	char *L, *R;
	short* codeTable;
	char* codeLen;
	char alphaNum;
	int count;

	stack<char> treeHead;


	void buildTree(const char*, int);
	void encode();
	inline void check(char);
	void writeFile(const char*, const char*);

public:
	encodeBlock(const char* input, const char* output, int len = 128)
		:codeTable(new short[len*2]), codeLen(new char[len*2])
	{
		buildTree(input,len);
		encode();
		writeFile(input,output);
	}
	~encodeBlock()
	{
		delete[] L;
		delete[] R;
		delete[] codeTable;
		delete[] codeLen;
	}
};

void encodeBlock::buildTree(const char* file,int len)
{
	heap CF(file,len,MIN_HEAP);
	count = CF.count();
	Node cf1(0), cf2(0), cf(0);

	alphaNum = char(CF.num());
	L = new char[alphaNum];
	R = new char[alphaNum];

	while(CF.num() > 1)
	{
		CF.removeTop(cf1);
		CF.removeTop(cf2);
		cf = Node(cf1, cf2);
		CF.insert(cf);
		L[cf.Data-0x80] = cf1.Data, R[cf.Data-0x80] = cf2.Data;
	}
}

void encodeBlock::encode()
{
	// Node::beta store the virtual charater of the root node
	// give the root node the code of 0x00
	codeTable[Node::beta-1] = 1;
	codeLen[Node::beta-1] = -1;

	stack<char> traverse;
	char parent = Node::beta-1;
	treeHead.push(parent).push(parent);
	
	while(!traverse.isempty() || parent > 0x7f)
	{
		if (parent <= 0x7f)
		{
			traverse.pop(parent);
			continue;
		}

		codeTable[L[parent-0x80]] = (codeTable[parent]<<1) | 0x01 & SHRT_MAX;
		codeTable[R[parent-0x80]] = (codeTable[parent]<<1) &  (SHRT_MAX-1);
		codeLen[L[parent-0x80]] = codeLen[R[parent-0x80]] = codeLen[parent] + 1;

		// check(parent);

		traverse.push(R[parent-0x80]);
		treeHead.push(R[parent-0x80]).push(L[parent-0x80]);
		parent = L[parent-0x80];
	}
}

inline void encodeBlock::check(char read)
{

		printf("Parent %d:%d Left %d:%d Right %d:%d\n", read,codeTable[read], L[read-0x80],codeTable[L[read-0x80]],R[read-0x80],codeTable[R[read-0x80]]);
		printf("Parent %d Left %d Right %d\n\n",codeLen[read],codeLen[L[read-0x80]],codeLen[R[read-0x80]]);
}


void encodeBlock::writeFile(const char* input, const char* output)
{
	ofstream target(output,ios::binary);

	char cache;
	while(treeHead.pop(cache))
		target.write(&cache, sizeof(cache));
	target.write((char*) &count, sizeof(count));

	ifstream source(input);
	int read;
	char scanCache = 0x80;
	cache = 0;

	while( (read = source.get()) != -1)
	{
		for (short scanCode = 0x1 << codeLen[read]; scanCode != 0; scanCode = (scanCode >> 1) & SHRT_MAX)
		{
			cache = (scanCode & codeTable[read]) > 0 ? cache | scanCache : cache & (~scanCache);
			if (scanCache == 0x01)
			{
				target.write(&cache, sizeof(cache));
				scanCache = 0x80;
				cache = 0;
				continue;
			}
			scanCache = scanCache >> 1;
		}
	}

	target.write(&cache, sizeof(cache));
}

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
		
		// check(parent);
		
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

class HuffmanTree
{
private:
	encodeBlock* w;
	decodeBlock* r;
public:
	HuffmanTree() : w(nullptr), r(nullptr) {}

	void encode(const char* source, const char* target)
	{
		w = new encodeBlock(source, target);
		delete w;
		w = nullptr;
	}
	void decode(const char* source, const char* target)
	{
		r = new decodeBlock(source, target);
		delete r;
		r = nullptr;
	}
};
