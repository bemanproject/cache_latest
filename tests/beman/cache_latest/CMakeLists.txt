# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

find_package(GTest REQUIRED)

add_executable(beman.cache_latest.tests.cache_latest)
target_sources(
    beman.cache_latest.tests.cache_latest
    PRIVATE cache_latest.test.cpp
)
target_link_libraries(
    beman.cache_latest.tests.cache_latest
    PRIVATE beman::cache_latest GTest::gtest GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(beman.cache_latest.tests.cache_latest DISCOVERY_TIMEOUT 60)
