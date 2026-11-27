# 04. Rows

## What you'll build
`insert` now reads real values and checks them:
```
db > insert 1 alice alice@example.com
This is where we would insert (1, alice, alice@example.com).
Executed.
db > insert -1 bob bob@example.com
ID must be positive.
db > insert 2 bob
Syntax error. Could not parse statement.
db > insert 3 aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa carol@example.com
String is too long.
db > select
This is where we would do a select.
Executed.
db > .exit
```
**New in this lesson:** fixed-width integers · string streams · `[[nodiscard]]`

| Word | Means |
|---|---|
| **row** | one record in a table, e.g. one user |
| **column** | one named field of a row, e.g. `username` |
| **schema** | the list of columns, with their types and limits |

This database has one fixed schema:
| Column | Type | Limit |
|---|---|---|
| `id` | whole number | 1 to 4,294,967,295 |
| `username` | text | at most 32 characters |
| `email` | text | at most 255 characters |

*Why fixed limits:* in lesson 08, every row is stored in a slot of bytes of the **same size**. A row only fits its slot if every column has a maximum size.

## Where this fits
```
REPL           read what you type, handle .meta-commands   ← 01–02 ✅
Parser         text → statement + row values, checked       ← you are here
Executor       run the statement                            ← placeholder messages
B-tree         find rows fast                               ← 16+
Pager          read/write the file                          ← 11–14
file on disk   mini.db
```

## How the pieces connect (end of this lesson)
```
main ──► PREPARE: text → (outcome, statement)
           outcome = success / unrecognized / syntax error / too long / negative id
         │
         ├─ not success → main prints the matching error message
         └─ success     → EXECUTE: insert prints the row it would insert
                                   → main prints "Executed."
```
**Prepare** still reads the text and execute still never sees it. What's new is that an insert statement now **carries a row**.

---

## Step 1: A row type

**Try this first:** after prepare reads `insert 1 alice alice@example.com`, where are `1`, `alice` and `alice@example.com`? Nowhere: they get thrown away.

### Know
**Fixed-width integers** (`#include <cstdint>`) are whole-number types with an exact size on every computer:
```cpp
std::uint8_t  level = 200;           // 8 bits:  0 to 255
std::uint32_t visitors = 4000000000; // 32 bits: 0 to 4,294,967,295
std::int64_t  balance = -500;        // 64 bits: about ±9.2 × 10^18
```
| Part of the name | Means |
|---|---|
| `int` / `uint` | **signed** (can be negative) / **unsigned** (0 and up, never negative) |
| the number | how many **bits** (binary digits) it uses. 8 bits = 1 **byte** |
| `_t` | "type", a naming habit from the standard library |

*Why not just `int`:* the size of plain `int` depends on the computer and compiler. A file format needs **exact** sizes, so a file written on one machine can be read on another.

### Do
Create a type for one row with the three columns from the schema. A statement now also carries the row to insert.

<details><summary>Hint 1</summary>Which fixed-width type fits "never negative, up to about 4 billion"?</details>
<details><summary>Hint 2</summary>A <code>struct</code> can hold another <code>struct</code> as a member.</details>

✅ Builds; behavior unchanged

---

## Step 2: Say *why* it failed

**Try this first:** prepare gives back a `std::optional`. Now there are four ways an insert can fail. How would `main` know whether to print `Syntax error…`, `String is too long.` or `ID must be positive.`? It can't, because "nothing" has no reason attached.

### Know
Nothing new about the result itself: you already know `enum class` (lesson 02) and `struct` (lesson 03).

New: **`[[nodiscard]]`** is an **attribute**, a note to the compiler written in `[[ ]]`. It means "don't ignore what this function gives back":
```cpp
[[nodiscard]] int roll_dice() { return 4; }

roll_dice();            // result ignored → warning → with -Werror, a build error
int n = roll_dice();    // fine
```
*Why:* a function that reports success or failure is useless if the caller never looks at the result. Now the compiler catches that.

### Do
Prepare gives back **one result** with exactly **two members**:
```
result of prepare
├─ outcome     one of: success · unrecognized keyword · syntax error · string too long · negative id
└─ statement   a plain statement (type + row), not optional
```
| Who | Does |
|---|---|
| **prepare** | prints **nothing**. Every failed check **returns right away** with its outcome. Only when every check passes does it return success together with the filled-in statement. Marked `[[nodiscard]]` |
| **the statement on failure** | stays in its `{}` state. Nobody reads it |
| **main** | looks at the outcome first: success → execute, then `Executed.`; any other outcome → print its message, and the statement is ignored |

