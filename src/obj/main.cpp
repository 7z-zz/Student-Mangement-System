#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#include "../Head/CmdHandler.h"
#include "../Head/StudentStore.h"

void init()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
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
