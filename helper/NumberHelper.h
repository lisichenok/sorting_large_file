#pragma once
#include <random>

namespace NumberHelper {
template<typename Type>
auto getRandomNum()
{
    static std::default_random_engine dre;
    auto min = std::numeric_limits<Type>::min();
    auto max = std::numeric_limits<Type>::max();
    std::uniform_int_distribution<Type> value(min, max);
    return value(dre);
}
}