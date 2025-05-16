#pragma once
#include"tree.hpp"
#include"heap.hpp"
#include"stack.hpp"
#include<fstream>
#include<iostream>
#include<climits>

struct charAndFreq
{
protected:
	static unsigned char alpha;
	static unsigned char beta;

	friend class encodeBlock;
public:
	unsigned char Data;
	unsigned long Freq;

	charAndFreq(unsigned char a = alpha) :Data(alpha), Freq(0) { if (a == alpha) alpha++; }
	charAndFreq(const charAndFreq& a) :Data(a.Data), Freq(a.Freq) {}
	charAndFreq(const charAndFreq& a, const charAndFreq& b) :Data(beta), Freq(1 + a.Freq + b.Freq) { beta++; }
	friend bool operator<(const charAndFreq& a, const charAndFreq& b)
	{
		return a.Freq < b.Freq;
	}
	friend bool operator>(const charAndFreq& a, const charAndFreq& b)
	{
		return a.Freq > b.Freq;
	}
	friend bool operator!=(const charAndFreq& a, const charAndFreq& b)
	{
		return a.Freq != b.Freq;
	}
	charAndFreq& operator=(const charAndFreq& a)
	{
		this->Data = a.Data;
		this->Freq = a.Freq;
		return *this;
	}
};
unsigned char charAndFreq::beta = 0x80;
unsigned char charAndFreq::alpha = 0;

class heap : public heapBase<charAndFreq>
{
private:
	std::ifstream textFile;
	int count;
public:
	heap(const char* file, int len = 128, bool t = MIN_HEAP)
		: heapBase<charAndFreq>(len, t), textFile(file), count(0)
	{
		init();
		buildHeap();
	}
	void init() override
	{
		int input;
		while ((input = textFile.get()) != -1)
		{
			(Array[input].Freq)++;
			count++;
		}
		textFile.clear();
		textFile.close();

		charAndFreq temp(0);
		top(temp);
		while (temp.Freq == 0 && num() != 0)
			removeTop(temp), top(temp);
	}

	int fileLen() const { return count; }
};

class huffmanTree : public fullBinaryTree
{
protected:
	unsigned char root;
	stack<unsigned char> treeHeader;
	bool check;
	int fileLen;
	virtual unsigned char buildTree(const char*) = 0;
	virtual bool checkParent(int parent)
	{
		if ((!check) || isleaf(parent)) return false;
		std::cout << "Parent: " << parent
				<< "Left child: " << child(parent, LEFT)
				<< "Right child: " << child(parent, RIGHT)
				<< std::endl;
		return true;

		/* 用于校验生成的树的结构 */
	}
public:
	huffmanTree(bool c = false) : fullBinaryTree(255), check(c) {}
	virtual bool writeFile(const char*, const char*) = 0;
};

class encodeBlock : public huffmanTree
{
protected:
	short *codeTable, *codeLen;
	static constexpr unsigned short SET0 = USHRT_MAX - 1;
	static constexpr unsigned short SET1 = 1;

	bool checkParent(int parent) override
	{
		using namespace std;
		if ((!check) || isleaf(parent)) return false;
		huffmanTree::checkParent(parent);
		cout << "Parent: " << codeTable[parent];
		cout << " Left child: " << codeTable[child(parent, LEFT)];
		cout << " Right child: " << codeTable[child(parent, RIGHT)];
		cout << endl << endl;
		return true;

		/* 校验哈夫曼编码 */
	}

