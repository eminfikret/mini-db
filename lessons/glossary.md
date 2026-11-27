# Glossary

Every term the lessons define, in one place. The number is the lesson where it first appears.

## Database
| Term | Means | # |
|---|---|---|
| REPL | Read–Eval–Print Loop: read input → evaluate it → print the result → repeat | 01 |
| prompt | text a program prints to say "your turn, type here" (`db > `) | 01 |
| meta-command | an instruction for the program itself, starting with `.` (`.exit`) | 02 |
| statement | a request about the data (`insert`, `select`) | 03 |
| SQL | Structured Query Language, the standard language for talking to databases | 03 |
| parser | turns text into something the program can work with | 03 |
| executor | acts on a parsed statement | 03 |
| row | one record in a table | 04 |
| column | one named field of a row | 04 |
| schema | the list of columns, with their types and limits | 04 |
| table | a collection of rows that share one schema | 05 |
| append-only | new rows go at the end; nothing is removed | 05 |
| full scan | looking at every row, one after another | 05 |
| module | one part of the program with one job | 06 |
| layer | a module that only talks to the module below it | 06 |

## C++
| Term | Means | # |
|---|---|---|
| compile | turn one `.cpp` file into an object file | 01 |
| object file | `.o`, machine code that isn't runnable yet | 01 |
| link | join object files + the standard library into an executable | 01 |
| executable | a program you can run | 01 |
| standard library | ready-made code that ships with every C++ compiler | 01 |
| namespace | a named group of names (`std`) | 01 |
| `::` | "look inside": `std::cout` = the `cout` inside `std` | 01 |
| stream | a flow of characters (`std::cin` in, `std::cout` out) | 01 |
| EOF | End Of File: there's no more input (Ctrl-D) | 01 |
| flush | push waiting output to the screen now | 01 |
| literal | a value written directly in code (`"text"`, `'a'`, `42`) | 01 |
| exit code | the number a program ends with; `0` = success | 01 |
| function | a named block of code you can call | 02 |
| parameter | an input of a function | 02 |
| return type | what a function gives back (`void` = nothing) | 02 |
| `enum class` | a type with a fixed list of named values | 02 |
| `switch` | picks one branch based on a value | 02 |
| `struct` | groups related values (members) into one type | 03 |
| member | a value inside a struct, reached with `.` | 03 |
| `std::optional` | holds a value or nothing (`std::nullopt`) | 03 |
| fixed-width integer | a whole-number type with an exact size (`std::uint32_t`) | 04 |
| signed / unsigned | can be negative / 0 and up only | 04 |
| bit / byte | one binary digit / 8 bits | 04 |
| string stream | `std::istringstream`: reads from text like `std::cin` reads from the keyboard | 04 |
| attribute | a note to the compiler in `[[ ]]` | 04 |
| `[[nodiscard]]` | "don't ignore this function's result" | 04 |
| `constexpr` | a constant fixed while compiling, which can never change | 04 |
| `std::size_t` | the standard library's type for sizes and counts, never negative | 04 |
| `std::numeric_limits` | gives a number type's limits, e.g. its largest value | 04 |
| macro | an old C text replacement done before compiling, with no type (`UINT32_MAX`) | 04 |
| `static_cast` | an explicit, visible conversion from one type to another | 04 |
| early return | leaving a function as soon as a check fails | 04 |
| `std::vector` | a list that grows | 05 |
| address | the number of a spot in memory | 05 |
| reference | `T&`: another name for an existing variable | 05 |
| `const` reference | `const T&`: read-only access to the caller's variable, no copy | 05 |
| range-based `for` | runs once for every element of a list | 05 |
| declaration | "this exists": a name and type, no body | 06 |
| definition | the actual body | 06 |
| header / source | `.hpp` with declarations and types / `.cpp` with definitions | 06 |
| include guard | `#ifndef`/`#define`/`#endif` that makes a header count only once | 06 |
| library | compiled code that programs can use | 06 |
| unnamed namespace | `namespace { … }`: its contents are visible only in that one source file | 06 |

## Data structures and algorithms (optional DSA corners)
| Term | Means | # |
|---|---|---|
| DSA | data structures and algorithms | 05 |
| Big-O | how the work grows as the data grows: O(1) stays the same, O(n) grows in step with n | 05 |
| dynamic array | one block of memory with elements side by side, which moves to a bigger block when full (`std::vector`) | 05 |
| size / capacity | elements in use / elements that fit before the array has to grow | 05 |
| amortized O(1) | usually O(1), sometimes slower, but O(1) on average | 05 |
| linear search | checking elements one by one until a match is found, O(n) | 05 |

## Tools
| Term | Means | # |
|---|---|---|
| CMake | generates build files from `CMakeLists.txt` | 01 |
| Ninja | a fast build tool that runs the compiler | 01 |
| target | something CMake builds (a program or library) | 01 |
| flag | an option for a tool (`-Wall`, `-Werror`) | 01 |
| pipe | `\|` sends one program's output into another program's input | 01 |
| `PUBLIC` / `PRIVATE` | (CMake) for this target and its users / for this target only | 06 |
