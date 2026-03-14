#pragma once

#include <libpq-fe.h>

#include <string>
#include <vector>

#include "Student.h"

namespace DatabaseOps
{
    // 创建数据库连接，成功返回 PGconn*，失败返回 nullptr。
    PGconn* connect(const std::string& conninfo);

    // 关闭数据库连接，允许传入 nullptr。
    void disconnect(PGconn* conn);

    // 创建学生表（如果不存在）。
    bool ensureStudentTable(PGconn* conn);

    // 重新创建学生测试表（会先删除旧表）。
    bool resetStudentTable(PGconn* conn);

    // 插入一条学生记录，数据直接来自 Student 对象。
    bool insertStudent(PGconn* conn, const Student& student);

    // 查询全部学生记录，并返回 Student 对象列表。
    std::vector<Student> queryAllStudents(PGconn* conn);

    // 按学号查询单个学生，成功时返回 true 并填充 out_student。
    bool queryStudentByNo(PGconn* conn, const std::string& student_no, Student& out_student);

    // 根据学号更新学生记录（major/name）。
    bool updateStudentByNo(PGconn* conn, const Student& student);

    // 根据学号删除学生记录。
    bool deleteStudentByNo(PGconn* conn, const std::string& student_no);

    // 从 JSON 文件自动导入学生数据，reset_table=true 时会先重建表。
    bool importStudentsFromJsonFile(PGconn* conn, const std::string& json_path, bool reset_table = true);
}
