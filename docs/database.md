# The suit-combination database

Gerrit used SuitC to analyse **every** suit combination and collect the
results in a database. The database answers questions such as "how many
tricks is AQ2 opposite 543 worth on average?" for every holding. It is the
basis of his playing-strength studies and of the essay
[Hand Valuation and Computers](hand-valuation.md).

## The data

The database consists of one file per pair of hand lengths: `<n><s>.xp`,
where `<n>` and `<s>` are the number of cards in North and South in
hexadecimal (`44.xp` is 4 cards opposite 4 cards, `a1.xp` is 10 cards
opposite 1). Each line describes one combination:

```text
N: 5432   S: 9876   EW: AKQJT   R:  0.6783   N1:  67.8261   N2:   0.0000   N3:   0.0000   N4:   0.0000
```

`R` is the expected number of tricks with the best play, and `N<k>` is the
best probability (in percent) of taking at least `<k>` tricks. The analysis
assumes North-South have enough entries to lead from either hand whenever
they want.

The complete database contains 105 files, about 190 MB in total (8.6 MB
compressed). It is attached to the GitHub releases as
`suitc-database.tar.gz`, rather than kept in git. Building it from scratch
takes about three days on a 16 GB machine.

## The scripts

Gerrit describes the procedure in
[How the database is built](building-the-database.md). All scripts are in
[scripts/](../scripts). They expect `SuitC` and `playc` on the `PATH`. They
are written in bash and Perl, and were developed under Cygwin and Linux.

**Building the database**

| Script | Purpose |
|---|---|
| `sc-install [-M] [<c>]` | Generate the SuitC command for every combination, using `playc` to enumerate the distributions, one directory per pair of hand lengths. `-M` adds a matchpoint analysis; `<c>` (such as `100k` or `001M`) adds a cut-off for the few combinations that need more resources than a laptop has. |
| `sc-runit ??`, `sc-doall` | Run the analyses of one directory (such as `35`), or of all of them, and produce the `??.xp` files. |
| `P-run` | Run the commands in one directory, skipping combinations whose insignificant small cards make no difference. |
| `sc-check` | Reuse an earlier result if a combination was already analysed. |
| `P-result`, `P-resultm` | Turn a SuitC output into one line, such as `N: J98 S: AK32 EW: QT7654 MAX(1): 2.8537 N3(2): 78.2609 2.7947 ...`. |
| `P-all` | Remove combinations where a higher North-South card does not improve the result. |
| `sc-expand`, `sc-alldist` | Expand a `??.res` file to all equivalent distributions (`sc-alldist` also for analyses made with `-D` or `-Dw`). |
| `sc-norm`, `sc-nm`, `sc-nmp`, `sc-ns` | Normalise and filter result files: remove multiplicities, matchpoint information, and 100 % results that are not safety plays. |

**Using the database**

| Script | Purpose |
|---|---|
| `sc-strength` | From an `.xp` file, tabulate the expected tricks and trick probabilities for the North and South holdings that match regular expressions (`-mn`, `-ms`, `-nn`, `-ns`). |
| `sc-weight` (also `sc-tricks`) | The expected number of tricks for South's holdings, depending on the high-card points in North (`-p`) and the honours South holds in the other suits (`-h`). |
| `sc-MPcompare`, `sc-IMPcompare` | Compare the matchpoint or IMP results of several lines of play, from SuitC's `-b` and `-a` output and, for IMPs, the score of each number of tricks. |
| `sc-IMPlist`, `sc-IMPscore` | Select and list the MP or IMP plays, compared with the MAX play and an `N<x>` safety play. |

## Playing-strength studies

[examples/strength](../examples/strength) contains Gerrit's studies built on
the database. Each script contains the commands it runs, followed by its
output after the `exit` statement.

| Study | Question |
|---|---|
| `A`, `Q`, `Q-K`, `x1`, `xt` | How much is an ace, a queen or a small card worth in each hand length? |
| `KQ` | Is it better to hold K and Q in the same hand, or divided? (The essay uses these results.) |
| `KQT` | Game trees for KQT opposite a range of South holdings. |

