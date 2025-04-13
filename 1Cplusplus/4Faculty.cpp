#include <iostream>

template <typename T>
void coutf(const char *str, T data)
{
	using namespace std;
	static const int WIDTH = 15;
	cout.fill(' '), cout.setf(ios::right);
	cout << '\t', cout.width(WIDTH), cout << str, cout.flush(), cout << data << endl;
}

class FacultyMember
{
protected:
	const char *Name;
	float basicSalary;

	virtual float Salary() = 0;

public:
	FacultyMember(const char *name, float salary) : Name(name), basicSalary(salary) {}

	template <typename T>
	friend void coutf(const char *str, T data);

	virtual void printInfo()
	{
		std::cout << "SALARY INFOMATION\nBasic" << std::endl;
		coutf("Name: ", Name);
		coutf("Total salary: ", Salary());
		coutf("Basic salary: ", basicSalary);
		std::cout << "Additional" << std::endl;
	}
};

enum perkForTeacher
{
	Professor = 50,
	AdjunctProfessor = 30,
	Lecturer = 20
};
class teacher : public FacultyMember
{
private:
	float fixedSalary;
	int classHour;
	perkForTeacher perkPerClass;
	const char *Title;

	float classPerk()
	{
		return classHour * perkPerClass;
	}

public:
	teacher(const char *name, float salary1, float salary2, int work, perkForTeacher title)
		: FacultyMember(name, salary1), fixedSalary(salary2), classHour(work), perkPerClass(title), Title(nullptr)
	{
		switch (perkPerClass)
		{
		case 50:
			Title = "Professor";
			break;
		case 30:
			Title = "Adjunct Professor";
			break;
		case 20:
			Title = "Lecturer";
			break;
		default:
			Title = "Other";
		}
	}
	void printInfo()
	{
		FacultyMember::printInfo();
		coutf("Faculty type: ", "Teacher");
		coutf("Teacher title: ", Title);
		coutf("Class hour: ", classHour);
		coutf("Class perk: ", classPerk());
		std::cout << std::endl;
	}

	float Salary()
	{
		return basicSalary + fixedSalary + classPerk();
	}
};

class manager : public FacultyMember
{
private:
	float facultySalary;

public:
	void printInfo()
	{
		FacultyMember::printInfo();
		coutf("Faculty type: ", "manager");
		coutf("Faculty salary: ", facultySalary);
		std::cout << std::endl;
	}
	manager(const char *name, float salary1, float salary2) : FacultyMember(name, salary1), facultySalary(salary2) {}
	float Salary()
	{
		return basicSalary + facultySalary;
	}
};

class labPersonnel : public FacultyMember
{
private:
	int workdays;
	float perkPerWorkday;

	float workdayPerk()
	{
		return workdays * perkPerWorkday;
	}

public:
	void printInfo()
	{
		FacultyMember::printInfo();
		coutf("Faculty type: ", "Lab personnel");
		coutf("Work days: ", workdays);
		coutf("Workday perk: ", workdayPerk());
		std::cout << std::endl;
	}
	labPersonnel(const char *name, float salary1, int work, float salary2)
		: FacultyMember(name, salary1), workdays(work), perkPerWorkday(salary2) {}
	float Salary()
	{
		return basicSalary + workdayPerk();
	}
};

int main()
{
	teacher A("Aris", 1000, 2000, 50, Professor);
	manager B("Yuuka", 1000, 2000);
	labPersonnel C("Momori", 1000, 50, 20);
	A.printInfo(), B.printInfo(), C.printInfo();
	return 0;
}
