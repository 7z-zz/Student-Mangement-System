#pragma once
#include <string>

class Student
{
    public:
        Student(std::string major, std::string name, std::string no);

    [[nodiscard]] const std::string& Get_major() const;
    [[nodiscard]] const std::string& Get_studentName() const;
    [[nodiscard]] const std::string& Get_studentNo() const;

    private:
    std::string major_;                  //专业
    std::string student_name_;           //名字
    std::string student_no_;            //学号
    friend std::ostream& operator<<(std::ostream& os, const Student& s);
};