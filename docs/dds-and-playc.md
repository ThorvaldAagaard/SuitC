# dds and playc

## dds

dds is a double-dummy solver for complete deals, with a twist. You give it
several layouts of the same deal (for example, every layout consistent with
the bidding), and it shows, for each card you could play, the number of tricks
on each layout. It is interactive: after each analysis you choose the card to
play and it continues from there.

```text
dds [-h] [-L{w|n|e|s}] [-P{w|n|e|s}] [-T{s|h|d|c|n}] <west> <north> <east> <south> [<west> <north> <east> <south> ...]
```

| Option | Meaning |
|---|---|
| `-L<p>` | The opening leader. |
| `-P<p>` | Each layout is entered starting with this player's hand. |
| `-T<s>` | The trump suit: `s`, `h`, `d`, `c`, or `n` for notrump. |

A hand is written as `<spades>.<hearts>.<diamonds>.<clubs>`, with English or
Dutch card symbols in either case, e.g. `AKT7.KJT5.QJ62.6`.

At the prompt, enter a card to play (e.g. `h9`), `b` to go back one ply, `b<n>`
to go back to ply `<n>`, `p` or `P` to print statistics, or `.` to stop.
The statistics start with the `cpu time` in seconds, measured as in SuitC's
`-T1` (see [Usage](usage.md)): elapsed time on Windows, processor time on
Linux and macOS.

[examples/dds](../examples/dds) contains Gerrit's examples:

- `doit`: a deal from IMP magazine, analysed over four layouts;
- `TA`: compares the double-dummy result with the single-dummy result for a
  two-suit ending;
- `ben`: a deal from a BEN match, with the layouts consistent with the
  auction;
- `H17` and `LV`: two further examples; `LV.out` is the output of `LV`;
- `dds.out`: a short interactive session on one layout of the `doit` deal;
- `s14.out`: the output of a run over 14 layouts (compare `TA`).

dds took the technique of shifted holdings from Bo Haglund's DDS.

Since dds Rev 2 the NODE tree has room for 800 million entries, about 19 GB,
which dds asks for at startup. If that much memory is not available, dds
halves the number until the allocation succeeds and prints
`Only <n> NODE entries fit in memory`. The analysis is the same as long as
the tree fits; if it does not, dds stops with
`No more memory for the NODE entries`. The `doit` example needs between 40
and 100 million entries.

The `.out` files in [examples/dds](../examples/dds) were made with dds 1.x.
dds Rev 2 lays out the boards differently, but gives the same trick counts.

## playc

playc lists all suit combinations. For every set of cards North-South can
hold in a suit, it prints each way of dividing those cards between North and
South, followed by the East-West cards:

```text
. 32 edcba987654
2 3 edcba987654
32 . edcba987654
```

`.` is a void, and the honours are written as hexadecimal digits (`e` = A,
`d` = K, `c` = Q, `b` = J, `a` = T). Combinations that differ only in
equivalent small cards are listed once. playc takes no arguments.
`sc-install` uses its output to generate the database runs (see
[The suit-combination database](database.md)).

playc is the oldest program in the repository: version 1.1 dates from
August 2008.
