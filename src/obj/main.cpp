#include <iostream>
#include <windows.h>

#include "../Head/CmdHandler.h"
#include "../Head/StudentStore.h"

void init()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

void test01()
{
    Student_Store test_store;
    Student test_obj("制糖师","奶龙","1145");
    test_store.add(test_obj);
    test_store.forEach([](const Student& student)
    {
        std::cout << student << std::endl;
    });
}

void test02()
{
    std::string arg = {};
    Student_Store test_store;
    while (getline(std::cin, arg))
    {
        CmdHandler::execute(arg);
    }
}

int main()
{
    init();
    test02();
    return 0;
}