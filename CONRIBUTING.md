# Contributing to cpp-project-template

Thank you for investing your time in contributing to our project!

In this guide you will get an overview of the contribution workflow
from opening an issue, creating a PR, reviewing, and merging the PR.

To get an overview of the project, read the [README](README.md).

## Code of Conduct

This project and everyone participating in it is governed by the [Code of Conduct](CODE_OF_CONDUCT.md).
By participating, you are expected to uphold this code.
Please report unacceptable behavior to [INSERT CONTACT METHOD].

## Issues

### Create a new issue

If you spot a problem with the docs, [search if an issue already exists](https://docs.github.com/en/github/searching-for-information-on-github/searching-on-github/searching-issues-and-pull-requests#search-by-the-title-body-or-comments).
If a related issue doesn't exist, you can [open a new issue](https://github.com/b1ackviking/cpp-project-template/issues/new).

### Solve an issue

Scan through our [existing issues](https://github.com/b1ackviking/cpp-project-template/issues)
to find one that interests you. You can narrow down the search using `labels` as filters.
As a general rule, we don’t assign issues to anyone.
If you find an issue to work on, you are welcome to open a PR with a fix.

## Make Changes

1. Fork the repository so that you can make your changes
   without affecting the original project until you're ready to merge them.
1. Set up the repository and make sure you are able to build the project
   using instructions provided in [README](README.md).
1. Create a new branch off the `main` and start with your changes.
1. Your changes should conform to the Style Guide provided below.
1. Commit your changes following the [conventional commits specification](https://www.conventionalcommits.org/en/v1.0.0/).

## Pull Request

Once your changes are ready, don't forget to self-review to speed up the review process.

- Does the new code conform to the style guide?
- Does the project compiles without warnings and passes all unit tests?
- Have you used pre-commit hooks to validate your commits?
- Are comments and documentation written in clear English, with no spelling or grammar errors?
- If you're unsure about any of these points, please open the Pull Request anyhow and then ask us for feedback.

When you create a pull request, please write a short description
to help reviewers understand your changes as well as the purpose of your pull request,
and [link PR to an issue](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue)
if you are solving one.

## Style guide

The purpose of the style guide is to encourage common coding practices
and to keep the code base manageable,
while still allowing coders to use C++ language features productively.

### The golden rule

If you are extending, enhancing, or bug fixing already implemented code,
use the style that is already being used so that the source is uniform and easy to follow.

### Language version

- Use the latest C++ standard.
- Do not use any non-standard extensions.

### C++ exceptions

- Use exceptions to report errors that can not be resolved locally.
- Avoid catching exceptions if an error can not be resolved.
- Do not use exceptions as the control flow!

### Catch errors as early as possible in the development process

- Treat warnings as errors.
- Use compile-time checks where possible (`static_assert`, [concepts](https://en.cppreference.com/w/cpp/concepts), etc).
- Use runtime checks in Debug builds (`assert`).

### Naming convention

- Avoid multi-word names.
- Do not use contractions.
- Use `snake_case` for file and directory names.
- Use `.hpp` extension for C++ header files and `.cpp` extension for C++ source files.
- Use `.h` extension for C-compatible header files and `.c` extension for C source files.
- Add a `-test` suffix for unit test filenames: `graphics_device-test.cpp`.
- Use `snake_case` for namespace, type, function, constant, variable names.
- Add a `_` suffix to private and protected non-static data members.
- Use `CamelCase` for template parameters.
- Use `UPPER_CASE` for macro names: `#define DO_NOT_USE_MACROS(x) ((x)*(x)*(x))`

#### Use inclusive language

In all code, including naming and comments, use inclusive language
and avoid terms that other programmers might find disrespectful or offensive
(such as "master" and "slave", "blacklist" and "whitelist", or "redline"),
even if the terms also have an ostensibly neutral meaning.
Similarly, use gender-neutral language unless you're referring to a specific person (and using their pronouns).
For example, use "they"/"them"/"their" for people of unspecified gender (even when singular),
and "it"/"its" for software, computers, and other things that aren't people.

### Use of macros

- Avoid using macros of any kind.
- Allowed macros are `NDEBUG` and `assert`.

### Header files

- Put public header files in a separate `include/modulename` folder.
- Put private header files in the same folder as the source files.
- Prefer using `#pragma once` to header guards.
- All header files should be self-contained, meaning that it does not have any include dependencies.
- Include as little as possible.

### Forward declarations

- Use forward declarations if the full definition of an object is not required.
- Do not use forward declarations for objects from other projects.

### Namespaces

- Do not use `using namespace` outside of the function scope in header files.
- Avoid defining any symbols in the global namespace.
- Put all definitions local to the translation unit into an anonymous `namespace`, i.e. do not use `static` for that purpose.
- Use namespace qualifiers to implement previously declared functions.

```cpp
// BAD
// file.hpp
namespace foo {
int bar(const char* s);
} // namespace foo

// file.cpp
#include "file.hpp"
namespace foo {
int bar(char*) { return 42; } // creates an overload
} // namespace foo


// OK
// file.hpp
namespace foo {
int bar(const char* s);
} // namespace foo

// file.cpp
#include "file.hpp"
int foo::bar(char*) { return 42; } // raises a compilation error
```

### Aliases

- Do not use macros to create type aliases.
- Use `using` instead of `typedef` in C++.

```cpp
// BAD
#define Type int // absolute evil
typedef int AnotherType; // bad in C++, OK in C
typedef int (*GetIndexedValue_PFN)(Object* pObj, unsigned index);

// OK
using Type = int;
using AnotherType = int;
using GetIndexedValue_PFN = int (*)(Object* pObj, unsigned index);
```

### Variables

- Place a function's variables in the narrowest scope possible, and initialize variables in the declaration.

```cpp
// BAD
void foo() {
  std::map<ID, std::string> name_lookup;
  name_lookup = get_lookup_table();

  auto it = name_lookup.find(id::player_name);
  if (it != name_lookup.end()) {
    bar(it->second);
  } else {
    bar("UNDEFINED"s);
  }
}

// OK
void foo() {
  auto name_lookup = get_lookup_table();
  if (auto it = name_lookup.find(id::player_name); it != name_lookup.end()) {
    bar(it->second);
  } else {
    bar("UNDEFINED"s);
  }
}
```
- Avoid a global state of any kind.
- Use `constexpr` to declare constants.
- Use anonymous `namespaces` to declare variables local to the translation unit.
- Do not mark an object as `static` if it is not [trivially destructible](https://en.cppreference.com/w/cpp/types/is_destructible).
- Do not use `thread_local` variables outside of function scope.

### Functions

- Cheap to copy arguments shall be passed by value, e.g. built-in types.
- An argument that can not be null shall be passed by reference.
- The result of a function shall be returned by value in most cases.
- Avoid using output parameters to return value or multiple values (return a `struct` or a tuple).
- When returning multiple values, prefer returning a named `struct` instead of a pair or tuple.
- Use overloaded functions (including constructors) only if a reader looking at a call site can get a good idea of what is happening without having to first figure out exactly which overload is being called.
- Do not use default arguments on `virtual` functions.
- Avoid using trailing return types.

### Structs

- In most cases prefer named `struct`s to pairs or tuples.

```cpp
// BAD
std::pair<float, float> foo();

// OK
struct point {
  float x;
  float y;
};
point foo();
```

### Unions

- Avoid using `union`s.
- Use `std::variant` to provide storage for objects of a limited number of types.
- Avoid using `std::any`.

### Classes

- Follow the [RAII](https://en.cppreference.com/w/cpp/language/raii) idiom.
- Use constructors instead of initialization methods.
- The constructor shall throw an exception if a class invariant can not be established.
- Use [The Inverse Two-Phase Initialization Pattern](https://youtu.be/S7I66lZX_zM) if exceptions are unavailable.
- Destructors shall be `noexcept`.
- A class shall either define no special member functions or all of them (rule of 0 and rule of 5).
- Constructors that take one argument shall be marked `explicit`.
- Do not omit access specifiers in the inheritance list.
- Avoid using `virtual` inheritance: it is an indicator of a poorly designed architecture.
- Do not redefine (shadow) non-virtual member functions.
- Virtual member functions shall only be marked as `virtual` in a base class and `override` or `final` in derived classes.

```cpp
// BAD
class base {
public:
  virtual void foo(int) {}
};

class derived : public base {
public:
  virtual void foo(float) {} // BUG: creates an overload
};

// OK
class base {
public:
  virtual void foo(int) {}
};

class derived : public base {
public:
  void foo(float) override {} // OK: raises a compilation error
};
```
- Avoid overloading operators unless the overload is obvious or the class implements an STL-like interface.
- Avoid using `friend` declarations for anything except operator overloads.
- All resource handles and implementation details shall be kept `private`.
- If a class represents just data, make it public.

```cpp
// BAD
class foo {
public:
  std::FILE* file; // BAD: direct access to a resource
};

// BAD: overcomplicated class for no reason
class vector3f {
private:
  float x_, y_, z_;
public:
  [[nodiscard]] float get_x() const noexcept { return x_; }
  [[nodiscard]] float get_y() const noexcept { return y_; }
  [[nodiscard]] float get_z() const noexcept { return z_; }


  void set_x(float value) noexcept { x_ = value; }
  void set_y(float value) noexcept { y_ = value; }
  void set_z(float value) noexcept { z_ = value; }
};

// OK
class foo {
private:
  std::FILE* file_;
};

class vector3f {
public:
  float x, y, z;
};
```

### Use of `volatile`

- Apply `volatile` qualifier only to objects that are outside of the language's memory model.

### Use of `const`

- All immutable variables and references must be declared `const`.

### Use of `constexpr`

- Apply `constexpr` to all compile-time known values.

```cpp
// BAD
#define A 2147483647 // 32-bit int
#define B 2147483647 // 32-bit int
#define SUM (A + B) // BUG: overflow
// OK
constexpr int a = 2147483647;
constexpr int b = 2147483647;
constexpr int sum = a + b; // raises a compilation error
```

- All functions that do not use runtime APIs and non-`constexpr` objects shall be declared `constexpr`.

```cpp
// BAD
#define MAX(a, b) ((a) >= (b)) ? (a) : (b) // BUG: MAX(a++, ++b)
// runtime evaluation only
static inline int max(int a, int b) noexcept { return a >= b ? a : b; }

// OK
// both runtime and compile-time evaluation
constexpr int max(int a, int b) noexcept { return a >= b ? a : b; }
```

### Use of `noexcept`

- All functions that do not throw any exceptions and do not call any non-`noexcept` functions shall be declared `noexcept`.

### Casting

- Do not use C-style casts in C++ even if the cast is obvious.
- Do not use `static_cast` to cast a pointer down the class hierarchy.
- Avoid using `reinterpret_cast`: use of `reinterpret_cast` most likely invokes undefined behavior.
- Do not use `const_cast` to cast away `const`.

### Pre- and post- increment/decrement

- Avoid post-increment/decrement if not necessary.

```cpp
// BAD
for (int i{}; i < n; i++) {
  count++;
  arr[i] = count;
}

std::uint8_t* deserialize_rgba(std::uint8_t* byte_stream, rgba& color) {
  color.r = *byte_stream;
  byte_stream++;
  color.g = *byte_stream;
  byte_stream++;
  color.b = *byte_stream;
  byte_stream++;
  color.a = *byte_stream;
  byte_stream++;
  return byte_stream;
}

// OK
for (int i{}; i < n; ++i) {
  arr[i] = ++count;
}

std::uint8_t* deserialize_rgba(std::uint8_t* byte_stream, rgba& color) {
  color.r = *byte_stream++;
  color.g = *byte_stream++;
  color.b = *byte_stream++;
  color.a = *byte_stream++;
  return byte_stream;
}
```

### Use of `enum`

- Use scoped `enums` to declare sets of related constants.
- Do not use `enums` as bitmasks.

### Use of `NULL`

- Use `nullptr` instead of `NULL` or `0` for pointers in C++.
- Use `NULL` instead of `0` for pointers in C.
- Use API-provided constants instead of `0` for API handles.

```cpp
// BAD
// source.cpp
some_type* ptr = 0;
some_api handle = 0;

// OK
// API.hpp
#if SOMETHING
using some_api = struct some_api_t*;
constexpr std::nullptr_t api_null_handle = nullptr;
#else
using some_api = std::uint64_t;
constexpr std::uint64_t api_null_handle = 0ULL;
#endif

// source.cpp
some_type* ptr = nullptr;
some_api handle = api_null_handle;
```

### Use of `sizeof`

- Use `sizeof` operator only to get the size of an object,
i.e. do not use `sizeof` operator to calculate the size of an array in C++ (allowed in C).
- Use `sizeof(object)` instead of `sizeof(type)`.

```cpp
// BAD
WNDCLASSEX wc{};
wc.cbsize = sizeof(WNDCLASS); // BUG

// OK
WNDCLASSEX wc{};
wc.cbsize = sizeof(wc); // OK
```

### Type deduction

- Use `auto` when the type is obvious: a result of a cast or a function call.

```cpp
// BAD
auto a = 42;  // a is an int
auto& b = a;  // b is an int&
auto c = b;   // c is an int
auto d{42};   // d is an int
auto e = {42} // e is std::initializer_list<int>

// OK
float f = 3.14f;
auto i = static_cast<int>(f);

std::vector<int> vec{ 1, 2, 3, 4, 5 };
auto it = vec.begin();
```

### RTTI

- Avoid using RTTI on the hot paths of a code.

### Memory management
- Do not use raw `new` and `delete` for dynamic memory management (use `std::make_unique` or `std::make_shared`).
- Never call `malloc`, `calloc`, `realloc`, `free`, and others in C++ code.
- Use polymorphic memory resources to provide static storage for allocating objects.
- Use `placement new` to start the life of an object in memory.
- Prefer using references rather than raw pointers as a non-owning view of an object.
- Use `std::unique_ptr` as an exclusive owning resource handle.
- Use `std::shared_ptr` as a non-exclusive owning resource handle.
- Use `std::weak_ptr` as a temporary non-exclusive owning resource handle.

### Use of `goto`

- Do not use `goto`.

### Use of attributes

[Attributes](https://en.cppreference.com/w/cpp/language/attributes)
provide the unified standard syntax for implementation-defined language extensions,
such as the GNU and IBM language extensions `__attribute__((...))`, Microsoft extension `__declspec()`, etc.

- Use attributes only if you are sure about their meaning.
- Use standard syntax for attributes.

```cpp
// BAD
int max(int x, int y) __attribute__((always_inline));

// OK
[[gnu::always_inline]] int max(int x, int y);
```

- Use `[[nodiscard]]` attribute to encourage the compiler to issue a warning if the return value is discarded.

``` cpp
// BAD
std::FILE* open(const char* filename, const char* mode);

int main() {
  open("file.txt", "r"); // BUG: lost file handle
}

// OK
[[nodiscard]] std::FILE* open(const char* filename, const char* mode);

int main() {
  open("file.txt", "r"); // OK: triggers a compiler warning
}
```

### Formatting rules

- Use [clang-format](https://clang.llvm.org/docs/ClangFormat.html) to automatically format the code.
The formatting rules for the project are set in the `.clang-format` file in the project root directory.

### Commenting

Comments are important for readability and maintainability.
When writing comments, write them in English prose, using proper capitalization, punctuation, etc.
Aim to describe what the code is trying to do and why but not how it does it at a micro-level.

- Use C++-style comments (`//` for normal comments, `///` for doxygen documentation comments).
- All public interfaces should have descriptive doxygen comments.
- Do not try to comment out a block of code using C-style comment (`/**/`). Use `#if 0` and `#endif` instead.
- Format special comments as follows: `// TYPE: text`. For example:

```cpp
// NOTE: result of reinterpret_cast is not a constant expression
...
// TODO: implement foo
```

- Always leave notes and references when it is not obvious what does the code does.
- Always add namespace comments at the end.

```cpp
namespace foo
{
...
} // namespace foo
```
