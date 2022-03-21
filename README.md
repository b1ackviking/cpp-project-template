# cpp-project-template

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
- [Clang tools](https://releases.llvm.org/)
- [Python](https://www.python.org/)
  - [pip](https://pypi.org/project/pip/)
  - [pre-commit](https://pypi.org/project/pre-commit/)
  - [cmakelang](https://pypi.org/project/cmakelang/)
  - [conan](https://pypi.org/project/conan/)
- [Node.js](https://nodejs.dev/download/)

Run the following commands in the root of the repository after cloning:

```bash
pre-commit install
pre-commit install --hook-type commit-msg
```

## Customization checklist

- [ ] Change the LICENSE file
- [ ] Write a README.md
- [ ] Create an ADMIN_TOKEN for CI
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

### Start the project

After you have set up everything, you can finally start your project.
To do that - simply commit everything with an `--amend` option
to overwrite the first commit.
You also need to set the first tag in the repository - the initial version.
By default, the version tags use the `MAJOR.MINOR.PATCH` format.
You can change it in the `.releaserc.js` file if you want.
