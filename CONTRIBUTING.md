# Contributing to SuitC

Thank you for your interest in SuitC. Bug reports, questions about the
analysis, documentation improvements and code changes are all welcome.

## Reporting a problem

Please open an issue and include:

- the exact command line (`SuitC <options> <north> <south> <east-west>`);
- what you expected and what SuitC printed;
- the platform, and whether you used a release binary or your own build.

If you think a result is wrong, say why: for example, a line of play that
does better than the one SuitC reports, together with its probability.

## Changing the code

- Build with CMake and run the regression tests before and after your change:
  ```sh
  cmake -S . -B build && cmake --build build --config Release
  ctest --test-dir build -C Release --output-on-failure
  ```
- A change that is not meant to affect the analysis must leave every test
  output identical. If a change intentionally changes results, regenerate the
  expected output with `tests/update_expected.cmake`, and explain the
  differences in the pull request.
- Add a test case to [tests/cases.txt](tests/cases.txt) for new options and
  for bugs you fix.
- Follow the style of the surrounding code: one function per `.c` file, with
  globals declared in `global.h`. Never edit `extern.h`; it is generated.
- Keep the code portable C99 with no dependencies beyond the standard library.
  CI builds on Linux, macOS and Windows.

## Versions

SuitC keeps Gerrit van der Velde's revision numbering (Rev 2.011, ...).
The tags `rev-<revision>` mark his revisions; published releases are tagged
`v<revision>`, which builds and attaches the binaries for all platforms. Add
an entry for your change to [CHANGELOG.md](CHANGELOG.md) under a section
*Unreleased* at the top.

## Licence

By contributing you agree that your contribution is released under the
[MIT licence](LICENSE).
