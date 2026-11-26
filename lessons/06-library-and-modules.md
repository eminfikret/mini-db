# 06. Library and modules

## What you'll build
The database behaves **exactly** like lesson 05. What changes is how the code is organized:
```
mini-db/
  CMakeLists.txt
  include/minidb/     headers: WHAT exists           ┐
  src/                sources: HOW it works          ┘→ library  minidb_core
  apps/main.cpp       only the REPL loop               → program  mini-db (uses the library)
```
**New in this lesson:** header + source files · include guards · namespaces

| Word | Means |
|---|---|
| **module** | one part of the program with one job, e.g. "statements" or "the table" |
| **library** | compiled code that programs can use. Here, every part of the database except the REPL loop |
| **layer** | a module that only talks to the module below it (parser → executor → table) |

*Why split:* in lesson 07, a test program must use prepare and the table without the REPL loop. That's only possible if they live in a library.

## Where this fits
Every layer from lessons 01–05, now each in its own files:
```
apps/main.cpp           REPL                     ← lessons 01–02
meta_command            meta-commands            ← lesson 02
statement               parser (prepare)         ← lessons 03–04
row                     the row and its limits   ← lesson 04
table                   table + execute          ← lesson 05
```

---

## Step 1: Declaration vs definition

**Try this first:** run `wc -l src/main.cpp` (counts its lines). Then change one message and rebuild: the **whole** file is compiled again. And if another program wanted prepare, it would have to copy it.

### Know
| Word | Means | Example |
|---|---|---|
| **declaration** | "this exists, here's its name and type". No body | `int square(int n);` |
| **definition** | the actual body | `int square(int n) { return n * n; }` |

| File | Contains | Why |
|---|---|---|
| **header** (`.hpp`) | declarations, and the types themselves (`struct`, `enum class`) | other files `#include` it to know what exists |
| **source** (`.cpp`) | definitions | compiled once, on its own |

```cpp
// geometry.hpp           ← what exists
int area(int width, int height);

// geometry.cpp           ← how it works
#include "geometry.hpp"
int area(int width, int height) { return width * height; }

// main.cpp               ← uses it, knows only the header
#include "geometry.hpp"
int main() { return area(3, 4) == 12 ? 0 : 1; }
```
- `#include "..."` (quotes) = **your own** header · `#include <...>` (angle brackets) = standard library
- Each **source** is compiled separately, and the **linker** (lesson 01) joins them. Change one source and only that one is recompiled.
- A module that only defines types (no function bodies) needs a header but no source.
- **Every header includes what it uses.** *Why:* it must compile no matter which file includes it first.

### Do
Split `main.cpp` into modules: meta-command, statement, row and table, each as a header plus a source (or only a header if it has no function bodies). `apps/main.cpp` keeps only the REPL loop.

Headers go in `include/minidb/`, and you include them as `"minidb/table.hpp"`. *Why the extra folder:* the include line then shows which project a header comes from, and your `table.hpp` can't clash with another library's `table.hpp`.

<details><summary>Hint 1</summary>Go through <code>main.cpp</code> from the top and ask about each piece: which layer does it belong to?</details>
<details><summary>Hint 2</summary>Types go fully in the header. A function is declared in the header and defined in the source.</details>

✅ (only after Step 4, when CMake knows the new files) the program behaves like lesson 05

---

## Step 2: Include guards

**Try this first:** statement uses row, and table uses row too. Picture a source that includes both statement and table: the row's header text gets pasted in **twice**, and the compiler stops with "redefinition of …".

### Know
An **include guard** makes the contents of a header count only once:
```cpp
#ifndef GEOMETRY_HPP      // if GEOMETRY_HPP isn't defined yet…
#define GEOMETRY_HPP      // …define it now, and keep the contents

int area(int width, int height);

#endif                    // end of the guarded part
```
The second time the header is pasted, `GEOMETRY_HPP` already exists, so everything in between is skipped.
- Name the guard after the project and file, in capitals: `MINIDB_TABLE_HPP`. *Why:* the guard name must be unique across every header the program sees.
- You'll also see `#pragma once` used for the same job. It's common, but it isn't part of the C++ standard, so this project uses guards.

