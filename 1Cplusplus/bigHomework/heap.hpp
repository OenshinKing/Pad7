#pragma once
#include"tree.hpp"
#define MIN_HEAP true
#define MAX_HEAP false

template <typename T>
class heapBase : public completeBinaryTree<T>
{
	/* 
	���������ȫ������ʵ�ֵĶѵĻ��ࣺ
	MIN_HEAP��MAX_HEAP: ģ���Դ󶥶�Ϊ��׼��д����Ҫʹ��С����ʱ�� MIN_HEAP ȡ������㽫�ȽϵĽ��ȡ��
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
		������siftdown�в�����ɸʱ��Ҫ�Ƚϵĺ��ӡ�
		����Һ��Ӳ����ڣ�����Ҫ�Ƚϵĺ��������ӣ�
		������Ҫ�Ƚϵĺ��������ݸ����һ����
		^Type ����֧��С���ѡ�
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
		����������и��ӹ�ϵ�Ľڵ�����ɸ��
		1. ѡ���������ӽڵ������ݸ������һ����
		2. �����ڵ�������뺢�ӽڵ�����ݱȽϣ�������ڵ������С��Ҷ�ڵ㣬�򽻻����ڵ��뺢�ӽڵ㣻
		   ���򣬵�ǰ�ĸ��ӹ�ϵ����ѵ�Ҫ��������������Ԥ����Ҳ������ѵ�Ҫ����˿�����ǰ�˳�ѭ����
		3. ׷�ٱ�����ɸ�ĸ��ڵ㣬ֱ�����ڵ㵽������ȷ��λ�á�
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