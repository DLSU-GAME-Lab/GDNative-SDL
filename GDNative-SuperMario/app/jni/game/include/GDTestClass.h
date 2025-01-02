#pragma once
#include <string>

class GDTestClass
{
public:
	GDTestClass(int someValue, std::string someName);
	~GDTestClass() = default;

	void doSomething();

private:
	int value;
	std::string someName;
};
