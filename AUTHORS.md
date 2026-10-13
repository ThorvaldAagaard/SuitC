# Authors and credits

## Author

**Gerrit A. van der Velde** (Leiden, the Netherlands) wrote SuitC, dds, playc
and the database scripts, and holds the copyright on them. Every commit in the
history up to the open-source release is his work, imported from his RCS
archives (see [HISTORY.md](HISTORY.md)).

## Maintainer

**Thorvald Aagaard** maintains this repository. At Gerrit's request he
prepared SuitC for its open-source release. He also maintains
[SuitCWinLib](https://github.com/codescrut/SuitCWinLib), the library build of
SuitC used by BEN. He suggested the secondary objective of winning tricks early
and using entries economically, which became the `-u` option.

## Contributors

- **Lars Kappel Hansen** ported SuitC to Microsoft Visual C++ and gave it a
  JSON interface. That port became the basis of SuitCWinLib and of the SuitC
  web service behind CCAnalyzer. The `SUITCAPP` build option was introduced
  for his application in 2021 (Rev 1.018).
- **Jeroen Warmerdam** introduced matchpoint plays for suit combinations in
  the Dutch IMP magazine in 1998 and wrote SuitPlay. SuitC's default ordering
  of East-West distributions in the app build follows his (Rev 1.028). He also
  suggested skipping insignificant small cards when building the database
  (`P-run`).
- **Jan Boerhout**, Gerrit's colleague at NEC, pointed out portability issues
  that were fixed in Rev 1.013x.
- **Huub van der Wouden**, owner of the Dutch *IMP* magazine: the dds examples
  `H17` and `LV` come from his collection (Gerrit kept them in a folder named
  HvdW).
- **Bo Haglund**, author of the DDS double-dummy solver: dds takes its
  shifted-holdings technique from his work.

## Sources Gerrit acknowledges

- Eric Crowhurst, the entry "Suit Combinations" in *The Official Encyclopedia of Bridge*
- J.-M. Roudinesco, *The Dictionary of Suit Combinations*
- Jeff Rubens, *The Secrets of Winning Bridge*: the idea behind the
  playing-strength studies
- David Caprera, *Bridge: The Cutting Edge*: matchpoint strategy problems
