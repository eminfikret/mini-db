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
B-tree         find rows fast                               ← 16+
Pager          read/write the file                          ← 11–14
file on disk   mini.db
```
The **parser** turns text into something the program can work with. The **executor** acts on it. *Why separate them:* the text is read once. Every later layer works with the result and never looks at raw text again.

## How the pieces connect (end of this lesson)
```
main loop
  │
  ├─ starts with '.' ──► meta-command function          (lesson 02)
  │
  └─ otherwise ───────► PREPARE: text → a statement, or "not recognized"
                            │
                            ├─ not recognized → "Unrecognized keyword at start of '…'."
                            │
                            └─ a statement ──► EXECUTE: prints the placeholder message
                                                 → main prints "Executed."
```
**Prepare** reads the text. **Execute** never sees text, only the statement.

---

## Step 1: Give a statement a shape

**Try this first:** picture the executor checking the text for `insert` itself. In lesson 04, `insert` gets three values (`1 alice alice@example.com`). Then every part of the program that touches the statement would have to cut up the text again.

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
Create a type that represents a statement. For now it only needs to say **which kind** of statement it is: insert or select. Nothing else is a valid kind.

In lesson 04, a statement gets more members (the row to insert), so make it a type that can grow.

<details><summary>Hint 1</summary>"Which kind, from a fixed list" is what lesson 02 used an `enum class` for.</details>
<details><summary>Hint 2</summary>The statement itself is a `struct` with one member: its kind.</details>
<details><summary>Hint 3</summary>Should `delete` be one of the kinds? Check what the transcript expects for `delete 1`.</details>

✅ Builds (nothing uses the types yet)

---

## Step 2: Prepare a statement, or report "not recognized"

**Try this first:** how would the prepare function say "I don't recognize this"? If it always gives back a statement, what kind would `delete 1` be? Any kind you pick is a lie.

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
Write the **prepare** function. It gets the line and gives back either a statement or "not recognized":
- starts with `insert` (values come after it) → an insert statement
- exactly `select` → a select statement
- anything else → not recognized

In `main`: when it's not recognized, print `Unrecognized keyword at start of '<line>'.`

<details><summary>Hint 1</summary>"A statement, or nothing" is exactly what the `Know` box above is about.</details>
<details><summary>Hint 2</summary>Two different checks: one where more text may follow, one that must match exactly. You used both in lesson 02.</details>
<details><summary>Hint 3</summary>Create the statement with `{}` initialization, the same way the `Point` example does.</details>

✅ `delete 1` → `Unrecognized keyword at start of 'delete 1'.`

---

## Step 3: Execute

Nothing new here: this uses `switch` from lesson 02.

### Do
Write the **execute** function. It gets a statement (never text), prints the placeholder message for its kind, and gives nothing back. Back in `main`, print `Executed.` after it.

<details><summary>Hint 1</summary>Pick the message by the statement's kind. Which tool from lesson 02 does that and catches a forgotten kind?</details>
<details><summary>Hint 2</summary>`main` only calls execute when prepare **did** recognize the line. Get the statement out before you pass it on.</details>

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

<details><summary>1. Why is <code>delete</code> not one of the statement kinds?</summary>
Because `delete` must be rejected. If the type existed, the program could pretend to understand a statement it can't run.
</details>

<details><summary>2. What happens if you call <code>.value()</code> on an empty <code>std::optional</code>?</summary>
It's an error that stops the program. Check `.has_value()` first.
</details>

<details><summary>3. (From lesson 02) You add an <code>update</code> kind. What tells you that the execute function must change too?</summary>
The build fails: the `switch` has no `case` for `Update`, and since there's no `default:`, `-Wall` + `-Werror` stop the build.
</details>

## Pitfalls
Mistakes made while building this lesson.

_Added as they come up._

## Q&A
Questions asked while building this lesson.

**How should I name a variable that holds a `std::optional`?**
There's no official rule. Common styles: name it after what's inside (`statement`), or add a prefix that shows it might be empty: `maybe_statement` or `opt_statement`. A prefix makes `maybe_statement.value()` read naturally ("take the statement out of the maybe"). Pick one style and use it everywhere.

## Compare with the reference code
```sh
git diff lesson-02-meta-commands..lesson-03-statements
```

## Go deeper (optional)
- [cppreference: `struct` / class declaration](https://en.cppreference.com/w/cpp/language/class)
- [cppreference: `std::optional`](https://en.cppreference.com/w/cpp/utility/optional)
- [SQLite: architecture](https://www.sqlite.org/arch.html) (how a real database splits parsing from execution)
