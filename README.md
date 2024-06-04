# cpp-project-template

[![CI](https://github.com/b1ackviking/cpp-project-template/actions/workflows/ci.yml/badge.svg)](https://github.com/b1ackviking/cpp-project-template/actions/workflows/ci.yml)
[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit)](https://pre-commit.com)

## About cpp-project-template

This is a template of a cross-platform CMake-based C++ project.

- Run [Cppcheck](http://cppcheck.net/) and [Clang-Tidy](https://clang.llvm.org/extra/clang-tidy/).
- Ensure clean and robust code with a pre-configured set of warnings for Clang, GCC, and MSVC.
- Create sanitizer-instrumented builds by changing a single flag.
- Validate files structure and follow
the [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/)
specification with [pre-commit](https://pre-commit.com/) hooks.
- Build and test your project with [GitHub Actions](https://github.com/features/actions).

## Required software

- Common
  - [Git](https://git-scm.com/)
  - [CMake](https://cmake.org/)
  - [Ninja](https://ninja-build.org/)
  - [uv](https://github.com/astral-sh/uv)
  - [LLVM](https://releases.llvm.org/)
- Windows
  - [PowerShell](https://github.com/PowerShell/PowerShell)
  - [Visual Studio Build Tools (Desktop development with C++ workload)](https://visualstudio.microsoft.com/downloads/)
  - [OpenCppCoverage](https://community.chocolatey.org/packages/opencppcoverage)
    - Install with [Chocolatey](https://chocolatey.org/)
- Optional
  - [cppcheck](https://github.com/danmar/cppcheck)
  - [Node.js](https://nodejs.dev/download/)
    - [Commitlint](https://commitlint.js.org/)
    - [Semantic Release](https://github.com/semantic-release/semantic-release)

Run the following commands in the root of the repository after cloning:

```bash
uv venv
uv sync
uv run pre-commit install
uv run conan profile detect
```

## Building the project

<details open>
<summary>On Linux/Mac</summary>

```bash
source .venv/bin/activate

# install libraries
conan install -pr:a default \
  -pr:h conan/<profile matching the compiler in use> \
  -c:a tools.cmake.cmaketoolchain:generator=<CMake generator> \
  -c:h tools.build:compiler_executables='{"c": "<CC>", "cpp": "<CXX>"}' \
  -c:h tools.build:skip_test=<True|False> \
  -s:h build_type=<Debug|RelWithDebInfo|Release|MinSizeRel> -b missing .

# activate build env
source build/<build_type>/generators/conanbuild.sh

# configure
cmake --preset <preset> \
  -D ENABLE_CPPCHECK=<bool> \
  -D ENABLE_CLANG_TIDY=<bool> \
  -D ENABLE_IPO=<bool> \
  -D ENABLE_CACHE=<bool> \
  -D CACHE_OPTION=<ccache or sccache> \
  -D ENABLE_COVERAGE=<bool> \
  -D ENABLE_HARDENINGS=<bool> \
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
```
</details>

<details>
<summary>On Windows</summary>

```powershell
.venv\Scripts\activate.ps1

# install libraries
conan install -pr:a default \
  -pr:h conan/<profile matching the compiler in use> \
  -c:a tools.cmake.cmaketoolchain:generator=<CMake generator> \
  -c:h tools.env.virtualenv:powershell=pwsh \
  -c:h tools.build:compiler_executables='{"c": "<CC>", "cpp": "<CXX>"}' \
  -c:h tools.build:skip_test=<True|False> \
  -s:h build_type=<Debug|RelWithDebInfo|Release|MinSizeRel> -b missing .

# activate build env
build/<build_type>/generators/conanbuild.ps1

# configure
cmake --preset <preset> \
  -D ENABLE_CPPCHECK=<bool> \
  -D ENABLE_CLANG_TIDY=<bool> \
  -D ENABLE_IPO=<bool> \
  -D ENABLE_CACHE=<bool> \
  -D CACHE_OPTION=<ccache or sccache> \
  -D ENABLE_COVERAGE=<bool> \
  -D ENABLE_HARDENINGS=<bool> \
  -D ENABLE_FORTIFY_SOURCE=<bool> \
  -D ENABLE_ASAN=<bool>

# build
cmake --build --preset <preset>

# run tests and collect test coverage (Windows)
OpenCppCoverage.exe --export_type cobertura:coverage.xml --cover_children -- ctest --preset <preset>
```
</details>

## Customization checklist

- [ ] Change the LICENSE file
- [ ] Write a `README.md`
- [ ] Fill in the community health files
  - [ ] Edit `.github/CONTRIBUTING.md` (don't forget to update links)
  - [ ] Edit `.github/CODE_OF_CONDUCT.md`
  - [ ] Edit `.github/SECURITY.md`
- [ ] Clean the `.git-blame-ignore-revs` file
- [ ] Configure the `pyproject.toml` file
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

### Configure the `pyproject.toml` file

The `pyproject.toml` file is a main configuration file for `uv`.
This template is not a python project, however, using `uv` makes it
very convenient to bring tool dependencies,
such as recent versions of `conan`, `cmake`, `pre-commit`, etc.
Also, keeping them contained in a virtual environment does not pollute
your system and provides a simple way to setup them on a CI pipeline.

Besides specifying tool dependencies, you may want to also update
the `name`, `version`, `description`, `authors`, and `license` fields.

### Configure CODEOWNERS

Code owners are automatically requested for review
when someone opens a pull request that modifies code that they own.
Code owners are not automatically requested to review draft pull requests.

### Configure Dependabot

Dependabot is a GitHub bot that helps you keep dependencies up to date.
Configure dependency scanning in the `.github/dependabot.yml` file.
