#include "../Head/StudentStore.h"

#include <ranges>

bool Student_Store::add(const Student& s)
{
    const std::string& no = s.Get_studentNo();
    auto [it, inserted] = Store_.try_emplace(no, s);
    return inserted;
}

bool Student_Store::remove(const std::string& student_no)
{
    return Store_.erase(student_no) > 0;
}

bool Student_Store::update(const std::string& student_no, const std::string& major, const std::string& name)
{
    auto it = Store_.find(student_no);
    if (it == Store_.end())
    {
        return false;
    }

    it->second = Student(major, name, student_no);
    return true;
}

const Student* Student_Store::find(const std::string& student_no) const
{
    auto it = Store_.find(student_no);
    if (it == Store_.end())
    {
        return nullptr;
    }

    return &it->second;
}

std::size_t Student_Store::size() const
{
    return Store_.size();
}

void Student_Store::forEach(const std::function<void(const Student&)>& f) const
{
    if (!f)
    {
        return;
    }

    for (const auto& student : Store_ | std::views::values)
    {
        f(student);
    }
}
