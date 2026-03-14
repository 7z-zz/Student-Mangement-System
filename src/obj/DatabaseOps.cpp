#include "../Head/DatabaseOps.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>

namespace
{
    // 执行无结果集 SQL。
    bool execCommand(PGconn* conn, const std::string& sql)
    {
        if (!conn)
        {
            std::cerr << "[DatabaseOps] conn is null.\n";
            return false;
        }

        PGresult* res = PQexec(conn, sql.c_str());
        const bool ok = (PQresultStatus(res) == PGRES_COMMAND_OK);
        if (!ok)
        {
            std::cerr << "[DatabaseOps] SQL failed: " << sql << "\n"
                      << PQerrorMessage(conn);
        }
        PQclear(res);
        return ok;
    }

    // 执行带参数 SQL（INSERT/UPDATE/DELETE）。
    bool execParamsCommand(PGconn* conn, const char* sql, const int n_params, const char* const* values)
    {
        if (!conn)
        {
            std::cerr << "[DatabaseOps] conn is null.\n";
            return false;
        }

        PGresult* res = PQexecParams(conn, sql, n_params, nullptr, values, nullptr, nullptr, 0);
        const bool ok = (PQresultStatus(res) == PGRES_COMMAND_OK);
        if (!ok)
        {
            std::cerr << "[DatabaseOps] SQL failed: " << sql << "\n"
                      << PQerrorMessage(conn);
        }
        PQclear(res);
        return ok;
    }
}

PGconn* DatabaseOps::connect(const std::string& conninfo)
{
    PGconn* conn = PQconnectdb(conninfo.c_str());
    if (PQstatus(conn) != CONNECTION_OK)
    {
        std::cerr << "[DatabaseOps] connect failed: " << PQerrorMessage(conn);
        PQfinish(conn);
        return nullptr;
    }
    return conn;
}

void DatabaseOps::disconnect(PGconn* conn)
{
    if (conn)
    {
        PQfinish(conn);
    }
}

bool DatabaseOps::resetStudentTable(PGconn* conn)
{
    if (!execCommand(conn, "DROP TABLE IF EXISTS sms_test04;"))
    {
        return false;
    }

    return execCommand(
        conn,
        "CREATE TABLE sms_test04 ("
        "student_no TEXT PRIMARY KEY, "
        "major TEXT NOT NULL, "
        "name TEXT NOT NULL);");
}

bool DatabaseOps::ensureStudentTable(PGconn* conn)
{
    return execCommand(
        conn,
        "CREATE TABLE IF NOT EXISTS sms_test04 ("
        "student_no TEXT PRIMARY KEY, "
        "major TEXT NOT NULL, "
        "name TEXT NOT NULL);");
}

bool DatabaseOps::insertStudent(PGconn* conn, const Student& student)
{
    const char* values[3] = {
        student.Get_studentNo().c_str(),
        student.Get_major().c_str(),
        student.Get_studentName().c_str()
    };

    return execParamsCommand(
        conn,
        "INSERT INTO sms_test04 (student_no, major, name) VALUES ($1, $2, $3);",
        3,
        values);
}

std::vector<Student> DatabaseOps::queryAllStudents(PGconn* conn)
{
    std::vector<Student> students;
    if (!conn)
    {
        std::cerr << "[DatabaseOps] conn is null.\n";
        return students;
    }

    PGresult* res = PQexec(conn, "SELECT major, name, student_no FROM sms_test04 ORDER BY student_no;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        std::cerr << "[DatabaseOps] SELECT failed: " << PQerrorMessage(conn);
        PQclear(res);
        return students;
    }

    for (int i = 0; i < PQntuples(res); ++i)
    {
        students.emplace_back(
            PQgetvalue(res, i, 0),
            PQgetvalue(res, i, 1),
            PQgetvalue(res, i, 2));
    }

    PQclear(res);
    return students;
}

