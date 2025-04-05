#include <iostream>
#include <string>

using namespace std;
class Employee
{
protected:
	const string number;
	const string name;
	float basicSalary;
public:
	Employee(const string& IDcode, const string& ID, float salary):
		number(IDcode), name(ID), basicSalary(salary) {}

	virtual float pay()
	{
		return basicSalary;
	}

	virtual void print()
	{
		printf("    Total salary: $%.2f\n", pay());
		printf("    Basic salary: $%.2f\n", basicSalary);
	}

	virtual void input()
	{
		int selection;
		cin >> selection;
		switch(selection)
		{
			case 1:
				cin >> number;
				cout << "Number ";
				break;
			case 2:
				cin >> name;
				cout << "Name ";
				break;
			case 3:
				cin >> basicSalary;
				cout << "Basic salary ";
				break;
		}
		cout << "changed.\n";
		system("pause");
	}
};

class Salesman : public Employee
{
protected:
	float sales;
	static float commrate;
public:
	Salesman(const string& IDcode, const string& ID, float salary, float sold = 0):
		Employee(IDcode, ID, salary), sales(sold) {}

	float pay() override
	{
		return this->Employee::pay() + sales * commrate;
	}
	void print() override
	{
		this->Employee::print();
		printf("Taken percentage: %.2f\n", sales*commrate);
	}
};

float Salesman::commrate = 5.0/1000.0;

class Salesmanager : public Salesman
{
protected:
	float jobSalary;
public:
	Salesmanager(const string& IDcode, const string& ID, float salary, float sold, float jobEarned = 0):
		Salesman(IDcode, ID, salary, sold), jobSalary(jobEarned) {}
	float pay() override
	{
		return this->Salesman::pay() + jobSalary;
	}
	void print() override
	{
		this->Salesman::print();
		printf("      Job salary: %.2f\n", jobSalary);
	}
};

int main()
{
	Employee A("114514","Lihua", 2000);
	A.print();
	Salesman B("!919810", "Lihua", 2000, 20000);
	B.print();
	Salesmanager C("1299", "Lihua", 2000, 20000, 3000);
	C.print();
	return 0;
}
