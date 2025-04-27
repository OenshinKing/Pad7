#pragma once
#define MIN_HEAP true
#define MAX_HEAP false

template <typename T>
class heapBase
{
public:
	heapBase(int len, bool t):Array(new T[len]), Head(0), Tail(len-1),Length(len),Type(t){}
	~heapBase(){ delete[] Array; }

	bool insert(T input)
	{
		if(Tail == Length-1)
			return false;
		Tail++;
		Array[Tail] = input;
		for (int i = Tail; i != 0; i = parent(i))
		{
			if ((Array[i] > Array[parent(i)])^Type)
				swap(Array[i],Array[parent(i)]);
			else break;
		}
		return true;
	}

	bool removeTop(T& output)
	{
		if (Tail == Head-1) return false;
		output = Array[Head];

		swap(Array[Head],Array[Tail]);
		Tail--;
		siftdown(Head);
		return true;
	}

	bool top(T& output)
	{
		if (Tail == Head-1) return false;
		output = Array[Head];
		return true;
	}

	int num()
	{
		return (Tail - Head + 1);
	}

protected:
	T *Array;
	int Head, Tail,Length;
	bool Type;

	virtual void init() = 0;
	virtual void swap(T&, T&) = 0;

	enum chi {left=1, right=2};
	inline int parent(int n) { return (n-1)/2; }
	inline int child(int n, chi w)
	{
		return (2*n+w);
	}
	inline int bro(int n)
	{
		return (n%2 ? n+1 : n-1);
	}

	void siftdown(int n)
	{
		chi next;
		if (child(n,left) > Tail) return;
		else if (child(n,right) > Tail) next = left;
		else next = (Array[child(n,left)] > Array[child(n,right)])^Type ? left : right;

		int i = n, j = child(n,next);
		while(i <= Tail)
		{
			if ((Array[i] < Array[j])^Type)
				swap(Array[i],Array[j]);
			else return;

			i = j;
			if (child(i,left) > Tail) return;
			else if (child(i,right) > Tail) next = left;
			else next = (Array[child(i,left)] > Array[child(i,right)])^Type ? left : right;
			j = child(i,next);
		}
	}
	
	void buildHeap()
	{
		for (int i = Tail; i != Head -1; i--)
			siftdown(i);
	}
};

class heapTest: public heapBase<int>
{
public:
	heapTest(int len, bool t):heapBase<int>(len,t)
	{
		init();
		buildHeap();
	}

	void swap(int& a, int& b) override
	{
		int temp = a;
		a = b;
		b = temp;
	}
	void init() override
	{
		for (int i = Head; i <= Tail; i++)
			Array[i] = int(i), printf("init %d ",i);
		printf("\n");
	}
};
