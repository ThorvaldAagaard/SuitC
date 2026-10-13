# Examples

- [dds/](dds): input scripts and output for the dds double-dummy solver.
  They are described in [docs/dds-and-playc.md](../docs/dds-and-playc.md#dds).
- [strength/](strength): Gerrit van der Velde's playing-strength studies,
  built on the suit-combination database. Each file is a script with its
  output appended after the `exit` statement. See
  [docs/database.md](../docs/database.md#playing-strength-studies).

For SuitC itself, the [regression tests](../tests/cases.txt) are a good set of
examples. Each case has its expected output in [tests/expected](../tests/expected).
