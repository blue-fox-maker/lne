#include <cassert>
#include <vector>
#include "../temporal.hpp"
using namespace Temporal;

template <typename T>
void unit_test();

template <>
void unit_test<Intervals>()
{
    Intervals a;
    Intervals b;

    // Test case 1: two intervals with no overlap
    a = {{1, 3}, {6, 9}};
    b = {{4, 5}, {10, 12}};
    assert(Intervals::combine(a, b) == Intervals({{1, 3}, {4, 5}, {6, 9}, {10, 12}}));
    assert(Intervals::combine(b, a) == Intervals({{1, 3}, {4, 5}, {6, 9}, {10, 12}}));
    assert(Intervals::intersection(a, b) == Intervals());
    assert(Intervals::intersection(b, a) == Intervals());
    assert(Intervals::subtract(a, b) == a);
    assert(Intervals::subtract(b, a) == b);

    // Test case 2: two intervals with overlapping intervals
    a = {{1, 4}, {6, 9}};
    b = {{3, 7}, {8, 12}};
    assert(Intervals::combine(a, b) == Intervals({{1, 12}}));
    assert(Intervals::combine(b, a) == Intervals({{1, 12}}));
    assert(Intervals::intersection(a, b) == Intervals({{3, 4}, {6, 7}, {8, 9}}));
    assert(Intervals::intersection(b, a) == Intervals({{3, 4}, {6, 7}, {8, 9}}));
    assert(Intervals::subtract(a, b) == Intervals({{1, 3}, {7, 8}}));
    assert(Intervals::subtract(b, a) == Intervals({{4, 6}, {9, 12}}));

    // Test case 3: many intervals are completely inside a interval
    a = {{1, 10}, {12, 15}, {17, 20}};
    b = {{2, 3}, {4, 5}, {10, 20}};
    auto res = Intervals::subtract(a, b);
    assert(Intervals::combine(a, b) == Intervals({{1, 20}}));
    assert(Intervals::combine(b, a) == Intervals({{1, 20}}));
    assert(Intervals::intersection(a, b) == Intervals({{2, 3}, {4, 5}, {12, 15}, {17, 20}}));
    assert(Intervals::intersection(b, a) == Intervals({{2, 3}, {4, 5}, {12, 15}, {17, 20}}));
    assert(Intervals::subtract(a, b) == Intervals({{1, 2}, {3, 4}, {5, 10}}));
    assert(Intervals::subtract(b, a) == Intervals({{10, 12}, {15, 17}}));

    // Test case 4: a interval starts when another ends
    a = {{1, 3}, {6, 9}, {10, 12}, {15, 17}};
    b = {{2, 6}, {10, 15}, {17, 20}};
    assert(Intervals::combine(a, b) == Intervals({{1, 9}, {10, 20}}));
    assert(Intervals::combine(b, a) == Intervals({{1, 9}, {10, 20}}));
    assert(Intervals::intersection(a, b) == Intervals({{2, 3}, {10, 12}}));
    assert(Intervals::intersection(b, a) == Intervals({{2, 3}, {10, 12}}));
    assert(Intervals::subtract(a, b) == Intervals({{1, 2}, {6, 9}, {15, 17}}));
    assert(Intervals::subtract(b, a) == Intervals({{3, 6}, {12, 15}, {17, 20}}));

    // Test case 5: two empty intervals
    a = {};
    b = {};
    assert(Intervals::combine(a, b) == Intervals());
    assert(Intervals::combine(b, a) == Intervals());
    assert(Intervals::intersection(a, b) == Intervals());
    assert(Intervals::intersection(b, a) == Intervals());
    assert(Intervals::subtract(a, b) == Intervals());
    assert(Intervals::subtract(b, a) == Intervals());

    std::cout << "--------------------"
              << "test finished" << std::endl;
}