### Do
Give every header an include guard.

✅ Including the same header twice in one file still compiles

---

## Step 3: Namespaces

### Know
Lesson 01 introduced `std::`, the standard library's **namespace**. You can make your own:
```cpp
namespace geometry {
    int area(int width, int height);
}

geometry::area(3, 4);    // used from outside the namespace
```
*Why:* `Table`, `Row` and `execute` are common names. Inside `namespace minidb` they can't clash with anyone else's.

### Do
Put everything in the library inside `namespace minidb`. `main` refers to it with `minidb::`. No `using namespace` anywhere.

<details><summary>Hint</summary>Both the header and the source of a module wrap their contents in the same namespace.</details>

✅ Builds once Step 4 is done

---

## Step 4: CMake: one library, one program

### Know
| Command | What it's for |
|---|---|
| `add_library` | creates a **library target** from source files. Name it `minidb_core` |
| `target_include_directories` | where a target's headers are found. `PUBLIC` = this target **and** every target that uses it |
| `target_link_libraries` | makes one target use another. `PRIVATE` = only this target needs it |
| `add_executable` | (lesson 01) the program, now built from `apps/main.cpp` only |

| Word | Means |
|---|---|
| `PUBLIC` | for this target and for everyone who uses it |
| `PRIVATE` | only for this target |

- Both targets need the warning flags. *Why:* the library holds most of the code, so it needs them most.
- List every source file explicitly. Don't let CMake find files automatically with a wildcard. *Why:* CMake then doesn't notice new files, and builds break mysteriously.
- Bigger projects have a `CMakeLists.txt` in each folder. With a handful of files, one is enough for now.

### Do
`CMakeLists.txt` builds the library `minidb_core` from `src/`, and the program `mini-db` from `apps/main.cpp`, linked to the library.

<details><summary>Hint 1</summary>The program needs the library's headers. Which keyword passes the include folder on to everyone using the library?</details>
<details><summary>Hint 2</summary>The order from lesson 01 still holds: a target must exist before you set its options.</details>

---

## Done when
- [ ] clean build from an empty `build/`, zero warnings
- [ ] the transcript from lesson 05 gives exactly the same output
- [ ] `apps/main.cpp` contains only the REPL loop
- [ ] every header has an include guard and includes what it uses
- [ ] everything except `main` is inside `namespace minidb`, and there's no `using namespace`
- [ ] **Notice:** `touch src/table.cpp && cmake --build build` recompiles only `table.cpp`, not every file (`touch` marks a file as changed without editing it)

## Check yourself
Answer before opening.

<details><summary>1. Why can a test program (lesson 07) use prepare after this lesson, but not before?</summary>
Before, prepare lived inside the program's own <code>main.cpp</code>. Now it's in a library that any program can link to.
</details>

<details><summary>2. A function is declared in a header but you forgot to put its source in <code>add_library</code>. Which step fails, and with what kind of error?</summary>
Linking (lesson 01): compiling works because the declaration is visible, but no definition exists anywhere → <code>undefined symbol</code>.
</details>

<details><summary>3. (From lesson 05) Why does select take the table as a <code>const</code> reference?</summary>
It only reads: the reference avoids a copy, and <code>const</code> forbids accidental changes.
</details>

## Pitfalls
Mistakes made while building this lesson.

_Added as they come up._

## Q&A
Questions asked while building this lesson.

_Added as they come up._

## Compare with the reference code
```sh
git diff lesson-05-table..lesson-06-library
```

## Go deeper (optional)
- [cppreference: `#include` and the preprocessor](https://en.cppreference.com/w/cpp/preprocessor/include)
- [cppreference: namespaces](https://en.cppreference.com/w/cpp/language/namespace)
- [CMake: `add_library`](https://cmake.org/cmake/help/latest/command/add_library.html) · [`target_link_libraries`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html) · [`target_include_directories`](https://cmake.org/cmake/help/latest/command/target_include_directories.html)
- [Modern CMake: project structure](https://cliutils.gitlab.io/modern-cmake/structure)
