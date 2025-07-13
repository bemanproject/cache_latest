# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

include(GoogleTest)

add_executable(beman.cache_latest.tests.cache_latest)

target_sources(
    beman.cache_latest.tests.cache_latest
    PRIVATE cache_latest.test.cpp
)
target_link_libraries(
    beman.cache_latest.tests.cache_latest
    PRIVATE beman::cache_latest GTest::gtest GTest::gtest_main
)

gtest_add_tests(beman.cache_latest.tests.cache_latest "" AUTO)
