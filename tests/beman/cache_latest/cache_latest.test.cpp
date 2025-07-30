#include <beman/cache_latest/cache_latest.hpp>
#include <gtest/gtest.h>
#include <ranges>
#include <vector>

TEST(CallTest, TestIterator) {
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto even_squares  = v | std::views::transform([](int i) { return i * i; }) | beman::views::cache_latest;
    auto iter          = even_squares.begin();
    EXPECT_EQ(*iter, 1);
}
