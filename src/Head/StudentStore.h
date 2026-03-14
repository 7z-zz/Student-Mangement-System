//此文件 是存储接口，提供了基础的函数，暂未实现所有功能。
#pragma once

#include "Student.h"
#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>

class Student_Store
{
    public:
        bool add(const Student& s);
        bool remove(const std::string& student_no);
        bool update(const std::string& student_no, const std::string& major, const std::string& name);
        const Student* find(const std::string& student_no) const;
        std::size_t size() const;
        void forEach(const std::function<void(const Student&)>& f) const;
    private:
        std::unordered_map<std::string, Student> Store_;
};