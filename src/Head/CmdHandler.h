//此文件定义了 一些cmd的操作命令，暂未实现
#pragma once
#include <string>
#include <vector>

class CmdHandler
{
    public:
        //命令行的输入
        static void execute(const std::string& line);
    private:
        //参数化函数
        static std::vector<std::string> tokenize(const std::string& line);
        //执行函数
        static void ADD(const std::vector<std::string>& tokens);       //sms add <major> <name> <student_no>
        static void LIST(const std::vector<std::string>& tokens);      //sms list
        static void HELP();
};