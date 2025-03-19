#include <iostream>

struct Node
{
	double Data;
	Node* Next;
	Node(double input, Node* link = nullptr) : Data(input), Next(link) {}
};
class Vector
{
	private:
		Node* Head;
		Node* Tail;
		int Length;
	public:
		Vector():Length(0)
		{
			Head = Tail = new Node(0);
		}
		~Vector()
		{
			Node* temp = Head->Next;
			while(temp != nullptr)
			{
				Head->Next = Head->Next->Next;
				delete temp;
				temp = Head->Next;
			}
			delete Head;
		}

		Vector(const Vector& another)
		{
			 Head = Tail = new Node(0);
			 const Node* read = another.Head->Next;
			 while(read != nullptr)
			 {
				Tail = Tail->Next = new Node(read->Data);
				read = read->Next;
			 }
		}

		Vector& operator= (const Vector& another)
		{
			Node* temp = Head->Next;
			while(temp != nullptr)
			{
				Head->Next = Head->Next->Next;
				delete temp;
				temp = Head->Next;
			}
			delete Head;
			this->Head = this->Tail = new Node(0);

			const Node* read = another.Head->Next;
			while(read != nullptr)
			{
				Tail = Tail->Next = new Node(read->Data);
				read = read->Next;
			}
		}
};
