#include <iostream>
#include <unordered_map>
#include <windows.h>
#include "Student.h"
#include "Print_util.h"
void init()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

void test01()
{
    std::unordered_map<std::string, Student> students;
    std::string major, num, no;

    PrintContainer(students);
}

int main()
{
    init();
    test01();
    return 0;
}