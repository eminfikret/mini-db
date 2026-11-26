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
Requirements: macOS or Linux (on Windows, use WSL), a C++20 compiler (Clang or GCC), CMake ≥ 3.20, Ninja (optional).

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/mini-db
```

## Goal
Build, in C++20, a single-file database that stores rows in an on-disk B+tree and answers `INSERT` and `SELECT … WHERE` through a hand-written parser, to learn modern C++ (classes, ownership and RAII, project structure, testing) and how a database stores and finds data.

## Lessons
Each lesson adds **one database idea and at most three new C++ ideas**, and starts by showing the problem the new idea solves. Finished lessons are tagged `lesson-NN-<name>` (e.g. `lesson-01-repl`). Terms are collected in the [glossary](lessons/glossary.md).

When a lesson uses a data structure or algorithm, an optional **DSA corner** (data structures and algorithms) at its end explains it, gives the cost of its operations, and links 1–3 focused resources. You don't need it to finish the lesson.

The 26 lessons ship as **5 releases**. Each release is tagged (`v0.1` … `v1.0`) and is a natural place to stop.

### v0.1: a tested in-memory database
| # | Lesson | Database idea | New C++ | Status |
|---|---|---|---|---|
| 01 | [REPL + CMake](lessons/01-repl-and-cmake.md) | the REPL (Read–Eval–Print Loop) is how you talk to a database | `std::string`, `std::getline`, `main`, CMake | ✅ Done |
| 02 | [Meta-commands](lessons/02-meta-commands.md) | commands for the program vs requests about data | functions, `enum class`, `switch` | ✅ Done |
| 03 | [Statements](lessons/03-statements.md) | parse once, execute later | `struct`, `{}` initialization, `std::optional` | ✅ Done |
| 04 | [Rows](lessons/04-rows.md) | a fixed schema; validating input | fixed-width integers, string streams, `[[nodiscard]]` | Next |
| 05 | [In-memory table](lessons/05-in-memory-table.md) | append-only table; `select` = full scan | `std::vector`, references, range-`for` | Planned |
| 06 | [Library and modules](lessons/06-library-and-modules.md) | layers that only talk to the layer below | headers vs sources, include guards, namespaces | Planned |
| 07 | Tests | golden transcripts, edge cases | GoogleTest, CTest | Planned |

### v0.2: data survives a restart
| # | Lesson | Database idea | New C++ | Status |
|---|---|---|---|---|
| 08 | Row serialization | on-disk row format | `std::byte`, `std::span`, `std::memcpy`, sanitizers | Planned |
| 09 | Pages | the page is the unit of disk I/O | `constexpr`, `static_assert`, type aliases | Planned |
| 10 | Table as a class | storage invariants | `class`, `private`, constructors, `const` member functions | Planned |
| 11 | Persistence | page N lives at byte N × 4096 | `argc`/`argv`, file descriptors, exceptions | Planned |
| 12 | Save on every exit | flush before close, on every path | destructors, RAII, `noexcept` | Planned |
| 13 | One owner per file | a resource has exactly one owner | move-only types, `std::move`, rule of five | Planned |
| 14 | Pager | page cache, cache miss, dirty page | `std::unique_ptr`, rule of zero | Planned |
| 15 | Cursor | a position in a table | pointers, `nullptr`, `std::size_t` | Planned |

### v0.3: a B+tree with two levels
| # | Lesson | Database idea | New C++ | Status |
|---|---|---|---|---|
| 16 | Leaf node | B-tree vs B+tree, node = page | bit shifts and masks, a view class over bytes | Planned |
| 17 | Sorted keys | binary search, duplicate keys | `std::memmove`, lambdas | Planned |
| 18 | Leaf split | split, new root, internal node format | `assert`, recursion | Planned |
| 19 | Search | descend from the root | `std::map`, `<random>` | Planned |
| 20 | Scan across leaves | next-leaf pointer, range scan | operator overloading, `begin()`/`end()` | Planned |

### v0.4: a tree of any height
| # | Lesson | Database idea | New C++ | Status |
|---|---|---|---|---|
| 21 | Parent update | separator keys | — (database-only lesson) | Planned |
| 22 | Internal split | the tree grows taller | — (database-only lesson) | Planned |

### v1.0: it speaks SQL
| # | Lesson | Database idea | New C++ | Status |
|---|---|---|---|---|
| 23 | Tokenizer | lexical analysis | `std::string_view`, lifetimes, dangling | Planned |
| 24 | Parser | grammar, recursive descent, AST (Abstract Syntax Tree) | `std::variant`, `std::visit`, custom exceptions | Planned |
| 25 | WHERE | predicates, operator precedence | recursive types with `std::unique_ptr` | Planned |
| 26 | Executor | iterator ("Volcano") model, point lookup by id | abstract classes, `virtual`, `override` | Planned |

**At v1.0 you can use:** `INSERT INTO users VALUES (…)` and `SELECT … FROM users WHERE …`, on one table (`id`, `username`, `email`) stored in a B+tree on disk.

**Each release ends with:** all tests green, a short release note, and the release tag. At v1.0, the same queries are also compared against the real `sqlite3`.

## Maybe later (after v1.0)
Each one would be its own small v1.x release, in this order:
1. `CREATE TABLE` + catalog (several tables)
2. `DELETE` + `UPDATE`
3. Crash safety (rollback journal)
4. Record format + `NULL` (more data types)
5. Slotted pages (variable-length rows)
6. `DROP TABLE` + free-page list
7. Secondary index
8. Writing a function template
9. C++23 refresh (`std::expected`)
10. CI with clang-format and clang-tidy

**Not in this project:** a query optimizer · network access · many users at once · logins · joins · `ORDER BY` / `GROUP BY` · buffer pool · overflow pages · WAL (Write-Ahead Log) · a bytecode VM.

## Credits
- Storage-layer order inspired by cstack's [Let's Build a Simple Database](https://cstack.github.io/db_tutorial/) (in C; this repo is a separate C++ implementation).
- Database theory: [CMU 15-445/645 Database Systems](https://15445.courses.cs.cmu.edu/) and the [SQLite documentation](https://www.sqlite.org/docs.html).
- B+tree deletion and crash safety: [Build Your Own Database From Scratch](https://build-your-own.org/database/).
- Parser and expressions: [Crafting Interpreters](https://craftinginterpreters.com/) and [Phil Eaton's database basics](https://notes.eatonphil.com/database-basics.html).
- C++ practices: [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines).
