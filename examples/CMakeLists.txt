# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(ALL_EXAMPLES identity_direct_usage)

message("Examples to be built: ${ALL_EXAMPLES}")

foreach(example ${ALL_EXAMPLES})
    add_executable(beman.cache_latest.examples.${example})
    target_sources(
        beman.cache_latest.examples.${example}
        PRIVATE ${example}.cpp
    )
    target_link_libraries(
        beman.cache_latest.examples.${example}
        PRIVATE beman::cache_latest
    )
endforeach()
