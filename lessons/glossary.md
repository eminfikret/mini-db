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

## Tools
| Term | Means | # |
|---|---|---|
| CMake | generates build files from `CMakeLists.txt` | 01 |
| Ninja | a fast build tool that runs the compiler | 01 |
| target | something CMake builds (a program or library) | 01 |
| flag | an option for a tool (`-Wall`, `-Werror`) | 01 |
| pipe | `\|` sends one program's output into another program's input | 01 |
