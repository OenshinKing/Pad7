#pragma once

/* enum chi����ΪchildIndex��child�ṩ���Ƿ������������������ֵ */
enum chi {LEFT = 0, RIGHT = 1};
template <typename T>
class binaryTree
{
	/*
	����ʹ������洢�Ķ�������
	childIndex:	���ڸ��ݸ����ĸ��ڵ������ֵ��ú��ӽڵ������ֵ
	child:		���ݸ��ڵ������ֵ���޸ĺ��ӽڵ������
	isleaf:		����һ������ֵ���жϸ�����ֵ�Ƿ����һ��Ҷ�ӽڵ�
	*/
protected:
	virtual int childIndex(int, chi) = 0;
	virtual T& child(int, chi) = 0;
	virtual bool isleaf(int) = 0;
};

template <typename T>
class completeBinaryTree : public binaryTree<T>
{
protected:
	/* Array[Tail] �洢������ */
	T* Array;
	const int Head;
	int Tail, Length;

	int childIndex(int p, chi w) override
	{
		return (2 * p + w + 1);
		/* ͨ�������ú��ӽڵ������ֵ */
	}
	T& child(int p, chi w)
	{
		return Array[childIndex(p, w)];
		/* ʹ�ú��ӽڵ������ֵ���������в��Һ��ӽڵ�����ݲ��������� */
	}
	int parentIndex(int n)
	{
		return ((n - 1) / 2);
	}
	T& parent(int n)
	{
		return Array[parentIndex(n)];
	}
	bool isroot(int n)
	{
		return (n == 0);
		/* ����ֵΪ0�Ľڵ��Ǹ��ڵ� */
	}
	bool childExist(int n, chi w) 
	{ 
		return (childIndex(n, w) <= Tail); 
		/* ����ֵС�ڻ����Tail����ڵ���� */
	}
	bool isleaf(int n) override
	{
		return !childExist(n, LEFT);
		/* ���Ӳ����ڱ�ʾ�ýڵ�ΪҶ�ڵ� */
	}

public:
	completeBinaryTree(int len) : Array(new T[len]), Head(0), Tail(-1), Length(len) {}
	~completeBinaryTree() { delete[] Array; }
};

class fullBinaryTree : public binaryTree<int>
{
	/*
	ʹ�ö�ά����洢������������
	�Խڵ��������Ϊ���������е���������������зֱ�洢�ڵ�����ӡ��Һ���
	ʹ�ø�������ֵ����Ҷ�ڵ�û�к���
	*/
	static constexpr int LEAF = -1;
protected:
	int** Array;

	int childIndex(int p, chi w) override
	{
		return Array[w][p];
		/* ����ú��ӽڵ������ֵ */
	}
	int& child(int p, chi w) 
	{
		return Array[w][p];
		/* ����ú��ӽڵ�����ݣ�����洢�����ݼ�������ֵҲ������ */
	}
	bool isleaf(int n)
	{
		return (childIndex(n, LEFT) == LEAF);
		/* ��Ҷ�ڵ�ض�ͬʱӵ�����Һ��� */
	}
public:
	fullBinaryTree(int len):Array(new int*[2])
	{
		Array[LEFT] = new int[len];
		Array[RIGHT] = new int[len];
		for (int i = 0; i < len; i++)
			Array[LEFT][i] = Array[RIGHT][i] = LEAF;
	}
	~fullBinaryTree()
	{
		delete[] Array[LEFT];
		delete[] Array[RIGHT];
		delete[] Array;
	}
};