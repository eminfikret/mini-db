# 05. In-memory table

## What you'll build
Inserted rows are kept, and `select` shows them:
```
db > insert 1 alice alice@example.com
Executed.
db > insert 2 bob bob@example.com
Executed.
db > select
(1, alice, alice@example.com)
(2, bob, bob@example.com)
Executed.
db > .exit
```
**New in this lesson:** `std::vector` · references (`&`, `const &`) · range-based `for`

| Word | Means |
|---|---|
| **table** | a collection of rows that share one schema |
| **append-only** | new rows always go at the end, and nothing gets removed (yet) |
| **full scan** | looking at every row, one after another. Here, `select` is a full scan |

The table lives in memory, so everything is gone when the program ends. Saving to a file comes in lesson 11.

## Where this fits
```
REPL           read what you type, handle .meta-commands   ← 01–02 ✅
Parser         text → statement + checked row               ← 03–04 ✅
Executor       insert into / scan the table                 ← you are here
Table          rows in memory                               ← you are here
B-tree         find rows fast                               ← 16+
Pager          read/write the file                          ← 11–14
```

## How the pieces connect (end of this lesson)
```
main ── owns ONE table for the whole session
  │
  ├─ PREPARE (lesson 04): text → statement
  │
  └─ EXECUTE: gets the statement AND the table
        insert → adds the row to the end of the table
        select → prints every row, in insert order
```

---

## Step 1: A table that grows

### Know
**`std::vector`** (`#include <vector>`) is a list that grows as you add to it:
```cpp
std::vector<int> scores;      // empty list of ints
scores.push_back(7);          // add 7 at the end
scores.push_back(9);          // now: 7, 9
scores.size();                // 2
scores[0];                    // 7 (counting starts at 0)
```
*Why a vector:* you don't know in advance how many rows will be inserted.

### Do
Create a table type that holds rows. `main` creates **one** table before the loop, so it lasts the whole session.

<details><summary>Hint</summary>The table is a <code>struct</code> whose one member is a list of rows.</details>

✅ Builds; behavior unchanged

---

## Step 2: Insert into the table

**Try this first:** pass the table to execute the way you've passed everything so far (`Table table` as the parameter), add the row inside execute, then run `insert …` followed by `select`. Nothing is there. Why?

### Know
**Memory and addresses.** Every variable lives at a spot in memory. Each spot has a number, its **address**, like a house number.

**Copy vs reference.** A normal parameter gets a **copy** in a new spot. A **reference** (`&` after the type) is another name for the **same spot**:
```cpp
void add_by_copy(std::vector<int> scores)       { scores.push_back(10); }
void add_by_reference(std::vector<int>& scores) { scores.push_back(10); }

std::vector<int> mine{7};
add_by_copy(mine);        // mine is still: 7
add_by_reference(mine);   // mine is now:   7, 10
```
```
add_by_copy:       address 1000 │ 7 │ ← mine      address 2000 │ 7, 10 │ ← scores (copy, thrown away)
add_by_reference:  address 1000 │ 7, 10 │ ← mine and scores (same spot, two names)
```
| Parameter | Function gets | Use when |
|---|---|---|
| `T x` | a **copy** | small values (`int`, an `enum`) |
| `T& x` | the caller's spot | the function must **change** the caller's value |
| `const T& x` | the caller's spot, **read-only** | big values the function only **reads**. *Why:* no copy is made |

### Do
- Execute gets the table so that an insert can **change** it and a select can only **read** it.
- An insert adds its row at the end and prints only `Executed.` (no placeholder anymore).
- Clean-up: every function that takes a `std::string` it only **reads** should stop copying it.

<details><summary>Hint 1</summary>Which of the three parameter kinds lets a function change the caller's table?</details>
<details><summary>Hint 2</summary>A <code>std::string</code> that's only read: which kind avoids the copy and also forbids changes?</details>

✅ The "Try this first" bug is gone: the row is still there after the insert

---

## Step 3: Select = full scan

### Know
A **range-based `for`** runs once for every element of a list:
```cpp
std::vector<std::string> names{"ann", "ben"};

for (const std::string& name : names) {
    std::cout << name << '\n';      // prints ann, then ben
}
```
`const std::string&`: each element is **read** in place, with no copy and no changes.