bool DatabaseOps::queryStudentByNo(PGconn* conn, const std::string& student_no, Student& out_student)
{
    if (!conn)
    {
        std::cerr << "[DatabaseOps] conn is null.\n";
        return false;
    }

    const char* values[1] = {student_no.c_str()};
    PGresult* res = PQexecParams(
        conn,
        "SELECT major, name, student_no FROM sms_test04 WHERE student_no = $1;",
        1,
        nullptr,
        values,
        nullptr,
        nullptr,
        0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        std::cerr << "[DatabaseOps] SELECT by no failed: " << PQerrorMessage(conn);
        PQclear(res);
        return false;
    }

    if (PQntuples(res) == 0)
    {
        PQclear(res);
        return false;
    }

    out_student = Student(
        PQgetvalue(res, 0, 0),
        PQgetvalue(res, 0, 1),
        PQgetvalue(res, 0, 2));
    PQclear(res);
    return true;
}

bool DatabaseOps::updateStudentByNo(PGconn* conn, const Student& student)
{
    const char* values[3] = {
        student.Get_major().c_str(),
        student.Get_studentName().c_str(),
        student.Get_studentNo().c_str()
    };

    PGresult* res = PQexecParams(
        conn,
        "UPDATE sms_test04 SET major = $1, name = $2 WHERE student_no = $3;",
        3,
        nullptr,
        values,
        nullptr,
        nullptr,
        0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::cerr << "[DatabaseOps] SQL failed: UPDATE by student_no\n"
                  << PQerrorMessage(conn);
        PQclear(res);
        return false;
    }

    const int affected = std::atoi(PQcmdTuples(res));
    PQclear(res);
    return affected > 0;
}

bool DatabaseOps::deleteStudentByNo(PGconn* conn, const std::string& student_no)
{
    const char* values[1] = {student_no.c_str()};
    PGresult* res = PQexecParams(
        conn,
        "DELETE FROM sms_test04 WHERE student_no = $1;",
        1,
        nullptr,
        values,
        nullptr,
        nullptr,
        0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::cerr << "[DatabaseOps] SQL failed: DELETE by student_no\n"
                  << PQerrorMessage(conn);
        PQclear(res);
        return false;
    }

    const int affected = std::atoi(PQcmdTuples(res));
    PQclear(res);
    return affected > 0;
}

bool DatabaseOps::importStudentsFromJsonFile(PGconn* conn, const std::string& json_path, const bool reset_table)
{
    if (!conn)
    {
        std::cerr << "[DatabaseOps] conn is null.\n";
        return false;
    }

    if (reset_table && !resetStudentTable(conn))
    {
        return false;
    }

    std::ifstream in(json_path);
    if (!in.is_open())
    {
        std::cerr << "[DatabaseOps] open json failed: " << json_path << "\n";
        return false;
    }

    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    const std::regex object_regex(R"(\{[^{}]*\})");
    const std::regex kv_regex(R"KV("([^"]+)"\s*:\s*"([^"]*)")KV");

    std::vector<Student> students;
    for (std::sregex_iterator it(content.begin(), content.end(), object_regex), end; it != end; ++it)
    {
        const std::string obj = it->str();
        std::unordered_map<std::string, std::string> kv;

        for (std::sregex_iterator kv_it(obj.begin(), obj.end(), kv_regex), kv_end; kv_it != kv_end; ++kv_it)
        {
            kv[(*kv_it)[1].str()] = (*kv_it)[2].str();
        }

        if (kv.contains("major") && kv.contains("name") && kv.contains("no"))
        {
            students.emplace_back(kv["major"], kv["name"], kv["no"]);
        }
    }

    if (students.empty())
    {
        std::cerr << "[DatabaseOps] no valid students found in json.\n";
        return false;
    }

    int imported = 0;
    for (const auto& s : students)
    {
        const char* values[3] = {
            s.Get_studentNo().c_str(),
            s.Get_major().c_str(),
            s.Get_studentName().c_str()
        };

        PGresult* res = PQexecParams(
            conn,
            "INSERT INTO sms_test04 (student_no, major, name) VALUES ($1, $2, $3) "
            "ON CONFLICT (student_no) DO UPDATE SET major = EXCLUDED.major, name = EXCLUDED.name;",
            3,
            nullptr,
            values,
            nullptr,
            nullptr,
            0);

        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            std::cerr << "[DatabaseOps] import failed for student_no=" << s.Get_studentNo() << "\n"
                      << PQerrorMessage(conn);
            PQclear(res);
            return false;
        }
        PQclear(res);
        ++imported;
    }

    std::cout << "[DatabaseOps] import finished, rows: " << imported << "\n";
    return true;
}
