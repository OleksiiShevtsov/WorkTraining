#pragma once

#include <iostream>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <concepts>

#include <cstdio>
#include <cstdint>
#include <cstddef>

template<class T>
concept IterToComparable =
    requires(T a, T b) {
        {*a < *b} -> std::convertible_to<bool>;
    };

template<IterToComparable InputIt>
void SortDefaultComparator(InputIt begin, InputIt end) {
    std::sort(begin, end);
}

struct X {
    auto operator<=>(const X&) const = default;
    int a;
};

void checkConcept2() {
    std::vector<X> v = { {10}, {9}, {11} };
    SortDefaultComparator(v.begin(), v.end());
}
