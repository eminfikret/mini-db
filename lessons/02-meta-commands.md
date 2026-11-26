# 02. Meta-commands

## What you'll build
Commands that start with `.` are now handled by their own function:
```
db > .foo
Unrecognized command '.foo'.
db > .exit
```
**New in this lesson:** functions · `enum class` · `switch`

A **meta-command** is an instruction for the program itself, not for the data. They start with `.` (like `.exit`). Requests about the data (`insert`, `select`) come in lesson 03.

## Where this fits
```
REPL           read what you type, handle .meta-commands   ← you are here
Parser         text → statement                             ← 03
B-tree         find rows fast                               ← 17+
Pager          read/write the file                          ← 11–14
file on disk   mini.db
```

## How the pieces connect (end of this lesson)
```
main loop
  │ line starts with '.'
  ▼
do_meta_command(line) ──returns──► MetaCommandResult
                                        │
main: switch on the result ◄────────────┘
  Exit          → end the program
  Unrecognized  → print "Unrecognized command '…'."
```

---

## Step 1: Your own function

**Try this first:** picture `main` handling `.exit`, `.help`, `.tables`, `.btree`… all inside the loop. Every new command makes `main` longer and harder to read.

### Know
A **function** is a named block of code you can run (**call**) from somewhere else.
```cpp
int add(int a, int b) {    // return type, name, parameters
    return a + b;          // send a value back
}

int sum = add(2, 3);       // call it: sum is 5
```
| Part | Means |
|---|---|
| `int` before the name | **return type**: what the function gives back. `void` = gives nothing back |
| `(int a, int b)` | **parameters**: the inputs. Each one is the function's own copy of the value passed in |
| `return a + b;` | ends the function and gives the value back |

- Write a function **above** the code that calls it. *Why:* the compiler reads top to bottom, once.
- **One job per function.** *Why:* small pieces are easier to read, test and change.

### Do
```cpp
void do_meta_command(std::string line)
```
Move "print `Unrecognized command '…'.`" into this function. Keep the `.exit` check in `main` for now. In `main`, call the function for every line that `line.starts_with('.')`.

✅ Behaves exactly like lesson 01

---

## Step 2: Results as `enum class`

**Try this first:** move the `.exit` check into `do_meta_command`.
- `return 0;` there won't compile: a `void` function returns nothing.
- `return;` compiles, but only ends the **function**. The loop keeps going and `.exit` stops working.

The function must **tell `main` what happened**. You could return a number (`0` = ok, `1` = exit, `2` = unknown), but which number meant what?

### Know
An **`enum class`** (*enumeration*) is a type with a fixed list of named values:
```cpp
enum class Color { Red, Green };

Color c = Color::Red;
if (c == Color::Red) { ... }
```
| Rule | *Why* |
|---|---|
| values need the prefix: `Color::Red` | no clash with other things named `Red` |
| it doesn't silently turn into a number | `Color::Red + 1` is an error, so mistakes get caught |
| `MetaCommandResult::Exit` instead of `1` | the name says what it means |

**Don't end the program inside a helper function.** Return a result and let `main` decide. *Why:* in lesson 12, the database must be saved to disk on the way out, and that only works if every exit goes through `main`.

### Do
```cpp
enum class MetaCommandResult { Exit, Unrecognized };

MetaCommandResult do_meta_command(std::string line)
```
The function only **decides** and returns a result. It prints nothing. `main` reacts to the result (for now with `if`).

✅ `.exit` quits again · `.foo` → `Unrecognized command '.foo'.` · the function contains no `std::cout`

---

## Step 3: React with `switch`

**Try this first:** add a new value to the enum, e.g. `Help`. Rebuild. Nothing complains, but `main` silently ignores it.

### Know
**`switch`** picks one branch based on a value:
```cpp
switch (c) {
    case Color::Red:
        ...
        break;     // stop here; without break, the next case runs too
    case Color::Green:
        ...
        break;
}
```
- **No `default:` when switching on an enum.** *Why:* then `-Wall` (the warnings flag from lesson 01) warns when a value has no `case`, and `-Werror` turns that into a build error. The compiler checks that you handled every value.

### Do
Replace the `if` in `main` with a `switch` on the result.

✅ Add `Help` to the enum → the build **fails** and names the missing case. Remove `Help` → it builds.

---

## Done when
- [ ] clean build, zero warnings
- [ ] `.foo` → `Unrecognized command '.foo'.`
- [ ] `.exit` and end of input (Ctrl-D) both quit with exit code `0`
- [ ] `do_meta_command` prints nothing and never ends the program
- [ ] `main` reacts to the result with a `switch` that has no `default:`

## Check yourself
Answer before opening.

<details><summary>1. Why does <code>do_meta_command</code> return <code>Exit</code> instead of ending the program itself?</summary>
So every exit goes through `main`. Later, `main` must save the database to disk before quitting.
</details>

<details><summary>2. You add <code>Help</code> to <code>MetaCommandResult</code> but forget it in the <code>switch</code>. What happens, and why is that good?</summary>
The build fails: `-Wall` warns about the unhandled value and `-Werror` makes it an error. The mistake is caught before the program ever runs. That's why there's no `default:`.
</details>

<details><summary>3. (From lesson 01) Why must <code>std::getline</code> be the loop condition?</summary>
At EOF (End Of File) it returns `false`. If it isn't the condition, the loop keeps calling it, it fails instantly every time, and the program never ends.
</details>

## Pitfalls
Mistakes made while building this lesson.

_Added as they come up._

## Q&A
Questions asked while building this lesson.

**My editor says `std::string` has no member `starts_with`, but it compiles. Why?**
The red underline comes from the editor's code checker, not the compiler. The checker was set to an older C++ version (C++17), and `starts_with` only exists since C++20. Set the editor to C++20, or let it read the settings from CMake. Trust the compiler's output when they disagree.

## Compare with the reference code
```sh
git diff lesson-01-repl..lesson-02-meta-commands
```

## Go deeper (optional)
- [cppreference: functions](https://en.cppreference.com/w/cpp/language/functions)
- [cppreference: `enum class`](https://en.cppreference.com/w/cpp/language/enum)
- [cppreference: `switch`](https://en.cppreference.com/w/cpp/language/switch)
