#include "../Head/CmdHandler.h"
#include "../Head/StudentStore.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>

// 作为“内存版数据库”
// 后续你接 PostgreSQL，可以把 Student_Store 改成 DBStore，CmdHandler 不用大改
static Student_Store g_store;

// 小工具：转小写（用于命令不区分大小写）
static std::string toLower(std::string s)
{
    std::ranges::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

void CmdHandler::execute(const std::string& line)
{
    auto tokens = tokenize(line);
    if (tokens.empty()) return;

    // 例：sms add ... / sms list
    // 你注释里写的是 sms add <major> <name> <student_no>
    // 所以 tokens[0] 应该是 "sms"
    const std::string app = toLower(tokens[0]);
    if (app != "sms")
    {
        std::cout << "Unknown app command: " << tokens[0] << "\n";
        std::cout << "Usage: sms add <major> <name> <student_no>\n";
        std::cout << "       sms list\n";
        return;
    }

    if (tokens.size() == 1)
    {
        std::cout << "Missing subcommand.\n";
        std::cout << "Usage: sms add <major> <name> <student_no>\n";
        std::cout << "       sms list\n";
        return;
    }

    const std::string sub = toLower(tokens[1]);

    if (sub == "add")
    {
        ADD(tokens);
    }
    else if (sub == "list")
    {
        LIST(tokens);
    }
    else if (sub == "help")
    {
        HELP();
    }
    else
    {
        std::cout << "Unknown subcommand: " << tokens[1] << "\n";
        std::cout << "Type: sms help\n";
    }
}

std::vector<std::string> CmdHandler::tokenize(const std::string& line)
{
    std::vector<std::string> out;
    std::string cur;
    bool in_quotes = false;

    for (size_t i = 0; i < line.size(); ++i)
    {
        char ch = line[i];

        if (ch == '"')
        {
            in_quotes = !in_quotes;
            continue;
        }

        if (!in_quotes && std::isspace(static_cast<unsigned char>(ch)))
        {
            if (!cur.empty())
            {
                out.push_back(cur);
                cur.clear();
            }
            continue;
        }

        cur.push_back(ch);
    }

    if (!cur.empty())
        out.push_back(cur);

    return out;
}

// sms add <major> <name> <student_no>
void CmdHandler::ADD(const std::vector<std::string>& tokens)
{
    // tokens: [0]=sms [1]=add [2]=major [3]=name [4]=student_no
    if (tokens.size() != 5)
    {
        std::cout << "Usage: sms add <major> <name> <student_no>\n";
        std::cout << "Example: sms add CS \"Zhang San\" 20260001\n";
        return;
    }

    const std::string& major = tokens[2];
    const std::string& name  = tokens[3];
    const std::string& no    = tokens[4];

    // ✅ 这里根据你的 Student 类来改：
    // 方案A：如果你有构造函数 Student(major, name, no)
    Student s(major, name, no);

    // 方案B：如果你没有构造函数，而是 setter，就改成类似：
    // Student s;
    // s.Set_major(major);
    // s.Set_name(name);
    // s.Set_studentNo(no);

    const bool ok = g_store.add(s);
    if (ok)
        std::cout << "[OK] added: " << no << " " << name << " (" << major << ")\n";
    else
        std::cout << "[FAIL] student_no already exists: " << no << "\n";
}

// sms list
void CmdHandler::LIST(const std::vector<std::string>& tokens)
{
    if (tokens.size() != 2)
    {
        std::cout << "Usage: sms list\n";
        return;
    }

    int count = 0;
    g_store.forEach([&](const Student& s) {
        // ✅ 这里根据你的 Student getter 来改：
        // 假设有 Get_studentNo / Get_name / Get_major
        std::cout << "专业：" << s.Get_major() << "\t"
                  << "名字：" << s.Get_studentName() << "\t"
                  << "学号：" <<s.Get_studentNo() << "\n";
        ++count;
    });

    std::cout << "Total: " << count << "\n";
}

//sms help
void CmdHandler::HELP()
{
    std::cout << "Commands:\n";
    std::cout << "  sms add <major> <name> <student_no>\n";
    std::cout << "  sms list\n";
    std::cout << "Tip: if name contains spaces, wrap with quotes: \"Zhang San\"\n";
}