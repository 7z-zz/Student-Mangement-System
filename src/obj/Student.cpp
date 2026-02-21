#include "../Head/Student.h"

#include <ostream>

Student::Student(std::string major, std::string name, std::string no)
        : major_(std::move(major)), student_name_(std::move(name)), student_no_(std::move(no)){}

const std::string& Student::Get_major() const
{
        return major_;
}
const std::string& Student::Get_studentName() const
{
        return student_name_;
}
const std::string& Student::Get_studentNo() const
{
        return student_no_;
}
std::ostream& operator<<(std::ostream& os, const Student& s)
{
        os << "专业: " << s.major_ <<" "
           << "姓名: " << s.student_name_ <<" "
           << "学号: " << s.student_no_ << std::endl;
        return os;
}