	unsigned char buildTree(const char* fileName) override
	{
		heap CF(fileName, 128, MIN_HEAP);

		fileLen = CF.fileLen();

		charAndFreq cf1(-1), cf2(-1), cf(-1);

		while (CF.num() > 1)
		{
			CF.removeTop(cf1);
			CF.removeTop(cf2);
			cf = charAndFreq(cf1, cf2);
			CF.insert(cf);

			child(cf.Data, LEFT) = cf1.Data;
			child(cf.Data, RIGHT) = cf2.Data;
		}

		unsigned char parent = cf.Data;
		codeTable[parent] = 1;
		codeLen[parent] = 0;

		stack<unsigned char> traverse;
		traverse.push(parent);
		treeHeader.push(parent).push(parent);
		while (!traverse.isempty())
		{
			traverse.pop(parent);
			if (isleaf(parent)) continue;

			codeTable[child(parent, LEFT)] = (codeTable[parent] << 1) | SET1;
			codeTable[child(parent, RIGHT)] = (codeTable[parent] << 1) & SET0;
			codeLen[child(parent, LEFT)] = codeLen[child(parent, RIGHT)] = codeLen[parent] + 1;

			checkParent(parent);

			traverse.push(child(parent, RIGHT));
			traverse.push(child(parent, LEFT));

			treeHeader.push(child(parent, RIGHT));
			treeHeader.push(child(parent, LEFT));
		}

		return cf.Data;
	}
public:
	encodeBlock(bool c = false) : huffmanTree(c), codeTable(new short[255]), codeLen(new short[255]) {}
	~encodeBlock()
	{
		delete[] codeTable;
		delete[] codeLen;
	}
	bool writeFile(const char* source, const char* target) override
	{
		using namespace std;
		unsigned char root = buildTree(source);

		ifstream input(source);
		ofstream output(target, ios::binary);
		if ((!input.is_open()) || (!output.is_open()))
		{
			cout << (!input.is_open() ? source : target) << " was unable to open in encodeBlock::writeFile." << endl;
			return false;
		}

		unsigned char parent;
		while (treeHeader.pop(parent))
			output.write((char*)&parent, sizeof(parent));
		output.write((char*)&fileLen, sizeof(fileLen));

		int read;
		unsigned char cache = 0, scanCache = 0x80, code;

		while ((read = input.get()) != -1)
		{
			code = codeTable[read];
			for (short scanCode = 1 << (codeLen[read] - 1); scanCode != 0; scanCode = scanCode >> 1)
			{
				cache = (scanCode & codeTable[read]) > 0 ? cache | scanCache : cache & (~scanCache);
				if (scanCache == 0x01)
				{
					output.write((char*)&cache, sizeof(cache));
					cache = 0, scanCache = 0x80;
					continue;
				}
				scanCache = scanCache >> 1;
			}
		}
		output.write((char*)&cache, sizeof(cache));
		return true;
	}
};

class decodeBlock : public huffmanTree
{
private:
	unsigned char buildTree(const char* fileName) override
	{
		using namespace std;
		ifstream input(fileName, ios::binary);
		if (!input.is_open())
		{
			cout << fileName << " was unable to open in decodeBlock::buildTree." << endl;
			return (root = 0);
		}

		short treeLen = 0;
		unsigned char readerPair[2];
		readerPair[0] = 0, readerPair[1] = 1;
		while (readerPair[0] != readerPair[1])
		{
			input.read((char*)readerPair, 2);
			treeHeader.push(readerPair[0]).push(readerPair[1]);
			treeLen += 2;
			if (treeLen > 256)
				return (root = 0);
		}
		unsigned char& parent = readerPair[0], temp;
		treeHeader.pop(parent), treeHeader.pop(root);

		stack<unsigned char> traverse;
		traverse.push(parent);

		while (!treeHeader.isempty())
		{
			traverse.pop(parent);
			if (isleaf(parent)) continue;

			treeHeader.pop(temp);
			child(parent,RIGHT) = temp;
			traverse.push(temp);

			treeHeader.pop(temp);
			child(parent, LEFT) = temp;
			traverse.push(temp);

			checkParent(parent);
		}

		return root;
	}
public:
	decodeBlock(bool c) : huffmanTree(c) {}
	bool writeFile(const char* source, const char* target) override
	{
		using namespace std;
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
		input.read((char*)&fileLen, 4);

		unsigned char cache, scanCache = 0x80, parent = root;
		input.read((char*)&cache, 1);
		while (fileLen)
		{
			if (parent <= 0x7F)
			{
				output.write((char*)&parent, 1);
				parent = root;
				fileLen--;
			}

			parent = (scanCache & cache) > 0 ? child(parent, LEFT) : child(parent, RIGHT);
			scanCache = (scanCache >> 1) & 0x7F;
			if (scanCache == 0)
			{
				input.read((char*)&cache, 1);
				scanCache = 0x80;
			}
		}
		return true;
	}
};
