#include "../Head/CmdHandler.h"
#include "../Head/StudentStore.h"

#include <algorithm>
#include <cctype>
#include <iostream>

static Student_Store g_store;

static std::string toLower(std::string s)
{
    std::ranges::transform(s.begin(), s.end(), s.begin(),
                           [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

void CmdHandler::execute(const std::string& line)
{
    auto tokens = tokenize(line);
    if (tokens.empty())
    {
        return;
    }

    const std::string app = toLower(tokens[0]);
    if (app != "sms")
    {
        std::cout << "Unknown app command: " << tokens[0] << "\n";
        std::cout << "Type: sms help\n";
        return;
    }

    if (tokens.size() == 1)
    {
        std::cout << "Missing subcommand.\n";
        std::cout << "Type: sms help\n";
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
    else if (sub == "get")
    {
        GET(tokens);
    }
    else if (sub == "del")
    {
        DEL(tokens);
    }
    else if (sub == "update")
    {
        UPDATE(tokens);
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

    for (char ch : line)
    {
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
    {
        out.push_back(cur);
    }

    return out;
}

void CmdHandler::ADD(const std::vector<std::string>& tokens)
{
    if (tokens.size() != 5)
    {
        std::cout << "Usage: sms add <major> <name> <student_no>\n";
        std::cout << "Example: sms add CS \"Zhang San\" 20260001\n";
        return;
    }

    const std::string& major = tokens[2];
    const std::string& name = tokens[3];
    const std::string& no = tokens[4];

    if (g_store.add(Student(major, name, no)))
    {
        std::cout << "[OK] added: " << no << " " << name << " (" << major << ")\n";
    }
    else
    {
        std::cout << "[FAIL] student_no already exists: " << no << "\n";
    }
}

void CmdHandler::LIST(const std::vector<std::string>& tokens)
{
    if (tokens.size() != 2)
    {
        std::cout << "Usage: sms list\n";
        return;
    }

    if (g_store.size() == 0)
    {
        std::cout << "(empty)\n";
        return;
    }

    g_store.forEach([](const Student& s)
    {
        std::cout << "major: " << s.Get_major() << "\t"
                  << "name: " << s.Get_studentName() << "\t"
                  << "student_no: " << s.Get_studentNo() << "\n";
    });

    std::cout << "Total: " << g_store.size() << "\n";
}

void CmdHandler::GET(const std::vector<std::string>& tokens)
{
    if (tokens.size() != 3)
    {
        std::cout << "Usage: sms get <student_no>\n";
        return;
    }

    const std::string& no = tokens[2];
    const Student* s = g_store.find(no);
    if (!s)
    {
        std::cout << "[FAIL] not found: " << no << "\n";
        return;
    }

    std::cout << "major: " << s->Get_major() << "\t"
              << "name: " << s->Get_studentName() << "\t"
              << "student_no: " << s->Get_studentNo() << "\n";
}

void CmdHandler::DEL(const std::vector<std::string>& tokens)
{
    if (tokens.size() != 3)
    {
        std::cout << "Usage: sms del <student_no>\n";
        return;
    }

    const std::string& no = tokens[2];
    if (g_store.remove(no))
    {
        std::cout << "[OK] deleted: " << no << "\n";
    }
    else
    {
        std::cout << "[FAIL] not found: " << no << "\n";
    }
}

void CmdHandler::UPDATE(const std::vector<std::string>& tokens)
{
    if (tokens.size() != 5)
    {
        std::cout << "Usage: sms update <student_no> <major> <name>\n";
        std::cout << "Example: sms update 20260001 AI \"Li Si\"\n";
        return;
    }

    const std::string& no = tokens[2];
    const std::string& major = tokens[3];
    const std::string& name = tokens[4];

    if (g_store.update(no, major, name))
    {
        std::cout << "[OK] updated: " << no << " -> " << name << " (" << major << ")\n";
    }
    else
    {
        std::cout << "[FAIL] not found: " << no << "\n";
    }
}

void CmdHandler::HELP()
{
    std::cout << "Commands:\n";
    std::cout << "  sms add <major> <name> <student_no>\n";
    std::cout << "  sms list\n";
    std::cout << "  sms get <student_no>\n";
    std::cout << "  sms del <student_no>\n";
    std::cout << "  sms update <student_no> <major> <name>\n";
    std::cout << "  sms help\n";
    std::cout << "Tip: if name contains spaces, wrap with quotes: \"Zhang San\"\n";
}
