# 03. Statements

## What you'll build
The database now recognizes two **statements**, requests about the data:
```
db > insert 1 alice alice@example.com
This is where we would do an insert.
Executed.
db > select
This is where we would do a select.
Executed.
db > delete 1
Unrecognized keyword at start of 'delete 1'.
db > .exit
```
**New in this lesson:** `struct` · `{}` initialization · `std::optional`

| Statement | Will mean (later lessons) |
|---|---|
| `insert` | add a row |
| `select` | show rows |

Statements are a tiny version of **SQL** (*Structured Query Language*), the standard language for talking to databases. Nothing is stored yet. This lesson only **recognizes** what was typed. Storing starts in lesson 05.

## Where this fits
```
REPL           read what you type, handle .meta-commands   ← 01–02 ✅
Parser         text → statement                             ← you are here
Executor       run the statement                            ← you are here (placeholder messages)
B-tree         find rows fast                               ← 17+
Pager          read/write the file                          ← 11–14
file on disk   mini.db
```
The **parser** turns text into something the program can work with. The **executor** acts on it. *Why separate them:* the text is read once. Every later layer works with the result and never looks at raw text again.

## How the pieces connect (end of this lesson)
```
main loop
  │
  ├─ starts with '.' ──► do_meta_command(line)       (lesson 02)
  │
  └─ otherwise ───────► prepare_statement(line) ──► std::optional<Statement>
                                                      │
                            empty ◄───────────────────┤
                              → "Unrecognized keyword at start of '…'."
                                                      │
                            has a Statement ◄─────────┘
                              → execute_statement(statement)   prints the placeholder
                              → main prints "Executed."
```

---

## Step 1: Give a statement a shape

**Try this first:** picture the executor checking `line.starts_with("insert")` itself. In lesson 04, `insert` gets three values (`1 alice alice@example.com`). Then every part of the program that touches the statement would have to cut up the text again.

### Know
A **`struct`** groups related values into one new type. Each value inside is a **member**.
```cpp
struct Point {
    int x;
    int y;
};

Point p{3, 4};     // create a Point with x = 3, y = 4
p.x = 10;          // . reaches a member
```
**`{}` initialization:** `Point p{3, 4};` fills the members in order. *Why always initialize:* a member you don't set holds random leftover memory, not `0`.

### Do
```cpp
enum class StatementType { Insert, Select };

struct Statement {
    StatementType type;
};
```
There's no `Delete`, because `delete` must be rejected. In lesson 04, `Statement` gets more members (the row to insert).

✅ Builds (nothing uses the types yet)

---

## Step 2: Prepare a statement, or report "not recognized"

**Try this first:** how would `prepare_statement` say "I don't recognize this"? It has to return a `Statement`, but `delete 1` isn't an insert or a select. Any type you pick is a lie.

### Know
**`std::optional<T>`** (`#include <optional>`): either holds a `T` **or nothing**.
```cpp
std::optional<int> find_age(std::string name) {
    if (name == "alice") {
        return 30;          // holds a value
    }
    return std::nullopt;    // holds nothing
}

std::optional<int> age = find_age("bob");
if (age.has_value()) {
    std::cout << age.value();
}
```
| Use | Means |
|---|---|
| `return value;` | the optional holds `value` |
| `return std::nullopt;` | the optional holds nothing (`nullopt` = *null optional*) |
| `.has_value()` | `true` if something is inside |
| `.value()` | the thing inside. ⚠️ Calling it on an empty optional is an error that stops the program, so check `has_value()` first |

### Do
```cpp
std::optional<Statement> prepare_statement(std::string line)
```
- `insert …` (anything that starts with `insert`) → a `Statement` with type `Insert`
- exactly `select` → type `Select`
- anything else → nothing

In `main`: if it's empty, print `Unrecognized keyword at start of '<line>'.`

✅ `delete 1` → `Unrecognized keyword at start of 'delete 1'.`

---

## Step 3: Execute

Nothing new here: this uses `switch` from lesson 02.

### Do
```cpp
void execute_statement(Statement statement)
```
Use a `switch` on `statement.type` (no `default:`) to print the placeholder message. Back in `main`, print `Executed.` after it.

✅ All of "Done when" below

---

## Done when
- [ ] clean build, zero warnings
- [ ] `insert 1 alice alice@example.com` → insert message + `Executed.`
- [ ] `select` → select message + `Executed.`
- [ ] `delete 1` → `Unrecognized keyword at start of 'delete 1'.`
- [ ] `.foo` → `Unrecognized command '.foo'.` (lesson 02 still works)
- [ ] `.exit` and end of input (Ctrl-D) both quit with exit code `0`
- [ ] `main` only routes: no statement messages printed inside `main` except `Executed.`

## Check yourself
Answer before opening.

<details><summary>1. Why does <code>StatementType</code> have no <code>Delete</code>?</summary>
Because `delete` must be rejected. If the type existed, the program could pretend to understand a statement it can't run.
</details>

<details><summary>2. What happens if you call <code>.value()</code> on an empty <code>std::optional</code>?</summary>
It's an error that stops the program. Check `.has_value()` first.
</details>

<details><summary>3. (From lesson 02) You add <code>Update</code> to <code>StatementType</code>. What tells you that <code>execute_statement</code> must change too?</summary>
The build fails: the `switch` has no `case` for `Update`, and since there's no `default:`, `-Wall` + `-Werror` stop the build.
</details>

## Pitfalls
Mistakes made while building this lesson.

_Added as they come up._

## Q&A
Questions asked while building this lesson.

_Added as they come up._

## Compare with the reference code
```sh
git diff lesson-02-meta-commands..lesson-03-statements
```

## Go deeper (optional)
- [cppreference: `struct` / class declaration](https://en.cppreference.com/w/cpp/language/class)
- [cppreference: `std::optional`](https://en.cppreference.com/w/cpp/utility/optional)
- [SQLite: architecture](https://www.sqlite.org/arch.html) (how a real database splits parsing from execution)
