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

## Lessons
Each lesson adds **one database idea and at most three new C++ ideas**, and starts by showing the problem the new idea solves. Finished lessons are tagged `lesson-NN-<name>` (e.g. `lesson-01-repl`). Terms are collected in the [glossary](lessons/glossary.md).

### Part 1: Talk to the database
| # | Lesson | Database idea | New C++ | Status |
|---|---|---|---|---|
| 01 | [REPL + CMake](lessons/01-repl-and-cmake.md) | the REPL (Read–Eval–Print Loop) is how you talk to a database | `std::string`, `std::getline`, `main`, CMake | ✅ Done |
| 02 | [Meta-commands](lessons/02-meta-commands.md) | commands for the program vs requests about data | functions, `enum class`, `switch` | ✅ Done |
| 03 | [Statements](lessons/03-statements.md) | parse once, execute later | `struct`, `{}` initialization, `std::optional` | Next |
| 04 | Rows | a fixed schema; validating input | fixed-width integers, string streams, `[[nodiscard]]` | Planned |
| 05 | In-memory table | append-only table; `select` = full scan | `std::vector`, references, range-`for` | Planned |

### Part 2: Engineer the project
| # | Lesson | Database idea | New C++ | Status |
|---|---|---|---|---|
| 06 | Library and modules | layers that only talk to the layer below | headers vs sources, include guards, namespaces | Planned |
| 07 | Tests | golden transcripts, edge cases | GoogleTest, CTest | Planned |

### Part 3: Bytes, pages, files
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
| 16 | Tooling and CI | — | clang-format, clang-tidy, CMake presets, GitHub Actions | Planned |

### Part 4: The B+tree
| # | Lesson | Database idea | New C++ | Status |
|---|---|---|---|---|
| 17 | Leaf node | B-tree vs B+tree, node = page | bit shifts and masks, a view class over bytes | Planned |
| 18 | Sorted keys | binary search, duplicate keys | `std::memmove`, lambdas | Planned |
| 19 | Leaf split | split, new root, internal node format | `assert`, recursion | Planned |
| 20 | Search | descend from the root | `std::map`, `<random>` | Planned |
| 21 | Scan across leaves | next-leaf pointer, range scan | operator overloading, `begin()`/`end()` | Planned |
| 22 | Parent update | separator keys | — (database-only lesson) | Planned |
| 23 | Internal split | the tree grows taller | — (database-only lesson) | Planned |

### Part 5: Real SQL
| # | Lesson | Database idea | New C++ | Status |
|---|---|---|---|---|
| 24 | Tokenizer | lexical analysis | `std::string_view`, lifetimes, dangling | Planned |
| 25 | Parser | grammar, recursive descent, AST (Abstract Syntax Tree) | `std::variant`, `std::visit`, custom exceptions | Planned |
| 26 | WHERE | predicates, operator precedence | recursive types with `std::unique_ptr` | Planned |
| 27 | Executor | iterator ("Volcano") model, point lookup | abstract classes, `virtual`, `override` | Planned |
| 28 | CREATE TABLE | the catalog: the schema stored in the file | `std::unordered_map` | Planned |

### Part 6: All basic SQL operations
| # | Lesson | Database idea | New C++ | Status |
|---|---|---|---|---|
| 29 | DELETE | delete rows that match `WHERE` (lazy: no tree rebalancing) | — (database-only lesson) | Planned |
| 30 | UPDATE | change rows in place; changing the id = delete + insert | — (database-only lesson) | Planned |
| 31 | DROP TABLE | remove a table from the catalog; a free-page list so its pages get reused | — (database-only lesson) | Planned |

### Part 7: Make it a real database
| # | Lesson | Database idea | New C++ | Status |
|---|---|---|---|---|
| 32 | Crash safety | atomic commit, rollback journal, `fsync` | `std::filesystem`, fault-injection fakes | Planned |
| 33 | Buffer pool | pin/unpin, LRU (Least Recently Used) eviction | `std::list`, RAII guards | Planned |
| 34 | Release v0.1.0 | compare results with SQLite | — | Planned |

**SQL you can use at the end:** `CREATE TABLE` · `DROP TABLE` · `INSERT` · `SELECT … WHERE …` · `UPDATE … WHERE …` · `DELETE … WHERE …`

**Stretch (optional):** B+tree merge on delete · variable-length rows · secondary indexes · WAL (Write-Ahead Log) · bytecode VM (virtual machine) · reading a real SQLite file · C++23 refresh · file locking.

**Later, as a separate track:** joins (combining tables).

The plan may change as the project grows.

## Credits
- Storage-layer order inspired by cstack's [Let's Build a Simple Database](https://cstack.github.io/db_tutorial/) (in C; this repo is a separate C++ implementation).
- Database theory: [CMU 15-445/645 Database Systems](https://15445.courses.cs.cmu.edu/) and the [SQLite documentation](https://www.sqlite.org/docs.html).
- B+tree deletion and crash safety: [Build Your Own Database From Scratch](https://build-your-own.org/database/).
- Parser and expressions: [Crafting Interpreters](https://craftinginterpreters.com/) and [Phil Eaton's database basics](https://notes.eatonphil.com/database-basics.html).
- C++ practices: [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines).
