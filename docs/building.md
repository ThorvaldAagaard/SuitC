# Building SuitC

SuitC, dds and playc are plain C programs that use only the C standard
library. They build on Linux, macOS and Windows.

## With CMake (all platforms)

You need CMake 3.21 or newer and a C compiler: gcc, clang, or Visual Studio
2022 or later.

```sh
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release        # run the regression tests
cmake --install build --prefix ~/.local  # optional: install SuitC, dds and playc
```

With Visual Studio the programs are written to `build\Release\`, otherwise to
`build/`.

| CMake option | Default | Meaning |
|---|---|---|
| `SUITC_APP` | `OFF` | Build with `SUITCAPP=YES`, the reduced output and memory profile used by embedding applications (see below). |
| `SUITC_BUILD_DDS` | `ON` | Build dds. |
| `SUITC_BUILD_PLAYC` | `ON` | Build playc. |
| `BUILD_TESTING` | `ON` | Add the regression tests. |

The build generates `extern.h` from `global.h` (in the build directory) and
sets an 8 MB stack on Windows to match Linux and macOS: the programs recurse
deeply.

## With Gerrit's Makefiles (Linux, macOS, Cygwin, WSL)

Gerrit's original Makefiles are kept, and they remain the way he builds
SuitC:

```sh
mkdir -p bin
(cd src && make)      # bin/SuitC, plus the links bin/sc and bin/ExpertPlay
(cd dds && make)      # bin/dds
(cd playc && make)    # bin/playc
```

The Makefiles generate `extern.h` next to the sources with `sed`. If you
switch between the two build systems, delete `src/extern.h` and
`dds/extern.h` first, so that CMake's generated copy is used.

## SUITCAPP

`SUITCAPP` is a compile-time switch, set in the Makefile (`-DSUITCAPP=NO`)
or with the CMake option `SUITC_APP`. It was introduced in Rev 1.018 for
embedding SuitC in other applications. With `SUITCAPP=YES`:

- at most 10 plays are reported, and diagnostic output is suppressed;
- the cut-off is always active (`CUTOFF` in `header.h`);
- the quirk pool is smaller (4 million entries instead of 80 million);
- the East-West distributions are printed in Jeroen Warmerdam's order, and
  card sequences are printed with the same symbol by default.

The analysis results themselves are the same.

## Memory limits

SuitC allocates its memory pools at start-up. Their sizes are set in
`src/header.h`:

| Constant | Default | Pool |
|---|---|---|
| `NNODES` | 10,000,000 | game tree nodes |
| `NINDICES` | 16,000,000 | board indices |
| `NQUIRKS` | 80,000,000 (4,000,000 with `SUITCAPP`) | quirk tree entries |

The defaults are sized for difficult combinations and allocate a few
gigabytes of address space. If memory is short, for example when many
copies run in parallel or SuitC is embedded in a long-running process, Gerrit
recommends `NINDICES 10000000L`, `NNODES 8000000L` and `NQUIRKS 3000000L`. That
reduces the allocated memory by a factor of about 25 and is enough for almost
all combinations. If a combination runs out of memory, SuitC prints
`ERROR: No more memory available`. Raise the relevant constant and rebuild.

dds has similar constants (`BHASHL`, `NNODE`, `NSEQC`) in `dds/header.h`.

## Compiler warnings

The code dates from a time of 32-bit pointers. A few diagnostic `printf`s
print pointers with `%lx`, which modern compilers warn about. These warnings
are harmless: they only affect the trace output of `-d`/`-f`/`-T`.
