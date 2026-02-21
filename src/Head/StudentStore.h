//此文件 是存储接口，提供了基础的函数，暂未实现所有功能。
#pragma once

#include "Student.h"
#include <functional>
#include <string>
#include <utility>

class Student_Store
{
    public:
        bool add(const Student& s);
        void forEach(const std::function<void(const Student&)>& f) const;
    private:
        std::unordered_map<std::string, Student> Store_;
};