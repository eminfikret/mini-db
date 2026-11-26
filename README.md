# mini-db

A small SQLite-style database built from scratch in modern C++ (C++20), **one lesson at a time**.

This repo is a curriculum. Every stage of the database has its own lesson. A lesson explains only the concepts that stage needs, lists what to build and how to check it works, and links to sources for going deeper. You can learn C++ and database internals together by following the lessons in order.

**Who it's for:** anyone, even if C++ is your first programming language. No prior knowledge of C++ or databases needed. Each lesson explains exactly what that stage uses, in compressed form, and ends with self-check questions.

> Built while learning. It's not production-ready and doesn't try to compete with SQLite.

## How to use this repo
1. Read a lesson in [`lessons/`](lessons/).
2. Build that stage yourself, without peeking.
3. Compare with the code at that lesson's git tag: `git checkout lesson-01-repl`.

## Build and run
Requirements: a C++20 compiler (Clang or GCC), CMake ≥ 3.20, Ninja (optional).

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/mini-db
```

## Lessons
| # | Lesson | You'll learn | Tag | Status |
|---|--------|--------------|-----|--------|
| 01 | [REPL and CMake](lessons/01-repl-and-cmake.md) | Compile/link model, CMake basics, reading input, `std::string` | `lesson-01-repl` | Done |
| 02 | Commands and statements | `enum class`, `std::string_view`, splitting parsing from running a command | `lesson-02-statements` | Planned |
| 03 | Tests and sanitizers | Multiple files and headers, GoogleTest, ASan/UBSan | `lesson-03-tests` | Planned |
| 04 | Rows and serialization | Fixed-width integers, byte layout, `memcpy`, `static_assert` | `lesson-04-rows` | Planned |
| 05 | Pages and an in-memory table | `std::array`, `std::vector`, fixed-size pages | `lesson-05-pages` | Planned |
| 06 | Persistence: the pager | RAII, destructors, move-only types, file I/O | `lesson-06-pager` | Planned |
| 07 | Cursors | Iteration over storage, keeping concerns separate | `lesson-07-cursor` | Planned |
| 08+ | B-tree index | Node layout, search, splits, primary key | — | Planned |
| — | First release | CI, comparing results with SQLite | `v0.1.0` | Planned |

The order of lessons may change as the project grows.

## Credits
- Stage order inspired by cstack's [Let's Build a Simple Database](https://cstack.github.io/db_tutorial/) (written in C; this repo is a separate C++ implementation).
- Database theory: [CMU 15-445/645 Database Systems](https://15445.courses.cs.cmu.edu/).
