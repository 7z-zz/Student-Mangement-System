//此文件夹用于重构 << 操作符号
//可以使用此文件直接打印类的所有成员。

#pragma once
#include <iostream>
#include <map>
#include <ranges>
#include <unordered_map>

// 适用于 sequence 容器
template <typename Container>
void PrintContainer(const Container& c, std::ostream& os = std::cout)
{
    for (const auto& item : c)
    {
        os << item << "\n";
    }
}

// 专门针对 map
template <typename K, typename V>
void PrintContainer(const std::map<K, V>& m, std::ostream& os = std::cout)
{
    for (const auto& [key, value] : m)
    {
        os << value << "\n";
    }
}

//专门针对unordered_map
template <typename K, typename V>
void PrintContainer(const std::unordered_map<K, V>& m, std::ostream& os = std::cout)
{
    for (const auto& [key, value] : m)
    {
        os << value << "\n";
    }
}
