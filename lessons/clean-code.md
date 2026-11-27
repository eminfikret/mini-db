# Clean-code rules

The rules this project follows. Each one is introduced (and taught) in the lesson shown, and from then on every lesson's **Clean up** section checks the code against all rules so far. A clean-up never changes behavior: the same input gives exactly the same output.

| # | Rule | *Why* | From |
|---|---|---|---|
| 1 | Name things after what they are. Types in `PascalCase`, functions, variables and constants in `snake_case`, no `ALL_CAPS` | names are the first documentation a reader sees | 02 |
| 2 | A `switch` over an `enum class` has one `case` per option and **no `default:`** | the compiler then reports a forgotten option | 02 |
| 3 | `main` only routes: it reads, calls, and prints results. The work happens in functions | each part can be understood and tested on its own | 03 |
| 4 | Initialize every variable, usually with `{}` | an uninitialized number holds leftover memory | 03 |
| 5 | Mark functions whose result must be used `[[nodiscard]]` | ignoring a success/failure result becomes a build error | 04 |
| 6 | Every path of a function that reports an outcome sets it **on purpose**, right before it returns. Never rely on the starting value | `{}` sets an enum to its first option, which may be "success" | 04 |
| 7 | **Return right away** on failure. No `else` after a `return`, and at most 2 levels of nesting | the success path reads top to bottom as a flat list of checks | 04 |
| 8 | Each limit appears **once**, as a `constexpr` constant. Type limits come from `std::numeric_limits`, never from C macros like `UINT32_MAX` | one edit changes it everywhere, and the type is always right | 04 |
| 9 | A conversion that could lose data is written as `static_cast<T>(…)`, after checking the range | the reader sees the conversion is intended and safe | 04 |
| 10 | Include only the headers the file uses | unused includes slow the build and mislead the reader | 04 |
| 11 | Pass big values the function only reads as `const&` | no copy, and no accidental change | 05 |
| 12 | One job per function. When a function handles several cases, each case gets its own function | short functions are easy to read, name and test | 05 |
| 13 | A header only declares, its source file defines. Every header has an include guard, includes what it uses, and has no `using namespace` | each file can be understood alone, and nothing leaks into other files | 06 |
| 14 | Everything except `main` lives in `namespace minidb`. Helpers only one source file uses go in an unnamed namespace in that file | the project's names can't clash, and helpers stay private to their file | 06 |
