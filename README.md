# Student Management System (CLI)

一个基于 C++ 的命令行学生信息管理系统，支持基础增删改查（CRUD）。

## 功能概览

- `add`：新增学生
- `list`：查看所有学生
- `get`：按学号查询学生
- `del`：按学号删除学生
- `update`：按学号更新学生专业与姓名
- `help`：查看帮助

当前版本使用内存存储，程序退出后数据不会持久化。

## 数据字段

- `major`：专业
- `name`：姓名
- `student_no`：学号（唯一键）

## 命令格式

统一前缀：`sms`

```text
sms add <major> <name> <student_no>
sms list
sms get <student_no>
sms del <student_no>
sms update <student_no> <major> <name>
sms help
```

如果姓名里有空格，请使用双引号：

```text
sms add CS "Zhang San" 20260001
sms update 20260001 AI "Li Si"
```

## 使用示例

```text
sms add CS "Zhang San" 20260001
sms add Math WangWu 20260002
sms list
sms get 20260001
sms update 20260001 AI "Li Si"
sms del 20260002
sms help
```

## 构建与运行

在项目根目录执行：

```bash
cmake -S . -B build
cmake --build build
./build/StudentManagementSystem
```

Windows PowerShell 可执行：

```powershell
cmake -S . -B build
cmake --build build
.\build\StudentManagementSystem.exe
```

启动后可持续输入命令，按 `Ctrl + C` 退出。

## 目录结构（核心）

- `src/obj/main.cpp`：程序入口，读取命令并分发
- `src/obj/CmdHandler.cpp`：命令解析与执行
- `src/obj/StudentStore.cpp`：学生数据存储与 CRUD
- `src/obj/Student.cpp`：学生对象定义

## 已知限制

- 数据仅保存在内存中，重启程序会丢失。
- 当前没有学号格式校验（只要求唯一）。
- `list` 输出顺序由 `unordered_map` 决定，不保证固定顺序。

## 后续建议

- 增加文件持久化（CSV / JSON / SQLite）。
- 增加按姓名、专业筛选查询。
- 增加输入校验与更详细的错误提示。
