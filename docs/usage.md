# Using SuitC

```text
SuitC [options] <north> <south> <east-west>
```

SuitC analyses one suit. You give it the North cards, the South cards and the
cards held by East and West together. SuitC then considers every way the
East-West cards can be divided, weighted by its a-priori probability.

```sh
SuitC AQ2 543 KJT9876          # the classic finesse
SuitC -a KJ9 432 AQT8765       # also list every acceptable play
SuitC -M AKJT32 54 Q9876       # best play at matchpoints
```

`SuitC -h` (or `SuitC -?`) prints the list of options.

## Cards

- Ranks are `A K Q J T 9 8 7 6 5 4 3 2`. The Dutch symbols `H` (heer, king),
  `V` (vrouw, queen) and `B` (boer, jack) are accepted too. Upper and lower
  case are both accepted.
- A void in North or South is written as `.`, `=` or `#`, for example
  `SuitC . AKJ9 QT8765432`.
- North-South are the declaring side and East-West the defenders. North and
  South each see only their own hand; East and West are assumed to know all
  four hands (see [How SuitC works](algorithm.md)).

## Options

### What to print

| Option | Meaning |
|---|---|
| `-h`, `-?` | Print the options and stop. |
| `-a` | Also print a summary: every play that is optimal for some objective, with its trick distribution. |
| `-A` | Print a full report of all results. This can be very long; a 7-card combination can produce hundreds of thousands of lines. |
| `-b` | Print the list of East-West distributions with their probabilities and the range of tricks North-South can take on each. |
| `-r` | Print only the results, without game trees. Uses much less memory. |
| `-d` | Print the truncated NODE tree (diagnostic). |
| `-f` | Print the full NODE tree (diagnostic). |
| `-T<n>` | Trace output for development: `1` = timing and memory counters, `2` = minimax progress. |

### How the game tree is printed

| Option | Meaning |
|---|---|
| `-F<n>` | Presentation of the game tree, as a sum of: `1` = fold East-West plays, `2` = fold North-South plays, `4` = select North-South plays. The default is `5`. `-F0` prints the full, unfolded tree, which contains all information. `-F3` gives a more compact tree. |
| `-P<n>` | Card symbols: `0` = English lower case, `1` = Dutch lower case, `2` = English upper case (default), `3` = Dutch upper case. |
| `-pH` | Show the highest card of a North-South card sequence instead of the lowest. |
| `-pI`, `-pM` | Print IMP or matchpoint information for all plays. |
| `-pS` | Print a different symbol for each card of a card sequence. |
| `-o` | Change the order in which the East-West distributions are printed. |

### Restricting the deal

| Option | Meaning |
|---|---|
| `-W<cards>`, `-E<cards>` | West (or East) holds these cards in every distribution, e.g. `-WK`. |
| `-wm<n>`, `-em<n>` | West (or East) holds at least `<n>` cards in the suit. |
| `-wv<n>`, `-ev<n>` | West (or East) has `<n>` vacant places. The distributions are weighted by the vacant places instead of by the a-priori odds. |

### Restricting the play

| Option | Meaning |
|---|---|
| `-L{n,e,s,w}[<card>]` | The opening lead at trick one is from North, East, South or West, optionally with a specific card, e.g. `-LsQ`. Without `-L`, SuitC tries every lead. |
| `-l{n,e,s,w}<n>` | Limit the number of free directed leads from that hand to `<n>`. A free directed lead is an entry outside the suit, so `-ls0` means South has no outside entries. Without `-l` the number of entries is unlimited. |
| `-R{n,e,s,w}<n>` | That hand has `<n>` ruffs available. |
| `-t<n>` | Play only `<n>` tricks in the suit. |
| `-S<card2>:<card3>` | Third-hand rule: if second hand plays a card from `<card2>`, third hand plays a card from `<card3>`. Use it together with `-L`, e.g. `-Ls -S9876:J`. |
| `-D` | The defenders lead to every trick, which is useful for stopper analysis. Use it only if East-West hold at least twice as many cards as North-South. `-DW` prints the East-West winners instead. |

### Objectives and selection

| Option | Meaning |
|---|---|
| *(default)* | Maximum expected tricks, plus the safety plays for each number of tricks. |
| `-M[<nodes>]` | Find the optimal matchpoint play. This may be a mixed strategy, solved with the simplex method on at most `<nodes>` nodes. |
| `-I<n>:<score>` | Find the optimal play at IMPs, where taking `<n>` tricks in the suit scores `<score>` points. Give the option once for each trick count, e.g. `-I2:-100 -I3:620 -I4:650`. |
| `-u` | Among equally good plays, prefer the one that wins tricks as early as possible, uses as few outside entries as possible, and uses the entries in the suit itself first. (Added in Rev 2.009 as `-v`.) |
| `-z<n>` | Remove weakly dominated plays: `1` = in `check_zermelo`, `2` = in `evaluate_mp_results`. |
| `-c<n>[k,m,g]` | Cut-off: a limit on the product of the number of quirks while quirk trees are merged (`k` = thousand, `m` = million, `g` = billion). Use it only for pathological combinations such as `KJ93 75 AQT8642`. It trades accuracy for time and memory. |

## Examples

```sh
# Finesse against the king, with one outside entry to South
SuitC -Ls -ls1 AQ2 543 KJT9876

# West is known to hold at least two cards; play three tricks only
SuitC -t3 -wm2 AQT2 J93 K87654

# Matchpoint play with a third-hand rule; show the boards and MP details
SuitC -b -d -M -pM -Ls -ln0 -S9876:J AKJT32 54 Q9876

# Prefer the line that wins tricks early and keeps entries
SuitC -u QJ6 KT73 A98542
```

The [regression tests](../tests/cases.txt) contain more examples. Each has
its expected output in [tests/expected](../tests/expected).

## Performance and memory

Most combinations take a fraction of a second. Large combinations, with
12 to 14 cards and many relevant cards, can take minutes or hours and need
several gigabytes. For example, `-Ls AQT3 75 KJ98642` takes about 20 minutes.
If SuitC stops with `ERROR: No more memory available`, either use `-r` (no game
tree), add a cut-off with `-c`, or rebuild with larger memory pools (see
[Building](building.md#memory-limits)).
