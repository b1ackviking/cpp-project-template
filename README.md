# cpp-project-template

[![CI](https://github.com/b1ackviking/cpp-project-template/actions/workflows/ci.yml/badge.svg)](https://github.com/b1ackviking/cpp-project-template/actions/workflows/ci.yml)
[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit)](https://pre-commit.com)

## About cpp-project-template

This is a template of a cross-platform CMake-based C++ project.

- Run [Cppcheck](http://cppcheck.net/),
[Clang-Tidy](https://clang.llvm.org/extra/clang-tidy/),
[IWYU](https://include-what-you-use.org/),
and [Doxygen](https://www.doxygen.nl/index.html) as a part of your build.
- Ensure clean and robust code with a pre-configured set of warnings
for Clang, GCC, and MSVC.
- Create sanitizer-instrumented builds by changing a single flag.
- Validate files structure and follow
the [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/)
specification with [pre-commit](https://pre-commit.com/) hooks.
- Build and test your project with [GitHub Actions](https://github.com/features/actions).

## Required software

- [Git](https://git-scm.com/)
- [CMake](https://cmake.org/)
- [Ninja](https://ninja-build.org/)
- [LLVM](https://releases.llvm.org/)
- [Python](https://www.python.org/)
  - [poetry](https://python-poetry.org/)
- [Node.js](https://nodejs.dev/download/)
- [Chocolatey (Windows)](https://chocolatey.org/)
  - [OpenCppCoverage](https://community.chocolatey.org/packages/opencppcoverage)

Run the following commands in the root of the repository after cloning:

```bash
poetry install
poetry run pre-commit install
poetry run conan profile detect
```

## Building the project

```bash
poetry shell

# install libraries
conan install -pr:b default -pr:h default \
  -c:h tools.cmake.cmaketoolchain:generator=<CMake generator> \
  -pr:h conan/<profile matching the compiler in use> \
  -c:h tools.build:compiler_executables='{"c": "<CC>", "cpp": "<CXX>"}' \
  -c:h tools.build:skip_test=<True|False> \
  -s build_type=<Debug|RelWithDebInfo|Release|MinSizeRel> -b missing .

# configure
cmake --preset <preset> \
  -D ENABLE_CPPCHECK=<bool> \
  -D ENABLE_CLANG_TIDY=<bool> \
  -D ENABLE_IWYU=<bool> \
  -D ENABLE_IPO=<bool> \
  -D ENABLE_CACHE=<bool> \
  -D CACHE_OPTION=<ccache or sccache> \
  -D ENABLE_DOXYGEN=<bool> \
  -D ENABLE_COVERAGE=<bool> \
  -D ENABLE_HARDENINGS=<bool> \
  -D ENABLE_FORTIFY_SOURCE=<bool> \
  -D ENABLE_ASAN=<bool> \
  -D ENABLE_LSAN=<bool> \
  -D ENABLE_UBSAN=<bool> \
  -D ENABLE_TSAN=<bool>

# build
cmake --build --preset <preset>

# run tests
ctest --preset <preset>

# collect test coverage if ENABLE_COVERAGE == TRUE
GCOV=<"gcov" for GCC, "llvm-cov gcov" for Clang> gcovr

# run tests and collect test coverage (Windows)
OpenCppCoverage.exe --export_type cobertura:coverage.xml --cover_children -- ctest --preset <preset>
```

## Customization checklist

- [ ] Change the LICENSE file
- [ ] Write a `README.md`
- [ ] Fill in the community health files
  - [ ] Edit `.github/CONTRIBUTING.md` (don't forget to update links)
  - [ ] Edit `.github/CODE_OF_CONDUCT.md` and insert contact method.
  - [ ] Edit `.github/SECURITY.md`
- [ ] Clean the `.git-blame-ignore-revs` file
- [ ] Configure CODEOWNERS (`.github/CODEOWNERS`)
- [ ] Configure Dependabot (`.github/dependabot.yml`)

### Select an appropriate LICENSE

The template uses the Unlicense license.
A license with no conditions whatsoever which dedicates works to the public domain.
Unlicensed works, modifications and larger works may be distributed
under different terms and without source code.
You likely want to use another license for your project.
If you are not yet sure which one to use,
[this](https://choosealicense.com/) resource can help you.

### Create a README

Describe your project to the audience.
People interested in your project don't need to read this text :)

And don't forget to update badge URLs if you decide to keep them.

### Fill in the community health files

Apart from README, there are a number of files in the .github directory
that help people to interact with your project:
submit issues and pull requests, security vulnerabilities.
Please update the contents of these files with the links to your repository and contact methods.

### Clean the `.git-blame-ignore-revs` file

The `.git-blame-ignore-revs` file contains commit SHAs that you may want to skip
while viewing the git blame on GitHub as if using `git blame --ignore-revs-file=.git-blame-ignore-revs`.

### Configure CODEOWNERS

Code owners are automatically requested for review
when someone opens a pull request that modifies code that they own.
Code owners are not automatically requested to review draft pull requests.

### Configure Dependabot

Dependabot is a GitHub bot that helps you keep dependencies up to date.
Configure dependency scanning in the `.github/dependabot.yml` file.
