//此文件定义了 一些cmd的操作命令，暂未实现
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Student.h"

class CmdHandler
{
    public:
        //命令行的输入
        void execute(const std::string& line);
    private:
        //参数化
        std::vector<std::string> tokenize(const std::string& line);
        //执行函数
        void ADD(const std::vector<std::string>& tokens);       //sms add <major> <name> <student_no>

        std::unordered_map<std::string, Student> students_;
};