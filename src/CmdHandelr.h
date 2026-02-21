#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Student.h"

class CmdHandler
{
    public:
        void execute(const std::string& args);
    private:
        std::vector<std::string> tokenize(const std::string& line);
        void ADD(const std::vector<std::string>& tokens);       //sms add <major> <name> <student_no>

        std::unordered_map<std::string, Student> students_;
};