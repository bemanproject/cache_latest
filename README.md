# beman.cache_latest: A Beman Library

<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

<!-- markdownlint-disable-next-line line-length -->
![Library Status](https://raw.githubusercontent.com/bemanproject/beman/refs/heads/main/images/badges/beman_badge-beman_library_under_development.svg) ![Continuous Integration Tests](https://github.com/bemanproject/cache_latest/actions/workflows/ci_tests.yml/badge.svg) [![Coverage](https://coveralls.io/repos/github/bemanproject/exemplar/badge.svg?branch=main)](https://coveralls.io/github/bemanproject/exemplar?branch=main) ![Standard Target](https://github.com/bemanproject/beman/blob/main/images/badges/cpp26.svg)

<!-- markdownlint-disable-next-line line-length -->
`beman.cache_latest` is a C++ ranges adaptor that caches the result of the last dereference of the underlying iterator. The reason for doing this is efficiency - specifically avoiding extra iterator dereferences.

The library conforms to [The Beman Standard](https://github.com/bemanproject/beman/blob/main/docs/BEMAN_STANDARD.md).

<!-- markdownlint-disable-next-line line-length -->
**Implements**: `std::views::cache_latest` proposed in [P3138 `views::cache_latest`](https://wg21.link/P3138) and in the working draft for C++26.

<!-- markdownlint-disable-next-line line-length -->
**Status**: [Under development and not yet ready for production use.](https://github.com/bemanproject/beman/blob/main/docs/BEMAN_LIBRARY_MATURITY_MODEL.md#under-development-and-not-yet-ready-for-production-use)

## Usage

The following code snippet illustrates using `beman::cache_latest`:

```cpp
#include <include/beman/cache_latest.hpp>

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

## Dependencies

### Build Environment

This project requires at least the following to build:

* C++23
* CMake 3.28

This project pulls [Google Test](https://github.com/google/googletest)
from GitHub as a development dependency for its testing framework,
thus requiring an active internet connection to configure.
You can disable this behavior by setting cmake option
[`BEMAN_CACHE_LATEST_BUILD_TESTS`](#beman_cache_latest_build_tests) to `OFF`
when configuring the project.

### Supported Platforms

This project officially supports:

* GNU GCC Compiler \[version 14+\]
* LLVM Clang++ Compiler \[version 19-20\]

> [!NOTE]
>
> Versions outside of this range would likely work as well,
> especially if you're using a version above the given range
> (e.g. HEAD/ nightly).
> These development environments are verified using our CI configuration.

## Development

### Develop using GitHub Codespace

This project supports [GitHub Codespace](https://github.com/features/codespaces)
via [Development Containers](https://containers.dev/),
which allows rapid development and instant hacking in your browser.
We recommend you using GitHub codespace to explore this project as this
requires minimal setup.

You can create a codespace for this project by clicking this badge:

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/bemanproject/cache_latest)

For more detailed documentation regarding creating and developing inside of
GitHub codespaces, please reference [this doc](https://docs.github.com/en/codespaces/).

> [!NOTE]
>
> The codespace container may take up to 5 minutes to build and spin-up,
> this is normal as we need to build a custom docker container to setup
> an environment appropriate for beman projects.

### Develop locally on your machines

<details>
<summary> For Linux based systems </summary>

Beman libraries requires [recent versions of CMake](#build-environment),
we advice you download CMake directly from [CMake's website](https://cmake.org/download/)
or install via the [Kitware apt library](https://apt.kitware.com/).

A [supported compiler](#supported-platforms) should be available from your package manager.
Alternatively you could use an install script from official compiler vendors.

Here is an example of how to install the latest stable version of clang
as per [the official LLVM install guide](https://apt.llvm.org/).

```bash
bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
```

</details>

<details>
<summary> For MacOS based systems </summary>

Beman libraries require [recent versions of CMake](#build-environment).
You can use [`Homebrew`](https://brew.sh/) to install the latest major version of CMake.

```bash
brew install cmake
```

A [supported compiler](#supported-platforms) is also available from brew.

For example, you can install latest major release of Clang++ compiler as:

```bash
brew install llvm
```

</details>

### Configure and Build the Project Using CMake Presets

This project recommends using [CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html)
to configure, build and test the project.
Appropriate presets for major compilers have been included by default.
You can use `cmake --list-presets` to see all available presets.

Here is an example to invoke the `gcc-debug` preset.

```shell
cmake --workflow --preset gcc-debug
```

Generally, there's two kinds of presets, `debug` and `release`.

The `debug` presets are designed to aid development, so it has debugging
instrumentation enabled and as many sanitizers turned on as possible.

> [!NOTE]
>
> The set of sanitizer supports are different across compilers.
> You can checkout the exact set compiler arguments by looking at the toolchain
> files under the [`cmake`](cmake/) directory.

The `release` presets are designed for use in production environments,
thus it has the highest optimization turned on (e.g. `O3`).

### Configure and Build Manually

While [CMake Presets](#configure-and-build-the-project-using-cmake-presets) are
convenient, you might want to set different configuration or compiler arguments
than any provided preset supports.

To configure, build and test the project with extra arguments,
you can run this sets of command.

```bash
cmake -B build -S . -DCMAKE_CXX_STANDARD=20 # Your extra arguments here.
cmake --build build
ctest --test-dir build
```

> [!IMPORTANT]
>
> Beman projects are
> [passive projects](https://github.com/bemanproject/beman/blob/main/docs/BEMAN_STANDARD.md#cmake),
> therefore,
> you will need to specify C++ version via `CMAKE_CXX_STANDARD`
> when manually configuring the project.

### Project specific configure arguments

When configuring the project manually,
you can pass an array of project specific CMake configs to customize your build.

Project specific options are prefixed with `BEMAN_EXEMPLAR`.
You can see the list of available options with:

```bash
cmake -LH | grep "BEMAN_EXEMPLAR" -C 2
```

<details>

<summary> Details of CMake arguments. </summary>

#### `BEMAN_CACHE_LATEST_BUILD_TESTS`

Enable building tests and test infrastructure. Default: ON.
Values: { ON, OFF }.

You can configure the project to have this option turned off via:

```bash
cmake -B build -S . -DCMAKE_CXX_STANDARD=20 -DBEMAN_EXEMPLAR_BUILD_TESTS=OFF
```

> [!TIP]
> Because this project requires Google Tests as part of its development
> dependency,
> disable building tests avoids the project from pulling Google Tests from
> GitHub.

#### `BEMAN_CACHE_LATEST_BUILD_EXAMPLES`

Enable building examples. Default: ON. Values: { ON, OFF }.

</details>

## Integrate beman.cache_latest into your project

To use `beman.cache_latest` in your C++ project,
include an appropriate `beman.cache_latest` header from your source code.

```c++
#include <include/beman/cache_latest/cache_latest.hpp>
```

> [!NOTE]
>
> `beman.cache_latest` headers are to be included with the `beman/cache_latest/` directories prefixed.
> It is not supported to alter include search paths to spell the include target another way. For instance,
> `#include <identity.hpp>` is not a supported interface.

How you will link your project against `beman.cache_latest` will depend on your build system.
CMake instructions are provided in following sections.

### Linking your project to beman.cache_latest with CMake

For CMake based projects,
you will need to use the `beman.cache_latest` CMake module
to define the `beman::cache_latest` CMake target:

```cmake
find_package(beman.cache_latest REQUIRED)
```

You will also need to add `beman::cache_latest` to the link libraries of
any libraries or executables that include beman.cache_latest's header file.

```cmake
target_link_libraries(yourlib PUBLIC beman::cache_latest)
```

### Produce beman.cache_latest static library locally

You can include cache_latest's headers locally
by producing a static `libbeman.cache_latest.a` library.

```bash
cmake --workflow --preset gcc-release
cmake --install build/gcc-release --prefix /opt/beman.cache_latest
```

This will generate such directory structure at `/opt/beman.cache_latest`.

```txt
/opt/beman.cache_latest
├── include
│   └── beman
│       └── cache_latest
│           └── identity.hpp
└── lib
    └── libbeman.cache_latest.a
```

## Contributing

Please do!
You encourage you to checkout our [contributor's guide](docs/README.md).
Issues and pull requests are appreciated.
