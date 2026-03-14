# Student Management System (CLI + PostgreSQL)

基于 C++ 的命令行学生管理系统，当前版本使用 PostgreSQL 持久化数据，支持通过 `sms` 命令进行增删改查。

## 功能

- `sms add <major> <name> <student_no>`：新增学生
- `sms list`：列出全部学生
- `sms get <student_no>`：按学号查询
- `sms del <student_no>`：按学号删除
- `sms update <student_no> <major> <name>`：按学号更新专业和姓名
- `sms help`：查看帮助

## 数据库说明

- 使用 PostgreSQL 客户端 `libpq`。
- 程序首次执行命令时会自动连接数据库，并自动建表（若不存在）：
  - 表名：`sms_test04`
  - 字段：`student_no`（主键）、`major`、`name`

## 构建与运行

```bash
cmake -S . -B build
cmake --build build
./build/StudentManagementSystem
```

Windows PowerShell：

```powershell
cmake -S . -B build
cmake --build build
.\build\StudentManagementSystem.exe
```

## 命令示例

```text
sms help
sms add CS "Zhang San" 20260001
sms get 20260001
sms update 20260001 AI "Li Si"
sms list
sms del 20260001
```

## 项目结构（核心）

- `src/obj/main.cpp`：程序入口，命令行循环
- `src/obj/CmdHandler.cpp`：命令解析与数据库操作调度
- `src/obj/DatabaseOps.cpp`：PostgreSQL 数据库读写实现
- `src/Head/Student.h`：学生实体定义

## 说明

- `src/data/students.json` 已可用于数据库导入测试（通过 `DatabaseOps::importStudentsFromJsonFile`），当前尚未提供独立命令（如 `sms import`）。