| Outcome | `main` prints |
|---|---|
| success | (execute's message) `Executed.` |
| unrecognized keyword | `Unrecognized keyword at start of '<line>'.` |
| syntax error | `Syntax error. Could not parse statement.` |
| string too long | `String is too long.` |
| negative id | `ID must be positive.` |

*Why no optional and no message text in the result:* the outcome already says whether the statement is meaningful, so an optional would say the same thing twice. Keeping the text in `main` means each message lives in one place, and a forgotten outcome makes the build fail.

`std::optional` did its job while there was only one way to fail. Now it's replaced, and its `#include` can go.

<details><summary>Hint 1</summary>"One of a fixed list" was <code>enum class</code> in lesson 02.</details>
<details><summary>Hint 2</summary>"Two values travelling together" was <code>struct</code> in lesson 03.</details>
<details><summary>Hint 3</summary>Which tool from lesson 02 picks a message per outcome and tells you if you forgot one?</details>

✅ Builds · try calling prepare without using its result: the build fails · `insert 1 alice alice@example.com` still works

---

## Step 3: Read the values with a string stream

**Try this first:** picture cutting `insert 1 alice alice@example.com` into four pieces yourself, finding each space and copying the parts. It's a lot of fiddly code for a simple job.

### Know
A **string stream** (`std::istringstream`, `#include <sstream>`) reads from a piece of text the same way `std::cin` reads from the keyboard:
```cpp
std::istringstream input{"bob 42"};
std::string name;
int age;

if (input >> name >> age) {
    // name is "bob", age is 42
}
```
| Fact | |
|---|---|
| `>>` reads the **next word**, skipping spaces | reading into an `int` also converts `"42"` into the number `42` |
| `input >> a >> b` gives `true` only if **every** read worked | a missing word, or `"abc"` read into an `int`, gives `false` |
| after the last word, another `>>` gives `false` | so you can check "was there anything extra?" |
| ⚠️ reading `-1` into an **unsigned** type does **not** fail | it silently gives `4294967295`. Read numbers into a **signed** type first, check the range, then store them |

### Do
Prepare reads the line word by word:
| Input | Outcome |
|---|---|
| first word is exactly `insert`, followed by id, username, email | success, with the row |
| first word is exactly `select`, nothing after it | success |
| any other first word | unrecognized keyword |
| `insert` with a value missing, an id that isn't a number, or extra words | syntax error |
| id below 1 | negative id → `ID must be positive.` |
| id above 4,294,967,295 | syntax error |
| username over 32 characters, or email over 255 | `String is too long.` |

Execute for an insert prints `This is where we would insert (<id>, <username>, <email>).`

✅ **Notice:** `insertx 1 a b` is now rejected too. The first word is read as a whole word, so `insertx` ≠ `insert`.

<details><summary>Hint 1</summary>The first word you read decides which statement it is.</details>
<details><summary>Hint 2</summary>After the email, try reading one more word. What should happen if that works?</details>
<details><summary>Hint 3</summary>Read the id into a signed type that's bigger than 32 bits, check it, and only then put it in the row.</details>
<details><summary>Hint 4</summary><code>std::string</code> can tell you how many characters it has. Look up <code>size</code> on cppreference.</details>

---

## Done when
- [ ] clean build, zero warnings
- [ ] every line of the transcript at the top gives exactly that output
- [ ] a 32-character username and a 255-character email are accepted; 33 and 256 are rejected
- [ ] `insert 0 a b` → `ID must be positive.` · `insert 4294967296 a b` → syntax error
- [ ] `insert 1 a b c` (extra word) → syntax error · `select x` → still rejected
- [ ] ignoring prepare's result fails the build

Testing long strings without counting by hand (`printf 'a%.0s' {1..33}` prints 33 `a`s):
```sh
printf 'insert 1 %s x@y.com\n' "$(printf 'a%.0s' {1..33})" | ./build/mini-db
```

## Check yourself
Answer before opening.

<details><summary>1. Why read the id into a signed type first, even though ids are never negative?</summary>
Reading <code>-1</code> into an unsigned type doesn't fail. It silently becomes 4294967295, a huge valid-looking id. A signed type keeps the minus sign, so you can reject it.
</details>

<details><summary>2. What does <code>input >> age</code> give if the next word is <code>abc</code> and <code>age</code> is an <code>int</code>?</summary>
<code>false</code>: the conversion fails, so the read fails.
</details>

<details><summary>3. (From lesson 03) Why does execute never look at the text?</summary>
The text is read once, in prepare. Everything after that works with the statement, so the cutting-up of text lives in one place only.
</details>

## Pitfalls
Mistakes made while building this lesson.

_Added as they come up._

## Q&A
Questions asked while building this lesson.

_Added as they come up._

## Compare with the reference code
```sh
git diff lesson-03-statements..lesson-04-rows
```

## Go deeper (optional)
- [cppreference: fixed-width integer types](https://en.cppreference.com/w/cpp/types/integer)
- [cppreference: `std::istringstream`](https://en.cppreference.com/w/cpp/io/basic_istringstream)
- [cppreference: `[[nodiscard]]`](https://en.cppreference.com/w/cpp/language/attributes/nodiscard)
