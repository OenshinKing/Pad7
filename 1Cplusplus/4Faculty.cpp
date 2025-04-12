#include <iostream>

class FacultyMember
{
protected:
	float basicSalary;
	const char* Name;
public:
	virtual void printInfo()
	{
		printf("Salary Information\n\tName: %s\nTotal Salary: %.2f\nBasic Salary: %.2f\n", Name, Salary(), basicSalary);
	}
	virtual float Salary() = 0;
	FacultyMember(const char* name, float salary) : Name(name), basicSalary(salary) {}
};

enum perkForTeacher {Professor = 50, AdjunctProfessor = 30, Lecturer = 20};
class teacher : public FacultyMember
{
private:
         float fixedSalary;
	 int classHour;
	 int perkPerClass;
	 const char* Title;
	 
	 float classPerk()
	 {
		return classHour * perkPerClass;
	 }
public:
	 void printInfo()
	 {
		FacultyMember::printInfo();
		printf("Teacher Title: %s\nClass Hours: %d\nClass Perk: %.2f\n\n",Title, classHour, classPerk());
	 }
	teacher(const char* name, float salary1, float salary2, int work, int title):
	FacultyMember(name,salary1), fixedSalary(salary2), classHour(work), perkPerClass(title),Title(nullptr)
	{
		switch(perkPerClass)
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
		printf("Faculty Salary: %.2f\n\n", facultySalary);
	}
	manager(const char* name, float salary1, float salary2): FacultyMember(name,salary1), facultySalary(salary2) {}
	float Salary()
	{
		return basicSalary + facultySalary;
	}
};

class laboratory : public FacultyMember
{
private:
	float perkPerWorkday;
	int workdays;

	float workdayPerk()
	{
		return workdays * perkPerWorkday;
	}
public:
	void printInfo()
	{
		FacultyMember::printInfo();
		printf("Work Days: %d\nWorkday Perk: %.2f\n\n", workdays, workdayPerk());
	}
	laboratory(const char* name, float salary1, int work, float salary2)
	: FacultyMember(name,salary1), workdays(work), perkPerWorkday(salary2) {}
	float Salary()
	{
		return basicSalary + workdayPerk();
	}
};

int main()
{
	std::cout << "Hello world!\n";
	teacher A("Aris",1000,2000,50,Professor);
	manager B("Yuuka",100,200);
	laboratory C("Momori",1000,50,20);
	A.printInfo(), B.printInfo(), C.printInfo();
	return 0;
}
