#pragma once
#include "../Head/StudentStore.h"
#include <ranges>

bool Student_Store::add(const Student& s)
{
    // 以学号作为 key
    const std::string& no = s.Get_studentNo();

    // try_emplace：key 不存在才插入；存在则不插入
    // 返回 pair<iterator, bool>，second 表示是否成功插入
    auto [it, inserted] = Store_.try_emplace(no, s);
    return inserted;
}
//参数 std::function<void(const Student&)> f 它接受一个包装好的可调用对象（可以是 Lambda 表达式、函数指针或仿函数）。
//该函数必须接受一个 const Student&（对学生对象的常量引用）作为输入，且没有返回值。
void Student_Store::forEach(const std::function<void(const Student&)>& f) const
{
    // 保护：传入空函数就不做任何事（可选）
    if (!f) return;
    //| 是管道操作符和Linux中的一样让 左边 的集合 传入 右边 的过滤器
    for (const auto& student : Store_ | std::views::values)
    {
        f(student);
    }
}