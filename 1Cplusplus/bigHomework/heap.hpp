#pragma once
#include"tree.hpp"
#define MIN_HEAP true
#define MAX_HEAP false

template <typename T>
class heapBase : public completeBinaryTree<T>
{
	/* 
	定义基于完全二叉树实现的堆的基类：
	MIN_HEAP、MAX_HEAP: 模板以大顶堆为基准编写，需要使用小顶堆时与 MIN_HEAP 取异或运算将比较的结果取反
	*/

protected:
	bool Type;

	virtual void init() = 0;
	void swap(T& a, T& b)
	{
		T c = a;
		a = b;
		b = c;
	}

	chi selectChild(int n)
	{
		if (!this->childExist(n, RIGHT)) return LEFT;
		return (this->child(n, LEFT) > this->child(n,RIGHT)) ^ this->Type ? LEFT : RIGHT;
		/*
		用于在siftdown中查找下筛时需要比较的孩子。
		如果右孩子不存在，则需要比较的孩子是左孩子；
		否则，需要比较的孩子是数据更大的一个。
		^Type 用于支持小顶堆。
		*/
	}
	void siftdown(int n)
	{
		while (!this->isleaf(n))
		{
			chi next = this->selectChild(n);
			if (this->Type ^ (this->Array[n] > this->child(n, next)))
				this->swap(this->Array[n], this->child(n, next));
			else break;
			n = this->childIndex(n, next);
		}
		/*
		将不满足堆中父子关系的节点向下筛：
		1. 选择两个孩子节点中数据更大的那一个；
		2. 将父节点的数据与孩子节点的数据比较，如果父节点的数据小于叶节点，则交换父节点与孩子节点；
		   否则，当前的父子关系满足堆的要求，由于子树经过预处理也已满足堆的要求，因此可以提前退出循环。
		3. 追踪被向下筛的父节点，直到父节点到达它正确的位置。
		*/
	}
	void buildHeap()
	{
		for (int i = this->Tail; i >= 0; i--)
			this->siftdown(i);
	}
public:
	heapBase(int len, bool t) : completeBinaryTree<T>(len), Type(t) { this->Tail = len - 1; }

	bool removeTop(T& output)
	{
		if (this->Tail == this->Head - 1) return false;

		output = this->Array[this->Head];
		this->swap(this->Array[this->Head], this->Array[this->Tail]);
		this->siftdown(this->Head);
		return true;
	}

	bool top(T& output)
	{
		if (this->Tail == this->Head - 1) return false;
		output = this->Array[this->Head];
		return true;
	}

	bool insert(T input)
	{
		if (this->Tail == this->Length - 1) return false;

		(this->Tail)++;
		this->Array[this->Tail] = input;
		int trace = this->Tail;
		while (!this->isroot(trace))
		{
			if ((this->Array[trace] > this->parent(trace)) ^Type) 
				swap(this->Array[trace], this->parent(trace));
			else break;

			trace = this->parentIndex(trace);
		}
		return true;
	}

	int num() { return (this->Tail - this->Head + 1) ; }
};