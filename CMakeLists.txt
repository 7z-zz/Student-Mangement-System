cmake_minimum_required(VERSION 4.1)
project(StudentManagementSystem)

set(CMAKE_CXX_STANDARD 23)

add_executable(StudentManagementSystem
        src/main.cpp
        src/Student.h
        src/Student.cpp
        src/Print_util.h
        src/CmdHandelr.cpp
        src/CmdHandelr.h)
