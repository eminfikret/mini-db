# 01. REPL + CMake

## What you'll build
A program that waits for you to type, and answers:
```
db > hello
Unrecognized command 'hello'.
db > .exit
```
This is a **REPL** (Read–Eval–Print Loop): read what you type → evaluate it → print the result → repeat. It's how you'll talk to the database in every later lesson.

## Where this fits
A database is a stack of layers. Each lesson builds one:
```
REPL           read what you type          ← you are here
Parser         text → command              ← 03
B-tree         find rows fast              ← 17+
Pager          read/write the file         ← 11–14
file on disk   mini.db
```

## Before you start: setup (once)
| OS | Install |
|---|---|
| macOS | `xcode-select --install` (Apple's command-line tools, includes the Clang compiler) · `brew install cmake ninja` |
| Ubuntu/Debian | `sudo apt install build-essential cmake ninja-build` (`build-essential` = compiler + basic build tools) |
| Windows | use **WSL** (Windows Subsystem for Linux: runs Ubuntu inside Windows), then the line above |

Check: `c++ --version` (Clang 14+ or GCC 11+, the two main C++ compilers) · `cmake --version` (3.20+) · `ninja --version`

---

## Step 1: Build a program that does nothing
Get the build working first, so every later step is just "change code → rebuild → run".

### Know
**Text → program.** The computer can't run `.cpp` text. It has to be turned into machine code:
```
main.cpp → compile → main.o → link → mini-db
```
| Step | Does | Typical error |
|---|---|---|
| Compile | turns one `.cpp` file into an **object file** (`.o`, machine code, not runnable yet) | typos, wrong types |
| Link | joins object files + the standard library into an **executable** (a program you can run) | `undefined symbol` = something you use was never written, or its file isn't listed in `add_executable` |

**CMake** (from *cross-platform make*) reads `CMakeLists.txt` and generates build files. **Ninja** is a fast build tool that reads those files and calls the compiler. *Why not call the compiler yourself:* it gets messy once there are many files, tests and libraries.

**`CMakeLists.txt`, top to bottom (order matters).** Each line is a command, written `command(arguments)`. Look up each command's arguments in the CMake reference (links at the bottom).
| # | Command | What it's for |
|---|---|---|
| 1 | `cmake_minimum_required` | the oldest CMake version allowed (use 3.20). Always first |
| 2 | `project` | the project's name and its language. CMake calls C++ `CXX` |
| 3 | `set` with `CMAKE_CXX_STANDARD` and `CMAKE_CXX_STANDARD_REQUIRED` | use C++20 (the 2020 version of C++), and fail if it isn't available |
| 4 | `set` with `CMAKE_CXX_EXTENSIONS` | turn compiler-specific extras off. *Why:* the code compiles the same everywhere |
| 5 | `add_executable` | creates a **target** (something CMake builds): the program's name + its source files |
| 6 | `target_compile_options` | compiler flags for one target. `PRIVATE` = only for this target |

*Why this order:* settings (3–4) apply to targets created after them, and line 6 can only name a target that already exists (line 5).

| Flag | Means |
|---|---|
| `-Wall` | "warn all": common warnings (despite the name, not all of them) |
| `-Wextra` | more warnings |
| `-Wpedantic` | warn on anything that isn't strict standard C++ |
| `-Werror` | every warning becomes an error. *Why:* warnings are often real bugs |

**Terminal commands:**
| Command | When |
|---|---|
| `cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug` | once (**configure**). `-S` source folder (`.` = here) · `-B` build folder · `-G` which build tool · `-D` set a variable. `Debug` = easy to debug, not optimized |
| `cmake --build build` | after **every** code change |
| `./build/mini-db` | run the program |
| `rm -rf build` | something weird → delete the build folder, configure again |

**Every program starts in `main`:**
| Write | Means |
|---|---|
| `int main() { ... }` | the **function** where every program starts. `int` = it returns a whole number, the **exit code** |
| `return 0;` | end `main` and report success. `;` ends every statement |

**Exit code:** `0` = success, anything else = failure. `echo $?` in the terminal shows the last program's exit code.

### Do
1. Write `CMakeLists.txt`: a C++20 program called `mini-db`, built from `src/main.cpp`, with all four warning flags.
2. Write `src/main.cpp`: a program that does nothing and reports success.
3. Configure, build, run, then `echo $?`.

<details><summary>Hint 1</summary>Write the six commands in the table's order. Each one's reference page lists its arguments.</details>
<details><summary>Hint 2</summary>The program's name appears twice: once where the target is created and once where its flags are set.</details>

✅ Builds with no warnings · prints nothing · `echo $?` → `0`

---

## Step 2: Print the prompt
`db > ` is the **prompt**: text a program prints to say "your turn, type here".

### Know
| Write | Means |
|---|---|
| `#include <iostream>` | use the standard library's input/output part. Goes at the top of the file |
| `std::cout << "Hello";` | print text. `cout` = *character output* (the screen) |
| `"Hello"` | text goes in **double quotes** |
| `'\n'` | newline character (single quotes = exactly one character) |
| `// note` | comment, ignored by the compiler |

**What `std::` means**
| Part | Means |
|---|---|
| **standard library** | ready-made code that ships with every C++ compiler: text, input/output, lists, files… |
| `std` | short for **standard**, the **namespace** (named group of names) that holds the standard library |
| `::` | "look inside". `std::cout` = the `cout` inside `std` |

*Why namespaces:* two libraries can both have a `count`. The prefix keeps them apart: `std::count` vs `mydb::count`. Skip `using namespace std;` (lets you drop `std::`). *Why:* your own names can then clash with standard ones.

### Do
Print `db > ` once. Rebuild, run.

<details><summary>Hint</summary>The prompt has no newline after it, so the cursor waits right behind it.</details>

✅ Shows `db > ` (no newline after it, so the terminal adds a `%`, see Q&A)

---

## Step 3: Read lines in a loop

### Know
| Write | Means |
|---|---|
| `#include <string>` | use text variables and `getline` |
| `std::string name;` | create a **variable** named `name` of **type** `std::string` (text). Type first, then name |
| `while (condition) { ... }` | repeat the block (`{ }`) as long as the condition is true |
| `std::getline(std::cin, name)` | wait for one line of typing and store it in `name`. `cin` = *character input* (the keyboard) |

`std::getline`:
- stores the line **without** the Enter key's `\n`
- gives `true` if it got a line, `false` at **EOF** (*End Of File*: no more input)
- you cause EOF by pressing **Ctrl-D** (or when piped input runs out)
- after EOF, every later call fails instantly

A prompt without `'\n'` still appears on screen because `cin` **flushes** `cout` before reading (flush = push waiting output to the screen now).

### Do
Keep asking for lines until the input ends. The prompt must show before **every** line the user types.

<details><summary>Hint 1</summary>`std::getline` gives `true` or `false`. Where in a `while` does a `true`/`false` go?</details>
<details><summary>Hint 2</summary>The prompt must appear before the first read and again after each line has been handled.</details>

✅ Prompt appears before every line you type · Ctrl-D ends the program

---

## Step 4: Handle commands

### Know
| Write | Means |
|---|---|
| `if (condition) { ... } else { ... }` | run the first block if true, otherwise the second |
| `name == "alice"` | does `name` contain exactly the text `alice`? (`==` compares; a single `=` **assigns**) |
| `return 0;` inside the loop | ends `main` immediately, so the program exits |

⚠️ `==` compares text only when one side is a `std::string` (like `line`). `"abc" == "abc"`, two raw text **literals** (text written directly in code), compares memory addresses, not text.

### Do
- `.exit` → end the program with success
- anything else → print `Unrecognized command '<what was typed>'.` then a newline, and keep going

<details><summary>Hint</summary>Single quotes inside double-quoted text are just normal characters.</details>

✅ All of "Done when" below

---

## Done when
- [ ] clean build, zero warnings
- [ ] `hello` → `Unrecognized command 'hello'.`
- [ ] `.exit` quits and `echo $?` prints `0`
- [ ] Ctrl-D quits (no endless loop)
- [ ] `printf 'hello\n.exit\n' | ./build/mini-db` gives the same result (`printf` prints text; `|`, a **pipe**, feeds it into your program as if typed)

## Check yourself
Answer before opening.

<details><summary>1. You changed <code>main.cpp</code>, ran the program, and it still behaves the old way. What did you forget?</summary>
To rebuild: `cmake --build build`. The program only changes when it's compiled again.
</details>

<details><summary>2. Why does the program loop forever on Ctrl-D if <code>getline</code> isn't the loop condition?</summary>
After EOF every `getline` fails instantly and `line` stays empty → the loop never waits and never ends.
</details>

<details><summary>3. Is <code>if ("abc" == "abc")</code> a safe text comparison?</summary>
No. Both sides are raw literals, so it compares memory addresses. One side must be a `std::string`.
</details>

## Pitfalls
Mistakes made while building this lesson.

| Symptom | Cause | Fix |
|---|---|---|
| A number like `1684160032` instead of the prompt | prompt written in single quotes: `'db> '` | text always uses double quotes: `"db > "` |
| You must type before the prompt appears | prompt printed **after** `getline` | print the prompt before every read |

## Q&A
Questions asked while building this lesson.

**What's the `%` after my output, e.g. `text%`?**
Not from your program. Your output didn't end with a newline, so the terminal's shell (zsh) marks the unfinished line with `%`. End output with `'\n'`. The same happens when Ctrl-D ends the program at `db > `, so decide what to print then.

**Why does `'db> '` print `1684160032`?**
Single quotes hold one character. Several characters in single quotes get packed into one number from their character codes: `d`=0x64, `b`=0x62, `>`=0x3E, space=0x20 → 0x64623E20 = 1684160032.

**What's the `db` in `db > ` called?**
The whole `db > ` is the **prompt**. `db` = short for database (which program you're talking to), `>` = "your turn", the space keeps your typing off the `>`. Your terminal has its own prompt (e.g. `➜`). When you see `db > `, you're typing into your program, not the terminal.

**Do I need `= ""` when creating a `std::string`?**
No. `std::string line;` already starts empty (`""`). Writing `= ""` does the same thing, just longer.

## Go deeper (optional)
- [CMake tutorial, Step 1](https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html): official walkthrough of this exact minimal file
- CMake reference: [`add_executable`](https://cmake.org/cmake/help/latest/command/add_executable.html) · [`target_compile_options`](https://cmake.org/cmake/help/latest/command/target_compile_options.html) · [all commands](https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html)
- [cppreference `std::getline`](https://en.cppreference.com/w/cpp/string/basic_string/getline): full reference
- [An Introduction to Modern CMake](https://cliutils.gitlab.io/modern-cmake/): free book for when the project grows
