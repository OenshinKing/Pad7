#include <iostream>

using namespace std;

template <typename T>
struct Node
{
public:
	T Data;
	Node<T>* Next;
	Node<T>(T input1, Node *input2 = nullptr) : Data(input1), Next(input2) {}
};

template <typename T>
class Vector
{
protected:
	Node<T>* Head;
	Node<T>* Tail;
	int len;
	Vector<T> &Enqueue(T input)
	{
		Tail = Tail->Next = new Node<T>(input, nullptr);
		len++;
		return *this;
	}

public:
	Vector<T>(T* input = nullptr, int size = 0) : len(0)
	{
		Head = Tail = new Node<T>(0, nullptr);
		while (len < size)
		{
			Tail = Tail->Next = new Node<T>(input[len], nullptr);
			++len;
		}
	}

	Vector<T>(const Vector<T> &another) : len(0)
	{
		Head = Tail = new Node<T>(0, nullptr);
		const Node<T>* read = another.Head->Next;
		while (read != nullptr)
		{
			this->Enqueue(read->Data);
			read = read->Next;
		}
	}

	~Vector<T>()
	{
		Node<T>* temp = Head->Next;
		while (temp != nullptr)
		{
			Head->Next = Head->Next->Next;
			delete[] temp;
			temp = Head->Next;
		}
		delete Head;
	}

	T &operator[](int index)
	{
		if (len == 0 || index > len)
		{
			cout << "Index out of range.\n";
			return Head->Data;
		}

		int count = 0;
		Node<T>* read = Head->Next;
		while (count < index && read != nullptr)
		{
			++count;
			read = read->Next;
		}
		return read->Data;
	}

	friend istream &operator>>(istream &input, Vector<T> &operated)
	{
		cout << "Enter elements of a vector and end with ')': \n(";
		T temp;
		int control;
		while ((control = input.peek()) != ')')
		{
			if (control > '9' || control < '0')
			{
				input.get();
				continue;
			}
			input >> temp;
			operated.Enqueue(temp);
		}
		input.get();
		return input;
	}

	friend ostream &operator<<(ostream &output, const Vector<T> &operated)
	{
		output << "(";
		Node<T>* read = operated.Head->Next;
		while (read != nullptr)
		{
			output << read->Data;
			if (read->Next != nullptr)
				output << ", ";
			read = read->Next;
		}
		output << ")";
		return output;
	}

	Vector<T> &operator=(const Vector<T> &source)
	{
		// clear this
		Node<T>* read = this->Head->Next;
		while (read != nullptr)
		{
			this->Head->Next = this->Head->Next->Next;
			delete[] read;
			read = this->Head->Next;
		}
		this->Tail = this->Head;
		len = 0;
		// copy source to this
		read = source.Head->Next;
		while (read != nullptr)
		{
			this->Enqueue(read->Data);
			read = read->Next;
		}

		return *this;
	}

	friend Vector<T> operator+(const Vector<T> &a, const Vector<T> &b)
	{
		static Vector<T> error;
		if (a.len != b.len)
			return error;
		Vector<T> temp;
		const Node<T>* read1 = a.Head->Next;
		const Node<T>* read2 = b.Head->Next;
		while (read1 != nullptr)
		{
			temp.Enqueue(read1->Data + read2->Data);
			read1 = read1->Next;
			read2 = read2->Next;
		}
		cout << temp;
		return Vector<T>(temp);
	}

	friend Vector<T> operator-(const Vector<T> &a, const Vector<T> &b)
	{
		static Vector<T> error;
		if (a.len != b.len)
			return error;
		Vector<T> temp;
		const Node<T>* read1 = a.Head->Next;
		const Node<T>* read2 = b.Head->Next;
		while (read1 != nullptr)
		{
			temp.Enqueue(read1->Data - read2->Data);
			read1 = read1->Next;
			read2 = read2->Next;
		}
		return temp;
	}
	friend T operator*(const Vector<T> &a, const Vector<T> &b)
	{
		if (a.len != b.len)
			return 0.0;
		const Node<T>* read1 = a.Head->Next;
		const Node<T>* read2 = b.Head->Next;
		double result = 0.0;
		while (read1 != nullptr)
		{
			result += read1->Data * read2->Data;
			read1 = read1->Next;
			read2 = read2->Next;
		}
		return result;
	}

	static void Test(istream& input, ostream& output)
	{
		static int count = 0;
		++count;
		output << "VECTOR TEST" << count << endl;

		Vector<T> A, B;
		input >> A >> B;
		output << "Inputted information:\nA: " << A << "\nB: " << B << endl;

		Vector<T> C = A + B;
		output << "Sum of A and B:\n Vector C: " << C << endl;
		C = A - B;
		output << "Difference of inputted vectors:\n Vector D: " << C << endl;

		output << "Product of A and B: " << A * B << endl;

		int j = 0;
		cout << "Enter the index of element in vector D you want to check:" << endl;
		cin >> j;
		cout << C[j] << endl << endl;
	}
};

int main()
{

	Vector<int>::Test(cin,cout);
	Vector<double>::Test(cin,cout);
	return 0;
}

/*
	cout << "Vector<int>:\n";
	Vector<int> A;
	Vector<int> B;
	cin >> A >> B;
	cout << A << B;

	Vector<int> C = A + B;
	cout << "Sum of inputted vectors:\n" << C;
	C = A - B;
	cout << "Difference of inputted vectors:\n" << C;
	cout << "The 3rd element of the vector above is:\n" << C[2] << endl;
	cout << "Product of inputted vectors:\n" << A * B << endl;
	cout << endl;


	cout << "Vector<double>:\n";
	Vector<double> D;
	Vector<double> E;
	cin >> D >> E;
	cout << D << E;

	Vector<double> F = D + E;
	cout << "Sum of inputted vectors:\n" << F;
	F = D - E;
	cout << "Difference of inputted vectors:\n" << F;
	cout << "The 3rd element of the vector above is:\n" << F[2] << endl;
	cout << "Product of inputted vectors:\n" << D * E << endl;
	cout << endl;
*/
