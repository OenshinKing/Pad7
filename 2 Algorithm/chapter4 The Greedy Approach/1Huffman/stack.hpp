#pragma once

template <typename T>
class node
{
public:
	T Data;
	node<T>* Next;

	node(T d = 0, node<T>* n = nullptr) : Data(d), Next(n){}
};

template <typename T>
class stack
{
protected:
	node<T>* Top;
public:
	stack() : Top() {}
	~stack()
	{
		T temp;
		while(pop(temp));
	}

	stack<T>& push(T input)
	{
		Top = new node<T>(input, Top);
		return *this;
	}

	bool pop(T& output)
	{
		if (Top == nullptr) return false;

		output = Top->Data;

		node<T>* temp = Top;
		Top = Top->Next;
		delete temp;
		return true;
	}

	bool isempty() { return (Top == nullptr); }
};
