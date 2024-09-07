#include<SpringEngine/Inc/SpringEngine.h>

using namespace SpringEngine;
using namespace SpringEngine::Core;

class Student
{
public:
	Student(const std::string& name)
		:mName(name)
		, mGrade(0)
	{

	}
	void SetGrade(int grade)
	{
		mGrade = grade;
	}
private:
	std::string mName;
	int mGrade;
};

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	TypedAllocator StudentPool = TypedAllocator<Student>("studentPool", 200);

	std::vector<Student*> students;
	for (uint32_t i = 0; i < 100; ++i)
	{
		std::string name = "student" + std::to_string(i);
		students.push_back(StudentPool.New(name));
	}
	for (uint32_t i = 0; i < 20; ++i)
	{
		auto student = students.back();
		StudentPool.Delete(student);
	}
	for (uint32_t i = 0; i < 50; ++i)
	{
		std::string name = "student_new" + std::to_string(i);
		students.push_back(StudentPool.New(name));
	}
	for (auto student : students)
	{
		StudentPool.Delete(student);
	}
	return (0);
}