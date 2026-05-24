# beman.cache_latest: A Beman Library

<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

<!-- markdownlint-disable line-length -->
[![Library Status](https://raw.githubusercontent.com/bemanproject/beman/refs/heads/main/images/badges/beman_badge-beman_library_under_development.svg)](https://github.com/bemanproject/beman/blob/main/docs/beman_library_maturity_model.md#the-beman-library-maturity-model)
[![Continuous Integration Tests](https://github.com/bemanproject/cache_latest/actions/workflows/ci_tests.yml/badge.svg)](https://github.com/bemanproject/cache_latest/actions/workflows/ci_tests.yml)
[![Lint Check (pre-commit)](https://github.com/bemanproject/cache_latest/actions/workflows/pre-commit-check.yml/badge.svg)](https://github.com/bemanproject/cache_latest/actions/workflows/pre-commit-check.yml)
[![Coverage](https://coveralls.io/repos/github/bemanproject/cache_latest/badge.svg?branch=main)](https://coveralls.io/github/bemanproject/cache_latest?branch=main)
![Standard Target](https://github.com/bemanproject/beman/blob/main/images/badges/cpp26.svg)
<!-- markdownlint-restore -->

<!-- markdownlint-disable-next-line line-length -->
`beman.cache_latest` is a C++ ranges adaptor that caches the result of the last dereference of the underlying iterator. The reason for doing this is efficiency - specifically avoiding extra iterator dereferences.

The library conforms to [The Beman Standard](https://github.com/bemanproject/beman/blob/main/docs/beman_standard.md).

<!-- markdownlint-disable-next-line line-length -->
**Implements**: `std::views::cache_latest` proposed in [P3138 `views::cache_latest`](https://wg21.link/P3138) and in the working draft for C++26.

<!-- markdownlint-disable-next-line line-length -->
**Status**: [Under development and not yet ready for production use.](https://github.com/bemanproject/beman/blob/main/docs/beman_library_maturity_model.md#under-development-and-not-yet-ready-for-production-use)

## License

`beman.cache_latest` is licensed under the Apache License v2.0 with LLVM Exceptions.

## Usage

The following code snippet illustrates using `beman::cache_latest`:

```cpp
#include <beman/cache_latest/cache_latest.hpp>

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5};

    auto even_squares = v
        | std::views::transform([](int i){
                std::print("transform: {}\n", i);
                return i * i;
            })
        | beman::cache_latest
        | std::views::filter([](int i){
                std::print("filter: {}\n", i);
                return i % 2 == 0;
            });

    for (int i : even_squares) {
        std::print("Got: {}\n", i);
    }
}

```

Full runnable examples can be found in [`examples/`](examples/).

## Dependencies

### Build Environment

This project requires at least the following to build:

* A C++ compiler that conforms to the C++23 standard or greater
* CMake 3.30 or later
* (Test Only) GoogleTest

You can disable building tests by setting CMake option `BEMAN_CACHE_LATEST_BUILD_TESTS` to
`OFF` when configuring the project.

### Supported Platforms

| Compiler | Version | C++ Standards | Standard Library  |
|----------|---------|---------------|-------------------|
| GCC      | 16-14   | C++26, C++23  | libstdc++         |
| Clang    | 20-19   | C++26, C++23  | libstdc++, libc++ |

## Development

See the [Contributing Guidelines](CONTRIBUTING.md).

## Integrate beman.cache_latest into your project

### Build

You can build cache_latest using a CMake workflow preset:

```bash
cmake --workflow --preset gcc-release
```

To list available workflow presets, you can invoke:

```bash
cmake --list-presets=workflow
```

For details on building beman.cache_latest without using a CMake preset, refer to the
[Contributing Guidelines](CONTRIBUTING.md).

### Installation

#### Vcpkg

The preferred way to install cache_latest is via vcpkg. To do so, after installing vcpkg
itself, you need to add support for the Beman project's [vcpkg
registry](https://github.com/bemanproject/vcpkg-registry) by configuring a
`vcpkg-configuration.json` file (which cache_latest [provides](vcpkg-configuration.json)).

Then, simply run `vcpkg install beman-cache-latest`.

#### Manual

To install beman.cache_latest globally after building with the `gcc-release` preset, you can
run:

```bash
sudo cmake --install build/gcc-release
```

Alternatively, to install to a prefix, for example `/opt/beman`, you can run:

```bash
sudo cmake --install build/gcc-release --prefix /opt/beman
```

This will generate the following directory structure:

```txt
/opt/beman
├── include
│   └── beman
│       └── cache_latest
│           ├── cache_latest.hpp
│           └── ...
└── lib
    └── cmake
        └── beman.cache_latest
            ├── beman.cache_latest-config-version.cmake
            ├── beman.cache_latest-config.cmake
            └── beman.cache_latest-targets.cmake
```

### CMake Configuration

If you installed beman.cache_latest to a prefix, you can specify that prefix to your CMake
project using `CMAKE_PREFIX_PATH`; for example, `-DCMAKE_PREFIX_PATH=/opt/beman`.

You need to bring in the `beman.cache_latest` package to define the `beman::cache_latest` CMake
target:

```cmake
find_package(beman.cache_latest REQUIRED)
```

You will then need to add `beman::cache_latest` to the link libraries of any libraries or
executables that include `beman.cache_latest` headers.

```cmake
target_link_libraries(yourlib PUBLIC beman::cache_latest)
```

### Using beman.cache_latest

To use `beman.cache_latest` in your C++ project,
include an appropriate `beman.cache_latest` header from your source code.

```c++
#include <beman/cache_latest/cache_latest.hpp>
```

## Contributing

Please do!
You encourage you to checkout our [contributor's guide](docs/README.md).
Issues and pull requests are appreciated.
