# History of SuitC

## Origins

Gerrit A. van der Velde trained as a quantum chemist. He wrote SuitC as a C
console program for Unix, and parts of the code are about 25 years old. Its
predecessor was called **ExpertPlay**: SuitC Rev 1.001 (25 April 2020) is
recorded as "identical to ExpertPlay-Rev6.102". The oldest code in this
repository is **playc**, first checked in on 3 August 2008. **dds**, a
double-dummy solver for comparing plays over several layouts of a deal, joined
the set in 2020/2021.

Gerrit kept his code under RCS. Each change was recorded with a numbered
history id and a comment, and those history files survive. The git history of
this repository was rebuilt from them, so every commit corresponds to one
revision in Gerrit's RCS history. Each commit carries his original comment and
timestamp, and an `RCS-History-Id` trailer. The rebuilt snapshots were checked
against the source archives he distributed (Rev 1.037, 1.039, 2.010, 2.011,
2.020, 2.022 and dds 1.030) and are byte-for-byte identical, apart from the generated `extern.h`.

## Milestones

| Date | Revision | Milestone |
|---|---|---|
| 2008-08-03 | playc 1.1 | playc: enumeration of suit distributions |
| 2020-04-25 | SuitC 1.001 | SuitC continues ExpertPlay Rev 6.102 |
| 2020-10 | 1.010 | Throw-ins: an opponent who wins a trick can be required to lead |
| 2021-03 | 1.013x | Portability fixes |
| 2021-06 | 1.018 – 1.019 | `SUITCAPP` build for Lars Kappel Hansen's web application |
| 2021-07-23 | dds 1.001 | dds enters RCS |
| 2022-07 | 1.022 – 1.024 | Matchpoint score printed for every play; optimal IMP plays |
| 2022-09 | 1.028 | Jeroen Warmerdam's ordering of East-West distributions |
| 2023-05 | 1.032 | Improved `-l` (free directed leads); copyright statement |
| 2024-02-09 | 1.037 | First version shared for use in BEN |
| 2024-06-16 | 1.038 | Bug fix in `print_all_results`, found through the BEN integration |
| 2024-07-28 | 1.039 | `SUITCAPP` moves from `header.h` to the Makefile |
| 2025-01-14 | 2.001 | Rev 2 line, restarted from the undistributed Rev 1.050 |
| 2025-02-06 | 2.009 | New option to win tricks early and save outside entries (`-v`, renamed `-u` in 2.010) |
| 2025-02-20 | 2.011 | First open-source release (v2.011) |
| 2025-10 – 2026-04 | 2.016 – 2.018 | Fold modes (`-m`) and unfolded first plies (`-Fs`) for clearer game trees |
| 2026-07-03 | 2.019 | Throw-in plays re-introduced (`-H`) |
| 2026-10-13 | 2.020 | IMP fixes; sent by Gerrit for the open-source release |
| 2026-10-18 | 2.021 – 2.022 | Throw-in removed again; `-I` without a score is rejected |

## SuitC in other programs

In 2021 Lars Kappel Hansen ported SuitC to Microsoft Visual C++ and gave it a
JSON interface for a web application. In April 2024 Thorvald Aagaard asked
Gerrit whether SuitC could be used in the bridge robot **BEN**. Gerrit agreed,
and Lars's port became the basis of **SuitCWinLib**, a library build for
Windows, Linux and macOS. BEN, Lia and Lars's SuitC web application now use
SuitC through it.

Working with BEN produced several improvements in 2024 and 2025: a bug fix in
1.038, the memory-pool settings for library use, and the `-u` option
(Rev 2.009/2.010). `-u` chooses, among equally good plays, the one that wins
tricks earliest and uses the fewest outside entries.

Between 2024 and 2026 Thorvald kept a private backup of Gerrit's source
distributions on GitHub. In October 2026 Gerrit agreed to release SuitC as open
source under the MIT licence, and asked Thorvald to take responsibility for the
public project. For the release he sent his latest revision (2.020, followed by 2.022), the
complete set of database scripts and a description of how the database is
built.
Gerrit continues to help with documentation and with questions about how
SuitC works.

## Not included

- Revisions 1.040 to 1.050 (August 2024 to January 2025) were never
  distributed. Rev 2.001 contains all of their changes.
- Revisions before playc 1.1 and SuitC 1.001 (the ExpertPlay era) are not
  available.
- The suit-combination database itself (the `*.xp` files, about 190 MB) is
  published as a release asset rather than in git. See
  [docs/database.md](docs/database.md).
