# Contributing to cpp-project-template

Thank you for investing your time in contributing to our project!

In this guide you will get an overview of the contribution workflow
from opening an issue, creating a PR, reviewing, and merging the PR.

To get an overview of the project, read the [README](../README.md).

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
   using instructions provided in [README](../README.md).
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

### Use inclusive language

In all code, including naming and comments, use inclusive language
and avoid terms that other programmers might find disrespectful or offensive
(such as "master" and "slave", "blacklist" and "whitelist", or "redline"),
even if the terms also have an ostensibly neutral meaning.
Similarly, use gender-neutral language unless you're referring to a specific person (and using their pronouns).
For example, use "they"/"them"/"their" for people of unspecified gender (even when singular),
and "it"/"its" for software, computers, and other things that aren't people.

### C++ language

- Use the latest language version available.
- Do not use any non-standard extensions.
- Use exceptions to report critical errors that can not be resolved locally.
- Avoid catching exceptions if an error can not be resolved.
- **Do not use exceptions as the control flow.**
- Use compile-time checks (`static_assert`, [concepts](https://en.cppreference.com/w/cpp/concepts), etc.).
- Use runtime checks in Debug builds (`assert`).
- Build the code with all warnings enabled.
- **Treat warnings as errors.**

### Naming convention

- Avoid multi-word names.
- Do not use contractions.
- Use `.hpp` extension for C++ header files and `.cpp` extension for C++ source files.
- Use `.h` extension for C-compatible header files and `.c` extension for C source files.
- Use `CONSTANT_CASE` for macro names: `#define DO_NOT_USE_MACROS(x) ((x)*(x)*(x))`
- Do not use double\_\_underscores and \_leading underscores.

### Formatting

- Use [clang-format](https://clang.llvm.org/docs/ClangFormat.html) to automatically format the code.
  The formatting rules for the project are set in the `.clang-format` file in the project root directory.

### Header files

- Put public header files in a separate `include/modulename` folder.
- Put private header files in the same folder as the source files.
- Prefer using `#pragma once` to header guards.
- All header files should be self-contained, meaning that it does not have any include dependencies.
- Include as little as possible.
- Do not use forward declarations for types from other projects/libraries.

### Namespaces

- Do not use `using namespace` outside of the function scope in header files.
- Avoid defining any symbols in the global namespace.
- Put all definitions local to the translation unit into an anonymous `namespace`, i.e. do not use `static` for that purpose.
- Use namespace qualifiers to implement previously declared functions.

### Types

- Do not use macros to create type aliases.
- Use `using` instead of `typedef` in C++.
- Prefer named `strcut`s over pairs or tuples.
- Avoid using `union`s.
- Follow the [RAII](https://en.cppreference.com/w/cpp/language/raii) idiom.
- Follow the [Rule of 0 or Rule of 5](https://en.cppreference.com/w/cpp/language/rule_of_three).
- Destructors shall be `noexcept`.
- Do not omit access specifiers in the inheritance list.
- Avoid using `virtual` inheritance.
- Do not redefine (shadow) non-virtual member functions.
- Virtual member functions shall only be marked as `virtual` in a base class and `override` or `final` in derived classes.
- Avoid overloading operators unless the overload is obvious or the class implements an STL-like interface.
- Do not use C-style casts in C++ even if the cast is obvious.
- Do not use `static_cast` to cast a pointer down the class hierarchy.
- Avoid using `reinterpret_cast`.
- Do not use `const_cast` to cast away const.

### Variables

- Place variables in the narrowest scope possible.
- Initialize variables in the declaration.
- Use `constexpr` to declare compile-time known constants.
- Use `const` to declare run-time constants.
- Do not mark an object as `static` if it is not [trivially destructible](https://en.cppreference.com/w/cpp/types/is_destructible).
- Do not use `thread_local` variables outside function scope.

### Functions

- Pass cheap-to-copy arguments by value.
- Return the result by value.
- When returning multiple values, prefer returning a named `struct` instead of a pair or tuple.
- Do not use default arguments on `virtual` member-functions.
- Avoid using trailing return types.

### Other

- Do not use macros to define constants or functions (use `constexpr` instead).
- Do not produce side effects in the `assert` macro.
- Avoid using naked `new` and `delete` for dynamic memory management.
- Use `nullptr` instead of `NULL` or `0` for pointers.
- Do not use `sizeof` operator to calculate the size of an array.
- Prefer `sizeof(object)` to `sizeof(type)`.
- Avoid using `goto`.
- Avoid using any non-standard attributes, but if you have to, use [standard syntax](https://en.cppreference.com/w/cpp/language/attributes).
- Do not try to comment out a block of code using C-style comment (`/**/`). Use `#if 0` and `#endif` instead.
