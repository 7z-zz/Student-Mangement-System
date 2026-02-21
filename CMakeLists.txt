cmake_minimum_required(VERSION 4.1)
project(StudentManagementSystem)

set(CMAKE_CXX_STANDARD 23)

add_executable(StudentManagementSystem
        src/obj/main.cpp
        src/Head/Student.h
        src/obj/Student.cpp
        src/Head/Print_util.h
        src/obj/CmdHandler.cpp
        src/Head/CmdHandler.h
        src/obj/Studentstore.cpp
        src/Head/Studentstore.h)
