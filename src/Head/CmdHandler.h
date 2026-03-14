#pragma once

#include <string>
#include <vector>

class CmdHandler
{
public:
    static void execute(const std::string& line);

private:
    static std::vector<std::string> tokenize(const std::string& line);

    static void ADD(const std::vector<std::string>& tokens);       // sms add <major> <name> <student_no>
    static void LIST(const std::vector<std::string>& tokens);      // sms list
    static void GET(const std::vector<std::string>& tokens);       // sms get <student_no>
    static void DEL(const std::vector<std::string>& tokens);       // sms del <student_no>
    static void UPDATE(const std::vector<std::string>& tokens);    // sms update <student_no> <major> <name>
    static void HELP();
    static bool ensureDbReady();
};
