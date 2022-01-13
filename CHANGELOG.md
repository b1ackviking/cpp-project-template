# [1.0.0](https://github.com/b1ackviking/cpp-project-template/compare/0.2.1...1.0.0) (2022-01-13)


### Reverts

* "ci: use clang-format pre-commit hook from pocc" ([b32c261](https://github.com/b1ackviking/cpp-project-template/commit/b32c26159520afcc70b3768b463abbc6a4411767))

## 0.2.1 (2021-09-28)

### Fix

- **clang-tidy**: google-runtime-int use std::(u)int

## 0.2.0 (2021-09-24)

### Feat

- **cmake**: add gcc toolchain
- **cmake**: add clang toolchain
- **pre-commit**: add a cmake-format configuration
- **pre-commit**: add cmake-lint hook

### Fix

- **cmake**: gcc warnings
- **pre-commit**: disable cmake-lint

### Refactor

- **cmake**: style changes in the warnings
- **cmake**: cmake-lint errors in the cache.cmake
- **cmake**: cmake-lint errors in the defaults

## 0.1.0 (2021-09-22)

### Fix

- **main**: change the main signature
- **iwyu**: add -Wno-unknown-warning-option flag
- **clang-format**: update more parameters

### Feat

- **clang-tidy**: update the configuration
- **clang-format**: update the configuration
- **cmake**: add thread library support

## 0.0.2 (2021-09-18)

### Fix

- **main**: add a new line to hello message

## 0.0.1 (2021-09-18)

### Feat

- **pre-commit**: start using conventional commits
