#include <iostream>

using namespace std;
struct Node
{
public:
	double Data;
	Node *Next;
	Node(double input1, Node *input2 = nullptr) : Data(input1), Next(input2) {}
};

class Vector
{
protected:
	Node *Head;
	Node *Tail;
	int len;
	Vector &Enqueue(double input)
	{
		Tail = Tail->Next = new Node(input, nullptr);
		len++;
		return *this;
	}

public:
	Vector(double *input = nullptr, int size = 0) : len(0)
	{
		Head = Tail = new Node(0, nullptr);
		while (len < size)
		{
			Tail = Tail->Next = new Node(input[len], nullptr);
			++len;
		}
	}

	Vector(const Vector &another) : len(0)
	{
		Head = Tail = new Node(0, nullptr);
		const Node *read = another.Head->Next;
		while (read != nullptr)
		{
			this->Enqueue(read->Data);
			read = read->Next;
		}
	}

	~Vector()
	{
		Node *temp = Head->Next;
		while (temp != nullptr)
		{
			Head->Next = Head->Next->Next;
			delete[] temp;
			temp = Head->Next;
		}
		delete Head;
	}

	double &operator[](int index)
	{
		if (len == 0 || index > len)
		{
			cout << "Index out of range.\n";
			return Head->Data;
		}

		int count = 0;
		Node *read = Head->Next;
		while (count < index && read != nullptr)
		{
			++count;
			read = read->Next;
		}
		return read->Data;
	}

	friend istream &operator>>(istream &input, Vector &operated)
	{
		cout << "Enter elements of a vector and end with ')': \n(";
		double temp;
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

	friend ostream &operator<<(ostream &output, const Vector &operated)
	{
		output << "Vector: \t(";
		Node *read = operated.Head->Next;
		while (read != nullptr)
		{
			output << read->Data;
			if (read->Next != nullptr)
				output << ", ";
			read = read->Next;
		}
		output << ")\n";
		return output;
	}

	Vector &operator=(const Vector &source)
	{
		// clear this
		Node *read = this->Head->Next;
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

	friend Vector operator+(const Vector &a, const Vector &b)
	{
		static Vector error;
		if (a.len != b.len)
			return error;
		Vector temp;
		const Node *read1 = a.Head->Next;
		const Node *read2 = b.Head->Next;
		while (read1 != nullptr)
		{
			temp.Enqueue(read1->Data + read2->Data);
			read1 = read1->Next;
			read2 = read2->Next;
		}
		cout << temp;
		return Vector(temp);
	}
	friend Vector operator-(const Vector &a, const Vector &b)
	{
		static Vector error;
		if (a.len != b.len)
			return error;
		Vector temp;
		const Node *read1 = a.Head->Next;
		const Node *read2 = b.Head->Next;
		while (read1 != nullptr)
		{
			temp.Enqueue(read1->Data - read2->Data);
			read1 = read1->Next;
			read2 = read2->Next;
		}
		return temp;
	}
	friend double operator*(const Vector &a, const Vector &b)
	{
		if (a.len != b.len)
			return 0.0;
		const Node *read1 = a.Head->Next;
		const Node *read2 = b.Head->Next;
		double result = 0.0;
		while (read1 != nullptr)
		{
			result += read1->Data * read2->Data;
			read1 = read1->Next;
			read2 = read2->Next;
		}
		return result;
	}
};

int main()
{
	Vector A;
	Vector B;
	std::cout << "Hello world!\n";
	cin >> A >> B;
	cout << A << B;

	Vector C = A + B;
	cout << "Sum of inputted vectors: " << C;
	C = A - B;
	cout << "Difference of inputted vectors: " << C;
	cout << "The 3rd element of the vector below is: " << C[2] << endl;

	cout << "Product of inputted vectors: " << A * B << endl;
	return 0;
}
