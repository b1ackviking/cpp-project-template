# cpp-project-template

[![CI](https://github.com/b1ackviking/cpp-project-template/actions/workflows/ci.yml/badge.svg)](https://github.com/b1ackviking/cpp-project-template/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/b1ackviking/cpp-project-template/branch/main/graph/badge.svg?token=FSWI6GZ1J9)](https://codecov.io/gh/b1ackviking/cpp-project-template)

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
- Build, test, and deploy your project with [GitHub Actions](https://github.com/features/actions).

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
poetry run pre-commit install --hook-type commit-msg
poetry run conan profile new default --detect
```

## Building the project

```bash
poetry shell

# install libraries
conan install -if build -pr:b default -pr:h default \
  -c:h tools.cmake.cmaketoolchain:generator=Ninja \
  -pr:h conan/<profile matching the compiler in use> \
  -c:h tools.build:compiler_executables='{"c": <C compiler path>, "cpp": <C++ compiler path>}'
  -c:h tools.build:skip_test=<True|False> \
  -s build_type=<Debug|RelWithDebInfo|Release|MinSizeRel> -b missing .

# configure
cmake --preset <preset> \
  -D ENABLE_CPPCHECK=<bool> `# use cppcheck for static analysis, default false` \
  -D ENABLE_CLANG_TIDY=<bool> `# use clang-tidy for static analysis, default false` \
  -D ENABLE_INCLUDE_WHAT_YOU_USE=<bool> `# run iwyu during the build, default false` \
  -D ENABLE_IPO=<bool> `# use inter-procedural optimization, default false` \
  -D ENABLE_CACHE=<bool> `# use caching for faster recompilation, default false` \
  -D CACHE_OPTION=<ccache or sccache> `# compilation cache driver, default ccache` \
  -D ENABLE_DOXYGEN=<bool> `# generate documentation during the build, default false` \
  -D ENABLE_COVERAGE=<bool> `# collect code coverage (for unit tests), default false` \
  -D ENABLE_SANITIZER_ADDRESS=<bool> `# default false`\
  -D ENABLE_SANITIZER_LEAK=<bool> `# Clang and GCC only, default false` \
  -D ENABLE_SANITIZER_UNDEFINED_BEHAVIOR=<bool> `# Clang and GCC only, default false` \
  -D ENABLE_SANITIZER_THREAD=<bool> `# Clang and GCC only, default false` \
  -D ENABLE_SANITIZER_MEMORY=<bool> `# Clang only, default false`


# build
cmake --build --preset <preset> --parallel --target all

# run tests
cmake --build --preset <preset> --parallel --target test

# collect test coverage if ENABLE_COVERAGE == TRUE
GCOV=<gcov for GCC, "llvm-cov gcov" for Clang> gcovr --txt

# run tests and collect test coverage (Windows)
OpenCppCoverage.exe --export_type cobertura:coverage.xml --cover_children -- cmake --build --preset <preset> --parallel --target test
```

## Customization checklist

- [ ] Change the LICENSE file
- [ ] Write a `README.md` and [INSERT CONTACT METHOD] in `CONTRIBUTING.md` and `CODE_OF_CONDUCT.md`
- [ ] Create an `ADMIN_TOKEN` for CI
- [ ] Clean the `.git-blame-ignore-revs` file
- [ ] `git add . && git commit --amend` your changes as `chore: initial commit`
- [ ] Create a tag `0.0.0` on the first commit

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

### Create an ADMIN_TOKEN

The template uses a CI job to generate and push changelogs to the `main` branch.
It is recommended to set up branch protection rules for the main branch.
However, it will disallow pushing commits from the CI job.
A way to overcome this limitation is to allow pushes from administrators
and use an administrator's token with repository access in the CI.
Go to your account *Settings* -> *Developer settings* -> *Personal access tokens*
to generate a token and set it in the repository *Settings* -> *Secrets* as an `ADMIN_TOKEN`.

### Clean the `.git-blame-ignore-revs` file

The `.git-blame-ignore-revs` file contains commit SHAs that you may want to skip
while viewing the git blame on GitHub as if using `git blame --ignore-revs-file=.git-blame-ignore-revs`.

### Start the project

After you have set up everything, you can finally start your project.
To do that - simply commit everything with an `--amend` option
to overwrite the first commit.
You also need to set the first tag in the repository - the initial version.
By default, the version tags use the `MAJOR.MINOR.PATCH` format.
You can change it in the `.releaserc.js` file if you want.
