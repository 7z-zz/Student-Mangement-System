#include "../Head/CmdHandler.h"

#include "../Head/DatabaseOps.h"

#include <algorithm>
#include <cctype>
#include <iostream>

namespace
{
    PGconn* g_conn = nullptr;
    void ignoreNotice(void*, const char*) {}

    std::string toLower(std::string s)
    {
        std::ranges::transform(s.begin(), s.end(), s.begin(),
                               [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return s;
    }

    std::string buildConnInfo()
    {
        const char* conninfo_env = std::getenv("PG_CONNINFO");
        if (conninfo_env && *conninfo_env)
        {
            return conninfo_env;
        }
        return "host=127.0.0.1 port=5432 dbname=my_postgresqldb user=sku7z password=sku7z";
    }

    void stripUtf8Bom(std::string& s)
    {
        if (s.size() >= 3 &&
            static_cast<unsigned char>(s[0]) == 0xEF &&
            static_cast<unsigned char>(s[1]) == 0xBB &&
            static_cast<unsigned char>(s[2]) == 0xBF)
        {
            s.erase(0, 3);
        }

        while (!s.empty() && !std::isalpha(static_cast<unsigned char>(s.front())))
        {
            s.erase(s.begin());
        }
    }
}

bool CmdHandler::ensureDbReady()
{
    if (g_conn && PQstatus(g_conn) == CONNECTION_OK)
    {
        return true;
    }

    if (g_conn)
    {
        DatabaseOps::disconnect(g_conn);
        g_conn = nullptr;
    }

    g_conn = DatabaseOps::connect(buildConnInfo());
    if (!g_conn)
    {
        std::cout << "[FAIL] database connection failed.\n";
        return false;
    }
    PQsetNoticeProcessor(g_conn, ignoreNotice, nullptr);
    PQexec(g_conn, "SET client_min_messages TO warning;");

    if (!DatabaseOps::ensureStudentTable(g_conn))
    {
        std::cout << "[FAIL] failed to prepare student table.\n";
        return false;
    }

    return true;
}

void CmdHandler::execute(const std::string& line)
{
    auto tokens = tokenize(line);
    if (tokens.empty())
    {
        return;
    }

    stripUtf8Bom(tokens[0]);
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
    if (sub == "help")
    {
        HELP();
        return;
    }

    if (!ensureDbReady())
    {
        return;
    }

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

    if (DatabaseOps::insertStudent(g_conn, Student(major, name, no)))
    {
        std::cout << "[OK] added: " << no << " " << name << " (" << major << ")\n";
    }
    else
    {
        std::cout << "[FAIL] student_no already exists or SQL failed: " << no << "\n";
    }
}

void CmdHandler::LIST(const std::vector<std::string>& tokens)
{
    if (tokens.size() != 2)
    {
        std::cout << "Usage: sms list\n";
        return;
    }

    const auto students = DatabaseOps::queryAllStudents(g_conn);
    if (students.empty())
    {
        std::cout << "(empty)\n";
        return;
    }

    for (const auto& s : students)
    {
        std::cout << "major: " << s.Get_major() << "\t"
                  << "name: " << s.Get_studentName() << "\t"
                  << "student_no: " << s.Get_studentNo() << "\n";
    }
    std::cout << "Total: " << students.size() << "\n";
}

void CmdHandler::GET(const std::vector<std::string>& tokens)
{
    if (tokens.size() != 3)
    {
        std::cout << "Usage: sms get <student_no>\n";
        return;
    }

    const std::string& no = tokens[2];
    Student student("", "", "");
    if (!DatabaseOps::queryStudentByNo(g_conn, no, student))
    {
        std::cout << "[FAIL] not found: " << no << "\n";
        return;
    }

    std::cout << "major: " << student.Get_major() << "\t"
              << "name: " << student.Get_studentName() << "\t"
              << "student_no: " << student.Get_studentNo() << "\n";
}

void CmdHandler::DEL(const std::vector<std::string>& tokens)
{
    if (tokens.size() != 3)
    {
        std::cout << "Usage: sms del <student_no>\n";
        return;
    }

    const std::string& no = tokens[2];
    if (DatabaseOps::deleteStudentByNo(g_conn, no))
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

    if (DatabaseOps::updateStudentByNo(g_conn, Student(major, name, no)))
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