### Do
Select prints every row, oldest first, one per line, as `(<id>, <username>, <email>)`, then `Executed.`

<details><summary>Hint</summary>The loop variable is one row. Which parameter kind from Step 2 fits "only reads"?</details>

---

## Done when
- [ ] clean build, zero warnings
- [ ] the transcript at the top gives exactly that output
- [ ] `select` on an empty table prints only `Executed.`
- [ ] 1000 inserts give 1000 rows:
  ```sh
  { for i in $(seq 1 1000); do echo "insert $i user$i user$i@x.com"; done; echo select; } | ./build/mini-db | grep -c '@x.com)'
  ```
  prints `1000` (`seq 1 1000` counts from 1 to 1000; `grep -c` counts matching lines)
- [ ] no function copies a `std::string` that it only reads

## Check yourself
Answer before opening.

<details><summary>1. Execute takes the table as <code>Table table</code>. Insert "works", but select shows nothing. Why?</summary>
Execute changed a <b>copy</b> of the table. The copy was thrown away when the function ended, and main's table never changed.
</details>

<details><summary>2. Why does select take the table as <code>const</code> reference, not plain reference?</summary>
Select only reads. <code>const</code> makes the compiler stop any accidental change, and the reference avoids copying every row.
</details>

<details><summary>3. (From lesson 04) Why does every column have a maximum size?</summary>
So every row fits the same fixed-size slot of bytes when rows are stored (lesson 08).
</details>

## Pitfalls
Mistakes made while building this lesson.

_Added as they come up._

## Q&A
Questions asked while building this lesson.

_Added as they come up._

## Compare with the reference code
```sh
git diff lesson-04-rows..lesson-05-table
```

## DSA corner (optional)
**DSA** = *data structures and algorithms*. You don't need this section to finish the lesson. *Why it matters here:* a database is fast or slow depending on how many rows it has to look at.

**Big-O** describes how the work grows as the data grows (**n** = number of rows):
| Notation | Means | Example |
|---|---|---|
| O(1) | the same work, whatever n is | reading `scores[0]` |
| O(n) | work grows in step with n | a full scan |

**Dynamic array:** what `std::vector` is inside. One block of memory, with elements side by side:
```
size 3, capacity 4:  │ 7 │ 9 │ 4 │   │          a free spot is left
push_back(5):        │ 7 │ 9 │ 4 │ 5 │          free spot → just write it
push_back(8):        full → get a bigger block, copy everything, free the old block
                     │ 7 │ 9 │ 4 │ 5 │ 8 │   │   │   │
```
| Word | Means |
|---|---|
| size | how many elements are in use |
| capacity | how many fit before the vector has to grow |
| amortized O(1) | most `push_back`s are O(1). The rare grow is O(n), but capacity multiplies each time (often ×2), so grows get rarer and the **average** stays O(1) |

| Operation on a vector | Cost |
|---|---|
| read element `i` | O(1) |
| `push_back` | amortized O(1) |
| find a row by id (**linear search**, which is what a full scan does) | O(n) |
| insert at the front | O(n): every element moves one spot |

**Try it:** push ten numbers into a vector one at a time and print `size()` and `capacity()` after each. When does the capacity jump?

<details><summary>Check: the table has 1,000,000 rows. About how many does <code>select</code> look at to find id 500,000?</summary>
Up to all 1,000,000, because a full scan is O(n). In lesson 17, keys are kept sorted and found by <b>binary search</b>: about 20 checks, because halving 1,000,000 twenty times gets you to 1 (that's O(log n)).
</details>

- [Open Data Structures (C++ edition), ch. 2: array-based lists](https://opendatastructures.org/ods-cpp/2_Array_Based_Lists.html): how a growable array works and why `push_back` is amortized O(1)
- [VisuAlgo: array](https://visualgo.net/en/array): animations of insert and search
- [Big-O cheat sheet](https://www.bigocheatsheet.com/): the costs of common data structures in one table

## Go deeper (optional)
- [cppreference: `std::vector`](https://en.cppreference.com/w/cpp/container/vector)
- [cppreference: references](https://en.cppreference.com/w/cpp/language/reference)
- [cppreference: range-based `for`](https://en.cppreference.com/w/cpp/language/range-for)
