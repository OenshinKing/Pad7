#pragma once

/* enum chi用于为childIndex和child提供助记符，并限制输入参数的值 */
enum chi {LEFT = 0, RIGHT = 1};
template <typename T>
class binaryTree
{
	/*
	定义使用数组存储的二叉树：
	childIndex:	用于根据给定的父节点的索引值获得孩子节点的索引值
	child:		根据父节点的索引值，修改孩子节点的数据
	isleaf:		给定一个索引值，判断该索引值是否代表一个叶子节点
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
	/* Array[Tail] 存储了数据 */
	T* Array;
	const int Head;
	int Tail, Length;

	int childIndex(int p, chi w) override
	{
		return (2 * p + w + 1);
		/* 通过计算获得孩子节点的索引值 */
	}
	T& child(int p, chi w)
	{
		return Array[childIndex(p, w)];
		/* 使用孩子节点的索引值，在数组中查找孩子节点的数据并返回引用 */
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
		/* 索引值为0的节点是根节点 */
	}
	bool childExist(int n, chi w) 
	{ 
		return (childIndex(n, w) <= Tail); 
		/* 索引值小于或等于Tail代表节点存在 */
	}
	bool isleaf(int n) override
	{
		return !childExist(n, LEFT);
		/* 左孩子不存在表示该节点为叶节点 */
	}

public:
	completeBinaryTree(int len) : Array(new T[len]), Head(0), Tail(-1), Length(len) {}
	~completeBinaryTree() { delete[] Array; }
};

class fullBinaryTree : public binaryTree<int>
{
	/*
	使用二维数组存储的满二叉树：
	以节点的数据作为它在数组中的索引，数组的两行分别存储节点的左孩子、右孩子
	使用负数索引值代表叶节点没有孩子
	*/
	static constexpr int LEAF = -1;
protected:
	int** Array;

	int childIndex(int p, chi w) override
	{
		return Array[w][p];
		/* 查表获得孩子节点的索引值 */
	}
	int& child(int p, chi w) 
	{
		return Array[w][p];
		/* 查表获得孩子节点的数据；数组存储的内容既是索引值也是数据 */
	}
	bool isleaf(int n)
	{
		return (childIndex(n, LEFT) == LEAF);
		/* 非叶节点必定同时拥有左右孩子 */
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