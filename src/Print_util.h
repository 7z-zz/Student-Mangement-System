#pragma once
#include <map>

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

template <typename K, typename V>
void PrintContainer(const std::unordered_map<K, V>& m, std::ostream& os = std::cout)
{
    for (const auto& [key, value] : m)
    {
        os << value << "\n";
    }
}
